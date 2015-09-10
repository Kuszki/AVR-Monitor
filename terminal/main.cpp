/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  AVR-Terminal - terminal for AVR-Monitor project                        *
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


#include <QCommandLineParser>
#include <QCoreApplication>

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "avrterminal.hpp"
#include "avruploader.hpp"
#include "avrdownloader.hpp"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	signal(SIGINT, [](int) -> void { QCoreApplication::quit(); });

	a.setApplicationName("AVR-Terminal");
	a.setOrganizationName("Łukasz \"Kuszki\" Dróżdż");
	a.setOrganizationDomain("https://github.com/Kuszki/AVR-Monitor");
	a.setApplicationVersion("1.0");

	QCommandLineParser Parser;

	Parser.setApplicationDescription(a.tr(

"\n\
AVR-Terminal - terminal application for AVR-Monitor platform.\n\
\n\
Copyright (C) 2015 Łukasz \"Kuszki\" Dróżdż - l.drozdz@o2.pl - https://github.com/Kuszki/AVR-Monitor\n\
\n\
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\
\n\
Before use be sure that device is not runing or connected to another application."

							   ));
	Parser.addHelpOption();
	Parser.addVersionOption();

	// device options
	Parser.addPositionalArgument(a.tr("device"), a.tr("device to open"));

	// main options
	QCommandLineOption tcpServer(QStringList() << "h" << "host", a.tr("Become a TCP server (device works as master)."));
	QCommandLineOption stdTerminal(QStringList() << "t" << "terminal", a.tr("Work as casual two-side terminal."));
	QCommandLineOption stdUpload(QStringList() << "u" << "upload", a.tr("Upload script from file into device."), a.tr("script"));
	QCommandLineOption stdDownload(QStringList() << "d" << "download", a.tr("Download script from device into file."), a.tr("script"));

	Parser.addOption(tcpServer);
	Parser.addOption(stdTerminal);
	Parser.addOption(stdUpload);
	Parser.addOption(stdDownload);

	Parser.process(a);

	if (Parser.isSet(tcpServer))
	{
	//	new AVRServer(Parser.positionalArguments().first(),
	//			    Parser.isSet(tcpPort) ? Parser.value(tcpPort) : QString());
	}
	else if (Parser.isSet(stdTerminal))
	{
		new AVRTerminal(Parser.positionalArguments().first());
	}
	else if (Parser.isSet(stdUpload))
	{
		new AVRUploader(Parser.positionalArguments().first(),
					 Parser.value(stdUpload));
	}
	else if (Parser.isSet(stdDownload))
	{
		new AVRDownloader(Parser.positionalArguments().first(),
					   Parser.value(stdDownload));
	}
	else Parser.showHelp(-1);

	return a.exec();
}