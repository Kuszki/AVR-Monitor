/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Bindable functions for AVR-Monitor UC program                          *
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

#include "bindings.hpp"

extern KASpi	SPI;

double get(KLList<double>& Vars)
{
	if (Vars.Size() == 0)
	{
		ADC_SendSensors();
	}
	else for (const auto& Var: Vars)
	{
		ADC_GetFeedback(int(Var));
	}

	return NAN;
}

double put(KLList<double>& Vars)
{
	if (Vars.Size() == 1)
	{
		SHR_SetOutputs(int(Vars[0]));
	}
	else if (Vars.Size() == 2)
	{
		int Code = SHR_SetPin(int(Vars[0]), bool(Vars[1]));
		return Code ? Code : NAN;
	}
	else if (Vars.Size() == 8)
	{
		char Mask = 0, i = 0;

		for (const auto& Var: Vars) Mask |= (bool(Var) << i++);

		SHR_SetOutputs(Mask);
	}
	else return SYS_PostError(WRONG_PARAMS);

	return NAN;
}

double pga(KLList<double>& Vars)
{
	if (Vars.Size() != 2) return SYS_PostError(WRONG_PARAMS);

	int Code = PGA_SetGain(int(Vars[0]), int(Vars[1]));

	return Code ? Code : NAN;
}

double out(KLList<double>& Vars)
{
	if (Vars.Size() != 1) return SYS_PostError(WRONG_PARAMS);

	SHR_SetState(bool(Vars[0]));

	return NAN;
}

double sys(KLList<double>& Vars)
{
	if (Vars.Size() == 0)
	{
		SYS_SendFeedback(GET_ALL);
	}
	else if (Vars.Size() == 1)
	{
		SYS_SendFeedback(int(Vars[0]));
	}
	else return SYS_PostError(WRONG_PARAMS);

	return NAN;
}

double dev(KLList<double>& Vars)
{
	if (Vars.Size() != 2) return SYS_PostError(WRONG_PARAMS);

	int Code = SYS_SetStatus(int(Vars[0]), int(Vars[1]));

	return Code ? Code : NAN;
}

double spi(KLList<double>& Vars)
{
	if (Vars.Size() == 0) return SYS_PostError(WRONG_PARAMS);

	SPI.Select(SPI_CS);
	for (const auto& Var: Vars) SPI << int(Var);
	SPI.Unselect(SPI_CS);

	return NAN;
}

double pwm(KLList<double>& Vars)
{
	if (Vars.Size() != 1) return SYS_PostError(WRONG_PARAMS);

	PWM_SetPulse(int(Vars[0]));

	return NAN;
}

double slp(KLList<double>& Vars)
{
	if (Vars.Size() != 1) return SYS_PostError(WRONG_PARAMS);

	DELAY(int(Vars[0]));

	return NAN;
}

