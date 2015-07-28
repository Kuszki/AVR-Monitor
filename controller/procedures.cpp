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

extern KAUart	UART;
extern KASpi	SPI;

extern DEVICE	Monitor;
extern SHIFT	Shift;
extern PGA	Gains;

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

int PGA_SetGain(char ID, char Gain)
{
	if (ID != 0 && ID != 1) return WRONG_PGA_ID;

	char Mask = 0b11111111;

	switch (Gain)
	{
			case 1:
				Mask = 0b000;
			break;
			case 2:
				Mask = 0b001;
			break;
			case 4:
				Mask = 0b010;
			break;
			case 5:
				Mask = 0b011;
			break;
			case 8:
				Mask = 0b100;
			break;
			case 10:
				Mask = 0b101;
			break;
			case 16:
				Mask = 0b110;
			break;
			case 32:
				Mask = 0b111;
			break;
			default: return WRONG_PGA_GAIN;
	}

	SPI.Select(KAPin::PORT_B, ID == 0 ? PORT_1 : PORT_0);
	SPI << 0b01000000 << Gain;
	SPI.Unselect(KAPin::PORT_B, ID == 0 ? PORT_1 : PORT_0);

	(ID == 0 ? Gains.Gain_0 : Gains.Gain_1) = Gain;

	if (Monitor.Online) SYS_SendFeedback(ID == 0 ? GET_PGA0 : GET_PGA1);

	return 0;
}

bool ADC_SendFeedback(char ID)
{
	if (ID > 5 || ID < 0) return false;

	UART << "set ADC" << char(ID + 48) << " "
		<< KAConverter::GetVoltage(KAConverter::PORT(ID))
		<< EOC;

	return true;
}

void SYS_SendFeedback(char Mask)
{
	if (Mask & GET_SHRD) UART << "set SHRD " << int(Shift.Values) << EOC;
	if (Mask & GET_SHRE) UART << "set SHRE " << int(Shift.Enable) << EOC;
	if (Mask & GET_PGA0) UART << "set PGA0 " << int(Gains.Gain_0) << EOC;
	if (Mask & GET_PGA1) UART << "set PGA0 " << int(Gains.Gain_1) << EOC;
	if (Mask & GET_WORK) UART << "set WORK " << int(Monitor.Master) << EOC;
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

			Monitor.Master = false;

		break;

		case WORK_MASTER:

			Monitor.Master = true;

		break;

		default: return WRONG_SYS_CODE;
	}

	return 0;
}
