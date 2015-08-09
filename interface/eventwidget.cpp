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

#include "eventwidget.hpp"
#include "ui_eventwidget.h"

EventWidget::EventWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::EventWidget)
{
	ui->setupUi(this);

	Dialog = new EventDialog(-1, this);

	for (const auto& Data: AppCore::getInstance()->GetEvents()) AddEvent(Data);

	connect(Dialog, &EventDialog::onDialogAccept, this, &EventWidget::AddEvent);

	connect(ui->addButton, &QPushButton::clicked, [this] (void) -> void { Dialog->open(); });
}

EventWidget::~EventWidget(void)
{
	delete ui;
}

void EventWidget::AddEvent(const EventData& Data)
{
	EventEntry* Entry = new EventEntry(Data, this);

	ui->eventsLayout->addWidget(Entry);
}
