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

#include "adcwidget.hpp"
#include "ui_adcwidget.h"

AdcWidget::AdcWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::AdcWidget)
{
	ui->setupUi(this);

	for (int i = 0; i < 6; i++)
	{
		QHBoxLayout* layout = new QHBoxLayout();

		QLabel* name = new QLabel(tr("ADC %1").arg(i), this);
		QLabel* value = new QLabel("0", this);
		QLabel* unit = new QLabel("V", this);

		value->setAlignment(Qt::AlignRight);
		unit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

		layout->addWidget(name);
		layout->addWidget(value);
		layout->addWidget(unit);

		ui->layoutConverters->addLayout(layout);

		Values[i] = value;

		connect(this, &AdcWidget::destroyed, layout, &QLayout::deleteLater);
	}
}

void AdcWidget::UpdateValues(const KLVariables& Vars)
{
	if (Vars.Size() == 6) for (const auto& Var: Vars) Values[Var.ID[1] - '0']->setText(QString("%1").arg(Var.Value.ToNumber(), 0, '.', 5));
}

AdcWidget::~AdcWidget(void)
{
	delete ui;
}
