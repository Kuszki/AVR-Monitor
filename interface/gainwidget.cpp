/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Gain control widget for AVR-Monitor                                    *
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

#include "gainwidget.hpp"
#include "ui_gainwidget.h"

const QMap<unsigned char, unsigned char> GainWidget::GainValues =
{
	{0, 1},
	{1, 2},
	{2, 4},
	{3, 5},
	{4, 8},
	{5, 10},
	{6, 16},
	{7, 32}
};

GainWidget::GainWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::GainWidget)
{
	ui->setupUi(this);
}

GainWidget::~GainWidget(void)
{
	delete ui;
}

void GainWidget::SetTitleWidget(TitleWidget* Widget)
{

}

void GainWidget::GainValueChanged(int Index)
{
	if (sender() == ui->gainValue_0) emit onGainChange(0, GainValues.value(Index, 1));
	else if (sender() == ui->gainValue_1) emit onGainChange(1, GainValues.value(Index, 1));
}

void GainWidget::GainChanged(unsigned char ID, unsigned char Gain)
{
	blockSignals(true);

	switch (ID)
	{
		case 0:
			ui->gainValue_0->setCurrentIndex(GainValues.key(Gain, 0));
		break;
		case 1:
			ui->gainValue_1->setCurrentIndex(GainValues.key(Gain, 0));
		break;
	}

	blockSignals(false);
}
