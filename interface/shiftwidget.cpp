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

#include "shiftwidget.hpp"
#include "ui_shiftwidget.h"

ShiftWidget::ShiftWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::ShiftWidget)
{
	ui->setupUi(this);

	for (int i = 0; i < 8; i++)
	{
		Pins[i] = new QCheckBox(tr("Pin %1").arg(i), this);

		ui->layoutPins->addWidget(Pins[i]);

		connect(Pins[i], &QCheckBox::clicked, this, &ShiftWidget::OutputChanged);
	}
}

ShiftWidget::~ShiftWidget(void)
{
	delete ui;
}

void ShiftWidget::EnableAllClicked(void)
{
	emit onShiftChanged(255);
}

void ShiftWidget::DisableAllClicked(void)
{
	emit onShiftChanged(0);
}

void ShiftWidget::OutputChanged(bool)
{
	unsigned char Values = 0;

	for (int i = 0; i < 8; i++)
	{
		Values |= Pins[i]->isChecked() << i;
	}

	emit onShiftChanged(Values);
}

void ShiftWidget::EnabledChanged(bool Enabled)
{
	emit onEnabledChanged(Enabled);
}

void ShiftWidget::UpdateShiftValues(unsigned char Values)
{
	for (unsigned char i = 0; i < 8; ++i)
	{
		Pins[i]->setChecked(Values & (1 << i));
	}
}

void ShiftWidget::UpdateShiftStatus(bool Enabled)
{
	ui->shiftActive->setChecked(Enabled);
}

void ShiftWidget::LayoutChanged(Qt::DockWidgetArea Area)
{
	switch (Area)
	{
		case Qt::LeftDockWidgetArea:
		case Qt::RightDockWidgetArea:
			ui->layoutPins->setDirection(QBoxLayout::TopToBottom);
			ui->layoutWidget->setDirection(QBoxLayout::TopToBottom);
		break;
		case Qt::TopDockWidgetArea:
		case Qt::BottomDockWidgetArea:
			ui->layoutPins->setDirection(QBoxLayout::LeftToRight);
			ui->layoutWidget->setDirection(QBoxLayout::LeftToRight);
		break;

		default: break;
	}
}
