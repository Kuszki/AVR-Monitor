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

#include "../../KALibs/KALibs.hpp"

// end of command
#define EOC			";\n"			//!< Terminator polecenia.
#define RUN			'\n'				//!< Znak "wykonaj".

// cs pins macros
#define SHR_CS			PD_5				//!< Pin wyprowadzenia CS dla rejestru szeregowego.
#define PGA0_CS		PB_1				//!< Pin wyprowadzenia CS dla pierwszego wzmacniacza.
#define PGA1_CS		PB_0				//!< Pin wyprowadzenia CS dla drugiego wzmacniacza.
#define SPI_CS			PD_4				//!< Pin wyprowadzenia CS dla wyprowadzenia SPI.

// led pins macros
#define ACT_LED		PD_7				//!< Pin wyprowadzenia diody aktywności programu.
#define SHR_LED		PD_6				//!< Pin wyprowadzenia diody aktywności wyprowadzeń rejestru szeregowego.

#define ERROR_MSK		0b11000000		//!< Maska błędu przy aktywacji watchdoga.
#define SLEEP_MSK		0b00111111		//!< Maska czasu oczekiwania pomiędzy iteracjami.

#define ADC_COUNT		6				//!< Liczba przetworników ADC.

#define TIME_MEM		1023				//!< Adres pamięci kodów czasu.
#define SHRD_MEM		1022				//!< Adres pamięci danych rejestru.
#define PGA1_MEM		1021				//!< Adres pamięci wzmicnienia wzmacniacza 1.
#define PGA0_MEM		1020				//!< Adres pamięci wzmicnienia wzmacniacza 0.

// struct declarations
struct DEVICE
{
	char Sleep;		//!< Czas oczekiwania pomiędzy kolejnymi iteracjami skryptu (x100 ms).

	bool Online;		//!< Urządzenie online.
	bool Master;		//!< Praca w trybie `master`.
	bool Worker;		//!< Urządzenie wykonujące skrypt (true - użądzenie, false - użytkownik).
};					//!< Struktura opisująca stan urządzenia.

struct SHIFT
{
	bool Enable;		//!< Wyprowadzenia aktywne.
	char Values;		//!< Wyprowadzone dane.
};					//!< Struktura opisująca stan rejestru szeregowego.

struct PGA
{
	char Gain_0;		//!< Wzmocnienie pierwszego wzmacniacza.
	char Gain_1;		//!< Wzmocnienie drugiego wzmacniacza.
};					//!< Struktura opisująca stan wzmacniaczy operacyjnych.

#endif
