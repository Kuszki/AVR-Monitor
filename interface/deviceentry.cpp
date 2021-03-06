/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Device entry widget for AVR-Monitor                                    *
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

#include "deviceentry.hpp"
#include "ui_deviceentry.h"

DeviceEntry::DeviceEntry(const DeviceData& Data, QWidget* Parent)
: QWidget(Parent), ui(new Ui::DeviceEntry), ID(Data.ID)
{
	ui->setupUi(this); UpdateDevice(Data);

	Dialog = new DeviceDialog(ID, this);

	connect(AppCore::getInstance(), &AppCore::onDeviceUpdate, this, &DeviceEntry::UpdateRequest);
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
	if (QMessageBox::question(this, tr("Delete device"), tr("Are you sure you wany to delete selected device (%1)?").arg(ui->Name->text())) == QMessageBox::Yes)
	{
		if (AppCore::getInstance()->DeleteDevice(ID)) deleteLater();
		else QMessageBox::warning(this, tr("Error"), tr("Can't delete device - %1").arg(AppCore::getError()));
	}
}

void DeviceEntry::UpdateDevice(const DeviceData& Data)
{
	if (Data.ID != ID) return;

	ui->Name->setText(tr("%1 on pin %2").arg(Data.Name).arg(Data.Output));

	emit onDeviceUpdate(Data);
}

void DeviceEntry::UpdateRequest(int Index)
{
	if (Index == ID) UpdateDevice(AppCore::getInstance()->GetDevice(ID));
}
