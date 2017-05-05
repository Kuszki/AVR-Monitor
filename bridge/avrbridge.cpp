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

#include "avrbridge.hpp"

AVRBridge::AVRBridge(KLVariables* Returns, QObject* Parent)
: QObject(Parent), Sensors(Returns)
{
	Script = new KLScriptbinding(&Sensors, this);
	Serial = new QSerialPort(this);

	Sensors.Add("V0", KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);
	Sensors.Add("V1", KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);
	Sensors.Add("V2", KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);
	Sensors.Add("V3", KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);
	Sensors.Add("V4", KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);
	Sensors.Add("V5", KLVariables::NUMBER, KLVariables::KLSCALLBACK(), false);

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
		emit onShiftValuesUpdate(unsigned(Value));
	});

	Script->Variables.Add("SHRE", KLVariables::BOOLEAN, [this] (double Value) -> void
	{
		emit onShiftStatusUpdate(bool(Value));
	});
	Script->Variables.Add("PGA0", KLVariables::INTEGER, [this] (double Value) -> void
	{
		emit onGainSettingsUpdate(0, unsigned(Value));
	});

	Script->Variables.Add("PGA1", KLVariables::INTEGER, [this] (double Value) -> void
	{
		emit onGainSettingsUpdate(1, unsigned(Value));
	});

	Script->Variables.Add("PWMV", KLVariables::INTEGER, [this] (double Value) -> void
	{
		emit onDutyValueUpdate(unsigned(Value));
	});

	Script->Variables.Add("SLPT", KLVariables::INTEGER, [this] (double Value) -> void
	{
		emit onSleepValueUpdate(Value / 10);
	});

	Script->Variables.Add("FRAM", KLVariables::INTEGER, [this] (double Value) -> void
	{
		emit onFreeRamUpdate(unsigned(Value));
	});

	Script->Bindings.Add("set", [this] (KLList<double>& Vars) -> double
	{
		for (int i = 0; i < 6; i++) Sensors[KLString('V') + KLString(i)] = Vars[i];

		emit onSensorValuesUpdate(Sensors);

		return 0;
	});

	Script->Bindings.Add("fail", [this] (KLList<double>& Vars) -> double
	{
		if (Vars.Size() == 1) switch (int(Vars[0]))
		{
			case WRONG_SCRIPT:
				emit onError(tr("Wrong scriptcode"));
			break;
			case WRONG_PARAMS:
				emit onError(tr("Wrong script params"));
			break;
			case WRONG_SHR_PIN:
				emit onError(tr("Wrong shift register pin"));
			break;
			case WRONG_PGA_ID:
				emit onError(tr("Wrong PGA ID"));
			break;
			case WRONG_PGA_GAIN:
				emit onError(tr("Wrong PGA gain"));
			break;
			case WRONG_ADC_ID:
				emit onError(tr("Wrong ADC ID"));
			break;
			case WRONG_SYS_CODE:
				emit onError(tr("Wrong system code"));
			break;
			case WRONG_SYS_STATE:
				emit onError(tr("Wrong system status"));
			break;
			case MASTER_TIMEOUT:
				emit onError(tr("Device frozen on master script evaluation"));
			break;
			case REMOTE_TIMEOUT:
				emit onError(tr("Device frozen on remote script evaluation"));
			break;
		}
		else if (Vars.Size() == 2 && int(Vars[0]) == WRONG_SCRIPT)
		{
			emit onError(tr("Wrong scriptcode - %1").arg(KLScriptbinding::Errorcode(KLScript::ERROR(int(Vars[1])))));
		}

		return 0;
	});

	Serial->setBaudRate(QSerialPort::Baud115200);
	Serial->setParity(QSerialPort::NoParity);
	Serial->setStopBits(QSerialPort::OneStop);
	Serial->setDataBits(QSerialPort::Data8);

	connect(Serial, &QSerialPort::readyRead, this, &AVRBridge::ReadData);

	connect(Serial, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(HandleError(QSerialPort::SerialPortError)));
}

AVRBridge::~AVRBridge(void)
{
	if (IsConnected()) Disconnect();
}

