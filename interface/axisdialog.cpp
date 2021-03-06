/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Axis settings dialog for AVR-Monitor                                   *
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

#include "axisdialog.hpp"
#include "ui_axisdialog.h"

AxisDialog::AxisDialog(int Axis, QWidget* Parent)
: QDialog(Parent), ui(new Ui::AxisDialog), ID(Axis)
{
	ui->setupUi(this);
}

AxisDialog::~AxisDialog(void)
{
	delete ui;
}

void AxisDialog::open(void)
{
	AxisData Data = AppCore::getInstance()->GetAxis(ID);

	if (Data.ID != -1)
	{
		ui->Name->setText(Data.Name);
		ui->Style->setCurrentIndex(Data.Style);
		ui->Min->setValue(Data.Min);
		ui->Max->setValue(Data.Max);
		ui->Label->setChecked(Data.Label);
		ui->Active->setChecked(Data.Active);
	}
	else
	{
		ui->Name->clear();
		ui->Style->setCurrentIndex(0);
		ui->Min->setValue(0);
		ui->Max->setValue(5);
		ui->Label->setChecked(false);
		ui->Active->setChecked(false);
	}

	QDialog::open();
}

void AxisDialog::MaxSpinChange(double Value)
{
	ui->Min->setMaximum(Value);
}

void AxisDialog::MinSpinChange(double Value)
{
	ui->Max->setMinimum(Value);
}

void AxisDialog::accept(void)
{
	AxisData Data; bool OK = false;

	Data.ID = ID;
	Data.Name = ui->Name->text();
	Data.Style = ui->Style->currentIndex();
	Data.Min = ui->Min->value();
	Data.Max = ui->Max->value();
	Data.Label = ui->Label->isChecked();
	Data.Active = ui->Active->isChecked();

	if (ID != -1)
	{
		OK = AppCore::getInstance()->UpdateAxis(Data);
	}
	else
	{
		OK = AppCore::getInstance()->AddAxis(Data);
	}

	if (OK)
	{
		QDialog::accept(); emit onDialogAccept(Data);
	}
	else
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't insert data into database - %1").arg(AppCore::getError()));
	}
}
