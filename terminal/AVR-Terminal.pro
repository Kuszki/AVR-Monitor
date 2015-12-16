#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T01:00:33
#
#-------------------------------------------------

QT		+=	serialport widgets core sql
QT		-=	gui

TARGET	=	avrterminal
CONFIG  	+=	c++14 console
CONFIG	-=	app_bundle

TEMPLATE	=	app

LIBS		+=	-L$$PWD/../../build-KLLibs -lkllibs
LIBS		+=	-L$$PWD/../../build-AVR-Bridge -lavrbridge

DEFINES	+=	AVRBRIDGE_LIBRARY USING_BOOST USING_QT

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

QMAKE_CXXFLAGS	+=	-s -fomit-frame-pointer -march=native
