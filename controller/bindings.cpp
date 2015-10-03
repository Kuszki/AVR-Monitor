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

double get(KLVariables& Vars)
{
	if (Vars.Size() == 0)
	{
		ADC_SendSensors();
	}
	else for (const auto& Var: Vars)
	{
		ADC_GetFeedback(Var.Value.ToInt());
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
		char Mask = 0, i = 0;

		for (const auto& Var: Vars) Mask |= (Var.Value.ToBool() << i);

		SHR_SetOutputs(Mask);
	}
	else return SYS_PostError(WRONG_PARAMS);

	return 0;
}

double pga(KLVariables& Vars)
{
	if (Vars.Size() != 2) return SYS_PostError(WRONG_PARAMS);

	return PGA_SetGain(Vars["0"].ToInt(), Vars["1"].ToInt());
}

double out(KLVariables& Vars)
{
	if (Vars.Size() != 1) return SYS_PostError(WRONG_PARAMS);

	SHR_SetState(Vars["0"].ToBool());

	return 0;
}

double sys(KLVariables& Vars)
{
	if (Vars.Size() == 0)
	{
		SYS_SendFeedback(GET_ALL);
	}
	else if (Vars.Size() == 1)
	{
		SYS_SendFeedback(Vars["0"].ToInt());
	}
	else return SYS_PostError(WRONG_PARAMS);

	return 0;
}

double dev(KLVariables& Vars)
{
	if (Vars.Size() != 2) return SYS_PostError(WRONG_PARAMS);

	return SYS_SetStatus(Vars["0"].ToInt(), Vars["1"].ToInt());
}

double spi(KLVariables& Vars)
{
	if (Vars.Size() == 0) return SYS_PostError(WRONG_PARAMS);

	SPI.Select(SPI_CS);
	for (const auto& Var: Vars) SPI << Var.Value.ToInt();
	SPI.Unselect(SPI_CS);

	return 0;
}

double slp(KLVariables& Vars)
{
	if (Vars.Size() != 1) return SYS_PostError(WRONG_PARAMS);

	DELAY(Vars["0"].ToInt());

	return 0;
}
