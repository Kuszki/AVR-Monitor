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

// common string converter
#define STR(x)			#x

// end of command
#define EOC			";\n"
#define EOM			";\n\a"

// special control codes
#define SOH			1
#define STX			2
#define ETX			3
#define EOT			4
#define EXE			10

// dev work status
#define WORK_OFFLINE 	1
#define WORK_ONLINE		2
#define WORK_SLAVE		4
#define WORK_MASTER		8

// script error code macro
#define WRONG_SCRIPT	-255

// common error codes macros
#define WRONG_PARAMS	-1

// shr error codes macros
#define WRONG_SHR_PIN	-2

// pga error codes macros
#define WRONG_PGA_ID	-3
#define WRONG_PGA_GAIN	-4

// adc error codes macros
#define WRONG_ADC_ID	-5

// system error codes
#define WRONG_SYS_CODE	-6

// sys var pulling codes
#define GET_SHRD		1
#define GET_SHRE		2
#define GET_PGA0		4
#define GET_PGA1		8
#define GET_WORK		16
#define GET_ALL		255

// cs pins macros
#define SHR_CS			PD_5
#define PGA0_CS		PB_1
#define PGA1_CS		PB_0

// led pins macros
#define ACT_LED		PD_7
#define SHR_LED		PD_6

// struct declarations
struct DEVICE
{
	bool Master;
	bool Online;
	bool Upload;
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
