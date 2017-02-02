/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Terminal widget for AVR-Monitor                                        *
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

#include "terminalwidget.hpp"
#include "ui_terminalwidget.h"

TerminalWidget::TerminalWidget(QWidget* Parent)
: QWidget(Parent), ui(new Ui::TerminalWidget)
{
	ui->setupUi(this);

	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Terminal");

	ui->rightSpacer->changeSize(ui->logCheck->sizeHint().width(), 0);
	ui->Clean->setChecked(Settings.value("clean", false).toBool());
	ui->logCheck->setChecked(Settings.value("log", true).toBool());
	ui->helperCheck->setChecked(Settings.value("helper", false).toBool());
	ui->Log->setVisible(ui->logCheck->isChecked());
	ui->Helper->setVisible(ui->helperCheck->isChecked());

	Settings.endGroup();
}

TerminalWidget::~TerminalWidget(void)
{
	QSettings Settings("AVR-Monitor");

	Settings.beginGroup("Terminal");
	Settings.setValue("clean", ui->Clean->isChecked());
	Settings.setValue("log", ui->logCheck->isChecked());
	Settings.setValue("helper", ui->helperCheck->isChecked());
	Settings.endGroup();

	delete ui;
}

void TerminalWidget::changeEvent(QEvent* Event)
{
	if (Event->type() == QEvent::EnabledChange)
	{
		const bool Enabled = isEnabled();

		ui->loadButton->setEnabled(Enabled);
		ui->saveButton->setEnabled(Enabled);
		ui->checkButton->setEnabled(Enabled);
		ui->sendButton->setEnabled(Enabled);
		ui->cleanButton->setEnabled(Enabled);
	}

	QWidget::changeEvent(Event);
}

void TerminalWidget::SetTitleWidget(TitleWidget* Widget)
{
	ui->leftSpacer->changeSize(ui->logCheck->sizeHint().width(), 0);

	while (ui->toolsLayout->count())
	{
		QLayoutItem* I = ui->toolsLayout->takeAt(0);

		if (QWidget* W = I->widget())
			Widget->addRightWidget(W);
		else if (QSpacerItem* S = I->spacerItem())
			Widget->addRightSpacer(S);
		else delete I;
	}
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
