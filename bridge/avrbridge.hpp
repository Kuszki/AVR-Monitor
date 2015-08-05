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

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "../controller/codes.hpp"

class AVRBRIDGE_EXPORT AVRBridge : public QObject
{

		Q_OBJECT

	private:

		KLScriptbinding* Script;
		QSerialPort* Serial;
		QThread* Thread;

		KLVariables Sensors;

		QString Input;

		bool Downloading = false;
		bool Connection = false;

		int Buffindex = 0;
		char Buffer[256];

	private slots:

		void ReadData(void);

		void GetResoult(double Value);

	public:

		explicit AVRBridge(QObject* Parent = nullptr);

		virtual ~AVRBridge(void) override;

		const KLVariables& Variables(void) const;

		void Command(const QString& Message);

		void Connect(const QString& Port);

		void Disconnect(void);

		bool IsConnected(void);

		void UpdateSensorVariables(void);

		void UpdateSystemVariables(unsigned char Mask = 255);

		void WriteGainSettings(unsigned char ID,
						   unsigned char Gain);

		void WriteShiftValues(unsigned char Values);

		void WriteShiftStatus(bool Enabled);

		void WriteMasterStatus(bool Master);

		void WriteSleepValue(double Time);

		void WriteDefaultShift(unsigned char Values);

		void WriteMasterScript(const QString& Code);

		void ReadMasterScript(void);

		bool ConnectSensorEvent(const QString& Name,
						    const boost::function<void (double)>& Callback);

	signals:

		void onConnectionUpdate(bool);

		void onShiftValuesUpdate(unsigned char);

		void onShiftStatusUpdate(bool);

		void onGainSettingsUpdate(unsigned char, unsigned char);

		void onMasterStatusUpdate(bool);

		void onSleepValueUpdate(double);

		void onFreeRamUpdate(unsigned);

		void onSensorValuesUpdate(const KLVariables&);

		void onMasterScriptReceive(const QString&);

		void onMessageReceive(const QString&);

		void onMessageSend(const QString&);

		void onError(const QString&);

};

#endif // AVRBRIDGE_HPP
