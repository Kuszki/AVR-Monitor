/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Codes definitions for AVR-Monitor UC program                           *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
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

// dev commands
#define DEV_LINE		1					//!< Ustal stan połączenia.
#define DEV_MASTER		2					//!< Ustal rolę użądzenia.
#define DEV_SLEEP		3					//!< Ustal czas pomiędzy iteracjami.
#define DEV_DEFAULT		4					//!< Ustal domyślną wartość rejestru.
#define DEV_SCRIPT		5					//!< Pobierz lub wyślij skrypt.
#define DEV_SPEC		6					//!< Opcje specjalne.

// dev special tasks
#define CLEAN_RAM		1					//!< Wyczyść dodatkowe zmienne.
#define POST_FRAM		2					//!< Pobierz ilość wolnej pamięci.

// script error code macro
#define WRONG_SCRIPT	255					//!< Skrypt nieprawidłowy.
#define MASTER_TIMEOUT	254					//!< Przekroczono limit czasu przez użądzenie.
#define REMOTE_TIMEOUT	253					//!< Przekroczono limit czasu przez użytkownika.

// frozen conditions
#define MASTER_FROZEN	0b11000000			//!< Użądzenie dokonało zamrożenia kodu.
#define REMOTE_FROZEN	0b01000000			//!< Użytkownik dokonał zamrożenia kodu.

// common error codes macros
#define WRONG_PARAMS	1					//!< Błędnie parametry funkcji.

// shr error codes macros
#define WRONG_SHR_PIN	2					//!< Błędne wyprowadzenie rejestru.

// pga error codes macros
#define WRONG_PGA_ID	3					//!< Błędne ID wzmacniacza.
#define WRONG_PGA_GAIN	4					//!< Błędnie wzmocnienie PGA.

// adc error codes macros
#define WRONG_ADC_ID	5					//!< Błędne ID przetwornika ADC.

// system error codes
#define WRONG_SYS_CODE	6					//!< Błędny kod systemowy.
#define WRONG_SYS_STATE	7					//!< Błędny kod stanu.
#define WRONG_SYS_UPLOAD	8					//!< Załadowano błędny skrypt.

// sys var pulling codes
#define GET_LINE		1					//!< Pobierz stan połączenia.
#define GET_WORK		2					//!< Pobierz tryb pracy.
#define GET_SHRD		4					//!< Pobierz stan danych rejestru.
#define GET_SHRE		8					//!< Pobierz stan wyprowadzeń rejesru.
#define GET_PGA0		16					//!< Pobierz wzmocnienie wzmacniacza 0.
#define GET_PGA1		32					//!< Pobierz wzmocnienie wzmacniacza 1.
#define GET_PWMV		64					//!< Pobierz wypełnienie wyjścia PWM.
#define GET_SLPT		128					//!< Pobierz czas pomiędzy iteracjami.
#define GET_ALL		255					//!< Pobierz wszystkie dane użądzenia.

// script send strings
#define SOS			"# BEGIN"				//!< Początek skryptu.
#define EOS			"# END"				//!< Koniec skryptu.

#endif
