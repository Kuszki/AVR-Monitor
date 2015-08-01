/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Control functions for AVR-Monitor UC program                           *
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

#include "procedures.hpp"

const char get_INFOSTR[] PROGMEM =
"# AVR-Monitor device;   Controller program v1.0\n"
"# Copyright (C) 2015     Łukasz \"Kuszki\" Dróżdż\n"
"#\n"
"# Web:    https://github.com/Kuszki/AVR-Monitor\n"
"# Built:                   " __DATE__ " " __TIME__ "\n"
"# Tools:                  GNU GCC version " __VERSION__ "\n"
"# System:     Debian 8 GNU/Linux 3.16.0-4-amd64\n"
"# Params:        -O3 -std=c++11 -fno-rtti -Wall\n"
"# Watchdog:       on every evaluation for 4 sec\n"
"# Program size:        29278 bytes (89.3% Full)\n"
"# Data size:             539 bytes (26.3% Full)\n";

const char get_FREEZED[] PROGMEM	= "# !!! THIS DEVICE HAS BEEN FREEZED DURING LAST SCRIPT EVALUATION !!!\n";

const char get_RETURN[] PROGMEM	= "return ";

const char get_SHRD[] PROGMEM		= "set SHRD ";
const char get_SHRE[] PROGMEM		= "set SHRE ";
const char get_PGA0[] PROGMEM		= "set PGA0 ";
const char get_PGA1[] PROGMEM		= "set PGA1 ";
const char get_WORK[] PROGMEM		= "set WORK ";
const char get_LINE[] PROGMEM		= "set LINE ";
const char get_FRAM[] PROGMEM		= "set FRAM ";
const char get_VARS[] PROGMEM		= "set VARS ";

const char get_SET[] PROGMEM		= "set ";

const char get_EOC[] PROGMEM		= EOC;

extern KAUart		UART;
extern KASpi		SPI;
extern KAFlash		Flash;

extern KLVariables	Inputs;
extern KLScript	Script;

extern DEVICE		Monitor;
extern SHIFT		Shift;
extern PGA		Gains;

extern double		Analog[];

void SHR_SetOutputs(char Mask)
{
	SPI.Send(Shift.Values = Mask);

	KAOutput::SwitchState(SHR_CS, 2, 0);

	if (Monitor.Online) SYS_SendFeedback(GET_SHRD);
}

void SHR_SetState(bool Enable)
{
	if (Enable != Shift.Enable)
	{
		if (Enable)
		{
			SPI.Send(Shift.Values);

			KAOutput::SwitchState(SHR_CS, 3, 0);
		}
		else KAOutput::SwitchState(SHR_CS, 1, 0);

		KAOutput::SetState(SHR_LED, Enable);

		Shift.Enable = Enable;
	}

	if (Monitor.Online) SYS_SendFeedback(GET_SHRE);
}

int SHR_SetPin(char Pin, bool Enable)
{
	if (Pin > 7 || Pin < 0) return WRONG_SHR_PIN;

	if (Enable) Shift.Values |= (1 << Pin);
	else Shift.Values &= ~(1 << Pin);

	SPI.Send(Shift.Values);

	KAOutput::SwitchState(SHR_CS, 2, 0);

	if (Monitor.Online) SYS_SendFeedback(GET_SHRD);

	return 0;
}

char PGA_GetMask(char Gain)
{
	switch (Gain)
	{
		case 1:	return 0b000;
		case 2:	return 0b001;
		case 4:	return 0b010;
		case 5:	return 0b011;
		case 8:	return 0b100;
		case 10:	return 0b101;
		case 16:	return 0b110;
		case 32:	return 0b111;

		default: return 0b11110000;
	}
}

int PGA_SetGain(char ID, char Gain)
{
	if (ID != 0 && ID != 1) return WRONG_PGA_ID;

	char Mask = PGA_GetMask(Gain);

	if (Mask & 0b11110000) return WRONG_PGA_GAIN;

	SPI.Select(KAPin::PORT_B, ID == 0 ? PORT_1 : PORT_0);
	SPI << 0b01000000 << Mask;
	SPI.Unselect(KAPin::PORT_B, ID == 0 ? PORT_1 : PORT_0);

	(ID == 0 ? Gains.Gain_0 : Gains.Gain_1) = Gain;

	if (Monitor.Online) SYS_SendFeedback(ID == 0 ? GET_PGA0 : GET_PGA1);

	return 0;
}

bool ADC_SendFeedback(const KLString& ID)
{
	if (!Script.Variables.Exists(ID)) return false;

	UART << PGM_V get_SET << ID << ' ' << Script.Variables[ID].ToString() << EOC;

	return true;
}

