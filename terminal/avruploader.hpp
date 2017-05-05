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

#ifndef AVRUPLOADER_HPP
#define AVRUPLOADER_HPP

#include <QCoreApplication>
#include <QTextStream>
#include <QObject>
#include <QTimer>
#include <QFile>

#include <avrbridge.hpp>

class AVRUploader : public QObject
{

		Q_OBJECT

	protected:

		const QString Script;

		AVRBridge* Device;
		QTimer* Timeout;

		QTextStream Cout;

	public:

		explicit AVRUploader(const QString& Port,
						 const QString& Code);

		virtual ~AVRUploader(void) override;

	public slots:

		void HandleConnect(bool Connected);

		void HandleError(const QString& Error);

		void HandleTimeout(void);

};

#endif // AVRUPLOADER_HPP
