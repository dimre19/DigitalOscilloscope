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

uint32_t i = 0;
uint32_t j = 0;
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
		i++;
	}
	if(ADC2->SR & 0x2)
	{
		EventFlag |= 0x02;
		ADC2->SR &=~ 0x2; //data read only in main
		j++;
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
