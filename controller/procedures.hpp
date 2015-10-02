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

#include "../../KLLibs/KLLibs.hpp"
#include "../../KALibs/KALibs.hpp"

#include "codes.hpp"
#include "defines.hpp"
#include "bindings.hpp"

/*! \brief		Ustala wyjścia na podstawie maski.
 *  \param [in]	Mask Maska bitowa wyprowadzeń.
 *
 * Wyprowadza na wyjście rejestru wybrane stany logiczne. Nie zmienia jednak stanu na wyjściach gdy rejestr był w stanie wysokiej impedancji.
 *
 */
void SHR_SetOutputs(char Mask);


/*! \brief		Ustala stan wyjścia na podstawie parametry.
 *  \param [in]	Enable Aktywność wyjść.
 *
 * Gdy wybrano aktywacje wejść na pinach pojawią sie wpisane wsześniej stany logiczne.
 *
 */
void SHR_SetState(bool Enable);

/*! \brief		Ustala wyjście pojedynczego pinu na zadaną wartość.
 *  \param [in]	Pin	ID pinu.
 *  \param [in]	Enable;
 *  \return		Powodzenie operacji.
 *
 * Wyprowadza na wyjście rejestru zadany stan logiczny zmieniając tylko wybraną wartość. Nie zmienia stanu na wyjściach gdy rejestr był w stanie wysokiej impedancji.
 *
 */
int SHR_SetPin(char Pin, bool Enable);


/*! \brief		Ustala wzmocnienie wybranego układu.
 *  \param [in]	Pin	ID wzmacniacza.
 *  \param [in]	Gain	Wzmocnienie.
 *  \return		Powodzenie operacji.
 *
 * Wprowadza do wybranego wzmacniacza podane wzmocnienie.
 *
 */
int PGA_SetGain(char ID, char Gain);

/*! \brief		Wyznacza maskę bitową na podstawie wzmocnienia.
 *  \param [in]	Gain	Wzmocnienie.
 *  \return		Maska bitowa .
 *
 * Oblicza maskę potrzebną do osiągnięcia wybranego wzmocnienia.
 *
 */
char PGA_GetMask(char Gain);

/*! \brief		Odświerza stan wybranego przetwornika.
 *  \param [in]	ID ID przetwornika.
 *  \return		Powodzenie operacji.
 *
 * Odświerza dane przetwornika w trakcie wykonywania skryptu.
 *
 */
bool ADC_GetFeedback(char ID);

/*! \brief		Wysyła do użytkownika stan wszystkich przetworników.
 *
 * Wysyła do użytkownika informację o napięciu na wszystkich czujnikach.
 *
 */
void ADC_SendSensors(void);

/*! \brief		Wysyła do użytkownika stan wybranych zmiennych systemowych.
 *  \param [in]	Maks	Maska wyprowadzeń.
 *
 * Wysyła do użytkownika informację o stanach wybranych zmiennych.
 *
 */
void SYS_SendFeedback(char Mask);

/*! \brief		Wysyła do użytkownika wartości wszystkich zmiennych.
 *
 * Wysyła do użytkownika informację o zadeklarowanych zmiennych.
 *
 */
void SYS_SendVariables(void);

/*! \brief		Ustala stan pracy przekazany przez użytkowika.
 *  \param [in]	Maks		Polecenie do wykonania.
 *  \param [in]	Value	Parametr polecenia.
 *  \return		Powodzenie operacji.
 *
 * Wysyła do użytkownika informację o napięciu na wybranym czujniku.
 *
 */
int SYS_SetStatus(char Mask, char Value);

/*! \brief		Wykonuje skrypt.
 *  \param [in]	Buffer	Skrypt do wykonania.
 *
 * Wykonuje zadany skrypt.
 *
 */
void SYS_Evaluate(KLString& Buffer);

/*! \brief		Inicjacja urządzeń.
 *  \param [in]	Boot Sygnał resetu.
 *
 * Inicjuje urządzenie biorąc pod uwagę przyczynę resetu.
 *
 */
void SYS_InitDevice(char Boot);

/*! \brief		Zgłoszenie błędu.
 *  \param [in]	Code Kod błędu.
 *  \return		Kod błędu.
 *
 *  Zgłasza wykryty w skrypcie błąd.
 *
 */
int SYS_PostError(int Code);

#endif
