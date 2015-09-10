#-------------------------------------------------
#
# Project created by QtCreator 2015-07-29T00:52:47
#
#-------------------------------------------------

QT		+=	serialport widgets
QT		-=	gui

TARGET	=	avrbridge
TEMPLATE	=	lib

CONFIG	+=	c++11 shared

LIBS		+=	-L/home/kuszki/Projekty/build-KLLibs -lkllibs

DEFINES	+=	AVRBRIDGE_LIBRARY USING_BOOST USING_QT

SOURCES	+=	avrbridge.cpp

HEADERS	+=	libbuild.hpp \
			avrbridge.hpp

INCLUDEPATH	+=	/home/kuszki/Projekty/KLLibs

QMAKE_CXXFLAGS	+=	-s -fomit-frame-pointer -march=native

unix {

    target.path = /usr/lib
    INSTALLS += target

}