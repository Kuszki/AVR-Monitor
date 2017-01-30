/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Sensor settings dialog for AVR-Monitor                                 *
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

#include "sensordialog.hpp"
#include "ui_sensordialog.h"

SensorDialog::SensorDialog(int Sensor, QWidget* Parent)
: QDialog(Parent), ui(new Ui::SensorDialog), ID(Sensor)
{
	ui->setupUi(this);

	ui->Label->setValidator(AppCore::getValidator());
}

SensorDialog::~SensorDialog(void)
{
	delete ui;
}

void SensorDialog::open(void)
{
	SensorData Data = AppCore::getInstance()->GetSensor(ID);

	if (Data.ID != -1)
	{
		ui->Name->setText(Data.Name);
		ui->Label->setText(Data.Label);
		ui->Unit->setText(Data.Unit);
		ui->Script->document()->setPlainText(Data.Script);
		ui->Active->setChecked(Data.Active);
	}
	else
	{
		ui->Name->clear();
		ui->Label->clear();
		ui->Unit->clear();
		ui->Script->document()->clear();
		ui->Active->setChecked(false);
	}

	QDialog::open();
}

void SensorDialog::TextBoxEdited(void)
{
	ui->saveButton->setEnabled(
				!ui->Name->text().isEmpty() &&
				!ui->Label->text().isEmpty());
}

void SensorDialog::accept(void)
{
	SensorData Data; bool OK = false;

	Data.ID = ID;
	Data.Name = ui->Name->text();
	Data.Label = ui->Label->text();
	Data.Unit = ui->Unit->text();
	Data.Script = ui->Script->document()->toPlainText();
	Data.Active = ui->Active->isChecked();

	if (ID != -1)
	{
		OK = AppCore::getInstance()->UpdateSensor(Data);
	}
	else
	{
		OK = AppCore::getInstance()->AddSensor(Data);
	}

	if (OK)
	{
		emit onDialogAccept(Data); QDialog::accept();
	}
	else
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't insert data into database - %1").arg(AppCore::getError()));
	}
}
