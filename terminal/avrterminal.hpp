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

#ifndef AVRTERMINAL_HPP
#define AVRTERMINAL_HPP

#include <QCoreApplication>
#include <QTextStream>
#include <QObject>
#include <QTimer>

#include <avrbridge.hpp>

#include "terminalreader.hpp"

class AVRTerminal : public QObject

{

		Q_OBJECT

	protected:

		Terminalreader* Worker;
		AVRBridge* Device;
		QTimer* Timeout;

		QTextStream Cin;
		QTextStream Cout;

	public:

		explicit AVRTerminal(const QString Port);

		virtual ~AVRTerminal(void) override;

	public slots:

		void HandleError(const QString& Error);

		void HandleMessage(const QString& Message);

		void HandleCommand(const QString& Message);

		void HandleConnect(bool Connected);

		void HandleTimeout(void);

};

#endif // AVRTERMINAL_HPP