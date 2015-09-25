/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  SQL server client for AVR-Monitor WEB interface                        *
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

#include "avrserver.hpp"

AVRServer::AVRServer(const QString& Port, const QString& Server)
: QObject(QCoreApplication::instance())
{
	auto dbCallback = [this] (const QString& Name, double Value) -> void
	{
		QSqlQuery(Database).exec(QString(
			"UPDATE "
				"`variables` "
			"SET "
				"`value`=%2 "
			"WHERE "
				"`name`='%1'")
		.arg(Name).arg(Value));
	};

	Database = QSqlDatabase::addDatabase("QMYSQL");
	Device = new AVRBridge(&Sensors, this);
	Timeout = new QTimer(this);

	Database.setHostName(Server);
	Database.setUserName("avr_monitor");
	Database.setPassword("kuszkiavrmonitor");
	Database.setDatabaseName("avr_monitor");

	if (!Database.open()) qFatal("Can't open database");

	QSqlQuery Query(Database);

	if (Query.exec("SELECT `name` FROM `variables`")) while (Query.next())
	{
		Sensors.Add(Query.value(0).toString().toStdString().c_str(), KLVariables::NUMBER, boost::bind<void>(dbCallback, Query.value(0).toString(), _1));
	}

	connect(Device, &AVRBridge::onConnectionUpdate, this, &AVRServer::HandleConnect);
	connect(Timeout, &QTimer::timeout, this, &AVRServer::HandleTimeout);

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