void AVRBridge::HandleError(QSerialPort::SerialPortError Error)
{
	if (Error == QSerialPort::ResourceError)
	{
		if (Script->Variables["LINE"].ToBool())
		{
			emit onError(tr("Device connection error - %1").arg(Serial->errorString()));

			Script->Variables["LINE"] = false;
		}

		Serial->close();
	}
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

KLVariables& AVRBridge::Variables(void)
{
	return Script->Variables;
}

void AVRBridge::Command(const QString& Message)
{
	if (!IsConnected()) emit onError(tr("Serial or device not connected"));
	else
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

		if (!Serial->open(QIODevice::ReadWrite)) emit onError(tr("Can not open serial - %1").arg(Serial->errorString()));
		else
		{
			Serial->write(QString("call dev %1,1;call dev %2,0;\n").arg(DEV_LINE).arg(DEV_MASTER).toUtf8());
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

		Serial->thread()->msleep(100);

		Serial->close();

		Script->Variables["LINE"] = false;
	}
}

bool AVRBridge::IsConnected(void)
{
	if (Serial->error() == QSerialPort::ResourceError) Serial->close();

	if (Script->Variables["LINE"].ToBool() != Serial->isOpen())
	{
		emit onError(tr("Device disconnected without information"));

		Script->Variables["LINE"] = false;
	}

	return Script->Variables["LINE"].ToBool() && Serial->isOpen();
}


void AVRBridge::UpdateSensorVariables(void)
{
	Command("call get;");
}

void AVRBridge::UpdateSystemVariables(unsigned char Mask)
{
	Command(QString("call sys %1;").arg(Mask));
}

void AVRBridge::WriteGainSettings(unsigned char ID, unsigned char Gain)
{
	if (ID > 1) { emit onError(tr("Wrong PGA ID")); return; }

	const KLString Label = KLString(ID ? "PGA1" : "PGA0");

	if (!Override && Script->Variables[Label].ToInt() == Gain) return;
	else Command(QString("call pga %1, %2;").arg(ID).arg(Gain));
}

void AVRBridge::WriteShiftValues(unsigned char Values)
{
	if (!Override && Script->Variables["SHRD"].ToInt() == Values) return;
	else Command(QString("call put %1;").arg(Values));
}

void AVRBridge::WriteShiftValue(unsigned char Index, bool Value)
{
	if (!Override && (Script->Variables["SHRD"].ToInt() & (1 << Index)) == Value) return;
	else Command(QString("call put %1,%2;").arg(Index).arg(Value));
}

void AVRBridge::WriteShiftStatus(bool Enabled)
{
	if (!Override && Script->Variables["SHRE"].ToBool() == Enabled) return;
	else Command(QString("call out %1;").arg(Enabled));
}

void AVRBridge::WriteMasterStatus(bool Master)
{
	if (!Override && Script->Variables["WORK"].ToBool() == Master) return;
	else Command(QString("call dev %1,%2;").arg(DEV_MASTER).arg(Master));
}

void AVRBridge::WriteDutyValue(unsigned char Value)
{
	if (!Override && Script->Variables["PWMV"].ToInt() == Value) return;
	else Command(QString("call pwm %1;").arg(Value));
}

void AVRBridge::WriteSleepValue(double Time)
{
	if (!Override && Script->Variables["SLPT"].ToInt() == Time * 10) return;
	else Command(QString("call dev %1,%2;").arg(DEV_SLEEP).arg((unsigned char)(Time * 10)));
}

void AVRBridge::WriteDefaultShift(unsigned char Values)
{
	Command(QString("call dev %1,%2;").arg(DEV_DEFAULT).arg(Values));
}

void AVRBridge::WriteSpiValues(const QList<unsigned char>& Values)
{
	if (Values.size())
	{
		QString Code = "call spi ";

		for (const auto& Value: Values) Code.append(QString("%1,").arg(Value));

		Code[Code.size() - 1] = ';';

		Command(Code);
	}
}

void AVRBridge::WriteMasterScript(const QString& Code)
{
	if (Code.size() > 1000) emit onError(tr("Script is too large"));
	else if (!IsConnected()) emit onError(tr("Serial or device not connected"));
	else if (Script->Variables["WORK"].ToBool()) emit onError(tr("Device is working as master"));
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
	else
	{
		Serial->write(QString("call dev %1,1;\n").arg(DEV_SCRIPT).toUtf8());
		Serial->flush();
	}
}

void AVRBridge::CleanMasterRam(void)
{
	Command(QString("call dev %1,%2;").arg(DEV_SPEC).arg(CLEAN_RAM));
}

void AVRBridge::SetWriteMode(bool Force)
{
	Override = Force;
}

bool AVRBridge::GetWriteMode(void) const
{
	return Override;
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

bool AVRBridge::DisconnectSensorEvent(const QString &Name)
{
	const KLString Sensor = Name.toStdString().c_str();

	if (Sensors.Exists(Sensor))
	{
		Sensors[Sensor].SetCallback(boost::function<void (double)>());
	}
	else return false;

	return true;
}
