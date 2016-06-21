/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Sensor settings dialog for AVR-Monitor                                 *
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

#ifndef SENSORDIALOG_HPP
#define SENSORDIALOG_HPP

#include <QMessageBox>
#include <QDialog>

#include "appcore.hpp"
#include "common.hpp"

namespace Ui
{
	class SensorDialog;
}

class SensorDialog : public QDialog
{

		Q_OBJECT

	private:

		Ui::SensorDialog* ui;

		const int ID;

	public:

		explicit SensorDialog(int Sensor, QWidget* Parent = nullptr);
		virtual ~SensorDialog(void) override;

		virtual void open(void) override;

	public slots:

		virtual void accept(void) override;

	signals:

		void onDialogAccept(const SensorData&);

};

#endif // SENSORDIALOG_HPP
