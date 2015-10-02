/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Bindable functions for AVR-Monitor UC program                          *
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

#ifndef _AVR_MONITOR_UC_BINDINGS
#define _AVR_MONITOR_UC_BINDINGS

#include "../../KLLibs/KLLibs.hpp"
#include "../../KALibs/KALibs.hpp"

#include "codes.hpp"
#include "defines.hpp"
#include "procedures.hpp"

/*! \brief		Bind funkcji `get`.
 *  \param [in]	Vars Przestrzeń zmiennych.
 *  \return		WRONG_PARAMS w przypadku nieprawidłowej ilości parametrów.
 *  				WRONG_ADC_ID w przypadku błędnego indeksu.
 *  				0 w przypadku powodzenia operacji.
 *
 * Umożliwia wysłanie zapytania o aktualny stan napięć wejściowych przetworników ADC.
 *
 * `call get ID_1 [, ID_1[, ...]]; # pobiera dane z wymienionych czujników.
 * `call get; # pobiera stan wszystkich czujników`
 *
 */
double get(KLVariables& Vars);

/*! \brief		Bind funkcji `put`.
 *  \param [in]	Vars Przestrzeń zmiennych.
 *  \return		WRONG_PARAMS w przypadku nieprawidłowej ilości parametrów.
 *  				WRONG_SHR_PIN w przypadku błędnego indeksu.
 *  				0 w przypadku powodzenia operacji.
 *
 * Umożliwia ustalenie stanu rejestru wyjściowego.
 *
 * `call put Słowo; # ustala słowo na wyjściu`
 * `call put ID, Stan; # ustala stan wybranego pinu`
 * `call put S0, S1, S2, S3, S4, S5, S6, S7; # ustala stany na kolejnych pinach`
 *
 */
double put(KLVariables& Vars);

/*! \brief		Bind funkcji `pga`.
 *  \param [in]	Vars Przestrzeń zmiennych.
 *  \return		WRONG_PARAMS w przypadku nieprawidłowej ilości parametrów.
 *  				WRONG_PGA_ID w przypadku błędnego indeksu.
 *  				WRONG_PGA_GAIN w przypadku błędnego wzmocnienia.
 *  				0 w przypadku powodzenia operacji.
 *
 * Umożliwia wybór wzmocnienia układu PGA.
 *
 * `call pga ID, Wzmocnienie; # ustala wzmocnienie wybranego układu`
 *
 */
double pga(KLVariables& Vars);

/*! \brief		Bind funkcji `out`.
 *  \param [in]	Vars Przestrzeń zmiennych.
 *  \return		WRONG_PARAMS w przypadku nieprawidłowej ilości parametrów.
 *  				0 w przypadku powodzenia operacji.
 *
 * Umożliwia ustalenie stanu rejestru wyjściowego na wysoką impedancje.
 *
 * `call out OE; # ustala aktywność wyjść układu`
 *
 */
double out(KLVariables& Vars);

/*! \brief		Bind funkcji `sys`.
 *  \param [in]	Vars Przestrzeń zmiennych.
 *  \return		WRONG_PARAMS w przypadku nieprawidłowej ilości parametrów.
 *  				WRONG_SYS_STATE w przypadku wywołania gdy urządzenie nie pracuje `online`.
 *  				0 w przypadku powodzenia operacji.
 *
 * Umożliwia pobranie stanu zmiennych systemowych.
 *
 * `call sys [Maska]; # pobiera stan zmiennych systemowych które pasują do maski, lub wszystkich zmiennych jeśli nie podano maski`
 *
 */
double sys(KLVariables& Vars);

/*! \brief		Bind funkcji `dev`.
 *  \param [in]	Vars Przestrzeń zmiennych.
 *  \return		WRONG_PARAMS w przypadku nieprawidłowej ilości parametrów.
 *  				WRONG_SYS_CODE w przypadku podania błędnego parametru.
 *  				0 w przypadku powodzenia operacji.
 *
 * Umożliwia modyfikacje stanu połączenia i pracy urządzenia.
 *
 * `call dev Polecenie; # wysyła do urządzenia specjalne polecenie mogące zmienić jego stan pracy`
 *
 */
double dev(KLVariables& Vars);

/*! \brief		Bind funkcji `spi`.
 *  \param [in]	Vars Przestrzeń zmiennych.
 *  \return		WRONG_PARAMS w przypadku nieprawidłowej ilości parametrów.
 *  				0 w przypadku powodzenia operacji.
 *
 * Umożliwia wysłanie do innego urządzenia danych za pomocą portu SPI. Na wyprowadzeniu SPI dostępntm na obudowie uaktywniony zostanie sygnał CS i wysłane zostaną kolejne bajty danych.
 *
 * `call spi Bajt [, Bajt_1[, Bajt 2[, ...]]] ; # wysyła kolejne bajty na wyjście SPI`
 *
 */
double spi(KLVariables& Vars);

/*! \brief		Bind funkcji `slp`.
 *  \param [in]	Vars Przestrzeń zmiennych.
 *  \return		WRONG_PARAMS w przypadku nieprawidłowej ilości parametrów.
 *  				0 w przypadku powodzenia operacji.
 *  \warning		Zbyt duży czas oczekiwania może spowodować aktywacje mechanizmu `watchdog` i reset mikrokontrolera.
 *
 * Umożliwia wstrzymanie pracy na n milisekund.
 *
 * `call slp Milisekundy; # wstrzymuje pracę na n milisekund`
 *
 */
double slp(KLVariables& Vars);

#endif
