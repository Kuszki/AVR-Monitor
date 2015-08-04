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
	Script = new KLScriptbinding(this, &Sensors);
	Serial = new QSerialPort(this);

	Sensors.Add("V0", KLVariables::NUMBER);
	Sensors.Add("V1", KLVariables::NUMBER);
	Sensors.Add("V2", KLVariables::NUMBER);
	Sensors.Add("V3", KLVariables::NUMBER);
	Sensors.Add("V4", KLVariables::NUMBER);
	Sensors.Add("V5", KLVariables::NUMBER);

	Script->Variables.Add("LINE", KLVariables::BOOLEAN, [this] (double Value) -> void
	{
		if (Connection != bool(Value)) emit onConnectionUpdate(Connection = bool(Value));
	});

	Script->Variables.Add("WORK", KLVariables::BOOLEAN, [this] (double Value) -> void
	{
		emit onMasterStatusUpdate(bool(Value));
	});

	Script->Variables.Add("SHRD", KLVariables::INTEGER, [this] (double Value) -> void
	{
		emit onShiftValuesUpdate((unsigned char)(Value));
	});

	Script->Variables.Add("SHRE", KLVariables::BOOLEAN, [this] (double Value) -> void
	{
		emit onShiftStatusUpdate(bool(Value));
	});
	Script->Variables.Add("PGA0", KLVariables::INTEGER, [this] (double Value) -> void
	{
		emit onGainSettingsUpdate(0, (unsigned char)(Value));
	});

	Script->Variables.Add("PGA1", KLVariables::INTEGER, [this] (double Value) -> void
	{
		emit onGainSettingsUpdate(1, (unsigned char)(Value));
	});

	Script->Variables.Add("FRAM", KLVariables::BOOLEAN, [this] (double Value) -> void
	{
		emit onFreeRamUpdate(unsigned(Value));
	});

	Script->Bindings.Add("set", [this] (KLVariables& Vars) -> double
	{
		for (int i = 0; i < 6; i++) Sensors[KLString('V') + i] = Vars[i].ToNumber();

		emit onSensorsUpdate(Sensors);

		return 0;
	});

	Serial->setBaudRate(QSerialPort::Baud57600);
	Serial->setParity(QSerialPort::NoParity);
	Serial->setStopBits(QSerialPort::OneStop);
	Serial->setDataBits(QSerialPort::Data8);

	connect(Serial, SIGNAL(readyRead()), SLOT(ReadData()));
	connect(Script, SIGNAL(onEvaluate(double)), SLOT(GetResoult(double)));

}

AVRBridge::~AVRBridge(void)
{
	if (IsConnected()) Disconnect();
}

void AVRBridge::ReadData(void)
{
	char c; while (Serial->getChar(&c))
	switch (c)
	{
		case '\n':
		{
			Buffer[Buffindex] = 0; Buffindex = 0;

			QString Command = QString::fromUtf8(Buffer);

			if (Command.startsWith('#'))
			{
				if (Command == SOS) Downloading = true;
				else if (Command == EOS)
				{
					emit onMasterScriptReceive(Input);

					Downloading = false; Input.clear();
				}
			}
			else if (Downloading)
			{
				Input.append(Buffer);
				Input.append('\n');
			}
			else
			{
				Script->SetCode(Command);
				Script->Evaluate();
			}

			emit onMessageReceive(Command);
		}
		break;

		default: Buffer[Buffindex++] = c;
	}
}

void AVRBridge::GetResoult(double Value)
{
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
}

const KLVariables& AVRBridge::Variables(void) const
{
	return Script->Variables;
}

void AVRBridge::Command(const QString& Message)
{
	if (!IsConnected()) emit onError(tr("Serial or device not connected"));
	{
		Serial->write(Message.toUtf8());
		Serial->write("\n");
		Serial->flush();

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
			Serial->write(QString("call dev %1,1;\n").arg(DEV_LINE).toUtf8());
			Serial->flush();
		}
	}
}

void AVRBridge::Disconnect(void)
{
	if (!Serial->isOpen()) emit onError(tr("Serial not connected"));
	else
	{
		Serial->write(QString("call dev %1,0;\n").arg(DEV_LINE).toUtf8());
		Serial->flush();

		Script->Variables["LINE"] = false;
	}
}

bool AVRBridge::IsConnected(void)
{
	if (Script->Variables["LINE"].ToBool() != Serial->isOpen())
	{
		emit onError(tr("Device disconnected without information"));

		Script->Variables["LINE"] = false;
	}

	return Script->Variables["LINE"].ToBool() && Serial->isOpen();
}


void AVRBridge::UpdateSensorVariables(void)
{
	Command("call get 0,1,2,3,4,5;");
}

void AVRBridge::UpdateSystemVariables(void)
{
	Command("call sys;");
}

void AVRBridge::WriteGainSettings(unsigned char ID, unsigned char Gain)
{
	Command(QString("call pga %1, %2;").arg(ID).arg(Gain));
}

void AVRBridge::WriteShiftValues(unsigned char Values)
{
	Command(QString("call put %1;").arg(Values));
}

void AVRBridge::WriteShiftStatus(bool Enabled)
{
	Command(QString("call out %1;").arg(Enabled));
}

void AVRBridge::WriteMasterStatus(bool Master)
{
	Command(QString("call dev %1,%2;").arg(DEV_MASTER).arg(Master));
}

void AVRBridge::WriteMasterScript(const QString& Code)
{
	if (!IsConnected() || Script->Variables["WORK"].ToBool()) emit onError(tr("Cannot upload master script"));
	else
	{
		Serial->write(QString("call dev %1,0;\n").arg(DEV_SCRIPT).toUtf8());

		for (const char Char: Code.trimmed().toUtf8())
		{
			Serial->putChar(Char);
			Serial->flush();

			thread()->msleep(5);
		}

		thread()->msleep(5);

		Serial->write("\n\0", 2);
		Serial->flush();

		thread()->msleep(5);
	}
}

void AVRBridge::ReadMasterScript(void)
{
	if (!IsConnected()) emit onError(tr("Serial or device not connected"));
	else if (Script->Variables["WORK"].ToBool()) emit onError(tr("Device is working as master"));
	{
		Serial->write(QString("call dev %1,1;\n").arg(DEV_SCRIPT).toUtf8());
		Serial->flush();
	}
}

bool AVRBridge::ConnectSensorEvent(const QString& Name, const boost::function<void (double)>& Callback)
{
	const KLString Sensor = Name.toStdString().c_str();

	if (Sensors.Exists(Sensor))
	{
		Sensors[Sensor].SetCallback(Callback);
	}
	else return false;

	return true;
}
