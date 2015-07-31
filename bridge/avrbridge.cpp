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

#include "avrbridge.hpp"

#include <QDebug>

AVRBridge::AVRBridge(QObject* Parent)
: QObject(Parent)
{
	Script = new KLScriptbinding(this);
	Serial = new QSerialPort(this);

	Script->Variables.Add("ADC0", KLVariables::NUMBER);
	Script->Variables.Add("ADC1", KLVariables::NUMBER);
	Script->Variables.Add("ADC2", KLVariables::NUMBER);
	Script->Variables.Add("ADC3", KLVariables::NUMBER);
	Script->Variables.Add("ADC4", KLVariables::NUMBER);
	Script->Variables.Add("ADC5", KLVariables::NUMBER);

	Script->Variables.Add("SHRD", KLVariables::INTEGER);
	Script->Variables.Add("SHRE", KLVariables::BOOLEAN);
	Script->Variables.Add("PGA0", KLVariables::INTEGER);
	Script->Variables.Add("PGA1", KLVariables::INTEGER);
	Script->Variables.Add("WORK", KLVariables::BOOLEAN);
	Script->Variables.Add("LINE", KLVariables::BOOLEAN);
	Script->Variables.Add("FRAM", KLVariables::BOOLEAN);

	Serial->setBaudRate(QSerialPort::Baud57600);
	Serial->setParity(QSerialPort::NoParity);
	Serial->setStopBits(QSerialPort::OneStop);
	Serial->setDataBits(QSerialPort::Data8);

	Buffer.reserve(256);

	connect(Serial, SIGNAL(readyRead()), SLOT(ReadData()));
	connect(Script, SIGNAL(onEvaluate(double)), SLOT(GetResoult(double)));

}

AVRBridge::~AVRBridge(void)
{
	if (Script->Variables["LINE"].ToBool()) Disconnect();
}

void AVRBridge::ReadData(void)
{
	static bool Emited = false;

	char c; while (Serial->getChar(&c)) switch (c)
	{

		case '\a':

			Script->SetCode(Buffer);
			Script->Evaluate();

			if (!Emited) {emit onConnected(); Emited = true;}

			emit onMessageReceive(Buffer);

			Buffer.clear();

		break;

		default: Buffer.append(c);
	}
}

void AVRBridge::GetResoult(double Value)
{
	if (!Script->Variables["LINE"].ToBool())
	{
		emit onDisconnected();

		Serial->close();
	}

	switch (int(Value))
	{
		case WRONG_SCRIPT:
			emit onError(tr("Wrong scriptcode."));
		break;
		case WRONG_PARAMS:
			emit onError(tr("Wrong script params."));
		break;
		case WRONG_SHR_PIN:
			emit onError(tr("Wrong shift register pin."));
		break;
		case WRONG_PGA_ID:
			emit onError(tr("Wrong PGA ID."));
		break;
		case WRONG_PGA_GAIN:
			emit onError(tr("Wrong PGA gain."));
		break;
		case WRONG_ADC_ID:
			emit onError(tr("Wrong ADC ID."));
		break;
		case WRONG_SYS_CODE:
			emit onError(tr("Wrong system code."));
		break;
		case WRONG_SYS_STATE:
			emit onError(tr("Wrong system status."));
		break;
	}

	emit onVariablesUpdate(&Script->Variables);
}

const KLVariables& AVRBridge::Variables(void) const
{
	return Script->Variables;
}

bool AVRBridge::Connected(void)
{
	if (Script->Variables["LINE"].ToBool() != Serial->isOpen())
	{
		emit onError(tr("Device disconnected without information"));

		Script->Variables["LINE"] = false;

		emit onDisconnected();
	}

	return Script->Variables["LINE"].ToBool() && Serial->isOpen();
}

void AVRBridge::Command(const QString& Message)
{
	if (!Connected()) emit onError(tr("Serial or device not connected"));
	{
		Serial->write(Message.toStdString().c_str());
		Serial->write("\n");

		emit onMessageSend(Message);
	}
}

void AVRBridge::Connect(const QString& Port)
{
	if (Serial->isOpen()) emit onError(tr("Serial allready opened"));
	else
	{
		Serial->setPortName(Port);

		if (!Serial->open(QIODevice::ReadWrite)) emit onError(tr("Can not open serial - ") + Serial->errorString());
		else
		{
			Serial->write("call dev 2;\n");
			Serial->flush();
		}
	}
}

void AVRBridge::Disconnect(void)
{
	if (!Serial->isOpen()) emit onError(tr("Serial not connected"));
	else
	{
		Serial->write("call dev 1;\n");
		Serial->flush();
	}
}

void AVRBridge::UpdateSensorVariables(void)
{
	Command("call get 0,1,2,3,4,5;\n");
}

void AVRBridge::UpdateSystemVariables(void)
{
	Command("call sys;\n");
}

void AVRBridge::WriteGainSettings(unsigned char ID, unsigned char Gain)
{
	Command(QString("call pga %1, %2;\n").arg(ID).arg(Gain));
}

void AVRBridge::WriteShiftValues(unsigned char Values)
{
	Command(QString("call put %1;\n").arg(Values));
}

void AVRBridge::WriteShiftStatus(bool Enabled)
{
	Command(QString("call out %1;\n").arg(Enabled));
}

void AVRBridge::WriteMasterStatus(bool Master)
{
	Command(QString("call dev %1;\n").arg(Master ? 8 : 4));
}

void AVRBridge::WriteMasterScript(const QString& Code)
{
	if (!Connected() || Script->Variables["WORK"].ToBool()) emit onError(tr("Cannot upload master script"));
	else
	{
		const char START[] = {1, 0};
		const char STOP[] = {4, 0};

		Serial->write(START);
		Serial->write(Code.toStdString().c_str());
		Serial->write(STOP);
	}
}
