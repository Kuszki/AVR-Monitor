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

#include "defines.hpp"
#include "bindings.hpp"
#include "procedures.hpp"

// main control objects
KAUart		UART(57600);
KASpi		SPI(KASpi::MASTER);
KAFlash		Flash;

// main script interpreter
KLScript		Script;

// global program structs
DEVICE		Monitor	= {false, false, false};
SHIFT		Shift	= {false, 0b00000000};
PGA			Gains	= {1, 1};

int main(void)
{

	// setup local objects
	KLString		Buffer;

	// setup local variables
	size_t		Pos 		= 0;

	// setup local states
	bool			OK 		= true;

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
	Script.Bindings.Add(BIND(dev));

	// setup input buffer
	Buffer.Reserve(150);

	// enable uart
	UART.Start();

	while (true)
	{
		while (UART.Ready())
		{

			char c = UART.Recv();

			if (!Monitor.Upload) switch (c)
			{

				case EXE:

					Buffer[Pos] = 0; Pos = 0; Buffer.Refresh();

					OK = Script.Evaluate(Buffer);

					if (Monitor.Online)
					{
						UART << "return ";

						if (OK) UART << Script.GetReturn();
						else UART << STR(WRONG_SCRIPT);

						UART << EOM;
					}

				break;

				case SOH:

					Flash.SetAdress(0);

					Monitor.Upload = true;

					UART << "return 0" << EOM;

				break;

				default: Buffer[Pos++] = c;

			}
			else switch (c)
			{

				case EOT:

					Monitor.Upload = false;

					UART << "return 0" << EOM;

				break;

				default: Flash.Write(c);

			}
		}

		if (Monitor.Master)
		{
			//TODO: implement self-work
		}
	}

	return 0;

}

ISR (INT0_vect)
{
	if (!Monitor.Online) SYS_SetStatus(Monitor.Master ? WORK_SLAVE : WORK_MASTER);
}

ISR (INT1_vect)
{
	SHR_SetState(!Shift.Enable);
}
