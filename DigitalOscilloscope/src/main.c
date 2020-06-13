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
#include "Global.h"
#include "Led.h"
#include "Timing.h"
#include "Timer.h"
#include "Spi.h"
#include "I2c.h"
#include "Usart.h"
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
	double tempSensorMeas[32];
	uint16_t adcInputMeas[32*5];
	uint32_t tempSensorIndex= 0;
	uint32_t adcInputIndex = 0;
	uint8_t usartTxIndex = 0;
	uint8_t rxBuffer[32];

	for(int i = 0; i<32; i++)
	{
		tempSensorMeas[i] = 0;
	}
	for(int i = 0; i<32*5; i++)
	{
		adcInputMeas[i] = 0;
	}

	TIM2_Init(); //for ADC internal temp read
	TIM5_Init(); //for ADC input signal measurement
	Dac_Init(); //sine wave for test
	TIM6_Init(); //shall be called after Dac_Init()?
	Adc_Init();
	Led_Init();
	USART2_Init();

	EventFlag = 0; //clear events

	HAL_NVIC_SetPriority(ADC_IRQn, 0, 1); //TODO: update priority
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0); //TODO: update priority
	HAL_NVIC_EnableIRQ(ADC_IRQn);
	HAL_NVIC_EnableIRQ(USART2_IRQn);


	while(1) //TODO: update to function pointer scheduling in order to call the handlers in the priority of interrupts
	{
	  if(EventFlag & 0x1) //ADC new internal temperature event (ADC1)
	  {
		  celsius = Adc_ReadIntTemp();
		  tempSensorMeas[tempSensorIndex++ % (32)] = celsius;
		  Led_Toggle(BLUE_LED); //LED indicator of ADC1 sampling in sec
		  EventFlag &=~ 0x1;
	  }
	  if(EventFlag & 0x2) //ADC new input measurement event (ADC2)
	  {
		  adcResult = Adc_Read();
		  adcInputMeas[adcInputIndex++ % (32*5)] = adcResult;
		  if(!(adcInputIndex % 1000)) //TODO: analyze how much processor time is this if condition check
			  Led_Toggle(RED_LED); //LED indicator of ADC2 sampling in ms
		  EventFlag &=~ 0x2;
	  }
	  if(EventFlag & 0x04) //USART2 Rx
	  {
		  USART2_ReadRxBuffer(rxBuffer);
		  while(rxBuffer[usartTxIndex] != 13 && usartTxIndex != 32) //Carriage return
		  {
			  USART2_Send(rxBuffer[usartTxIndex++]);
		  }
		  USART2_Send(10);//new line
		  USART2_Send(13);//carriage return
		  usartTxIndex = 0;
		  EventFlag &=~ 0x04;
	  }

	}

}

#endif
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
