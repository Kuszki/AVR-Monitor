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

AVRTerminal::AVRTerminal(const QString Port, const QString Script, bool Upload)
: QObject(QCoreApplication::instance()), Cin(stdin), Cout(stdout), Input(Script), Uploading(Upload)
{
	Device = new AVRBridge(this);
	Worker = new Terminalreader(this);
	Timeout = new QTimer(this);

	connect(Device, SIGNAL(onError(const QString&)), SLOT(HandleError(const QString&)));
	connect(Device, SIGNAL(onMessageReceive(const QString&)), SLOT(HandleMessage(const QString&)));
	connect(Device, SIGNAL(onConnected()), SLOT(HandleConnect()));
	connect(Device, SIGNAL(onDisconnected()), SLOT(HandleDisconnect()));

	connect(Worker, SIGNAL(onRead(const QString&)), SLOT(HandleCommand(const QString&)));

	connect(Timeout, SIGNAL(timeout()), SLOT(HandleTimeout()));

	Timeout->setSingleShot(true);
	Timeout->setInterval(3000);
	Timeout->start();

	Device->Connect(Port);

	Cout << tr("Waiting 3 seconds for connection...\t") << flush;
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

	Cout << Message << "$ " << flush;
}

void AVRTerminal::HandleCommand(const QString& Message)
{
	if (Message.isEmpty()) Cout << "$ ";
	else Device->Command(Message);
}

void AVRTerminal::HandleConnect(void)
{
	Cout << tr("Connected after ~%n milisecond(s).", 0, (Timeout->interval() - Timeout->remainingTime() + 10)) << "\n" << flush;

	Timeout->stop();

	if (Uploading)
	{
		if (Input.isEmpty())
		{
			Device->WriteMasterScript(Cin.readLine() + '\n');
		}
	}
	else
	{
		if (Input.isEmpty()) Worker->start();
		else
		{
			Cout << tr("Wait for output and press Ctrl+c to exit.") << "\n" << flush;

			Device->Command(QFile(Input).readAll());
		}
	}
}

void AVRTerminal::HandleDisconnect(void)
{
	HandleError(tr("Device disconnected."));

	QCoreApplication::exit(-1);
}

void AVRTerminal::HandleTimeout(void)
{
	HandleError(tr("Cannot connect to device - connection timeout."));

	QCoreApplication::exit(-1);
}
