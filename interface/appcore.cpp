/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Main AppCore module for AVR-Monitor                                    *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
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
QRegExpValidator* AppCore::Validator = nullptr;
QString AppCore::LastError = QString();

AppCore::AppCore(void)
: QObject(nullptr), Adc(&Params), Script(&Adc), Worker(&Script, &Tasks, &Locker)
{
	const QString DB = QSettings("AVR-Monitor").value("database", "database.sqlite").toString();

	if (THIS) qFatal("Core object duplicated"); else THIS = this;
	if (!QFile::exists(DB)) QFile::copy(":/data/database", DB);

	Worker.moveToThread(&Thread);
	Script.moveToThread(&Thread);

	Watchdog.setInterval(1000);
	Interval.setInterval(1000);

	Database = QSqlDatabase::addDatabase("QSQLITE");
	Device = new AVRBridge(&Script.Variables, this);
	Validator = new QRegExpValidator(QRegExp("\\b[a-zA-Z_]+[a-zA-Z0-9_]*\\b"), this);

	Database.setDatabaseName(DB);
	Database.open();

	Thread.start();

	Script.Bindings.Add("get", [this] (KLList<double>& Vars) -> double
	{
		Device->UpdateSensorVariables(); return 0;
	});

	Script.Bindings.Add("put", [this] (KLList<double>& Vars) -> double
	{
		unsigned char Values = Device->Variables()["SHRD"].ToInt();

		if (Vars.Size() == 1)
		{
			Values = int(Vars[0]);
		}
		else if (Vars.Size() == 2)
		{
			if (bool(Vars[1])) Values |= (1 << int(Vars[0]));
			else Values &= ~(1 << int(Vars[0]));
		}
		else if (Vars.Size() == 8)
		{
			for (int i = Values = 0; i < 8; i++) Values = Values | (int(Vars[i]) << i);
		}

		Device->WriteShiftValues(Values); return 0;
	});

	Script.Bindings.Add("pga", [this] (KLList<double>& Vars) -> double
	{
		if (Vars.Size() == 2) Device->WriteGainSettings(int(Vars[0]), int(Vars[1])); return 0;
	});

	Script.Bindings.Add("out", [this] (KLList<double>& Vars) -> double
	{
		if (Vars.Size() == 1) Device->WriteShiftStatus(bool(Vars[0])); return 0;
	});

	Script.Bindings.Add("sys", [this] (KLList<double>& Vars) -> double
	{
		Device->UpdateSystemVariables(); return 0;
	});

	Script.Bindings.Add("slp", [this] (KLList<double>& Vars) -> double
	{
		return 0;
	});

	Script.Bindings.Add("spi", [this] (KLList<double>& Vars) -> double
	{
		QList<unsigned char> Data;

		for (const auto& Var: Vars) Data.append(unsigned(Var));

		Device->WriteSpiValues(Data); return 0;
	});

	Script.Bindings.Add("dev", [this] (KLList<double>& Vars) -> double
	{
		return 0;
	});

	connect(Device, &AVRBridge::onConnectionUpdate, [this] (bool Active) -> void
	{
		if (!Active) { emit onEmergencyStop(); Interval.stop(); }
	});

	connect(Device, &AVRBridge::onMasterStatusUpdate, [this] (bool Master) -> void
	{
		if (Master && Interval.isActive()) { emit onEmergencyStop(); Interval.stop(); }
	});

	connect(Device, &AVRBridge::onError, [this] (void) -> void
	{
		if (Interval.isActive()) emit onEmergencyStop(); Interval.stop();
	});

	connect(&Interval, &QTimer::timeout, [this] (void) -> void
	{
		if (Done) Device->UpdateSensorVariables(); Done = false;
	});

	connect(&Watchdog, &QTimer::timeout, this, &AppCore::TerminateEvaluations);

	connect(Device, &AVRBridge::onSensorValuesUpdate, this, &AppCore::PerformTasks);

	connect(&Worker, &ScriptWorker::onEvaluationComplete, this, &AppCore::CompleteEvaluations);

	connect(this, &AppCore::onEvaluationRequest, &Worker, &ScriptWorker::PerformEvaluations);

	UpdateScriptTasks();
}

