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
: QObject(nullptr), SensorsVar(&SlidersVar), AdcVar(&SensorsVar), Script(&AdcVar), Locker(QMutex::Recursive), Worker(&Script, &Tasks, &Locker)
{
	if (THIS) qFatal("Core object duplicated"); else THIS = this;

	Database = QSqlDatabase::addDatabase("QSQLITE");
	Device = new AVRBridge(&Script.Variables, this);
	Validator = new QRegExpValidator(QRegExp("^[A-z]+[A-z0-9]*$"), this);

	const QString DB = QSettings("AVR-Monitor").value("database", DBNAME).toString();

	if (!QFile::exists(DB))
	{
		QSettings("AVR-Monitor").setValue("database", DBNAME);
		QFile::copy(":/data/database", DBNAME);
		QFile::setPermissions(DBNAME, QFileDevice::ReadOwner | QFileDevice::WriteOwner);

		Database.setDatabaseName(DBNAME);
	}
	else Database.setDatabaseName(DB);

	Worker.moveToThread(&Thread);
	Script.moveToThread(&Thread);

	Watchdog.setInterval(1000);
	Interval.setInterval(1000);

	Database.open();
	Thread.start();

	Script.Bindings.Add("get", [this] (KLList<double>& Vars) -> double
	{
		Device->UpdateSensorVariables(); return 0;
	});

	Script.Bindings.Add("put", [this] (KLList<double>& Vars) -> double
	{
		if (Vars.Size() == 1)
		{
			Device->WriteShiftValues(int(Vars[0]));
		}
		else if (Vars.Size() == 2)
		{
			Device->WriteShiftValue(int(Vars[0]), bool(Vars[1]));
		}
		else if (Vars.Size() == 8)
		{
			unsigned char Values = Device->Variables()["SHRD"].ToInt();
			for (int i = Values = 0; i < 8; i++) Values = Values | (int(Vars[i]) << i);

			Device->WriteShiftValues(Values);
		}

		return 0;
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

	Script.Bindings.Add("pwm", [this] (KLList<double>& Vars) -> double
	{
		if (Vars.Size() == 1) Device->WriteDutyValue(int(Vars[0])); return 0;
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

	connect(Device, &AVRBridge::onSensorValuesUpdate, this, &AppCore::PerformTasks);

	connect(&Watchdog, &QTimer::timeout, this, &AppCore::TerminateEvaluations);

	connect(&Worker, &ScriptWorker::onEvaluationComplete, this, &AppCore::CompleteEvaluations);
	connect(this, &AppCore::onEvaluationRequest, &Worker, &ScriptWorker::PerformEvaluations);

	connect(this, &AppCore::onSensorUpdate, this, &AppCore::UpdateScriptTasks);
	connect(this, &AppCore::onEventUpdate, this, &AppCore::UpdateScriptTasks);
	connect(this, &AppCore::onSliderUpdate, this, &AppCore::UpdateScriptTasks);

	GetSensors(); GetEvents(); GetDevices(); GetAxes(); GetPlots(); GetSliders();

	UpdateScriptTasks();
}

AppCore::~AppCore(void)
{
	Thread.exit();
	Thread.wait();

	THIS = nullptr;
}

bool AppCore::SaveDatabase(const QString& Path)
{
	const QString oldPath = Database.databaseName();

	if (Path.isEmpty()) return false;
	else if (Path == oldPath) return true;
	else
	{
		if (!QFile::copy(oldPath, Path)) return false;
		else
		{
			QSettings("AVR-Monitor").setValue("database", Path);

			Database.close();
			Database.setDatabaseName(Path);
			Database.open();
		}
	}

	return true;
}

bool AppCore::LoadDatabase(const QString& Path)
{
	const QString oldPath = Database.databaseName();

	if (Path.isEmpty()) return false;
	else if (Path == oldPath) return true;
	else
	{
		Database.close();
		Database.setDatabaseName(Path);

		if (Database.open())
		{
			QSettings("AVR-Monitor").setValue("database", Path);

			Sensors.clear(); GetSensors();
			Events.clear(); GetEvents();
			Devices.clear(); GetDevices();
			Axes.clear(); GetAxes();
			Plots.clear(); GetPlots();
			Sliders.clear(); GetSliders();

			UpdateScriptTasks();

			return true;
		}
		else
		{
			Database.setDatabaseName(oldPath);
			Database.open();

			return false;
		}
	}
}

bool AppCore::IsRefreshOk(void) const
{
	return Done;
}

void AppCore::UpdateVariables(const KLVariables& Vars)
{
	for (const auto& Var: Vars) if (AdcVar.Exists(Var.Index))
	{
		const double Current = Var.Value.ToNumber();
		auto& Variable = AdcVar[Var.Index];

		if (Samples > 1)
		{
			auto& Vector = History[Var.Index];
			double Sum(0.0), Divider(0.0);

			while (Vector.size() <= Samples) Vector.append(Current);
			while (Vector.size() > Samples) Vector.removeFirst();

			for (int i = 0; i < Vector.size(); ++i)
			{
				Sum += Vector[i] * Weights[i];
				Divider += Weights[i];
			}

			Variable = Sum / Divider;
		}
		else Variable = Current;
	}
	else
	{
		History.insert(Var.Index, QVector<double>());
		AdcVar.Add(Var.Index, Var.Value);
	}

	AdcVar["DT"] = Starttime.msecsTo(QTime::currentTime()) / 1000.0;
	Starttime = QTime::currentTime();
}

void AppCore::PerformTasks(const KLVariables& Vars)
{
	if (Interval.isActive() && !Device->Variables()["WORK"].ToBool())
	{
		UpdateVariables(Vars);
		Watchdog.start();

		emit onEvaluationRequest();
	}
}

void AppCore::CompleteEvaluations(void)
{
	Done = true; Watchdog.stop();

	emit onValuesUpdate(SensorsVar);
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

void AppCore::UpdateVariable(int ID, double Value)
{
	if (Sliders.contains(ID))
	{
		QMutexLocker AutoLocker(&Locker);

		SlidersVar[Sliders[ID].Label.toKls()] = Value;
	}
}

void AppCore::UpdateAverage(int Count)
{
	Samples = Count > 0 ? Count : 1;

	if (Samples > 0) UpdateWeight(Weight);
}

void AppCore::UpdateWeight(int Type)
{
	Type = (Type > -1 && Type < 3) ? Type : 0;

	if (Type != Weight || Samples != Weights.size())
	{
		QVector<double> Half;

		Weights.clear();
		Weights.reserve(Samples);
		Half.reserve(Samples / 2);

		switch (Weight = Type)
		{
			case 1:
			{
				for (int i = 0; i < Samples / 2 ; ++i)
				{
					Half.append(1.0 / (1 << ((Samples / 2) - i)));
				}
			}
			break;

			case 2:
			{
				double W = 1 - (2.0 / (Samples + 1));

				for (int i = 0; i < Samples / 2 ; ++i)
				{
					Half.append(1.0 - W); W *= W;
				}
			}
			break;

			default:
			{
				for (int i = 0; i < Samples / 2; ++i)
				{
					Half.append(1.0);
				}
			}
		}

		Weights.append(Half);
		std::reverse(Half.begin(), Half.end());
		Weights.append(Half);

		if (Samples % 2) Weights.insert(Samples / 2, 1.0);
	}
}

void AppCore::UpdateStatus(bool Active)
{
	QMutexLocker AutoLocker(&Locker);

	if (Active && Device->Variables()["WORK"].ToBool()) emit onEmergencyStop();
	else
	{
		if (Active)
		{
			for (auto& Var: Script.Variables) Var.Value = 0;

			History.clear();
			AdcVar.Clean();

			AdcVar.Add("DT", VREADONLY);
			AdcVar["DT"] = qQNaN();

			Interval.start();
		}
		else Interval.stop();
	}

	Done = true;
}

void AppCore::SynchronizeDevice(void)
{
	QMutexLocker AutoLocker(&Locker);

	unsigned char Values = 0;
	QString Code;

	for (const auto& Device : Devices) Values |= (1 << Device.Output);

	for (const auto& Sensor : Sensors) if (Sensor.Active)
	{
		Code.append(Sensor.Label).append(',');
	}

	if (Code.size())
	{
		Code[Code.size() - 1] = ';';
		Code = QString("export %1").arg(Code);
	}

	for (const auto& Task: Tasks) Code.append('\n').append(Task);

	for (const auto& Var: SlidersVar) Code.replace(QRegExp(QString("\\b%1\\b").arg((const char*) Var.Index)),
										  QString::number(Var.Value.ToNumber()));

	Code.replace(QRegExp("\\bDT\\b"), QString::number(Interval.interval() / 1000.0));

	Device->WriteSleepValue(Interval.interval() / 1000.0);
	Device->WriteDefaultShift(Values);
	Device->WriteMasterScript(Code);
}

bool AppCore::SensorScriptOk(const QString& Code, const QString& Label)
{
	static const char* AdcLabels[] = { "V0", "V1", "V2", "V3", "V4", "V5", "DT" };

	QMutexLocker AutoLocker(&Locker);
	KLScriptbinding Tester;

	for (const auto Label : AdcLabels) Tester.Variables.Add(Label, KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);

	for (const auto Sensor : Sensors) if (Sensor.Active) Tester.Variables.Add(Sensor.Label.toKls());
	for (const auto Slider : Sliders) if (Slider.Active) Tester.Variables.Add(Slider.Label.toKls());

	if (!Label.isEmpty()) Tester.Variables.Add(Label.toKls());

	if (Tester.Validate(Code)) return true;
	else
	{
		LastError = tr("Invalid script at line %1: %2").arg(Tester.GetLine()).arg(Tester.GetMessage());
	}

	return false;
}

bool AppCore::EventScriptOk(const QString& Code)
{
	static const char* AdcLabels[] = { "V0", "V1", "V2", "V3", "V4", "V5" };
	static const char* BindNames[] = { "get", "put", "out", "pga", "pwm", "slp", "spi" };

	static const auto NullBind = [] (KLList<double>&) -> double { return 0; };

	QMutexLocker AutoLocker(&Locker);
	KLScriptbinding Tester;

	for (const auto Bind : BindNames) Tester.Bindings.Add(Bind, NullBind);
	for (const auto Label : AdcLabels) Tester.Variables.Add(Label, KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);

	for (const auto Sensor : Sensors) if (Sensor.Active) Tester.Variables.Add(Sensor.Label.toKls());
	for (const auto Slider : Sliders) if (Slider.Active) Tester.Variables.Add(Slider.Label.toKls());

	if (Tester.Validate(Code)) return true;
	else
	{
		LastError = tr("Invalid script at line %1: %2").arg(Tester.GetLine()).arg(Tester.GetMessage());
	}

	return false;
}

void AppCore::UpdateInvalidItems(void)
{
	QMutexLocker AutoLocker(&Locker);

	disconnect(this, &AppCore::onSensorUpdate, this, &AppCore::UpdateScriptTasks);
	disconnect(this, &AppCore::onEventUpdate, this, &AppCore::UpdateScriptTasks);

	bool OK; do
	{
		OK = true;

		for (auto& Sensor : Sensors) if (Sensor.Active && !SensorScriptOk(Sensor.Script))
		{
			Sensor.Active = false; DisableSensor(Sensor.ID); OK = false;
		}

		for (auto& Event : Events) if (Event.Active && !EventScriptOk(Event.Script))
		{
			Event.Active = false; DisableEvent(Event.ID); OK = false;
		}
	}
	while (!OK);

	connect(this, &AppCore::onSensorUpdate, this, &AppCore::UpdateScriptTasks);
	connect(this, &AppCore::onEventUpdate, this, &AppCore::UpdateScriptTasks);
}

void AppCore::UpdateScriptTasks(void)
{
	QMutexLocker AutoLocker(&Locker);

	Script.Variables.Clean();

	Tasks.clear();

	UpdateInvalidItems();

	for (const auto& Sensor : Sensors) if (Sensor.Active)
	{
		Tasks.append(KLScriptbinding::Optimize(QString(Sensor.Script)
					   .remove(QRegExp("\\s*#[^\n]*\\s*"))
					   .replace(QRegExp("return\\s+([^;]+);"),
							  QString("set %1 \\1;exit;").
							  arg(Sensor.Label))
					   .remove(QRegExp("exit\\s*;\\s*$"))));
	}

	for (const auto& Event : Events) if (Event.Active)
	{
		Tasks.append(KLScriptbinding::Optimize(Event.Script));
	}

	emit onScriptUpdate();
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

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Data.ID = Query.lastInsertId().toInt();

		Sensors.insert(Data.ID, Data);
		SensorsVar.Add(Data.Label.toKls());

		emit onSensorUpdate(Data.ID);
	}

	return true;
}

bool AppCore::UpdateSensor(SensorData& Data)
{
	if (!Sensors.contains(Data.ID)) EmitNotFound

	if (!SensorScriptOk(Data.Script, Data.Label)) return false;

	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"sensors "
		"SET "
			"name	= :name, "
			"label	= :label, "
			"unit	= :unit, "
			"script	= :script, "
			"active	= :active "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":unit", Data.Unit);
	Query.bindValue(":script", Data.Script);
	Query.bindValue(":active", Data.Active);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		SensorsVar.Rename(Sensors[Data.ID].Label.toKls(),
					   Data.Label.toKls());
		Sensors[Data.ID] = Data;
		SensorsVar[Data.Label.toKls()] = 0.0;

		for (auto& Plot : Plots) if (Plot.SENSOR_ID == Data.ID)
		{
			Plot.Varlabel = Data.Label;
			Plot.Varname = Data.Name;

			emit onPlotUpdate(Plot.ID);
		}

		emit onSensorUpdate(Data.ID);
	}

	return true;
}

bool AppCore::DeleteSensor(int ID)
{
	if (!Sensors.contains(ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare("DELETE FROM sensors WHERE ID = :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError

	Query.prepare("DELETE FROM plots WHERE SENSOR_ID = :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError

	QMutexLocker AutoLocker(&Locker);

	SensorsVar.Delete(Sensors[ID].Label.toKls());
	History.remove(Sensors[ID].Label.toKls());
	Sensors.remove(ID);

	for (const auto& Index : Plots.keys()) if (Plots[Index].SENSOR_ID == ID)
	{
		Plots.remove(Index); emit onPlotUpdate(Index);
	}

	emit onSensorUpdate(ID);

	return true;
}

bool AppCore::DisableSensor(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"sensors "
		"SET "
			"active	= 0 "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		SensorsVar[Sensors[ID].Label.toKls()] = 0;
		Sensors[ID].Active = false;

		emit onSensorUpdate(ID);
	}

	return true;
}

const SensorData& AppCore::GetSensor(int ID)
{
	static const SensorData SensorDummy = SensorData();

	if (Sensors.contains(ID)) return Sensors[ID];
	else return SensorDummy;
}

const QMap<int, SensorData>& AppCore::GetSensors(void)
{
	if (!Sensors.empty()) return Sensors;

	QSqlQuery Query(Database);

	Query.prepare(
		"SELECT "
			"ID, name, label, unit, script, active "
		"FROM "
			"sensors");

	QMutexLocker AutoLocker(&Locker);

	if (Query.exec()) while (Query.next())
	{
		SensorData Data;

		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Label = Query.value(2).toString();
		Data.Unit = Query.value(3).toString();
		Data.Script = Query.value(4).toString();
		Data.Active = Query.value(5).toBool();

		SensorsVar.Add(Data.Label.toKls());

		Sensors.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Sensors;
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

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Data.ID = Query.lastInsertId().toInt();

		Events.insert(Data.ID, Data);

		emit onEventUpdate(Data.ID);
	}

	return true;
}

bool AppCore::UpdateEvent(EventData& Data)
{
	if (!Events.contains(Data.ID)) EmitNotFound

	if (!EventScriptOk(Data.Script)) return false;

	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"events "
		"SET "
			"name	= :name, "
			"script	= :script, "
			"active	= :active "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":script", Data.Script);
	Query.bindValue(":active", Data.Active);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Events[Data.ID] = Data;

		emit onEventUpdate(Data.ID);
	}

	return true;
}

bool AppCore::DeleteEvent(int ID)
{
	if (!Events.contains(ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare("DELETE FROM events WHERE ID = :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError

	QMutexLocker AutoLocker(&Locker);

	Events.remove(ID);

	emit onEventUpdate(ID);

	return true;
}

bool AppCore::DisableEvent(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"events "
		"SET "
			"active	= 0 "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Events[ID].Active = false;

		emit onEventUpdate(ID);
	}

	return true;
}

const EventData& AppCore::GetEvent(int ID)
{
	static const EventData EventDummy = EventData();

	if (Events.contains(ID)) return Events[ID];
	else return EventDummy;
}

const QMap<int, EventData>& AppCore::GetEvents(void)
{
	if (!Events.empty()) return Events;

	QSqlQuery Query(Database);

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

		Events.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Events;
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

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Data.ID = Query.lastInsertId().toInt();

		Devices.insert(Data.ID, Data);

		emit onDeviceUpdate(Data.ID);
	}

	return true;
}

bool AppCore::UpdateDevice(DeviceData& Data)
{
	if (!Devices.contains(Data.ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"devices "
		"SET "
			"name	= :name, "
			"output	= :output, "
			"active	= :active "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":output", Data.Output);
	Query.bindValue(":active", Data.Active);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Devices[Data.ID] = Data;

		emit onDeviceUpdate(Data.ID);
	}

	return true;
}

bool AppCore::DeleteDevice(int ID)
{
	if (!Devices.contains(ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare("DELETE FROM devices WHERE ID = :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError

	QMutexLocker AutoLocker(&Locker);

	Devices.remove(ID);

	emit onDeviceUpdate(ID);

	return true;
}

bool AppCore::DisableDevice(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"devices "
		"SET "
			"active	= 0 "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Devices[ID].Active = false;

		emit onDeviceUpdate(ID);
	}

	return true;
}

const DeviceData& AppCore::GetDevice(int ID)
{
	static const DeviceData DeviceDummy = DeviceData();

	if (Devices.contains(ID)) return Devices[ID];
	else return DeviceDummy;
}

const QMap<int, DeviceData>& AppCore::GetDevices(void)
{
	if (!Devices.empty()) return Devices;

	QSqlQuery Query(Database);

	Query.prepare(
		"SELECT "
			"ID, name, output, active "
		"FROM "
			"devices");

	QMutexLocker AutoLocker(&Locker);

	if (Query.exec()) while (Query.next())
	{
		DeviceData Data;

		Data.ID = Query.value(0).toInt();
		Data.Name = Query.value(1).toString();
		Data.Output = Query.value(2).toInt();
		Data.Active = Query.value(3).toBool();

		Devices.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Devices;
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

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Data.ID = Query.lastInsertId().toInt();

		Axes.insert(Data.ID, Data);

		emit onAxisUpdate(Data.ID);
	}

	return true;
}

bool AppCore::UpdateAxis(AxisData& Data)
{
	if (!Axes.contains(Data.ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"axes "
		"SET "
			"name	= :name, "
			"style	= :style, "
			"min		= :min, "
			"max		= :max, "
			"label	= :label, "
			"active	= :active "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":style", Data.Style);
	Query.bindValue(":min", Data.Min);
	Query.bindValue(":max", Data.Max);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":active", Data.Active);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Axes[Data.ID] = Data;

		for (auto& Plot : Plots) if (Plot.AXIS_ID == Data.ID)
		{
			Plot.Axisname = Data.Name;

			emit onPlotUpdate(Plot.ID);
		}

		emit onAxisUpdate(Data.ID);
	}

	return true;
}

bool AppCore::DeleteAxis(int ID)
{
	if (!Axes.contains(ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare("DELETE FROM axes WHERE ID = :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError

	Query.prepare("DELETE FROM plots WHERE AXIS_ID = :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError

	QMutexLocker AutoLocker(&Locker);

	Axes.remove(ID);

	for (const auto& Index : Plots.keys()) if (Plots[Index].AXIS_ID == ID)
	{
		Plots.remove(Index); emit onPlotUpdate(Index);
	}

	emit onAxisUpdate(ID);

	return true;
}

bool AppCore::DisableAxis(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"axes "
		"SET "
			"active	= 0 "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Axes[ID].Active = false;

		emit onAxisUpdate(ID);
	}

	return true;
}

const AxisData& AppCore::GetAxis(int ID)
{
	static const AxisData AxisDummy = AxisData();

	if (Axes.contains(ID)) return Axes[ID];
	else return AxisDummy;
}

const QMap<int, AxisData>& AppCore::GetAxes(void)
{
	if (!Axes.empty()) return Axes;

	QSqlQuery Query(Database);
	QMap<int, AxisData> List;

	Query.prepare(
		"SELECT "
			"ID, name, style, min, max, label, active "
		"FROM "
			"axes");

	QMutexLocker AutoLocker(&Locker);

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

	return Axes = List;
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

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Data.ID = Query.lastInsertId().toInt();

		Data.Varname = Sensors[Data.SENSOR_ID].Name;
		Data.Varlabel = Sensors[Data.SENSOR_ID].Label;
		Data.Axisname = Axes[Data.AXIS_ID].Name;

		Plots.insert(Data.ID, Data);

		emit onPlotUpdate(Data.ID);
	}

	return true;
}

bool AppCore::UpdatePlot(PlotData& Data)
{
	if (!Plots.contains(Data.ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"plots "
		"SET "
			"active	= :active "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":active", Data.Active);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Plots[Data.ID] = Data;

		emit onPlotUpdate(Data.ID);
	}

	return true;
}

bool AppCore::DeletePlot(int ID)
{
	if (!Plots.contains(ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare("DELETE FROM plots WHERE ID=:ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError

	QMutexLocker AutoLocker(&Locker);

	Plots.remove(ID);

	emit onPlotUpdate(ID);

	return true;
}

bool AppCore::DisablePlot(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"plots "
		"SET "
			"active	= 0 "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Plots[ID].Active = false;

		emit onPlotUpdate(ID);
	}

	return true;
}

const PlotData& AppCore::GetPlot(int ID)
{
	static const PlotData PlotDummy = PlotData();

	if (Plots.contains(ID)) return Plots[ID];
	else return PlotDummy;
}

const QMap<int, PlotData>& AppCore::GetPlots(void)
{
	if (!Plots.empty()) return Plots;

	QSqlQuery Query(Database);

	Query.prepare(
		"SELECT "
			"plots.ID, plots.AXIS_ID, plots.SENSOR_ID, plots.active, "
			"sensors.name, sensors.label, "
			"axes.name "
		"FROM "
			"plots "
		"INNER JOIN "
			"sensors "
		"ON "
			"plots.SENSOR_ID = sensors.ID "
		"INNER JOIN "
			"axes "
		"ON "
			"plots.AXIS_ID = axes.ID");

	QMutexLocker AutoLocker(&Locker);

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

		Plots.insert(Data.ID, Data);
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Plots;
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

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		Data.ID = Query.lastInsertId().toInt();

		SlidersVar.Add(Data.Label.toKls(), VREADONLY);
		Sliders.insert(Data.ID, Data);
		SlidersVar[Data.Label.toKls()] = Data.Init;

		emit onSliderUpdate(Data.ID);
	}

	return true;
}

bool AppCore::UpdateSlider(SliderData& Data)
{
	if (!Sliders.contains(Data.ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"sliders "
		"SET "
			"name	= :name, "
			"label	= :label, "
			"min		= :min, "
			"max		= :max, "
			"init	= :init, "
			"steps	= :steps, "
			"active	= :active "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", Data.ID);

	Query.bindValue(":name", Data.Name);
	Query.bindValue(":label", Data.Label);
	Query.bindValue(":min", Data.Min);
	Query.bindValue(":max", Data.Max);
	Query.bindValue(":init", Data.Init);
	Query.bindValue(":steps", Data.Steps);
	Query.bindValue(":active", Data.Active);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		SlidersVar.Rename(Sliders[Data.ID].Label.toKls(),
					   Data.Label.toKls());
		Sliders[Data.ID] = Data;
		SlidersVar[Data.Label.toKls()] = Data.Init;

		emit onSliderUpdate(Data.ID);
	}

	return true;
}

bool AppCore::DeleteSlider(int ID)
{
	if (!Sliders.contains(ID)) EmitNotFound

	QSqlQuery Query(Database);

	Query.prepare("DELETE FROM sliders WHERE ID = :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		SlidersVar.Delete(Sliders[ID].Label.toKls());
		Sliders.remove(ID);
	}

	emit onSliderUpdate(ID);

	return true;
}

bool AppCore::DisableSlider(int ID)
{
	QSqlQuery Query(Database);

	Query.prepare(
		"UPDATE "
			"sliders "
		"SET "
			"active	= 0 "
		"WHERE "
			"ID		= :ID");

	Query.bindValue(":ID", ID);

	if (!Query.exec()) EmitError
	else
	{
		QMutexLocker AutoLocker(&Locker);

		SlidersVar[Sliders[ID].Label.toKls()] = Sliders[ID].Init;
		Sliders[ID].Active = false;

		emit onSliderUpdate(ID);
	}

	return true;
}

SliderData AppCore::GetSlider(int ID)
{
	static const SliderData SliderDummy = SliderData();

	if (Sliders.contains(ID)) return Sliders[ID];
	else return SliderDummy;
}

QMap<int, SliderData> AppCore::GetSliders(void)
{
	if (!Sliders.empty()) return Sliders;

	QSqlQuery Query(Database);

	Query.prepare(
		"SELECT "
			"ID, name, label, min, max, init, steps, active "
		"FROM "
			"sliders");

	QMutexLocker AutoLocker(&Locker);

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

		SlidersVar.Add(Data.Label.toKls(), VREADONLY);
		Sliders.insert(Data.ID, Data);
		SlidersVar[Data.Label.toKls()] = Data.Init;
	}
	else
	{
		LastError = Query.lastError().text();
	}

	return Sliders;
}

void AppCore::ConnectVariable(const QString& Var, const boost::function<void (double)>& Callback)
{
	const KLString ID = Var.toKls();

	if (SensorsVar.Exists(ID, true)) SensorsVar[ID].SetCallback(Callback);
}

void AppCore::DisconnectVariable(const QString& Var)
{
	const KLString ID = Var.toKls();

	if (SensorsVar.Exists(ID, true)) SensorsVar[ID].SetCallback(KLVariables::KLSCALLBACK());
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
