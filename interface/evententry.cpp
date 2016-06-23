/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Event entry widget for AVR-Monitor                                     *
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

#include "evententry.hpp"
#include "ui_evententry.h"

EventEntry::EventEntry(const EventData& Data, QWidget* Parent)
: QWidget(Parent), ui(new Ui::EventEntry), ID(Data.ID)
{
	ui->setupUi(this); UpdateEvent(Data);

	Dialog = new EventDialog(ID, this);

	connect(Dialog, &EventDialog::onDialogAccept, this, &EventEntry::UpdateEvent);

	connect(AppCore::getInstance(), &AppCore::onEventUpdate, [this] (int Index) -> void
	{
		if (Index == ID) UpdateEvent(AppCore::getInstance()->GetEvent(ID));
	});
}

EventEntry::~EventEntry(void)
{
	delete ui;
}

void EventEntry::SettingsButtonClicked(void)
{
	Dialog->open();
}

void EventEntry::DeleteButtonClicked(void)
{
	if (QMessageBox::question(this, tr("Delete event"), tr("Are you sure you wany to delete selected event (%1)?").arg(ui->Name->text())) == QMessageBox::Yes)
	{
		if (AppCore::getInstance()->DeleteEvent(ID)) deleteLater();
		else QMessageBox::warning(this, tr("Error"), tr("Can't delete event - %1").arg(AppCore::getError()));
	}
}

void EventEntry::UpdateEvent(const EventData& Data)
{
	ui->Name->setText(Data.Name);
	ui->Name->setEnabled(Data.Active);

	emit onEventUpdate(Data);
}
