/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Control functions for AVR-Monitor UC program                           *
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

#include "procedures.hpp"
#include "progmem.hpp"

extern KAUart		UART;
extern KASpi		SPI;
extern KAFlash		Flash;

extern KLVariables	Inputs;
extern KLScript	Script;

extern DEVICE		Monitor;
extern SHIFT		Shift;
extern PGA		Gains;

extern double		Analog[];

void SHR_SetOutputs(char Mask)
{
	if (Shift.Values != Mask)
	{
		Shift.Values = Mask;

		if (Shift.Enable)
		{
			SPI.Send(Shift.Values);

			KAOutput::SwitchState(SHR_CS, 2, 0);
		}

		if (Monitor.Online) SYS_SendFeedback(GET_SHRD);
	}
}

void SHR_SetState(bool Enable)
{
	if (Enable != Shift.Enable)
	{
		if (Enable) SPI.Send(Shift.Values);

		KAOutput::SwitchState(SHR_CS, Enable ? 3 : 1, 0);
		KAOutput::SetState(SHR_LED, Shift.Enable = Enable);
	}

	if (Monitor.Online) SYS_SendFeedback(GET_SHRE);
}

int SHR_SetPin(char Pin, bool Enable)
{
	if (Pin > 7) return SYS_PostError(WRONG_SHR_PIN);
	else
	{
		SHR_SetOutputs(Enable ? Shift.Values | (1 << Pin) : Shift.Values & ~(1 << Pin));
	}

	return 0;
}

char PGA_GetMask(char Gain)
{
	switch (Gain)
	{
		case 1:	return __LPM(&get_GAINS[0]);
		case 2:	return __LPM(&get_GAINS[1]);
		case 4:	return __LPM(&get_GAINS[2]);
		case 5:	return __LPM(&get_GAINS[3]);
		case 8:	return __LPM(&get_GAINS[4]);
		case 10:	return __LPM(&get_GAINS[5]);
		case 16:	return __LPM(&get_GAINS[6]);
		case 32:	return __LPM(&get_GAINS[7]);

		default: return __LPM(&get_GAINS[8]);
	}
}

int PGA_SetGain(char ID, char Gain)
{
	if (ID > 1) return SYS_PostError(WRONG_PGA_ID);
	else
	{
		char Mask = PGA_GetMask(Gain);

		if (Mask & 0b11110000) return WRONG_PGA_GAIN;

		SPI.Select(KAPin::PORT_B, ID == 0 ? PORT_1 : PORT_0);
		SPI << 0b01000000 << Mask;
		SPI.Unselect(KAPin::PORT_B, ID == 0 ? PORT_1 : PORT_0);

		(ID == 0 ? Gains.Gain_0 : Gains.Gain_1) = Gain;

		KAFlash::Write(ID == 0 ? PGA0_MEM : PGA1_MEM, Gain);

		if (Monitor.Online) SYS_SendFeedback(ID == 0 ? GET_PGA0 : GET_PGA1);
	}

	return 0;
}

void PWM_SetPulse(unsigned char Pulse)
{
	OCR1B = Monitor.Pulse = Pulse;

	if (Monitor.Online) SYS_SendFeedback(GET_PWMV);
}

bool ADC_GetFeedback(char ID)
{
	if (ID > 5) return false;
	else
	{
		Analog[ID] = KAConverter::GetVoltage(KAConverter::PORT(ID));
	}

	return true;
}

void ADC_SendSensors(void)
{
	if (!Monitor.Online) return;

	char i = 0; UART << PGM_V get_CALL << PGM_V get_SET;

	for (const auto& Var: Inputs)
	{
		++i; UART << Var.Value.ToNumber();

		if (i != ADC_COUNT) UART << ',';
		else UART << EOC;
	}
}

void SYS_SendFeedback(char Mask)
{
	if (Mask & GET_LINE) UART << PGM_V get_LINE << int(Monitor.Online)		<< PGM_V get_EOC;
	if (Mask & GET_WORK) UART << PGM_V get_WORK << int(Monitor.Master)		<< PGM_V get_EOC;
	if (Mask & GET_SHRD) UART << PGM_V get_SHRD << int(Shift.Values)			<< PGM_V get_EOC;
	if (Mask & GET_SHRE) UART << PGM_V get_SHRE << int(Shift.Enable)			<< PGM_V get_EOC;
	if (Mask & GET_PGA0) UART << PGM_V get_PGA0 << int(Gains.Gain_0)			<< PGM_V get_EOC;
	if (Mask & GET_PGA1) UART << PGM_V get_PGA1 << int(Gains.Gain_1)			<< PGM_V get_EOC;
	if (Mask & GET_SLPT) UART << PGM_V get_SLPT << int(Monitor.Sleep)		<< PGM_V get_EOC;
	if (Mask & GET_PWMV) UART << PGM_V get_PWMV << int(Monitor.Pulse)		<< PGM_V get_EOC;
}

void SYS_SendVariables(void)
{
	for (const auto& Var: Script.Variables) UART << PGM_V get_EXPORT << Var.Index << ';' << PGM_V get_SET << Var.Index << ' ' << Var.Value.ToNumber() << EOC;
}

