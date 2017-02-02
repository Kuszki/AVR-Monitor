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

	Dialog = new SensorDialog(-1, this);

	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Sensors");

	ui->sensorsLayout->setAlignment(Qt::AlignTop);
	ui->Average->setValue(Settings.value("interval", 1).toInt());

	Settings.endGroup();

	for (const auto& Data: AppCore::getInstance()->GetSensors()) AddSensor(Data);

	connect(Dialog, &SensorDialog::onDialogAccept, this, &SensorWidget::AddSensor);
	connect(ui->addButton, &QPushButton::clicked, Dialog, &SensorDialog::open);
}

SensorWidget::~SensorWidget(void)
{
	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Sensors");
	Settings.setValue("interval", ui->Average->value());
	Settings.endGroup();

	delete ui;
}

void SensorWidget::SetTitleWidget(TitleWidget* Widget)
{
	ui->horizontalSpacer->changeSize(ui->addButton->sizeHint().width(), 0);
	ui->gridLayout->removeItem(ui->horizontalSpacer);

	Widget->addRightWidget(ui->Average);
	Widget->addRightSpacer(ui->horizontalSpacer);
	Widget->addRightWidget(ui->addButton);
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

void SensorWidget::ReloadSensors(void)
{
	while (ui->sensorsLayout->count()) ui->sensorsLayout->takeAt(0)->widget()->deleteLater();
	for (const auto& Data: AppCore::getInstance()->GetSensors()) AddSensor(Data);
}
