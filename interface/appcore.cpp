/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  {description}                                                          *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż            l.drozdz@o2.pl   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "appcore.hpp"

AppCore* AppCore::THIS = nullptr;
AVRBridge* AppCore::Device = nullptr;
QString AppCore::LastError = QString();

AppCore::AppCore(void)
: QObject(nullptr), Script(&Adc)
{
	if (THIS) qFatal("Core object duplicated");
	else THIS = this;

	Interval.setInterval(1000);

	Database = QSqlDatabase::addDatabase("QSQLITE");
	Device = new AVRBridge(&Script.Variables, this);

	Database.setDatabaseName("database.sqlite");

	if (!Database.open()) qFatal("Can't open database");

	Script.Bindings.Add("get", [this] (KLVariables& Vars) -> double
	{
		Device->UpdateSensorVariables(); return 0;
	});

	Script.Bindings.Add("put", [this] (KLVariables& Vars) -> double
	{
		unsigned char Values = Device->Variables()["SHRD"].ToInt();

		if (Vars.Size() == 1)
		{
			Values = Vars["0"].ToInt();
		}
		else if (Vars.Size() == 2)
		{
			if (Vars["1"].ToBool()) Values |= (1 << Vars["0"].ToInt());
			else Values &= ~(1 << Vars["0"].ToInt());
		}
		else if (Vars.Size() == 8)
		{
			for (int i = Values = 0; i < 8; i++) Values = Values | (Vars[KLString(i)].ToInt() << i);
		}

		Device->WriteShiftValues(Values); return 0;
	});

	Script.Bindings.Add("pga", [this] (KLVariables& Vars) -> double
	{
		if (Vars.Size() == 2) Device->WriteGainSettings(Vars["0"].ToInt(), Vars["1"].ToInt()); return 0;
	});

	Script.Bindings.Add("out", [this] (KLVariables& Vars) -> double
	{
		if (Vars.Size() == 1) Device->WriteShiftStatus(Vars["0"].ToBool()); return 0;
	});

	Script.Bindings.Add("sys", [this] (KLVariables& Vars) -> double
	{
		Device->UpdateSystemVariables(); return 0;
	});

	Script.Bindings.Add("slp", [this] (KLVariables& Vars) -> double
	{
		return 0;
	});

	Script.Bindings.Add("spi", [this] (KLVariables& Vars) -> double
	{
		QList<unsigned char> Data;

		for (const auto& Var: Vars) Data.append(Var.Value.ToInt());

		Device->WriteSpiValues(Data); return 0;
	});

	connect(&Interval, &QTimer::timeout, [this] (void) -> void { Device->UpdateSensorVariables(); });

	connect(Device, &AVRBridge::onSensorValuesUpdate, this, &AppCore::PerformTasks);

	UpdateScriptTasks();
}

AppCore::~AppCore(void)
{
	THIS = nullptr;
}

void AppCore::UpdateVariables(const KLVariables &Vars)
{
	for (const auto& Var: Vars) if (Script.Variables.Exists(Var.ID)) Script.Variables[Var.ID] = Var.Value.ToNumber();
}

void AppCore::PerformTasks(const KLVariables& Vars)
{
	Adc = Vars; for (const auto& Task: Tasks)
	{
		Script.SetCode(Task);
		Script.Evaluate();
	}
}

void AppCore::UpdateInterval(double Time)
{
	Interval.setInterval(Time * 1000);
}

void AppCore::UpdateStatus(bool Active)
{
	if (Active) Interval.start();
	else Interval.stop();
}
#include <QDebug>
void AppCore::SynchronizeDevice(void)
{
	QString Code = Initscript;

	for (const auto& Task: Tasks) Code.append('\n').append(Task);

	Device->WriteSleepValue(Interval.interval() / 1000.0);
	Device->WriteMasterScript(Code);
}

bool AppCore::SensorScriptOk(const QString& Code)
{
	KLScriptbinding Tester(&Device->Variables());

	if (Tester.Validate(Code)) return true;
	else
	{
		LastError = QString(tr("Invalid script at line %1 (%2)").arg(Tester.GetLine()).arg(Tester.GetMessage()));
	}

	return false;
}

bool AppCore::EventScriptOk(const QString& Code)
{
	KLScriptbinding Tester(&Device->Variables());

	Tester.Bindings.Add("get", [] (KLVariables&) -> double {return 0;});
	Tester.Bindings.Add("put", [] (KLVariables&) -> double {return 0;});
	Tester.Bindings.Add("out", [] (KLVariables&) -> double {return 0;});
	Tester.Bindings.Add("pga", [] (KLVariables&) -> double {return 0;});
	Tester.Bindings.Add("sys", [] (KLVariables&) -> double {return 0;});
	Tester.Bindings.Add("slp", [] (KLVariables&) -> double {return 0;});
	Tester.Bindings.Add("spi", [] (KLVariables&) -> double {return 0;});

	if (Tester.Validate(Code)) return true;
	else
	{
		LastError = QString(tr("Invalid script at line %1 (%2)").arg(Tester.GetLine()).arg(Tester.GetMessage()));
	}

	return false;
}

