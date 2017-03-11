/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Program memory constants of AVR-Monitor UC program                     *
 *  Copyright (C) 2016  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
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

#ifndef _AVR_MONITOR_UC_PROGMEM
#define _AVR_MONITOR_UC_PROGMEM

#include "defines.hpp"
#include "codes.hpp"

const char get_INFOSTR[] PROGMEM =
"\n"
"# AVR-Monitor device;   Controller program v1.8\n"
"# Copyright (C) 2015     Łukasz \"Kuszki\" Dróżdż\n"
"#\n"
"# Info at https://github.com/Kuszki/AVR-Monitor\n"
"#\n"
"# Release date:            " __DATE__ " " __TIME__ "\n"
"# Used tools:         GNU AVR-GCC version " __VERSION__ "\n"
"# Built on:    Debian 9 GNU/Linux 4.9.0-2-amd64\n"
"# GCC flags:    -Os -mmcu=atmega328p -std=c++11\n"
"# Watchdog set:   on every evaluation for 8 sec\n"
"#\n"
"# Program size:        24382 bytes (74.4% Full)\n"
"# Data size:             522 bytes (25.5% Full)\n"
"\n";

const char get_GAINS[]	PROGMEM 	= { 0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111, 0b11110000 };

const char get_EXPORT[]	PROGMEM	= "export ";
const char get_RETURN[]	PROGMEM	= "return ";

const char get_LINE[]	PROGMEM	= "set LINE ";
const char get_WORK[]	PROGMEM	= "set WORK ";
const char get_SHRD[]	PROGMEM	= "set SHRD ";
const char get_SHRE[]	PROGMEM	= "set SHRE ";
const char get_PGA0[]	PROGMEM	= "set PGA0 ";
const char get_PGA1[]	PROGMEM	= "set PGA1 ";
const char get_PWMV[]	PROGMEM	= "set PWMV ";
const char get_SLPT[]	PROGMEM	= "set SLPT ";
const char get_FRAM[]	PROGMEM	= "set FRAM ";

const char get_FAIL[]	PROGMEM	= "fail ";
const char get_CALL[]	PROGMEM	= "call ";
const char get_SET[]	PROGMEM	= "set ";

const char get_SOS[]	PROGMEM	= SOS;
const char get_EOS[]	PROGMEM	= EOS;

const char get_EOC[]	PROGMEM	= EOC;

#endif

