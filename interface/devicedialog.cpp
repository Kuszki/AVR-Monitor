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

#include "devicedialog.hpp"
#include "ui_devicedialog.h"

DeviceDialog::DeviceDialog(int Device, QWidget* Parent)
: QDialog(Parent), ui(new Ui::DeviceDialog), ID(Device)
{
	ui->setupUi(this);
}

DeviceDialog::~DeviceDialog(void)
{
	delete ui;
}

void DeviceDialog::open(void)
{
	DeviceData Data = AppCore::getInstance()->GetDevice(ID);

	ui->Name->setText(Data.Name);
	ui->Output->setValue(Data.Output);
	ui->Active->setCurrentIndex(Data.Active);

	QDialog::open();
}

void DeviceDialog::accept(void)
{
	DeviceData Data; bool OK = false;

	Data.ID = ID;
	Data.Name = ui->Name->text();
	Data.Output = ui->Output->value();
	Data.Active = ui->Active->currentIndex();

	if (ID != -1)
	{
		OK = AppCore::getInstance()->UpdateDevice(Data);
	}
	else
	{
		OK = AppCore::getInstance()->AddDevice(Data);
	}

	if (OK)
	{
		emit onDialogAccept(Data); QDialog::accept();
	}
	else
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't insert data into database - %1").arg(AppCore::getError()));
	}
}