void SYS_SendFeedback(char Mask)
{
	if (Mask & GET_SHRD) UART << PGM_V get_SHRD << int(Shift.Values)			<< PGM_V get_EOC;
	if (Mask & GET_SHRE) UART << PGM_V get_SHRE << int(Shift.Enable)			<< PGM_V get_EOC;
	if (Mask & GET_PGA0) UART << PGM_V get_PGA0 << int(Gains.Gain_0)			<< PGM_V get_EOC;
	if (Mask & GET_PGA1) UART << PGM_V get_PGA1 << int(Gains.Gain_1)			<< PGM_V get_EOC;
	if (Mask & GET_WORK) UART << PGM_V get_WORK << int(Monitor.Master)		<< PGM_V get_EOC;
	if (Mask & GET_LINE) UART << PGM_V get_LINE << int(Monitor.Online)		<< PGM_V get_EOC;
	if (Mask & GET_FRAM) UART << PGM_V get_FRAM << FREE_RAM 				<< PGM_V get_EOC;
}

int SYS_SetStatus(char Mask)
{
	switch (Mask)
	{
		case WORK_OFFLINE:

			Monitor.Online = false;

		break;

		case WORK_ONLINE:

			Monitor.Online = true;

			SYS_SendFeedback(GET_ALL);

		break;

		case WORK_SLAVE:
		case WORK_MASTER:

			Monitor.Master = (Mask == WORK_MASTER);

			if (Monitor.Online) SYS_SendFeedback(GET_WORK);

			KAOutput::SetState(ACT_LED, Monitor.Master);

		break;
		case UPLOAD_CODE:

			Flash.SetAdress(0); do  UART << (Mask = Flash.Read()); while (Mask);

		break;
		case DOWNLOAD_CODE:

			Flash.SetAdress(0); do Flash.Write(Mask = UART.Recv()); while (Mask);

		break;

		case CLEAN_RAM:

			Script.Variables.Clean();

		break;

		case INFO_STR:

			UART << PGM_V get_INFOSTR;

		break;

		default: return WRONG_SYS_CODE;
	}

	return 0;
}

void SYS_Evaluate(KLString& Buffer)
{
	KAOutput::SetState(ACT_LED, !Monitor.Master);

	for (int i = 0; i < 6; ++i) Analog[i] = KAConverter::GetVoltage(KAConverter::PORT(i));

	wdt_intenable(WDTO_4S);

	const bool OK = Script.Evaluate(Buffer);

	wdt_disable();

	if (Monitor.Online && !Monitor.Master) UART << PGM_V get_RETURN << (OK ? Script.GetReturn() : WRONG_SCRIPT) << PGM_V get_EOC;

	KAOutput::SetState(ACT_LED, Monitor.Master);

	Buffer.Clean();
}

void SYS_InitDevice(char Boot)
{
	char Buff[] = "Vx";

	// setup interrupts
	KAInt::SetMode(KAInt::INT_0, KAInt::ON_RISING);
	KAInt::SetMode(KAInt::INT_1, KAInt::ON_RISING);

	// enable interrupts
	KAInt::Enable(KAInt::INT_0);
	KAInt::Enable(KAInt::INT_1);

	// setup outputs
	KAOutput::SetState(KAPin::PORT_D, 0b00110000, true);
	KAOutput::SetState(KAPin::PORT_D, 0b11000000, false);
	KAOutput::SetState(KAPin::PORT_B, 0b00000011, true);

	// setup bindings

	Script.Bindings.Add(BIND(get));
	Script.Bindings.Add(BIND(put));
	Script.Bindings.Add(BIND(pga));
	Script.Bindings.Add(BIND(out));
	Script.Bindings.Add(BIND(sys));
	Script.Bindings.Add(BIND(dev));
	Script.Bindings.Add(BIND(spi));
	Script.Bindings.Add(BIND(slp));

	// setup adc variables
	for (char i = 0; i < 6; i++) { Buff[1] = '0' + i; Inputs.Add(Buff, Analog[i]); }

	// enable uart
	UART.Start();

	if (Boot & 0b00001000) switch (KAFlash::Read(1023))
	{
		case MASTER_FROZEN:
			UART << PGM_V get_FREEZED << PGM_V get_RETURN << MASTER_TIMEOUT << PGM_V EOC; Monitor.Online = true;
		break;
		case REMOTE_FROZEN:
			UART << PGM_V get_FREEZED << PGM_V get_RETURN << REMOTE_TIMEOUT << PGM_V EOC; Monitor.Online = true;
		break;
	}
}
