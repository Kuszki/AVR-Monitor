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
#include <QDebug>
SystemWidget::SystemWidget(QWidget *Parent)
: QWidget(Parent), ui(new Ui::SystemWidget)
{
	ui->setupUi(this);
}

SystemWidget::~SystemWidget(void)
{
	delete ui;
}

void SystemWidget::RefreshRamClick(void)
{
	emit onRamRefreshRequest();
}

void SystemWidget::UpdateLink(bool Online)
{
	ui->Online->setText(Online ? tr("Yes") : tr("No"));
}

void SystemWidget::UpdateStatus(bool Master)
{
	ui->Master->setText(Master ? tr("Master") : tr("Slave"));
}

void SystemWidget::UpdateShiftValues(unsigned char Values)
{
	ui->ShiftValue->setText(QString("%1").arg(Values, 8, 2, QChar('0')));
}

void SystemWidget::UpdateShiftStatus(bool Active)
{
	ui->ShiftActive->setText(Active ? tr("Enabled") : tr("Disabled"));
}

void SystemWidget::UpdateGainValue(unsigned char ID, unsigned char Gain)
{
	switch (ID)
	{
		case 0:
			ui->Gain_0->setText(QString("x%1").arg(Gain));
		break;
		case 1:
			ui->Gain_1->setText(QString("x%1").arg(Gain));
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
