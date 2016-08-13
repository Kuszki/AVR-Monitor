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

#ifndef GAINWIDGET_HPP
#define GAINWIDGET_HPP

#include <QWidget>
#include <QMap>

#include "titlewidget.hpp"

namespace Ui
{
	class GainWidget;
}

class GainWidget : public QWidget
{

		Q_OBJECT

	private:

		static const QMap<unsigned char, unsigned char> GainValues;

		Ui::GainWidget *ui;

	public:

		explicit GainWidget(QWidget* Parent = nullptr);
		virtual ~GainWidget(void) override;

		void SetTitleWidget(TitleWidget* Widget);

	private slots:

		void GainValueChanged(int Index);

		void DutyValueChanged(int Value);

	public slots:

		void GainChanged(unsigned char ID, unsigned char Gain);

		void DutyChanged(unsigned char Value);

	signals:

		void onGainChange(unsigned char, unsigned char);

		void onDutyChange(unsigned char);

};

#endif // GAINWIDGET_HPP