AppCore::~AppCore(void)
{
	Thread.exit();
	Thread.wait();

	THIS = nullptr;
}

void AppCore::UpdateVariables(const KLVariables &Vars)
{
	for (const auto& Var: Vars) if (Script.Variables.Exists(Var.ID)) Script.Variables[Var.ID] = Var.Value.ToNumber();
}

void AppCore::PerformTasks(const KLVariables& Vars)
{
	if (Interval.isActive() && !Device->Variables()["WORK"].ToBool())
	{
		Adc = Vars; Watchdog.start();

		emit onEvaluationRequest();
	}
}

void AppCore::CompleteEvaluations(void)
{
	Done = true; Watchdog.stop();

	emit onValuesUpdate(Script.Variables);
}

void AppCore::TerminateEvaluations(void)
{
	if (!Watchdog.isActive()) return;

	Script.Terminate();
	Interval.stop();

	Done = true;

	emit onEmergencyStop();
	emit onScriptTermination();
}

void AppCore::UpdateInterval(double Time)
{
	Interval.setInterval(Time * 1000);
}

void AppCore::UpdateVariable(const QString& Label, double Value)
{
	if (Params.Exists(Label.toStdString().c_str()))
	{
		Params[Label.toStdString().c_str()] = Value;
	}
}

void AppCore::UpdateStatus(bool Active)
{
	if (Active && Device->Variables()["WORK"].ToBool()) emit onEmergencyStop();
	else
	{
		if (Active)
		{
			for (auto& Var: Script.Variables) Var.Value = 0;

			Interval.start();
		}
		else Interval.stop();
	}

	Done = true;
}

void AppCore::SynchronizeDevice(void)
{
	QString Code = Initscript; UpdateDefaultOutputs();

	for (const auto& Task: Tasks) Code.append('\n').append(Task);

	for (const auto& Var: Params) Code.replace(QRegExp(QString("\\b%1\\b").arg((const char*) Var.ID)), QString::number(Var.Value.ToNumber()));

	Device->WriteSleepValue(Interval.interval() / 1000.0);
	Device->WriteDefaultShift(Values);
	Device->WriteMasterScript(Code);
}

bool AppCore::SensorScriptOk(const QString& Code, const QString& Label)
{
	KLScriptbinding Tester(&Device->Variables());

	if (!Label.isEmpty()) Tester.Variables.Add(Label.toStdString().c_str());

	if (Tester.Validate(Code)) return true;
	else
	{
		LastError = tr("Invalid script at line %1 (%2)").arg(Tester.GetLine()).arg(Tester.GetMessage());
	}

	return false;
}

bool AppCore::EventScriptOk(const QString& Code)
{
	KLScriptbinding Tester(&Device->Variables());

	Tester.Bindings.Add("get", [] (KLList<double>&) -> double {return 0;});
	Tester.Bindings.Add("put", [] (KLList<double>&) -> double {return 0;});
	Tester.Bindings.Add("out", [] (KLList<double>&) -> double {return 0;});
	Tester.Bindings.Add("pga", [] (KLList<double>&) -> double {return 0;});
	Tester.Bindings.Add("slp", [] (KLList<double>&) -> double {return 0;});
	Tester.Bindings.Add("spi", [] (KLList<double>&) -> double {return 0;});

	if (Tester.Validate(Code)) return true;
	else
	{
		LastError = tr("Invalid script at line %1 (%2)").arg(Tester.GetLine()).arg(Tester.GetMessage());
	}

	return false;
}

