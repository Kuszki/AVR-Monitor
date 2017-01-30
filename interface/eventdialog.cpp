/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Event options dialog for AVR-Monitor                                   *
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

#include "eventdialog.hpp"
#include "ui_eventdialog.h"

EventDialog::EventDialog(int Event, QWidget* Parent)
: QDialog(Parent), ui(new Ui::EventDialog), ID(Event)
{
	ui->setupUi(this);
}

EventDialog::~EventDialog(void)
{
	delete ui;
}

void EventDialog::open(void)
{
	EventData Data = AppCore::getInstance()->GetEvent(ID);

	if (Data.ID != -1)
	{
		ui->Name->setText(Data.Name);
		ui->Script->document()->setPlainText(Data.Script);
		ui->Active->setChecked(Data.Active);
	}
	else
	{
		ui->Name->clear();
		ui->Script->document()->clear();
		ui->Active->setChecked(false);
	}

	QDialog::open();
}

void EventDialog::TextBoxEdited(void)
{
	ui->saveButton->setEnabled(!ui->Name->text().isEmpty());
}

void EventDialog::accept(void)
{
	EventData Data; bool OK = false;

	Data.ID = ID;
	Data.Name = ui->Name->text();
	Data.Script = ui->Script->document()->toPlainText();
	Data.Active = ui->Active->isChecked();

	if (ID != -1)
	{
		OK = AppCore::getInstance()->UpdateEvent(Data);
	}
	else
	{
		OK = AppCore::getInstance()->AddEvent(Data);
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
