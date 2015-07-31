/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Control functions for AVR-Monitor UC program                           *
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

#ifndef _AVR_MONITOR_UC_PROCEDURES
#define _AVR_MONITOR_UC_PROCEDURES

#include "KLLibs/KLLibs.hpp"
#include "KALibs/KALibs.hpp"

#include "codes.hpp"
#include "defines.hpp"
#include "bindings.hpp"

void SHR_SetOutputs(char Mask);

void SHR_SetState(bool Enable);

int SHR_SetPin(char Pin, bool Enable);

int PGA_SetGain(char ID, char Gain);

char PGA_GetMask(char Gain);

bool ADC_SendFeedback(const KLString& ID);

void SYS_SendFeedback(char Mask);

int SYS_SetStatus(char Mask);

void SYS_Evaluate(KLString& Buffer);

void SYS_InitDevice(void);

#endif
