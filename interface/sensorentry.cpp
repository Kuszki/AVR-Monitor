/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  {description}                                                          *
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

#include "sensorentry.hpp"
#include "ui_sensorentry.h"

SensorEntry::SensorEntry(const SensorData& Data, QWidget* Parent)
: QWidget(Parent), ui(new Ui::SensorEntry), ID(Data.ID)
{
	ui->setupUi(this); UpdateSensor(Data); ReconnectSensor();

	Dialog = new SensorDialog(ID, this);

	connect(Dialog, &SensorDialog::onDialogAccept, this, &SensorEntry::UpdateSensor);

	connect(AppCore::getInstance(), &AppCore::onScriptUpdate, this, &SensorEntry::ReconnectSensor);
}

SensorEntry::~SensorEntry(void)
{
	delete ui;
}

void SensorEntry::SettingsButtonClicked(void)
{
	Dialog->open();
}

void SensorEntry::DeleteButtonClicked(void)
{
	if (AppCore::getInstance()->DeleteSensor(ID)) deleteLater();
	else QMessageBox::warning(this, tr("Error"), tr("Can't delete sensor - %1").arg(AppCore::getError()));
}

void SensorEntry::UpdateSensor(const SensorData& Data)
{
	ui->Name->setText(Data.Name);
	ui->Name->setEnabled(Data.Active);

	ui->Value->display(0);
	ui->Value->setEnabled(Data.Active);

	ui->Unit->setText(Data.Unit);
	ui->Unit->setEnabled(Data.Active);
}

void SensorEntry::ReconnectSensor(void)
{
	const SensorData Data = AppCore::getInstance()->GetSensor(ID);

	if (Data.Active) AppCore::getInstance()->ConnectVariable(Data.Label, [this] (double Value) -> void
	{
		ui->Value->display(Value);
	});
}