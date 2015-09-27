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

#include "avrdownloader.hpp"

AVRDownloader::AVRDownloader(const QString& Port, const QString& Out)
: QObject(nullptr), Script(Out), Cout(stdout)
{
	Device = new AVRBridge(nullptr, this);
	Timeout = new QTimer(this);

	connect(Device, &AVRBridge::onError, this, &AVRDownloader::HandleError);
	connect(Device, &AVRBridge::onConnectionUpdate, this, &AVRDownloader::HandleConnect);
	connect(Device, &AVRBridge::onMasterScriptReceive, this, &AVRDownloader::HandleMessage);

	connect(Timeout, &QTimer::timeout, this, &AVRDownloader::HandleTimeout);

	Timeout->setSingleShot(true);
	Timeout->setInterval(3000);
	Timeout->start();

	Device->Connect(Port);

	Cout << tr("Waiting 3 seconds for connection...\n") << flush;
}

AVRDownloader::~AVRDownloader(void) {}

void AVRDownloader::HandleError(const QString& Error)
{
	Cout << Error << "\n" << flush;
}

void AVRDownloader::HandleConnect(bool Connected)
{
	if (Connected)
	{
		Cout << tr("Connected after %n milisecond(s)\n", 0,
				 (Timeout->interval() - Timeout->remainingTime()))
			<< flush;

		Timeout->stop();

		Device->ReadMasterScript();
	}
	else
	{
		HandleError(tr("Device disconnected"));

		QCoreApplication::exit(-1);
	}
}

void AVRDownloader::HandleMessage(const QString& Message)
{
	Cout << tr("Master script downloaded\n\n") << flush;
	Cout << Message << flush;

	QFile File(Script);

	File.open(QFile::WriteOnly);
	File.write(Message.toUtf8());

	QCoreApplication::quit();
}

void AVRDownloader::HandleTimeout(void)
{
	HandleError(tr("Cannot connect to device - connection timeout"));

	QCoreApplication::exit(-1);
}

