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

#ifndef AVRBRIDGE_HPP
#define AVRBRIDGE_HPP

#include "libbuild.hpp"

#include <KLLibs.hpp>

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "../controller/codes.hpp"

class AVRBRIDGE_EXPORT AVRBridge : public QObject
{

		Q_OBJECT

	private:

		KLScriptbinding* Script;

		QSerialPort* Serial;

		QString Buffer;

	private slots:

		void ReadData(void);

		void GetResoult(double Value);

	public:

		explicit AVRBridge(QObject* Parent = nullptr);

		virtual ~AVRBridge(void) override;

		const KLVariables& Variables(void) const;

		bool Connected(void);

		void Command(const QString& Message);

		void Connect(const QString& Port);

		void Disconnect(void);

		void UpdateSensorVariables(void);

		void UpdateSystemVariables(void);

		void WriteGainSettings(unsigned char ID,
						   unsigned char Gain);

		void WriteShiftValues(unsigned char Values);

		void WriteShiftStatus(bool Enabled);

		void WriteMasterStatus(bool Master);

		void WriteMasterScript(const QString& Code);

	signals:

		void onVariablesUpdate(const KLVariables*);

		void onMessageReceive(const QString&);

		void onMessageSend(const QString&);

		void onConnected(void);

		void onDisconnected(void);

		void onError(const QString&);

};

#endif // AVRBRIDGE_HPP
