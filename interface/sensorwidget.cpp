/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Sensor widget for AVR-Monitor                                          *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
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

	ui->sensorsLayout->setAlignment(Qt::AlignTop);
	ui->rightSpacer->changeSize(ui->addButton->sizeHint().width(), 0);
	ui->Average->setValue(QSettings("AVR-Monitor").value("sensoravg", 1).toInt());

	Dialog = new SensorDialog(-1, this);

	for (const auto& Data: AppCore::getInstance()->GetSensors()) AddSensor(Data);

	connect(Dialog, &SensorDialog::onDialogAccept, this, &SensorWidget::AddSensor);
	connect(ui->addButton, &QPushButton::clicked, Dialog, &QDialog::open);
}

SensorWidget::~SensorWidget(void)
{
	QSettings("AVR-Monitor").setValue("sensoravg", ui->Average->value());

	delete ui;
}

void SensorWidget::SetTitleWidget(TitleWidget* Widget)
{
	ui->gridLayout->removeItem(ui->leftSpacer);
	ui->gridLayout->removeItem(ui->rightSpacer);

	Widget->addRightWidget(ui->Average);
	Widget->addRightSpacer(ui->rightSpacer);
	Widget->addRightWidget(ui->addButton);

	delete ui->leftSpacer;
}

void SensorWidget::AddSensor(const SensorData& Data)
{
	SensorEntry* Entry = new SensorEntry(Data, this);

	ui->sensorsLayout->addWidget(Entry); RefreshSize();

	connect(Entry, &SensorEntry::onSensorUpdate, this, &SensorWidget::RefreshSize);
	connect(this, &SensorWidget::onSamplesCountUpdate, Entry, &SensorEntry::UpdateSamples);

	Entry->UpdateSamples(ui->Average->value());
}

void SensorWidget::AverageSpinChanged(int Value)
{
	ui->Average->setSuffix(tr(" sample(s)", 0, Value));

	emit onSamplesCountUpdate(Value);
}

void SensorWidget::RefreshSize(void)
{
	const int Items = ui->sensorsLayout->minimumSize().width();
	const int Scroll = ui->scrollArea->verticalScrollBar()->width();

	ui->scrollArea->setMinimumWidth(Items + Scroll);
}