void AppCore::UpdateScriptTasks(void)
{
	Locker.lock();

	QSqlQuery Query(Database);

	Script.Variables.Clean();
	Params.Clean();

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
		Script.Variables.Add(Query.value(0).toString().toStdString().c_str());

		Tasks.append(KLScriptbinding::Optimize(
					   Query.value(1).toString()
					   .remove(QRegExp("\\s*#[^\n]*\\s*"))
					   .replace(QRegExp("return\\s+([^;]+);"),
							  QString("set %1 \\1;exit;").
							  arg(Query.value(0).toString()))
					   .remove(QRegExp("exit\\s*;\\s*$"))));
	}
	else
	{
		LastError = Query.lastError().text();
	}

	Query.prepare(
		"SELECT "
			"label, "
			"init "
		"FROM "
			"sliders "
		"WHERE "
			"active>0");

	if (Query.exec()) while (Query.next())
	{
		Params.Add(Query.value(0).toString().toStdString().c_str(),
				 KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	if (!Tasks.isEmpty())
	{
		for (const auto& Var: Script.Variables) Initscript.append(Var.ID).append(',');

		Initscript[Initscript.size() - 1] = ';';
		Initscript = QString("export %1").arg(Initscript);
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

	Locker.unlock();

	emit onScriptUpdate();
}

void AppCore::UpdateDefaultOutputs(void)
{
	QSqlQuery Query(Database); Values = 0;

	Query.prepare(
		"SELECT "
			"output "
		"FROM "
			"devices "
		"WHERE "
			"active>0");

	if (Query.exec()) while (Query.next())
	{
		Values |= (1 << Query.value(0).toInt());
	}
	else
	{
		LastError = Query.lastError().text();
	}
}

bool AppCore::AddSensor(SensorData& Data)
{
	if (!SensorScriptOk(Data.Script, Data.Label)) return false;

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

	if (Query.exec()) UpdateScriptTasks();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	Data.ID = Query.lastInsertId().toInt();

	return true;
}

bool AppCore::UpdateSensor(SensorData& Data)
{
	if (Data.Active) if (!SensorScriptOk(Data.Script, Data.Label)) return false;

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

	emit onSensorUpdate();

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

	Query.prepare(
		"DELETE FROM "
			"plots "
		"WHERE "
			"SENSOR_ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec()) emit onSensorUpdate();
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

QMap<int, SensorData> AppCore::GetSensors(void)
{
	QSqlQuery Query(Database);
	QMap<int, SensorData> List;

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

		List.insert(Data.ID, Data);
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

	if (Query.exec())
	{
		Data.ID = Query.lastInsertId().toInt(); UpdateScriptTasks();
	}
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::UpdateEvent(EventData& Data)
{
	if (Data.Active) if (!EventScriptOk(Data.Script)) return false;

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

QMap<int, EventData> AppCore::GetEvents(void)
{
	QSqlQuery Query(Database);
	QMap<int, EventData> List;

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

		List.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return List;
}


bool AppCore::AddDevice(DeviceData& Data)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"INSERT INTO "
			"devices (name, output, active) "
		"VALUES "
			"(:name, :output, :active)");

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":output", Data.Output);
	Query.bindValue(":active", Data.Active);

	if (Query.exec())
	{
		Data.ID = Query.lastInsertId().toInt(); UpdateDefaultOutputs();
	}
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::UpdateDevice(DeviceData& Data)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"devices "
		"SET "
			"name=:name, "
			"output=:output, "
			"active=:active "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":output", Data.Output);
	Query.bindValue(":active", Data.Active);

	if (Query.exec()) UpdateDefaultOutputs();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::DeleteDevice(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"DELETE FROM "
			"devices "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec()) UpdateDefaultOutputs();
	else
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

DeviceData AppCore::GetDevice(int ID)
{
	if (ID < 0) return DeviceData();

	QSqlQuery Query(Database);
	DeviceData Data;

	Query.prepare(
		"SELECT "
			"ID, name, output, active "
		"FROM "
			"devices "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec() && Query.next())
	{
		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Output = Query.value(2).toInt();
		Data.Active = Query.value(3).toBool();
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Data;
}

QMap<int, DeviceData> AppCore::GetDevices(void)
{
	QSqlQuery Query(Database);
	QMap<int, DeviceData> List;

	Query.prepare(
		"SELECT "
			"ID, name, output, active "
		"FROM "
			"devices");

	if (Query.exec()) while (Query.next())
	{
		DeviceData Data;

		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Output = Query.value(2).toInt();
		Data.Active = Query.value(3).toBool();

		List.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return List;
}

bool AppCore::AddAxis(AxisData& Data)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"INSERT INTO "
			"axes (name, style, min, max, label, active) "
		"VALUES "
			"(:name, :style, :min, :max, :label, :active)");

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":style", Data.Style);
	Query.bindValue(":min", Data.Min);
	Query.bindValue(":max", Data.Max);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":active", Data.Active);

	if (Query.exec())
	{
		Data.ID = Query.lastInsertId().toInt(); return true;
	}
	else
	{
		LastError = Query.lastError().text(); return false;
	}
}

bool AppCore::UpdateAxis(AxisData& Data)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"axes "
		"SET "
			"name=:name, "
			"style=:style, "
			"min=:min, "
			"max=:max, "
			"label=:label, "
			"active=:active "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":style", Data.Style);
	Query.bindValue(":min", Data.Min);
	Query.bindValue(":max", Data.Max);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":active", Data.Active);

	if (!Query.exec())
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::DeleteAxis(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"DELETE FROM "
			"axes "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec())
	{
		LastError = Query.lastError().text(); return false;
	}

	Query.prepare(
		"DELETE FROM "
			"plots "
		"WHERE "
			"AXIS_ID=:ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec())
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

AxisData AppCore::GetAxis(int ID)
{
	if (ID < 0) return AxisData();

	QSqlQuery Query(Database);
	AxisData Data;

	Query.prepare(
		"SELECT "
			"ID, name, style, min, max, label, active "
		"FROM "
			"axes "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec() && Query.next())
	{
		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Style = Query.value(2).toInt();
		Data.Min = Query.value(3).toDouble();
		Data.Max = Query.value(4).toDouble();
		Data.Label = Query.value(5).toBool();
		Data.Active = Query.value(6).toBool();
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Data;
}

QMap<int, AxisData> AppCore::GetAxes(void)
{
	QSqlQuery Query(Database);
	QMap<int, AxisData> List;

	Query.prepare(
		"SELECT "
			"ID, name, style, min, max, label, active "
		"FROM "
			"axes");

	if (Query.exec()) while (Query.next())
	{
		AxisData Data;

		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Style = Query.value(2).toInt();
		Data.Min = Query.value(3).toDouble();
		Data.Max = Query.value(4).toDouble();
		Data.Label = Query.value(5).toBool();
		Data.Active = Query.value(6).toBool();

		List.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return List;
}

bool AppCore::AddPlot(PlotData& Data)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"INSERT INTO "
			"plots (AXIS_ID, SENSOR_ID, active) "
		"VALUES "
			"(:AXIS_ID, :SENSOR_ID, :active)");

	Query.bindValue(":AXIS_ID", Data.AXIS_ID);
	Query.bindValue(":SENSOR_ID", Data.SENSOR_ID);
	Query.bindValue(":active", Data.Active);

	if (Query.exec())
	{
		Data.ID = Query.lastInsertId().toInt(); return true;
	}
	else
	{
		LastError = Query.lastError().text(); return false;
	}
}

bool AppCore::UpdatePlot(PlotData& Data)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"plots "
		"SET "
			"active=:active "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":active", Data.Active);

	if (!Query.exec())
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::DeletePlot(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"DELETE FROM "
			"plots "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec())
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

