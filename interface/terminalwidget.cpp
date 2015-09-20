/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Terminal widget for AVR-Monitor                                        *
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

#include "terminalwidget.hpp"
#include "ui_terminalwidget.h"

TerminalWidget::TerminalWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::TerminalWidget)
{
	ui->setupUi(this);

	ui->Helper->hide();
}

TerminalWidget::~TerminalWidget(void)
{
	delete ui;
}

void TerminalWidget::SaveButtonClicked(void)
{
	QString Path = QFileDialog::getSaveFileName(this, tr("Select file to save script"));

	if (!Path.isEmpty())
	{
		QFile File(Path);

		if (!File.open(QFile::WriteOnly)) QMessageBox::warning(this, tr("Error"), tr("Can't open selected file in write mode"));
		else
		{
			File.write(ui->Script->document()->toPlainText().toUtf8());
		}
	}
}

void TerminalWidget::LoadButtonClicked(void)
{
	QString Path = QFileDialog::getOpenFileName(this, tr("Select file to load script"));

	if (!Path.isEmpty())
	{
		QFile File(Path);

		if (!File.open(QFile::ReadOnly)) QMessageBox::warning(this, tr("Error"), tr("Can't open selected file in read mode"));
		else
		{
			ui->Script->document()->setPlainText(File.readAll());
		}
	}
}

void TerminalWidget::ExecuteButtonClicked(void)
{
	emit onScriptExecute(ui->Script->document()->toPlainText());

	if (ui->Clean->isChecked()) ui->Script->document()->clear();
}

void TerminalWidget::CheckButtonClicked(void)
{
	emit onScriptValidate(ui->Script->document()->toPlainText());
}

void TerminalWidget::AppendInput(const QString& Code)
{
	ui->Log->appendPlainText(QString(">> %1").arg(Code));
}

void TerminalWidget::AppendOutput(const QString& Code)
{
	ui->Log->appendPlainText(QString("<< %1").arg(Code));
}
