/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Device widget for AVR-Monitor                                          *
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

#include "devicewidget.hpp"
#include "ui_devicewidget.h"

DeviceWidget::DeviceWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::DeviceWidget)
{
	ui->setupUi(this);

	Dialog = new DeviceDialog(-1, this);

	for (const auto& Data: AppCore::getInstance()->GetDevices()) AddDevice(Data);

	connect(Dialog, &DeviceDialog::onDialogAccept, this, &DeviceWidget::AddDevice);

	connect(ui->addButton, &QPushButton::clicked, [this] (void) -> void
	{
		Dialog->open();
	});
}

DeviceWidget::~DeviceWidget(void)
{
	delete ui;
}

void DeviceWidget::AddDevice(const DeviceData& Data)
{
	DeviceEntry* Entry = new DeviceEntry(Data, this);

	ui->devicesLayout->addWidget(Entry);
}
