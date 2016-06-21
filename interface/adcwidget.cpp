/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  ADC widget for AVR-Monitor                                             *
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

#include "adcwidget.hpp"
#include "ui_adcwidget.h"

AdcWidget::AdcWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::AdcWidget)
{
	ui->setupUi(this);

	ui->adcLayout->setAlignment(Qt::AlignTop);

	for (unsigned i = 0; i < 6; i++)
	{
		Widgets[i] = new AdcEntry(this, i);

		ui->adcLayout->addWidget(Widgets[i]);
	}
}

void AdcWidget::UpdateValues(const KLVariables& Vars)
{
	if (Vars.Size() == 6) for (int i = 0; i < 6; i++)
	{
		Widgets[i]->UpdateValue(Vars[KLString('V') + KLString(i)].ToNumber());
	}
}

AdcWidget::~AdcWidget(void)
{
	delete ui;
}
