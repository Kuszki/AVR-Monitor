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

#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP

#include <QSocketNotifier>
#include <QObject>

class Signalhandler : public QObject
{
		Q_OBJECT

	public:
		Signalhandler(QObject *parent = 0);
		~Signalhandler();

		// Unix signal handlers.
		static void hupSignalHandler(int unused);
		static void termSignalHandler(int unused);

	public slots:
		// Qt signal handlers.
		void handleSigHup();
		void handleSigTerm();

	private:
		static int sighupFd[2];
		static int sigtermFd[2];

		QSocketNotifier *snHup;
		QSocketNotifier *snTerm;
};

#endif // SIGNALHANDLER_HPP
