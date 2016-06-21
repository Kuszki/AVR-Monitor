/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Script editor helper for AVR-Monitor                                   *
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

#include "helperentry.hpp"
#include "ui_helperentry.h"

HelperEntry::HelperEntry(const QString& Label, const QString& Code, QWidget* Parent)
: QWidget(Parent), ui(new Ui::HelperEntry)
{
	ui->setupUi(this);

	ui->Name->setText(Label);
	ui->Name->setToolTip(Code);
}

HelperEntry::~HelperEntry(void)
{
	delete ui;
}

void HelperEntry::CopyButtonClicked(void)
{
	QApplication::clipboard()->setText(ui->Name->toolTip());
}

void HelperEntry::PasteButtonClicked(void)
{
	emit onScriptPaste(ui->Name->toolTip() + '\n');
}