PlotData AppCore::GetPlot(int ID)
{
	if (ID < 0) return PlotData();

	QSqlQuery Query(Database);
	PlotData Data;

	Query.prepare(
		"SELECT "
			"plots.ID, plots.AXIS_ID, plots.SENSOR_ID, plots.active, "
			"sensors.name, sensors.label, "
			"axes.name "
		"FROM "
			"plots, sensors, axes "
		"WHERE "
			"plots.ID=:ID AND plots.AXIS_ID=axes.ID AND plots.SENSOR_ID=sensors.ID");

	Query.bindValue(":ID", ID);

	if (Query.exec() && Query.next())
	{
		Data.ID = Query.value(0).toInt();
		Data.AXIS_ID = Query.value(1).toInt();
		Data.SENSOR_ID = Query.value(2).toInt();
		Data.Active = Query.value(3).toInt();
		Data.Varname = Query.value(4).toString();
		Data.Varlabel = Query.value(5).toString();
		Data.Axisname = Query.value(6).toString();
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Data;
}

QMap<int, PlotData> AppCore::GetPlots(void)
{
	QSqlQuery Query(Database);
	QMap<int, PlotData> List;

	Query.prepare(
		"SELECT "
			"plots.ID, plots.AXIS_ID, plots.SENSOR_ID, plots.active, "
			"sensors.name, sensors.label, "
			"axes.name "
		"FROM "
			"plots, sensors, axes "
		"WHERE "
			"plots.AXIS_ID=axes.ID AND plots.SENSOR_ID=sensors.ID");

	if (Query.exec()) while (Query.next())
	{
		PlotData Data;

		Data.ID = Query.value(0).toInt();
		Data.AXIS_ID = Query.value(1).toInt();
		Data.SENSOR_ID = Query.value(2).toInt();
		Data.Active = Query.value(3).toInt();
		Data.Varname = Query.value(4).toString();
		Data.Varlabel = Query.value(5).toString();
		Data.Axisname = Query.value(6).toString();

		List.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return List;
}

bool AppCore::AddSlider(SliderData& Data)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"INSERT INTO "
			"sliders (name, label, min, max, init, steps, active) "
		"VALUES "
			"(:name, :label, :min, :max, :init, :steps, :active)");

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":min", Data.Min);
	Query.bindValue(":max", Data.Max);
	Query.bindValue(":init", Data.Init);
	Query.bindValue(":steps", Data.Steps);
	Query.bindValue(":active", Data.Active);

	if (Query.exec())
	{
		Data.ID = Query.lastInsertId().toInt(); return true;
	}
	else
	{
		LastError = Query.lastError().text(); return false;
	}
}

