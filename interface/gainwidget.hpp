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

#ifndef GAINWIDGET_HPP
#define GAINWIDGET_HPP

#include <QWidget>
#include <QMap>

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

		explicit GainWidget(QWidget *Parent = nullptr);
		virtual ~GainWidget(void) override;

	private slots:

		void GainValueChanged(int Index);

	public slots:

		void GainChanged(unsigned char ID, unsigned char Gain);

	signals:

		void onGainChange(unsigned char, unsigned char);

};

#endif // GAINWIDGET_HPP