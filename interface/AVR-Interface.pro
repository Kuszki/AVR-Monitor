#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T19:58:41
#
#-------------------------------------------------

QT		+=	core gui widgets serialport printsupport

TARGET	=	AVR-Interface
TEMPLATE	=	app

CONFIG	+=	c++11

LIBS		+=	-L/home/kuszki/Projekty/build-KLLibs -lkllibs
LIBS		+=	-L/home/kuszki/Projekty/build-AVR-Bridge -lavrbridge
LIBS		+=	-L/home/kuszki/Projekty/build-QCustomPlot -lqcustomplot

DEFINES	+=	USING_BOOST USING_QT

SOURCES	+= 	main.cpp \
			mainwindow.cpp \
			shiftwidget.cpp \
			gainwidget.cpp \
			adcwidget.cpp \
			systemwidget.cpp

HEADERS	+=	mainwindow.hpp \
			shiftwidget.hpp \
			gainwidget.hpp \
			adcwidget.hpp \
			systemwidget.hpp

FORMS	+=	mainwindow.ui \
			shiftwidget.ui \
			gainwidget.ui \
			adcwidget.ui \
			systemwidget.ui

INCLUDEPATH	+=	/home/kuszki/Projekty/KLLibs
INCLUDEPATH	+=	/home/kuszki/Projekty/QCustomPlot
