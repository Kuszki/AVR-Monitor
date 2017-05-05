/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Terminal for AVR-Monitor device                                        *
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

#ifndef AVRSERVER_HPP
#define AVRSERVER_HPP

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QObject>
#include <QTimer>

#include <avrbridge.hpp>

class AVRServer : public QObject
{

		Q_OBJECT

	private:

		KLVariables Sensors;

		QSqlDatabase Database;

		AVRBridge* Device;

		QTimer* Timeout;

	public:

		explicit AVRServer(const QString& Port, const QString& Server);
		virtual ~AVRServer(void) override;

	private slots:

		void HandleConnect(bool Connected);

		void HandleTimeout(void);

		void HandleExec(const QString& Script);

		void HandleCallback(const QString& Name, double Value);

		void HandleSystem(const QString& Name, int Value);

		void HandleConverters(const KLVariables& Vars);

};

#endif // AVRSERVER_HPP
