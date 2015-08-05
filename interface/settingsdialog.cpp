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

#include "settingsdialog.hpp"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget* Parent)
: QDialog(Parent), ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);
}

SettingsDialog::~SettingsDialog(void)
{
	delete ui;
}

void SettingsDialog::SetIntervalValues(double Master, double Slave)
{
	ui->intervalMaster->setValue(Master);
	ui->intervalSlave->setValue(Slave);
}

void SettingsDialog::UpdateMasterInterval(double Interval)
{
	ui->intervalMaster->setValue(LastMasterInterval = Interval);
}

void SettingsDialog::open(void)
{
	LastMasterInterval = ui->intervalMaster->value();
	LastSlaveInterval = ui->intervalSlave->value();

	QDialog::open();
}

void SettingsDialog::accept(void)
{
	apply(); QDialog::accept();
}

void SettingsDialog::reject(void)
{
	ui->intervalMaster->setValue(LastMasterInterval);
	ui->intervalSlave->setValue(LastSlaveInterval);

	QDialog::reject();
}

void SettingsDialog::apply(void)
{
	emit onMasterIntervalChange(ui->intervalMaster->value());
	emit onSlaveIntervalChange(ui->intervalSlave->value());
}
