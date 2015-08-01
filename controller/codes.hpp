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

// dev work status
#define WORK_OFFLINE 	0			//!< Pozostań offline.
#define WORK_ONLINE		1			//!< Przejdź online.
#define WORK_SLAVE		2			//!< Pracuj jako slave.
#define WORK_MASTER		3			//!< Pracuj jako master.

// dev memory
#define CLEAN_RAM		10			//!< Wyczyść dodatkowe zmienne.

// dev special tasks
#define INFO_STR		253			//!< Wyślij informacje.
#define UPLOAD_CODE		254			//!< Wyślij kod skryptu.
#define DOWNLOAD_CODE	255			//!< Pobierz skrypt.

// script error code macro
#define WRONG_SCRIPT	-255			//!< Skrypt nieprawidłowy.
#define MASTER_TIMEOUT	-254			//!< Przekroczono limit czasu przez użytkownika.
#define REMOTE_TIMEOUT	-253			//!< Przekroczono limit czasu przez użądzenie.

// frozen conditions
#define MASTER_FROZEN	0b10000000	//!< Użytkownik dokonał zamrożenia kodu.
#define REMOTE_FROZEN	0b10001000	//!< Użądzenie dokonało zamrożenia kodu.

// common error codes macros
#define WRONG_PARAMS	-1			//!< Błędnie parametry funkcji.

// shr error codes macros
#define WRONG_SHR_PIN	-2			//!< Błędne wyprowadzenie rejestru.

// pga error codes macros
#define WRONG_PGA_ID	-3			//!< Błędne ID wzmacniacza.
#define WRONG_PGA_GAIN	-4			//!< Błędnie wzmocnienie PGA.

// adc error codes macros
#define WRONG_ADC_ID	-5			//!< Błędne ID przetwornika ADC.

// system error codes
#define WRONG_SYS_CODE	-6			//!< Błędny kod systemowy.
#define WRONG_SYS_STATE	-7			//!< Błędny kod stanu.
#define WRONG_SYS_UPLOAD	-8			//!< Załadowano błędny skrypt.

// sys var pulling codes
#define GET_SHRD		1			//!< Pobierz stan danych rejestru.
#define GET_SHRE		2			//!< Pobierz stan wyprowadzeń rejesru.
#define GET_PGA0		4			//!< Pobierz wzmocnienie wzmacniacza 0.
#define GET_PGA1		8			//!< Pobierz wzmocnienie wzmacniacza 1.
#define GET_WORK		16			//!< Pobierz tryb pracy.
#define GET_LINE		32			//!< Pobierz stan połączenia.
#define GET_FRAM		64			//!< Pobierz ilość owlnej pamięci RAM.
#define GET_ALL		255			//!< Pobierz wszystkie dane użądzenia.

#endif
