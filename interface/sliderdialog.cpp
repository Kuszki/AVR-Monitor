/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Slider dialog implementation for AVR-Interface                         *
 *  Copyright (C) 2016  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
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

#include "sliderdialog.hpp"
#include "ui_sliderdialog.h"

SliderDialog::SliderDialog(int Slider, QWidget* Parent)
: QDialog(Parent), ui(new Ui::SliderDialog), ID(Slider)
{
	ui->setupUi(this);

	ui->Label->setValidator(AppCore::getValidator());
}

SliderDialog::~SliderDialog(void)
{
	delete ui;
}

void SliderDialog::open(void)
{
	SliderData Data = AppCore::getInstance()->GetSlider(ID);

	ui->Name->setText(Data.Name);
	ui->Label->setText(Data.Label);
	ui->Min->setValue(Data.Min);
	ui->Max->setValue(Data.Max);
	ui->Init->setValue(Data.Init);
	ui->Steps->setValue(Data.Steps);
	ui->Active->setChecked(Data.Active);

	QDialog::open();
}

void SliderDialog::MaxSpinChange(double Value)
{
	ui->Min->setMaximum(Value);
	ui->Init->setMaximum(Value);
}

void SliderDialog::MinSpinChange(double Value)
{
	ui->Max->setMinimum(Value);
	ui->Init->setMinimum(Value);
}

void SliderDialog::StepValueChange(void)
{
	const int Steps = ui->Steps->value();

	const double Max = ui->Max->value();
	const double Min = ui->Min->value();
	const double Step = (Max - Min) / (Steps - 1);

	ui->Steps->setSuffix(QString(tr(" step(s) by %1", 0, Steps)
					 .arg(QString::number(Step, 'f', 3))));
}

void SliderDialog::accept(void)
{
	SliderData Data; bool OK = false;

	Data.ID = ID;
	Data.Name = ui->Name->text();
	Data.Label = ui->Label->text();
	Data.Min = ui->Min->value();
	Data.Max = ui->Max->value();
	Data.Init = ui->Init->value();
	Data.Steps = ui->Steps->value();
	Data.Active = ui->Active->isChecked();

	if (ID != -1)
	{
		OK = AppCore::getInstance()->UpdateSlider(Data);
	}
	else
	{
		OK = AppCore::getInstance()->AddSlider(Data);
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
