/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Terminal for AVR-Monitor device                                        *
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

#include "avrserver.hpp"

AVRServer::AVRServer(const QString& Port, const QString& Server)
: QObject(nullptr)
{
	const QStringList SYS = { "LINE", "WORK", "SHRD", "SHRE", "SLPT", "FRAM" };

	Database = QSqlDatabase::addDatabase("QMYSQL");
	Device = new AVRBridge(&Sensors, this);
	Timeout = new QTimer(this);

	Database.setHostName(Server);
	Database.setUserName("avr_monitor");
	Database.setPassword("kuszkiavrmonitor");
	Database.setDatabaseName("avr_monitor");

	if (!Database.open()) qFatal("Can't open database");

	connect(Timeout, &QTimer::timeout, this, &AVRServer::HandleTimeout);

	connect(Device, &AVRBridge::onMessageReceive, this, &AVRServer::HandleExec);
	connect(Device, &AVRBridge::onConnectionUpdate, this, &AVRServer::HandleConnect);
	connect(Device, &AVRBridge::onSensorValuesUpdate, this, &AVRServer::HandleConverters);

	connect(Device, &AVRBridge::onConnectionUpdate, boost::bind(&AVRServer::HandleSystem, this, SYS[0], _1));
	connect(Device, &AVRBridge::onMasterStatusUpdate, boost::bind(&AVRServer::HandleSystem, this, SYS[1], _1));
	connect(Device, &AVRBridge::onShiftValuesUpdate, boost::bind(&AVRServer::HandleSystem, this, SYS[2], _1));
	connect(Device, &AVRBridge::onShiftStatusUpdate, boost::bind(&AVRServer::HandleSystem, this, SYS[3], _1));
	connect(Device, &AVRBridge::onSleepValueUpdate, boost::bind(&AVRServer::HandleSystem, this, SYS[4], _1));
	connect(Device, &AVRBridge::onFreeRamUpdate, boost::bind(&AVRServer::HandleSystem, this, SYS[5], _1));

	connect(Device, &AVRBridge::onGainSettingsUpdate, [this, &SYS] (char ID, char Gain) -> void
	{
		if (ID == 0) HandleSystem("PGA0", Gain);
		else HandleSystem("PGA1", Gain);
	});

	Timeout->setSingleShot(true);
	Timeout->setInterval(3000);
	Timeout->start();

	Device->Connect(Port);
}

AVRServer::~AVRServer(void)
{
	if (Device->IsConnected()) Device->WriteMasterStatus(false);
}

void AVRServer::HandleConnect(bool Connected)
{
	if (Connected) Timeout->stop();
	else qFatal("Device disconnected");

	Device->WriteMasterStatus(true);
}

void AVRServer::HandleTimeout(void)
{
	qFatal("Connection timeout");
}

void AVRServer::HandleExec(const QString& Script)
{
	QRegExp Regexp("\\bexport\\s+(.*)\\s*\\b;"); Regexp.setMinimal(true);

	if (Regexp.indexIn(Script) != -1)
	{
		const KLString Label = Regexp.capturedTexts().at(1).toStdString().c_str();
		const QString qLabel = Regexp.capturedTexts().at(1);

		if (Device->Variables().Exists(Label) && !Device->Variables()[Label].GetCallback())
		{
			Device->Variables()[Label].SetCallback(boost::bind(&AVRServer::HandleCallback, this, qLabel, _1));

			HandleCallback(qLabel, Device->Variables()[Label].ToNumber());
		}
	}
}

void AVRServer::HandleCallback(const QString& Name, double Value)
{
	if (!Database.isOpen()) return;
	else QSqlQuery(Database).exec(QString(
			"INSERT INTO "
				"`variables` (`name`, `value`) "
			"VALUES "
				"('%1', %2) "
			"ON DUPLICATE KEY UPDATE "
				"`value`=%2")
		.arg(Name).arg(Value));
}

void AVRServer::HandleSystem(const QString& Name, int Value)
{
	if (!Database.isOpen()) return;
	else QSqlQuery(Database).exec(QString(
			"INSERT INTO "
				"`system` (`name`, `value`) "
			"VALUES "
				"('%1', %2) "
			"ON DUPLICATE KEY UPDATE "
				"`value`=%2")
		.arg(Name).arg(Value));
}

void AVRServer::HandleConverters(const KLVariables& Vars)
{
	if (!Database.isOpen()) return;

	int i = 0; for (const auto& Var: Vars) QSqlQuery(Database).exec(QString(
			"INSERT INTO "
				"`converters` (`ID`, `value`) "
			"VALUES "
				"('%1', %2) "
			"ON DUPLICATE KEY UPDATE "
				"`value`=%2")
		.arg(++i).arg(Var.Value.ToNumber()));
}
