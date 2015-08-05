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

#ifndef SHIFTWIDGET_HPP
#define SHIFTWIDGET_HPP

#include <QDockWidget>
#include <QCheckBox>
#include <QWidget>

namespace Ui
{
	class ShiftWidget;
}

class ShiftWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::ShiftWidget *ui;

		QCheckBox* Pins[8];

	public:

		explicit ShiftWidget(QWidget* Parent = nullptr);
		virtual ~ShiftWidget(void) override;

	private slots:

		void EnableAllClicked(void);
		void DisableAllClicked(void);

		void OutputChanged(bool Enabled);
		void EnabledChanged(bool Enabled);

	public slots:

		void UpdateShiftValues(unsigned char Values);
		void UpdateShiftStatus(bool Enabled);

	signals:

		void onShiftChanged(unsigned char);

		void onEnabledChanged(bool);

};

#endif // SHIFTWIDGET_HPP
