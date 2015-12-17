#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T19:58:41
#
#-------------------------------------------------

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
			scriptworker.cpp

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
			scriptworker.hpp

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
			plotwidget.ui

RESOURCES	+=	resources.qrc

TRANSLATIONS	+=	avrmonitor_interface_pl.ts

INCLUDEPATH	+=	$$PWD/../bridge
INCLUDEPATH	+=	$$PWD/../../KLLibs
INCLUDEPATH	+=	$$PWD/../../QCustomPlot

QMAKE_CXXFLAGS	+=	-s -march=native -std=c++14