bool AppCore::UpdateSlider(SliderData& Data)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"sliders "
		"SET "
			"name=:name, "
			"label=:label, "
			"min=:min, "
			"max=:max, "
			"init=:init, "
			"steps=:steps, "
			"active=:active "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":min", Data.Min);
	Query.bindValue(":max", Data.Max);
	Query.bindValue(":init", Data.Init);
	Query.bindValue(":steps", Data.Steps);
	Query.bindValue(":active", Data.Active);

	if (!Query.exec())
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

bool AppCore::DeleteSlider(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"DELETE FROM "
			"sliders "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec())
	{
		LastError = Query.lastError().text(); return false;
	}

	return true;
}

SliderData AppCore::GetSlider(int ID)
{
	if (ID < 0) return SliderData();

	QSqlQuery Query(Database);
	SliderData Data;

	Query.prepare(
		"SELECT "
			"ID, name, label, min, max, init, steps, active "
		"FROM "
			"sliders "
		"WHERE "
			"ID=:ID");

	Query.bindValue(":ID", ID);

	if (Query.exec() && Query.next())
	{
		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Label = Query.value(2).toString();
		Data.Min = Query.value(3).toDouble();
		Data.Max = Query.value(4).toDouble();
		Data.Init = Query.value(5).toDouble();
		Data.Steps = Query.value(6).toInt();
		Data.Active = Query.value(7).toBool();
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Data;
}

QMap<int, SliderData> AppCore::GetSliders(void)
{
	QSqlQuery Query(Database);
	QMap<int, SliderData> List;

	Query.prepare(
		"SELECT "
			"ID, name, label, min, max, init, steps, active "
		"FROM "
			"sliders");

	if (Query.exec()) while (Query.next())
	{
		SliderData Data;

		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Label = Query.value(2).toString();
		Data.Min = Query.value(3).toDouble();
		Data.Max = Query.value(4).toDouble();
		Data.Init = Query.value(5).toDouble();
		Data.Steps = Query.value(6).toInt();
		Data.Active = Query.value(7).toBool();

		List.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return List;
}

void AppCore::ConnectVariable(const QString& Var, const boost::function<void (double)>& Callback)
{
	const KLString ID = Var.toStdString().c_str();

	if (Script.Variables.Exists(ID)) Script.Variables[ID].SetCallback(Callback);
}

void AppCore::DisconnectVariable(const QString& Var)
{
	const KLString ID = Var.toStdString().c_str();

	if (Script.Variables.Exists(ID)) Script.Variables[ID].SetCallback(boost::function<void (double)>());
}

QString AppCore::getValidation(const QString& Code)
{
	getInstance()->Script.Validate(Code);

	return getInstance()->Script.GetMessage();
}

QRegExpValidator* AppCore::getValidator(void)
{
	return Validator;
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
