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

#ifndef TERMINALWIDGET_HPP
#define TERMINALWIDGET_HPP

#include <QMessageBox>
#include <QFileDialog>
#include <QWidget>

namespace Ui
{
	class TerminalWidget;
}

class TerminalWidget : public QWidget
{

		Q_OBJECT

	private:

		Ui::TerminalWidget* ui;

	public:

		explicit TerminalWidget(QWidget* Parent = nullptr);
		virtual ~TerminalWidget(void) override;

	private slots:

		void SaveButtonClicked(void);
		void LoadButtonClicked(void);

		void ExecuteButtonClicked(void);
		void CheckButtonClicked(void);

	public slots:

		void AppendInput(const QString& Code);
		void AppendOutput(const QString& Code);

	signals:

		void onScriptExecute(const QString&);
		void onScriptValidate(const QString&);

};

#endif // TERMINALWIDGET_HPP
