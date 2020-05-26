/*
 * Adc.c
 *
 *  Created on: May 12, 2020
 *      Author: David Imre
 */

#include "Adc.h"

/**
  * @brief ADC Init
  * @note  Timer triggered ADC1 internal temperature sensor processing
  * @param none
  * @retval none
  */
void Adc_Init()
{

	#define TIM_TRIGGERED_SAMPLING 1

	RCC->AHB1ENR |= 1; 	//Enable GPIOA CLK (for PA1)
	RCC->APB2ENR |= 0x100; //Enable clock for ADC1
	ADC1->CR2 = 0; 		//Disable ADC1. Note: Is '=' useful instead of &=~1? Note2: it was not part of this Udemy lession
	ADC->CCR |= 0x00800000; //Enable Temperature sensor and VREFINT. Note. This is common ADC register!

	//1. TIPP//ADC->CCR &= ~0x00400000; //Disable VBAT channel. According to datasheet, this feature shall be only
	// disabled on STM32F42x and STM32F43x, because battery monitoring and temp sensor is on the same channel
	//2. TIPP
	ADC1->SMPR1 = 0x00100000; //Set greater sampling time than minimum temperature sensor Ts (10us, datasheet).
							   //Pend bit 20 (84 sample). 84/ADCLK
	ADC1->SQR3 = 16; // Set channel 16 to first sequence

	#if TIM_TRIGGERED_SAMPLING
	ADC1->CR2 |= 0x13000000; //External trigger enable + Start measuring at Timer 2 CC2 event
	#endif

	ADC1->CR2 |= 1;

}

/**
  * @brief ADC Read
  * @note  Read Internal Temperature sensor value
  * @param none
  * @retval temperature value in °C
  */
double Adc_Read()
{
	uint32_t adcData;
	double voltage, celsius;

	while(!(ADC1->SR & 2)){}
	adcData = ADC1->DR;
	voltage = (double)adcData/4095*2.966; //not 3V3: Discovery uses 3V for VDD (2.966V is measured)
	celsius = (voltage - 0.76)/0.0025 + 25; //Reading the temperature section in complete ref manual
	IntTemp = celsius;
	return celsius;
}

double Adc_GetInternalTemp()
{
	return IntTemp;
}
