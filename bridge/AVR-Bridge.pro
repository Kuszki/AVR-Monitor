#-------------------------------------------------
#
# Project created by QtCreator 2015-07-29T00:52:47
#
#-------------------------------------------------

QT		+=	serialport widgets
QT		-=	gui

TARGET	=	avrbridge
TEMPLATE	=	lib

CONFIG	+=	c++14 shared

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
