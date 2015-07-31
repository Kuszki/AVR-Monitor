/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Main.cpp file from AVR-Monitor UC program                              *
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

#include "KLLibs/KLLibs.hpp"
#include "KALibs/KALibs.hpp"

#include "codes.hpp"
#include "defines.hpp"
#include "bindings.hpp"
#include "procedures.hpp"

// main control objects
KAUart		UART(57600);
KASpi		SPI(KASpi::MASTER);
KAFlash		Flash;

// main script interpreter
KLVariables	Inputs;
KLScript		Script(&Inputs);

// global program structs
DEVICE		Monitor	= {false, false};
SHIFT		Shift	= {false, 0b00000000};
PGA			Gains	= {1, 1};

double		Analog[]	= {0, 0, 0, 0, 0, 0};

int main(void)
{

	// global system init
	SYS_InitDevice();

	// setup input buffers
	KLString Serial;
	KLString Master;

	// a main loop
	while (true)
	{

		// handle serial event
		while (UART.Ready())
		{
			Serial << UART.Recv();

			if (Serial.Last() == RUN)
			{
				SYS_Evaluate(Serial);
			}
		}

		// enter master device loop
		if (Monitor.Master)
		{
			Flash.SetAdress(0);

			while (Master.Insert(Flash.Read()) != -1)
			if (Master.Last() == RUN)
			{
				SYS_Evaluate(Master);
			}

			if (Flash.GetAdress() == 1)
			{
				SYS_SetStatus(WORK_SLAVE);
			}
			else if (Monitor.Online)
			{
				for (const auto& Var: Script.Variables) UART << "set " << Var.ID << ' ' << Var.Value.ToString() << EOC;
			}

			Script.Variables.Clean();
		}

	}

}
