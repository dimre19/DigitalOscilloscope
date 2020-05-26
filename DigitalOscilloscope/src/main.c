/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "Led.h"
#include "Timing.h"
#include "Spi.h"
#include "I2c.h"
#include "Adc.h"
// ----------------------------------------------------------------------------
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#if  !MODULE_TEST_ON
int main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.
	//uint32_t spiRxBuff;
	//TODO: manual test of SysTick in SysTick_TickStop()
	uint8_t tempSensorVal; //TODO: update to double --> how to handle in I2C communication?
	double celsius;
	Led_Init();
	I2c_Init();
	TIM2_Init(); //for ADC Internal Temp Read
	Adc_Init();

	while (1)
	{
		//SPI test
//		Spi_WriteData();
//		spiRxBuff = Spi_ReadData();
		celsius = Adc_Read(); //1sec period
		tempSensorVal = (uint8_t)celsius;
		//at start generation this function shall be already called.
		I2c_SlaveTransmit(tempSensorVal); //period = elapsed time between two read request (from Raspberry Pi)
		Led_Toggle();
		SysTick_DelayInMs(300);
	}
}

#endif
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
