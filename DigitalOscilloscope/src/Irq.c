/*
 * Irq.c
 *
 *  Created on: Jun 10, 2020
 *      Author: David Imre
 */

#include "stm32f4xx.h"
#include "Adc.h"
#include "Global.h"

/**
  * @brief Interrupt handler of ADCs
  * @param none
  * @retval none
  */
void ADC_IRQHandler()
{
	if(ADC1->SR & 0x2)
	{
		EventFlag |= 0x01;
		ADC1->SR &=~ 0x2;
	}
	if(ADC2->SR & 0x2)
	{
		EventFlag |= 0x02;
		ADC2->SR &=~ 0x2;
	}
}
