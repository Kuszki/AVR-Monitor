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

#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>

namespace Ui
{
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{

		Q_OBJECT

	private:

		Ui::SettingsDialog *ui;

		double LastMasterInterval;
		double LastSlaveInterval;

	public:

		explicit SettingsDialog(QWidget* Parent = nullptr);
		virtual ~SettingsDialog(void) override;

		void SetIntervalValues(double Master, double Slave);

	public slots:

		void UpdateMasterInterval(double Interval);

		virtual void open(void) override;

		virtual void accept(void) override;
		virtual void reject(void) override;

		virtual void apply(void);

	signals:

		void onMasterIntervalChange(double);
		void onSlaveIntervalChange(double);

};

#endif // SETTINGSDIALOG_HPP
