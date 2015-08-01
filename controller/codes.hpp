/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Codes definitions for AVR-Monitor UC program                           *
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

#ifndef _AVR_MONITOR_UC_CODES
#define _AVR_MONITOR_UC_CODES

#define BUF_LEN		48

// special control codes
#define SOH			1
#define STX			2
#define ETX			3
#define EOT			4

// dev work status
#define WORK_OFFLINE 	0
#define WORK_ONLINE		1
#define WORK_SLAVE		2
#define WORK_MASTER		3

// dev flash tasks
#define UPLOAD_CODE		4
#define DOWNLOAD_CODE	5

// dev memory
#define CLEAN_RAM		6

// dev infostring
#define INFO_STR		10

// script error code macro
#define WRONG_SCRIPT	-255

// script timeout watchdog
#define MASTER_TIMEOUT	-254
#define REMOTE_TIMEOUT	-253

// frozen conditions
#define MASTER_FROZEN	0b10000000
#define REMOTE_FROZEN	0b10001000

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
#define WRONG_SYS_STATE	-7
#define WRONG_SYS_UPLOAD	-8

// sys var pulling codes
#define GET_SHRD		1
#define GET_SHRE		2
#define GET_PGA0		4
#define GET_PGA1		8
#define GET_WORK		16
#define GET_LINE		32
#define GET_FRAM		64
#define GET_VARS		128
#define GET_ALL		255

#endif
