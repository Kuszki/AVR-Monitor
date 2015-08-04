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

#include "avrterminal.hpp"

#include <QDebug>

AVRTerminal::AVRTerminal(const QString Port)
: QObject(QCoreApplication::instance()), Cin(stdin), Cout(stdout)
{
	Device = new AVRBridge(this);
	Worker = new Terminalreader(this);
	Timeout = new QTimer(this);

	connect(Device, SIGNAL(onError(const QString&)), SLOT(HandleError(const QString&)));
	connect(Device, SIGNAL(onConnectionUpdate(bool)), SLOT(HandleConnect(bool)));

	connect(Timeout, SIGNAL(timeout()), SLOT(HandleTimeout()));

	connect(Device, SIGNAL(onMessageReceive(const QString&)), SLOT(HandleMessage(const QString&)));
	connect(Worker, SIGNAL(onRead(const QString&)), SLOT(HandleCommand(const QString&)));

	Timeout->setSingleShot(true);
	Timeout->setInterval(3000);
	Timeout->start();

	Device->Connect(Port);

	Cout << tr("Waiting 3 seconds for connection...\n") << flush;
}

AVRTerminal::~AVRTerminal(void)
{
	Worker->terminate();
}

void AVRTerminal::HandleError(const QString& Error)
{
	Cout << Error << "\n" << flush;
}

void AVRTerminal::HandleMessage(const QString& Message)
{
	Cout << Message << "\n" << flush;
}

void AVRTerminal::HandleCommand(const QString& Message)
{
	if (!Message.isEmpty()) Device->Command(Message);
}

void AVRTerminal::HandleConnect(bool Connected)
{
	if (Connected)
	{
		Cout << tr("Connected after %n milisecond(s).\n", 0,
				 Timeout->interval() - Timeout->remainingTime() + 100)
			<< flush;

		Timeout->stop();
		Worker->start();
	}
	else
	{
		HandleError(tr("Device disconnected."));

		QCoreApplication::exit(-1);
	}
}

void AVRTerminal::HandleTimeout(void)
{
	HandleError(tr("Cannot connect to device - connection timeout."));

	QCoreApplication::exit(-1);
}
