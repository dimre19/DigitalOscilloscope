/*
 * Irq.c
 *
 *  Created on: Jun 10, 2020
 *      Author: David Imre
 */

#include "stm32f4xx.h"
#include "Adc.h"
#include "Global.h"
#include "Usart.h"
#include "Led.h"
#include "Timing.h"

#define SWITCH_BOUNCE_THRESHOLD_MS 800 //TODO: reduce this value once user button is applied instead of connecting a wire
/**
  * @brief Interrupt handler of ADCs
  * @param none
  * @retval none
  */
void ADC_IRQHandler() //TODO: worth to switch to non irq mode in case of high sample time? Time analysis needed
{
	if(ADC1->SR & 0x2) //TODO: IT flag will only be cleared once it is read (which happens now in main)
	{
		EventFlag |= 0x01;
		ADC1->SR &=~ 0x2; //data read only in main
	}
	if(ADC2->SR & 0x2)
	{
		EventFlag |= 0x02;
		ADC2->SR &=~ 0x2; //data read only in main
	}
}

void USART2_IRQHandler() //only handle Rx - need to be set up in USART2_Init()
{
	uint8_t txData;

	if(USART2->SR & 0x0020)
	{
		txData  = USART2->DR;
		USART2_UpdateRxBuffer(txData); //reading clears the IT flag
	}

	if(txData == 13) //Enter
		EventFlag |= 0x04;
}

void EXTI0_IRQHandler(void)
{
	static uint8_t lastLedStatus = 1;
	static uint32_t timeSinceLastIrq = 0;

	uint32_t time = SysTick_GetElapsedTimeSinceStartInMs();

	// Check switch bounce effect - return in this case
	if(time - timeSinceLastIrq < SWITCH_BOUNCE_THRESHOLD_MS)
	{
		timeSinceLastIrq = time;
		EXTI->PR = 0x01; //clear the interrupt.
		return;
	}


	if(lastLedStatus)
	{
		Led_Enable(LED_DISABLE);
		Led_TurnOffAll(); //turn off all LEDs
		lastLedStatus = 0;
	}
	else
	{
		Led_Enable(LED_ENABLE);
		lastLedStatus = 1;
	}

	timeSinceLastIrq = time;

	EXTI->PR = 0x01; //clear the interrupt.
}
