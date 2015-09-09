#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T19:58:41
#
#-------------------------------------------------

QT		+=	core gui widgets serialport printsupport sql

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
			adcentry.cpp \
			terminalwidget.cpp \
			helperwidget.cpp \
			helperentry.cpp \
			sensorwidget.cpp \
			sensorentry.cpp \
			sensordialog.cpp \
			appcore.cpp \
			logwidget.cpp \
			eventdialog.cpp \
			evententry.cpp \
			eventwidget.cpp \
			devicedialog.cpp \
			deviceentry.cpp \
			devicewidget.cpp \
    plotdialog.cpp \
    axisdialog.cpp

HEADERS	+=	mainwindow.hpp \
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
			common.hpp \
			appcore.hpp \
			logwidget.hpp \
			eventdialog.hpp \
			evententry.hpp \
			eventwidget.hpp \
			devicedialog.hpp \
			deviceentry.hpp \
			devicewidget.hpp \
    plotdialog.hpp \
    axisdialog.hpp

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
			logwidget.ui \
			eventdialog.ui \
			evententry.ui \
			eventwidget.ui \
			devicedialog.ui \
			deviceentry.ui \
			devicewidget.ui \
    plotdialog.ui \
    axisdialog.ui

RESOURCES	+=	resources.qrc

INCLUDEPATH	+=	/home/kuszki/Projekty/AVR-Monitor/bridge
INCLUDEPATH	+=	/home/kuszki/Projekty/KLLibs
INCLUDEPATH	+=	/home/kuszki/Projekty/QCustomPlot
