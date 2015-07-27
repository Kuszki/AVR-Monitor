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

#include <util/delay.h>

#include "defines.hpp"
#include "bindings.hpp"

KAUart		UART(57600);
KASpi		SPI(KASpi::MASTER);
KLScript		Script;

int main(void)
{

	// setup local variables
	KLString		Buffer;
	size_t		Pos = 0;

	// setup interrupts
	KAInt::SetMode(KAInt::INT_0, KAInt::ON_RISING);
	KAInt::SetMode(KAInt::INT_1, KAInt::ON_RISING);

	// enable interrupts
	KAInt::Enable(KAInt::INT_0);
	KAInt::Enable(KAInt::INT_1);

	// setup outputs
	KAOutput::SetState(KAPin::PORT_D, 0b11010000, true);
	KAOutput::SetState(KAPin::PORT_D, 0b00100000, false);
	KAOutput::SetState(KAPin::PORT_B, 0b00000011, true);

	// setup bindings
	Script.Bindings.Add(BIND(shift));
	Script.Bindings.Add(BIND(pull));
	Script.Bindings.Add(BIND(get));

	// setup input buffer
	Buffer.Reserve(150);

	// enable uart
	UART.Start();

	while (true)
	{
		while (UART.Ready())
		switch (char c = UART.Recv())
		{
			case '\n':

				Buffer[Pos] = 0; Pos = 0; Buffer.Refresh();

				Script.Evaluate(Buffer);

				UART << "free ram: " << FREE_RAM << "B\n"
					<< "msg: " << Buffer << " [" << Buffer.Size() << "]\n"
					<< "ok: " << (Script.GetError() == KLScript::NO_ERROR) << '\n'
					<< "return: " << Script.GetReturn() << '\n';

			break;
			default:

				Buffer[Pos++] = c;

			break;
		}
	}

	return 0;

}

ISR (INT0_vect)
{
	static bool yup = true;

	KAOutput::SetState(PD_7, yup);

	yup = !yup;
}

ISR (INT1_vect)
{

}
