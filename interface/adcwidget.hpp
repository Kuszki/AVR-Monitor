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

#ifndef ADCWIDGET_HPP
#define ADCWIDGET_HPP

#include <QHBoxLayout>
#include <QDockWidget>
#include <QWidget>
#include <QLabel>

#include <KLLibs.hpp>

#include "titlewidget.hpp"
#include "adcentry.hpp"

namespace Ui
{
	class AdcWidget;
}

class AdcWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::AdcWidget* ui;

		AdcEntry* Widgets[6];

	public:

		explicit AdcWidget(QWidget* Parent = nullptr);
		virtual ~AdcWidget(void) override;

		void SetTitleWidget(TitleWidget* Widget);

	public slots:

		void UpdateValues(const KLVariables& Vars);

};

#endif // ADCWIDGET_HPP
