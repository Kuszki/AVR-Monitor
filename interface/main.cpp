/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Main file of AVR-Monitor                                               *
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

#include <QLibraryInfo>
#include <QApplication>
#include <QTranslator>

#include "mainwindow.hpp"
#include "appcore.hpp"
#include <QDebug>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationName("AVR-Interface");
	a.setOrganizationName("Łukasz \"Kuszki\" Dróżdż");
	a.setOrganizationDomain("https://github.com/Kuszki/AVR-Monitor");
	a.setApplicationVersion("1.0");

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&qtTranslator);

	QTranslator baseTranslator;
	baseTranslator.load("qtbase_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&baseTranslator);

	QTranslator appTranslator;
	appTranslator.load("avrmonitor_interface_" + QLocale::system().name());
	a.installTranslator(&appTranslator);

	QTranslator bridgeTranslator;
	bridgeTranslator.load("avrmonitor_bridge_" + QLocale::system().name());
	a.installTranslator(&bridgeTranslator);

	QTranslator kllibsTranslator;
	kllibsTranslator.load("kllibs_" + QLocale::system().name());
	a.installTranslator(&kllibsTranslator);

	AppCore app;
	MainWindow w;

	w.show();

	return a.exec();
}
