/***********************************************************************
 *
 *  AVR-Monitor main. cpp file
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 **********************************************************************/

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* Parent)
: QMainWindow(Parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setTabPosition(Qt::DockWidgetArea::TopDockWidgetArea |
				Qt::LeftDockWidgetArea |
				Qt::RightDockWidgetArea,
				QTabWidget::TabPosition::North);

	QSettings INI("layout.ini", QSettings::IniFormat);

	restoreState(INI.value("layout").toByteArray(), 1);

	connect(ui->shiftDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
		   ui->shiftWidget, SLOT(LayoutChanged(Qt::DockWidgetArea)));
}

MainWindow::~MainWindow(void)
{
	QSettings INI("layout.ini", QSettings::IniFormat);

	INI.setValue("layout", saveState(1));

	delete ui;
}