void AppCore::UpdateScriptTasks(void)
{
	QSqlQuery Query(Database);

	Script.Variables.Clean();

	Tasks.clear();
	Initscript.clear();

	Query.prepare(
		"SELECT "
			"label, "
			"script "
		"FROM "
			"sensors "
		"WHERE "
			"active>0");

	if (Query.exec()) while (Query.next())
	{
		Script.Variables.Add(Query.value(0).toString().toStdString().c_str(), KLVariables::NUMBER);

		Tasks.append(KLScriptbinding::Optimize(
					   Query.value(1).toString()
					   .replace(QRegExp("return\\s+([^;]+);"),
							  QString("set %1 \\1;exit;").
							  arg(Query.value(0).toString()))));
	}
	else
	{
		LastError = Query.lastError().text();
	}

	if (!Tasks.isEmpty())
	{
		for (const auto& Var: Script.Variables) Initscript.append(Var.ID).append(',');

		Initscript[Initscript.size() - 1] = ';'; Initscript = QString("export %1").arg(Initscript);
	}

	Query.prepare(
		"SELECT "
			"script "
		"FROM "
			"events "
		"WHERE "
			"active>0");

	if (Query.exec()) while (Query.next())
	{
		Tasks.append(KLScriptbinding::Optimize(Query.value(0).toString()));
	}
	else
	{
		LastError = Query.lastError().text();
	}

	emit onScriptUpdate();
}

bool AppCore::AddSensor(SensorData& Data)
{
	if (!SensorScriptOk(Data.Script)) return false;

	QSqlQuery Query(Database);

	Query.prepare(
		"INSERT INTO "
			"sensors (name, label, unit, script, active) "
		"VALUES "
			"(:name, :label, :unit, :script, :active)");

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":unit", Data.Unit);

	Query.bindValue(":script", Data.Script);

	Query.bindValue(":active", Data.Active);

	if (Query.exec()) Data.ID = Query.lastInsertId().toInt();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::UpdateSensor(SensorData& Data)
{
	if (!SensorScriptOk(Data.Script)) return false;

	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"sensors "
		"SET "
			"name=:name, "
			"label=:label, "
			"unit=:unit, "
			"script=:script, "
			"active=:active "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":unit", Data.Unit);

	Query.bindValue(":script", Data.Script);

	Query.bindValue(":active", Data.Active);

	if (Query.exec()) UpdateScriptTasks();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::DeleteSensor(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"DELETE FROM "
			"sensors "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec()) UpdateScriptTasks();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

SensorData AppCore::GetSensor(int ID)
{
	if (ID < 0) return SensorData();

	QSqlQuery Query(Database);
	SensorData Data;

	Query.prepare(
		"SELECT "
			"ID, name, label, unit, script, active "
		"FROM "
			"sensors "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec() && Query.next())
	{
		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Label = Query.value(2).toString();
		Data.Unit = Query.value(3).toString();
		Data.Script = Query.value(4).toString();
		Data.Active = Query.value(5).toBool();
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Data;
}

QList<SensorData> AppCore::GetSensors(void)
{
	QSqlQuery Query(Database);
	QList<SensorData> List;

	Query.prepare(
		"SELECT "
			"ID, name, label, unit, script, active "
		"FROM "
			"sensors");

	if (Query.exec()) while (Query.next())
	{
		SensorData Data;

		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Label = Query.value(2).toString();
		Data.Unit = Query.value(3).toString();
		Data.Script = Query.value(4).toString();
		Data.Active = Query.value(5).toBool();

		List.append(Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return List;
}

bool AppCore::AddEvent(EventData& Data)
{
	if (!EventScriptOk(Data.Script)) return false;

	QSqlQuery Query(Database);

	Query.prepare(
		"INSERT INTO "
			"events (name, script, active) "
		"VALUES "
			"(:name, :script, :active)");

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":script", Data.Script);
	Query.bindValue(":active", Data.Active);

	if (Query.exec()) Data.ID = Query.lastInsertId().toInt();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::UpdateEvent(EventData& Data)
{
	if (!EventScriptOk(Data.Script)) return false;

	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"events "
		"SET "
			"name=:name, "
			"script=:script, "
			"active=:active "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":script", Data.Script);
	Query.bindValue(":active", Data.Active);

	if (Query.exec()) UpdateScriptTasks();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::DeleteEvent(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"DELETE FROM "
			"events "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec()) UpdateScriptTasks();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

EventData AppCore::GetEvent(int ID)
{
	if (ID < 0) return EventData();

	QSqlQuery Query(Database);
	EventData Data;

	Query.prepare(
		"SELECT "
			"ID, name, script, active "
		"FROM "
			"events "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec() && Query.next())
	{
		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Script = Query.value(2).toString();
		Data.Active = Query.value(3).toBool();
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Data;
}

QList<EventData> AppCore::GetEvents(void)
{
	QSqlQuery Query(Database);
	QList<EventData> List;

	Query.prepare(
		"SELECT "
			"ID, name, script, active "
		"FROM "
			"events");

	if (Query.exec()) while (Query.next())
	{
		EventData Data;

		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Script = Query.value(2).toString();
		Data.Active = Query.value(3).toBool();

		List.append(Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return List;
}

void AppCore::ConnectVariable(const QString &Var, const boost::function<void (double)>& Callback)
{
	const KLString ID = Var.toStdString().c_str();

	if (Script.Variables.Exists(ID)) Script.Variables[ID].SetCallback(Callback);
}

void AppCore::DisconnectVariable(const QString& Var)
{
	const KLString ID = Var.toStdString().c_str();

	if (Script.Variables.Exists(ID)) Script.Variables[ID].SetCallback(boost::function<void (double)>());
}

AppCore* AppCore::getInstance(void)
{
	return THIS;
}

AVRBridge* AppCore::getDevice(void)
{
	return Device;
}

QString AppCore::getError(void)
{
	return LastError;
}