int SYS_SetStatus(char Mask, char Value)
{
	switch (Mask)
	{
		case DEV_LINE:

			Monitor.Online = Value;

			if (Monitor.Online) SYS_SendFeedback(GET_ALL);

		break;

		case DEV_MASTER:

			Monitor.Master = Value;

			if (Monitor.Online) SYS_SendFeedback(GET_WORK);
			if (!Value) SYS_SetStatus(DEV_SPEC, CLEAN_RAM);

		break;
		case DEV_SLEEP:

			Monitor.Sleep = Value ? Value : 1;

			if (Monitor.Online) SYS_SendFeedback(GET_SLPT);

			KAFlash::Write(TIME_MEM, Value);

		break;
		case DEV_DEFAULT:

			KAFlash::Write(SHRD_MEM, Value);

		break;
		case DEV_SCRIPT:

			Flash.SetAdress(0);

			if (Value) UART << PGM_V get_SOS << '\n';

			if (Value) while (Mask = Flash.Read()) UART << Mask;
			else do Flash.Write(Mask = UART.Recv()); while (Mask);

			if (Value) UART << PGM_V get_EOS << '\n';

		break;

		case DEV_SPEC:

			switch (Value)
			{
				case CLEAN_RAM:
					Script.Functions.Clean();
					Script.Variables.Clean();

				case POST_FRAM:
					if (Monitor.Online) UART << PGM_V get_FRAM << FREE_RAM << PGM_V get_EOC;
				break;

				default:
					UART << PGM_V get_INFOSTR;
			}

		break;

		default: return SYS_PostError(WRONG_SYS_CODE);
	}

	return 0;
}

void SYS_Evaluate(KLString& Buffer)
{
	for (int i = 0; i < 6; ++i) Analog[i] = KAConverter::GetVoltage(KAConverter::PORT(i));

	KAOutput::SetState(ACT_LED, true);
	wdt_intenable(WDTO_4S);

	const bool OK = Script.Evaluate(Buffer);
	const double Val = Script.GetReturn();

	wdt_disable();
	KAOutput::SetState(ACT_LED, false);

	if (Monitor.Online && !Monitor.Master)
	{
		if (!OK) UART << PGM_V get_CALL << PGM_V get_FAIL << WRONG_SCRIPT << ',' << Script.GetError() << '#' << Buffer << PGM_V get_EOC;
		if (Val) UART << PGM_V get_RETURN << Val << PGM_V get_EOC;
	}

	Buffer.Clean();
}

void SYS_InitDevice(char Boot)
{
	char Buff[] = "Vx";

	// enable uart
	UART.Start();

	// setup interrupts
	KAInt::SetMode(KAInt::INT_0, KAInt::ON_RISING);
	KAInt::SetMode(KAInt::INT_1, KAInt::ON_RISING);

	// enable interrupts
	KAInt::Enable(KAInt::INT_0);
	KAInt::Enable(KAInt::INT_1);

	// det pin mode
	KAOutput::SetOutputMode(KAPin::PORT_D, 0b11110000);
	KAOutput::SetOutputMode(KAPin::PORT_B, 0b00000111);

	// setup outputs
	KAOutput::SetState(KAPin::PORT_D, 0b00110000, true);
	KAOutput::SetState(KAPin::PORT_D, 0b11000000, false);
	KAOutput::SetState(KAPin::PORT_B, 0b00000011, true);
	KAOutput::SetState(KAPin::PORT_B, 0b00000100, false);

	//set saved pga gains
	PGA_SetGain(0, KAFlash::Read(PGA0_MEM));
	PGA_SetGain(1, KAFlash::Read(PGA1_MEM));

	// set default shr states
	Shift.Values = KAFlash::Read(SHRD_MEM);

	// read sleep time
	Monitor.Sleep = KAFlash::Read(TIME_MEM) & SLEEP_MSK;

	// setup counter
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << WGM12);
	TCCR1A |= (1 << COM1B1);
	TCCR1B |= (1 << CS10);

	// setup PWM
	OCR1B = 0;

	// setup bindings
	Script.Bindings.Add(BIND(get));
	Script.Bindings.Add(BIND(put));
	Script.Bindings.Add(BIND(pga));
	Script.Bindings.Add(BIND(out));
	Script.Bindings.Add(BIND(sys));
	Script.Bindings.Add(BIND(dev));
	Script.Bindings.Add(BIND(spi));
	Script.Bindings.Add(BIND(pwm));
	Script.Bindings.Add(BIND(slp));

	// setup adc variables
	for (char i = 0; i < ADC_COUNT; i++) { Buff[1] = '0' + i; Inputs.Add(Buff, Analog[i], nullptr, false); }

	if (Boot & 0b00001000) switch (KAFlash::Read(TIME_MEM) & ERROR_MSK)
	{
		case MASTER_FROZEN:
			Monitor.Online = true; SYS_PostError(MASTER_TIMEOUT);
		break;
		case REMOTE_FROZEN:
			Monitor.Online = true; SYS_PostError(REMOTE_TIMEOUT);
		break;
	}

	if (Monitor.Online) SYS_SendFeedback(GET_ALL);
}

int SYS_PostError(int Code)
{
	if (Monitor.Online && !Monitor.Master) UART << PGM_V get_CALL << PGM_V get_FAIL << Code << PGM_V get_EOC; return Code;
}

