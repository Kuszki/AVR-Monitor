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

#include "avruploader.hpp"

AVRUploader::AVRUploader(const QString& Port, const QString& Code)
: QObject(QCoreApplication::instance()), Script(Code), Cout(stdout)
{
	Device = new AVRBridge(this);
	Timeout = new QTimer(this);

	connect(Device, &AVRBridge::onError, this, &AVRUploader::HandleError);
	connect(Device, &AVRBridge::onConnectionUpdate, this, &AVRUploader::HandleConnect);

	connect(Timeout, &QTimer::timeout, this, &AVRUploader::HandleTimeout);

	Timeout->setSingleShot(true);
	Timeout->setInterval(3000);
	Timeout->start();

	Device->Connect(Port);

	Cout << tr("Waiting 3 seconds for connection...\n") << flush;
}

AVRUploader::~AVRUploader(void) {}

void AVRUploader::HandleError(const QString& Error)
{
	Cout << Error << "\n" << flush;
}

void AVRUploader::HandleConnect(bool Connected)
{
	if (Connected)
	{
		Cout << tr("Connected after %n milisecond(s)\n", 0,
				 (Timeout->interval() - Timeout->remainingTime()))
			<< flush;

		Timeout->stop();

		QFile Inputfile(Script);
		Inputfile.open(QFile::ReadOnly);

		if (Inputfile.isOpen())
		{
			Cout << tr("Uploading script from file...\n") << flush;

			Device->WriteMasterScript(Inputfile.readAll());

			Cout << tr("Upload complete\n") << flush;
		}
		else emit HandleError(tr("Can't open provided file"));

		QCoreApplication::exit(0);
	}
	else
	{
		HandleError(tr("Device disconnected"));

		QCoreApplication::exit(-1);
	}
}

void AVRUploader::HandleTimeout(void)
{
	HandleError(tr("Cannot connect to device - connection timeout"));

	QCoreApplication::exit(-1);
}
