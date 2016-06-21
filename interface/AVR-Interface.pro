# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                                                                         *
#  User interface for AVR-Monitor project                                 *
#  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
#                                                                         *
#  This program is free software: you can redistribute it and/or modify   *
#  it under the terms of the GNU General Public License as published by   *
#  the  Free Software Foundation, either  version 3 of the  License, or   *
#  (at your option) any later version.                                    *
#                                                                         *
#  This  program  is  distributed  in the hope  that it will be useful,   *
#  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
#  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
#  GNU General Public License for more details.                           *
#                                                                         *
#  You should have  received a copy  of the  GNU General Public License   *
#  along with this program. If not, see http://www.gnu.org/licenses/.     *
#                                                                         *
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

QT		+=	core gui widgets serialport printsupport sql opengl

TARGET	=	avrinterface
TEMPLATE	=	app

CONFIG	+=	c++14

LIBS		+=	-L$$PWD/../../build-KLLibs -lkllibs
LIBS		+=	-L$$PWD/../../build-AVR-Bridge -lavrbridge
LIBS		+=	-L$$PWD/../../build-QCustomPlot -lqcustomplot

DEFINES	+=	USING_BOOST USING_QT

SOURCES	+= 	main.cpp \
			appcore.cpp \
			mainwindow.cpp \
			shiftwidget.cpp \
			gainwidget.cpp \
			adcwidget.cpp \
			systemwidget.cpp \
			aboutdialog.cpp \
			adcentry.cpp \
			terminalwidget.cpp \
			helperwidget.cpp \
			helperentry.cpp \
			sensorwidget.cpp \
			sensorentry.cpp \
			sensordialog.cpp \
			eventdialog.cpp \
			evententry.cpp \
			eventwidget.cpp \
			devicedialog.cpp \
			deviceentry.cpp \
			devicewidget.cpp \
			plotdialog.cpp \
			axisdialog.cpp \
			plotwidget.cpp \
			scriptworker.cpp \
			sliderentry.cpp \
			sliderdialog.cpp \
			sliderwidget.cpp

HEADERS	+=	common.hpp \
			appcore.hpp \
			mainwindow.hpp \
			shiftwidget.hpp \
			gainwidget.hpp \
			adcwidget.hpp \
			systemwidget.hpp \
			aboutdialog.hpp \
			adcentry.hpp \
			terminalwidget.hpp \
			helperwidget.hpp \
			helperentry.hpp \
			sensorwidget.hpp \
			sensorentry.hpp \
			sensordialog.hpp \
			eventdialog.hpp \
			evententry.hpp \
			eventwidget.hpp \
			devicedialog.hpp \
			deviceentry.hpp \
			devicewidget.hpp \
			plotdialog.hpp \
			axisdialog.hpp \
			plotwidget.hpp \
			scriptworker.hpp \
			sliderentry.hpp \
			sliderdialog.hpp \
			sliderwidget.hpp

FORMS	+=	mainwindow.ui \
			shiftwidget.ui \
			gainwidget.ui \
			adcwidget.ui \
			systemwidget.ui \
			aboutdialog.ui \
			adcentry.ui \
			terminalwidget.ui \
			helperentry.ui \
			sensorwidget.ui \
			sensorentry.ui \
			sensordialog.ui \
			eventdialog.ui \
			evententry.ui \
			eventwidget.ui \
			devicedialog.ui \
			deviceentry.ui \
			devicewidget.ui \
			plotdialog.ui \
			axisdialog.ui \
			plotwidget.ui \
			sliderentry.ui \
			sliderdialog.ui \
			sliderwidget.ui

RESOURCES	+=	resources.qrc

TRANSLATIONS	+=	avrmonitor_interface_pl.ts

INCLUDEPATH	+=	$$PWD/../bridge
INCLUDEPATH	+=	$$PWD/../../KLLibs
INCLUDEPATH	+=	$$PWD/../../QCustomPlot

QMAKE_CXXFLAGS	+=	-s -march=native -std=c++14
