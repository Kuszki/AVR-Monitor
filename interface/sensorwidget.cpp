/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Sensor widget for AVR-Monitor                                          *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż            l.drozdz@o2.pl   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "sensorwidget.hpp"
#include "ui_sensorwidget.h"

SensorWidget::SensorWidget(QWidget *Parent)
: QWidget(Parent), ui(new Ui::SensorWidget)
{
	ui->setupUi(this);

	Dialog = new SensorDialog(-1, this);

	for (const auto& Data: AppCore::getInstance()->GetSensors()) AddSensor(Data);

	connect(Dialog, &SensorDialog::onDialogAccept, this, &SensorWidget::AddSensor);

	connect(ui->addButton, &QPushButton::clicked, [this] (void) -> void { Dialog->open(); });
}

SensorWidget::~SensorWidget(void)
{
	delete ui;
}

void SensorWidget::AddSensor(const SensorData& Data)
{
	SensorEntry* Entry = new SensorEntry(Data, this);

	ui->sensorsLayout->addWidget(Entry);
}
