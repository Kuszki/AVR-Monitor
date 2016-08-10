# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                                                                         *
#  Terminal interface for AVR-Monitor project                             *
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

QT		+=	serialport widgets core sql
QT		-=	gui

TARGET	=	avrterminal
CONFIG  	+=	c++14 console
CONFIG	-=	app_bundle

TEMPLATE	=	app

LIBS		+=	-L$$PWD/../../build-KLLibs -lkllibs
LIBS		+=	-L$$PWD/../../build-AVR-Bridge -lavrbridge

DEFINES	+=	USING_BOOST USING_QT

SOURCES	+=	main.cpp \
			avrterminal.cpp \
			terminalreader.cpp \
			avruploader.cpp \
			avrdownloader.cpp \
			avrserver.cpp

HEADERS	+=	avrterminal.hpp \
			terminalreader.hpp \
			avruploader.hpp \
			avrdownloader.hpp \
			avrserver.hpp

TRANSLATIONS	+=	avrmonitor_terminal_pl.ts

INCLUDEPATH	+=	$$PWD/../bridge
INCLUDEPATH	+=	$$PWD/../../KLLibs

QMAKE_CXXFLAGS	+=	-s -march=native -std=c++14
