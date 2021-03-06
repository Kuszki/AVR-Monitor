/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  AVR-Terminal - terminal for AVR-Monitor project                        *
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

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QTranslator>

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "avrterminal.hpp"
#include "avruploader.hpp"
#include "avrdownloader.hpp"
#include "avrserver.hpp"

QObject* APP_object = nullptr;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	signal(SIGHUP, [] (int) -> void { if (APP_object && !qobject_cast<AVRServer*>(APP_object)) delete APP_object; });
	signal(SIGINT, [] (int) -> void { if (APP_object) delete APP_object; });
	signal(SIGTERM, [] (int) -> void { if (APP_object) delete APP_object; });
	signal(SIGABRT, [] (int) -> void { if (APP_object) delete APP_object; });

	a.setApplicationName("AVR-Terminal");
	a.setOrganizationName("Łukasz \"Kuszki\" Dróżdż");
	a.setOrganizationDomain("https://github.com/Kuszki/AVR-Monitor");
	a.setApplicationVersion("1.0");

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
				   QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);

	QTranslator baseTranslator;
	baseTranslator.load("qtbase_" + QLocale::system().name(),
					QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&baseTranslator);

	QTranslator appTranslator;
	appTranslator.load("avrmonitor_terminal_" + QLocale::system().name());
	a.installTranslator(&appTranslator);

	QTranslator bridgeTranslator;
	bridgeTranslator.load("avrmonitor_bridge_" + QLocale::system().name());
	a.installTranslator(&bridgeTranslator);

	QTranslator kllibsTranslator;
	kllibsTranslator.load("kllibs_" + QLocale::system().name());
	a.installTranslator(&kllibsTranslator);

	QCommandLineParser Parser;

	Parser.setApplicationDescription(qApp->translate("main",

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
	Parser.addPositionalArgument(qApp->translate("main", "device"), qApp->translate("main", "device to open"));

	// main options
	QCommandLineOption stdTerminal(QStringList() << "t" << "terminal", qApp->translate("main", "Work as casual two-side terminal."));
	QCommandLineOption stdUpload(QStringList() << "u" << "upload", qApp->translate("main", "Upload script from file into device."), qApp->translate("main", "script"));
	QCommandLineOption stdDownload(QStringList() << "d" << "download", qApp->translate("main", "Download script from device into file."), qApp->translate("main", "script"));

	// server options
	QCommandLineOption sqlServer(QStringList() << "s" << "server", qApp->translate("main", "Cooperate with SQL server."), qApp->translate("main", "address"));

	Parser.addOption(stdTerminal);
	Parser.addOption(stdUpload);
	Parser.addOption(stdDownload);
	Parser.addOption(sqlServer);

	Parser.process(a);

	if (Parser.isSet(stdTerminal))
	{
		APP_object = new AVRTerminal(Parser.positionalArguments().takeFirst());
	}
	else if (Parser.isSet(stdUpload))
	{
		APP_object = new AVRUploader(Parser.positionalArguments().takeFirst(), Parser.value(stdUpload));
	}
	else if (Parser.isSet(stdDownload))
	{
		APP_object = new AVRDownloader(Parser.positionalArguments().takeFirst(), Parser.value(stdDownload));
	}
	else if (Parser.isSet(sqlServer))
	{
		APP_object = new AVRServer(Parser.positionalArguments().takeFirst(), Parser.value(sqlServer));
	}
	else Parser.showHelp(-1);

	if (APP_object) a.connect(APP_object, &QObject::destroyed, [] (void) { QCoreApplication::exit(); });
	else return -1;

	return a.exec();
}
