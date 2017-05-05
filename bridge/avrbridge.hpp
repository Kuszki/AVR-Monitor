/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Bridge library for AVR-Monitor platform                                *
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

#ifndef AVRBRIDGE_HPP
#define AVRBRIDGE_HPP

#include "libbuild.hpp"

#include <KLLibs.hpp>

#include <QList>
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
		bool Override = false;

		int Buffindex = 0;
		char Buffer[256];

	private slots:

		void HandleError(QSerialPort::SerialPortError Error);

		void ReadData(void);

	public:

		explicit AVRBridge(KLVariables* Returns = nullptr, QObject* Parent = nullptr);

		virtual ~AVRBridge(void) override;

		KLVariables& Variables(void);

		void Command(const QString& Message);

		void Connect(const QString& Port);

		void Disconnect(void);

		bool IsConnected(void);

		void UpdateSensorVariables(void);

		void UpdateSystemVariables(unsigned char Mask = 255);

		void WriteGainSettings(unsigned char ID,
						   unsigned char Gain);

		void WriteShiftValues(unsigned char Values);

		void WriteShiftValue(unsigned char Index,
						 bool Value);

		void WriteShiftStatus(bool Enabled);

		void WriteMasterStatus(bool Master);

		void WriteDutyValue(unsigned char Value);

		void WriteSleepValue(double Time);

		void WriteDefaultShift(unsigned char Values);

		void WriteSpiValues(const QList<unsigned char>& Values);

		void WriteMasterScript(const QString& Code);

		void ReadMasterScript(void);

		void CleanMasterRam(void);

		void SetWriteMode(bool Force);

		bool GetWriteMode(void) const;

		bool ConnectSensorEvent(const QString& Name,
						    const boost::function<void (double)>& Callback);

		bool DisconnectSensorEvent(const QString& Name);

	signals:

		void onConnectionUpdate(bool);

		void onShiftValuesUpdate(unsigned char);

		void onShiftStatusUpdate(bool);

		void onGainSettingsUpdate(unsigned char, unsigned char);

		void onMasterStatusUpdate(bool);

		void onSleepValueUpdate(double);

		void onDutyValueUpdate(unsigned char);

		void onFreeRamUpdate(unsigned);

		void onSensorValuesUpdate(const KLVariables&);

		void onMasterScriptReceive(const QString&);

		void onMessageReceive(const QString&);

		void onMessageSend(const QString&);

		void onError(const QString&);

};

#endif // AVRBRIDGE_HPP
