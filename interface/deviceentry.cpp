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

#include "deviceentry.hpp"
#include "ui_deviceentry.h"

DeviceEntry::DeviceEntry(const DeviceData& Data, QWidget* Parent)
: QWidget(Parent), ui(new Ui::DeviceEntry), ID(Data.ID)
{
	ui->setupUi(this);

	Dialog = new DeviceDialog(ID, this); UpdateDevice(Data);

	connect(Dialog, &DeviceDialog::onDialogAccept, this, &DeviceEntry::UpdateDevice);
}

DeviceEntry::~DeviceEntry(void)
{
	delete ui;
}

void DeviceEntry::SettingsButtonClicked(void)
{
	Dialog->open();
}

void DeviceEntry::DeleteButtonClicked(void)
{
	if (AppCore::getInstance()->DeleteDevice(ID)) deleteLater();
	else QMessageBox::warning(this, tr("Error"), tr("Can't delete device - %1").arg(AppCore::getError()));
}

void DeviceEntry::UpdateDevice(const DeviceData& Data)
{
	ui->Name->setText(tr("%1 on pin %2").arg(Data.Name).arg(Data.Output));
}
