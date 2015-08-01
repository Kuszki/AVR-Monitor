/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Macros and definitions for AVR-Monitor UC program                      *
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

#ifndef _AVR_MONITOR_UC_DEFINES
#define _AVR_MONITOR_UC_DEFINES

#include "KALibs/KALibs.hpp"

#define STR_M(x)		#x

// end of command
#define EOC			";\n"
#define RUN			'\n'

// cs pins macros
#define SHR_CS			PD_5
#define PGA0_CS		PB_1
#define PGA1_CS		PB_0
#define SPI_CS			PD_4

// led pins macros
#define ACT_LED		PD_7
#define SHR_LED		PD_6

// struct declarations
struct DEVICE
{
	bool Online;
	bool Master;
	bool Worker;
};

struct SHIFT
{
	bool Enable;
	char Values;
};

struct PGA
{
	char Gain_0;
	char Gain_1;
};

#endif
