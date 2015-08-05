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
			systemwidget.cpp \
			aboutdialog.cpp \
			settingsdialog.cpp \
			adcentry.cpp \
			terminalwidget.cpp \
			helperwidget.cpp \
			helperentry.cpp

HEADERS	+=	mainwindow.hpp \
			shiftwidget.hpp \
			gainwidget.hpp \
			adcwidget.hpp \
			systemwidget.hpp \
			aboutdialog.hpp \
			settingsdialog.hpp \
			adcentry.hpp \
			terminalwidget.hpp \
			helperwidget.hpp \
			helperentry.hpp

FORMS	+=	mainwindow.ui \
			shiftwidget.ui \
			gainwidget.ui \
			adcwidget.ui \
			systemwidget.ui \
			aboutdialog.ui \
			settingsdialog.ui \
			adcentry.ui \
			terminalwidget.ui \
			helperentry.ui

RESOURCES	+=	resources.qrc

INCLUDEPATH	+=	/home/kuszki/Projekty/AVR-Monitor/bridge
INCLUDEPATH	+=	/home/kuszki/Projekty/KLLibs
INCLUDEPATH	+=	/home/kuszki/Projekty/QCustomPlot
