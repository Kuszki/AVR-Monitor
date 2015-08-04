/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Bindable functions for AVR-Monitor UC program                          *
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

#include "bindings.hpp"

extern KASpi	SPI;

extern DEVICE	Monitor;

double get(KLVariables& Vars)
{
	if (Vars.Size() == 0)
	{
		ADC_SendSensors();
	}
	else for (const auto& Var: Vars)
	{
		if (!ADC_SendFeedback(KLString('V') + KLString(Var.Value.ToInt()))) return WRONG_ADC_ID;
	}

	return 0;
}

double put(KLVariables& Vars)
{
	if (Vars.Size() == 1)
	{
		SHR_SetOutputs(Vars["0"].ToInt());
	}
	else if (Vars.Size() == 2)
	{
		return SHR_SetPin(Vars["0"].ToInt(), Vars["1"].ToBool());
	}
	else if (Vars.Size() == 8)
	{
		char Mask = 0;

		for (char i = 0; i < 8; i++) Mask |= (Vars[KLString(int(i))].ToBool() << i);

		SHR_SetOutputs(Mask);
	}
	else return WRONG_PARAMS;

	return 0;
}

double pga(KLVariables& Vars)
{
	if (Vars.Size() != 2) return WRONG_PARAMS;

	return PGA_SetGain(Vars["0"].ToInt(), Vars["1"].ToInt());
}

double out(KLVariables& Vars)
{
	if (Vars.Size() != 1) return WRONG_PARAMS;

	SHR_SetState(Vars["0"].ToBool());

	return 0;
}

double sys(KLVariables& Vars)
{
	if (!Monitor.Online) return WRONG_SYS_STATE;

	if (Vars.Size() == 0)
	{
		SYS_SendFeedback(GET_ALL);
	}
	else if (Vars.Size() == 1)
	{
		SYS_SendFeedback(Vars["0"].ToInt());
	}
	else return WRONG_PARAMS;

	return 0;
}

double dev(KLVariables& Vars)
{
	if (Vars.Size() != 2) return WRONG_PARAMS;

	return SYS_SetStatus(Vars["0"].ToInt(), Vars["1"].ToInt());
}

double spi(KLVariables& Vars)
{
	if (Vars.Size() == 0) return WRONG_PARAMS;

	SPI.Select(SPI_CS);

	for (const auto& Var: Vars) SPI << Var.Value.ToInt();

	SPI.Unselect(SPI_CS);

	return 0;
}

double slp(KLVariables& Vars)
{
	if (Vars.Size() != 1) return WRONG_PARAMS;

	unsigned i = Vars["0"].ToInt(); DELAY(i);

	return 0;
}
