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

#ifndef HELPERENTRY_HPP
#define HELPERENTRY_HPP

#include <QApplication>
#include <QClipboard>
#include <QWidget>

namespace Ui
{
	class HelperEntry;
}

class HelperEntry : public QWidget
{

		Q_OBJECT

	private:

		Ui::HelperEntry* ui;

	public:

		explicit HelperEntry(const QString& Label, const QString& Code, QWidget* Parent = nullptr);
		virtual ~HelperEntry(void) override;

	private slots:

		void CopyButtonClicked(void);
		void PasteButtonClicked(void);

	signals:

		void onScriptPaste(const QString&);

};

#endif // HELPERENTRY_HPP
