# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                                                                         *
#  Bridge library for AVR-Monitor project                                 *
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

QT		+=	serialport widgets
QT		-=	gui

TARGET	=	avrbridge
TEMPLATE	=	lib

CONFIG	+=	c++14

LIBS		+=	-L$$PWD/../../build-KLLibs -lkllibs

DEFINES	+=	AVRBRIDGE_LIBRARY USING_BOOST USING_QT

SOURCES	+=	avrbridge.cpp

HEADERS	+=	libbuild.hpp \
			avrbridge.hpp

TRANSLATIONS	+=	avrmonitor_bridge_pl.ts

INCLUDEPATH	+=	$$PWD/../../KLLibs

QMAKE_CXXFLAGS	+=	-s -march=native -std=c++14

unix {

    target.path = /usr/lib
    INSTALLS += target

}
