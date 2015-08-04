/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Interrupts definitions for AVR-Monitor UC program                      *
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
#include "procedures.hpp"

extern DEVICE	Monitor;
extern SHIFT	Shift;

extern char	Reboot_Code;

void REBOOT_PROC wdt_reboot(void)
{
	Reboot_Code = MCUSR; MCUSR = 0;

	wdt_disable();
}

ISR(INT0_vect)
{
	if (!Monitor.Online) SYS_SetStatus(DEV_MASTER, !Monitor.Master);
}

ISR(INT1_vect)
{
	SHR_SetState(!Shift.Enable);
}

ISR(WDT_vect)
{
	const char Status = (KAFlash::Read(1023) & SLEEP_MSK) | (Monitor.Worker << 7) | (Monitor.Online << 6);

	KAFlash::Write(1023, Status);
}
