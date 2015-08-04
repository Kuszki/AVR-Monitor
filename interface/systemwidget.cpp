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

#include "systemwidget.hpp"
#include "ui_systemwidget.h"

SystemWidget::SystemWidget(QWidget *Parent)
: QWidget(Parent), ui(new Ui::SystemWidget)
{
	ui->setupUi(this);
}

SystemWidget::~SystemWidget(void)
{
	delete ui;
}

void SystemWidget::UpdateLink(bool Online)
{
	ui->Online->setChecked(Online);
}

void SystemWidget::UpdateStatus(bool Master)
{
	ui->Master->setChecked(Master);
}

void SystemWidget::UpdateShiftValues(unsigned char Values)
{
	ui->ShiftValue->setText(QString("0b%1").arg(Values, 0, 2));
}

void SystemWidget::UpdateShiftStatus(bool Active)
{
	ui->ShiftActive->setCheckable(Active);
}

void SystemWidget::UpdateGainValue(unsigned char ID, unsigned char Gain)
{
	switch (ID)
	{
		case 0:
			ui->Gain_0->setText(QString::number(Gain));
		break;
		case 1:
			ui->Gain_1->setText(QString::number(Gain));
		break;
	}
}

void SystemWidget::UpdateFreeRam(unsigned Value)
{
	ui->FreeRam->setValue(Value);
}

void SystemWidget::UpdateInterval(double Value)
{
	ui->Interval->setText(QString("%1 s").arg(Value, 0, '.', 1));
}
