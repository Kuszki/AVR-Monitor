/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  System info widget for AVR-Monitor                                     *
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

#ifndef SYSTEMWIDGET_HPP
#define SYSTEMWIDGET_HPP

#include <QWidget>

namespace Ui
{
	class SystemWidget;
}

class SystemWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::SystemWidget *ui;

	public:

		explicit SystemWidget(QWidget* Parent = nullptr);
		virtual ~SystemWidget(void) override;

	private slots:

		void CleanButtonClicked(void);

	public slots:

		void UpdateLink(bool Online);

		void UpdateStatus(bool Master);

		void UpdateShiftValues(unsigned char Values);

		void UpdateShiftStatus(bool Active);

		void UpdateGainValue(unsigned char ID, unsigned char Gain);

		void UpdateFreeRam(unsigned Value);

		void UpdateInterval(double Value);

	signals:

		void onRamCleanRequest(void);

};

#endif // SYSTEMWIDGET_HPP
