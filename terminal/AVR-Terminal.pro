#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T01:00:33
#
#-------------------------------------------------

QT		+=	serialport widgets core
QT		-=	gui

TARGET	=	avrterminal
CONFIG  	+=	c++11 console
CONFIG	-=	app_bundle

TEMPLATE	=	app

LIBS		+=	-L/home/kuszki/Projekty/build-KLLibs -lkllibs
LIBS		+=	-L/home/kuszki/Projekty/build-AVR-Bridge -lavrbridge

DEFINES	+=	AVRBRIDGE_LIBRARY USING_BOOST USING_QT

INCLUDEPATH	+=	/home/kuszki/Projekty/KLLibs
INCLUDEPATH	+=	/home/kuszki/Projekty/AVR-Monitor/bridge

SOURCES	+=	main.cpp \
			avrterminal.cpp \
			terminalreader.cpp \
			avruploader.cpp \
			avrdownloader.cpp

HEADERS	+=	avrterminal.hpp \
			terminalreader.hpp \
			avruploader.hpp \
			avrdownloader.hpp

QMAKE_CXXFLAGS	+=	-s -fomit-frame-pointer -march=native
