// ----- main() ---------------------------------------------------------------

// ----------------------------------------------------------------------------
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "Led.h"
#include "Timing.h"
#include "Timer.h"
#include "Spi.h"
#include "I2c.h"
#include "Adc.h"
#include "Dac.h"

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

	uint16_t tempSensorVal, adcResult; //TODO: update to double --> how to handle in I2C communication?
	double celsius;
	uint16_t results[SINE_SAMPLES*5]; //oversampling by x5
	uint32_t i = 0;
	TIM2_Init(); //for ADC Internal Temp Read

	Dac_Init();
	TIM6_Init(); //shall be called after Dac_Init()?
	Adc_Init();

	// Done; a low-res 440Hz sine wave should be playing on PA4.
	while(1)
	{
	  adcResult = Adc_Read();
	  results[i++ % (SINE_SAMPLES*5)] = adcResult;
	}
}

#endif
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
