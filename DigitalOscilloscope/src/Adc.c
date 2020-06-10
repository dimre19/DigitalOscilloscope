/*
 * Adc.c
 *
 *  Created on: May 12, 2020
 *      Author: David Imre
 */

#include "Adc.h"


double IntTemp = 0;
/**
  * @brief ADC Init
  * @note  Timer triggered ADC1 for internal temperature sensor processing
  * @note  Timer triggered ADC2 for input signal measurement
  * @param none
  * @retval none
  */
void Adc_Init()
{

	#define TIM_TRIGGERED_SAMPLING 1

	RCC->AHB1ENR |= 1; 	//Enable GPIOA CLK (for PA1)
	GPIOA->MODER |= 0xC; 	//Set PA1 analog mode to ADC

	RCC->APB2ENR |= 0x100; //Enable clock for ADC1

	// ADC1 settings - Internal Temperature sensor
	ADC1->CR2 = 0; 		//Disable ADC1. Note: Is '=' useful instead of &=~1? Note2: it was not part of this Udemy lession
	ADC->CCR |= 0x00800000; //Enable Temperature sensor and VREFINT. Note. This is common ADC register!

	//1. TIPP//ADC->CCR &= ~0x00400000; //Disable VBAT channel. According to datasheet, this feature shall be only
	// disabled on STM32F42x and STM32F43x, because battery monitoring and temp sensor is on the same channel
	//2. TIPP
	ADC1->SMPR1 = 0x00100000; //Set greater sampling time than minimum temperature sensor Ts (10us, datasheet).
							   //Pend bit 20 (84 sample). 84/ADCLK
	ADC1->SQR3 = 16; // Set channel 16 to first sequence
	//ADC1->SQR3 = 1; // Set channel 16 to first sequence

	#if TIM_TRIGGERED_SAMPLING
	ADC1->CR2 |= 0x13000000; //External trigger enable + Start measuring at Timer 2 CC2 event
	#endif

	ADC1->CR1 = 0x20; //Enable Interrupt mode
	ADC1->CR2 |= 1;

	// ADC2 settings - Input analog signal measurement
	RCC->APB2ENR |= 0x200; //Enable clock for ADC1
	ADC2->CR2 = 0; 		//Disable ADC1. Note: Is '=' useful instead of &=~1? Note2: it was not part of this Udemy lession

	ADC2->SMPR1 = 0x00100000; //Pend bit 20 (84 sample). 84/ADCLK TODO: find ideal value, this may too much
	ADC2->SQR3 = 1; // Set channel 1 to first sequence


	#if TIM_TRIGGERED_SAMPLING
	ADC2->CR2 |= 0x1B000000; //External trigger enable + Start measuring at Timer 5 CC2 event TODO: update to different Timer
	#endif

	ADC2->CR1 = 0x20; //Enable Interrupt mode
	ADC2->CR2 |= 1;

}

/**
  * @brief Read ADC internap temperature
  * @param none
  * @retval temperature value in °C
  */
double Adc_ReadIntTemp() //TODO: synchronize it with other ADC reads
{
	uint32_t adcData;
	double voltage, celsius;

	//while(!(ADC1->SR & 2)){}
	adcData = ADC1->DR;
	voltage = (double)adcData/4095*2.966; //not 3V3: Discovery uses 3V for VDD (2.966V is measured)
	celsius = (voltage - 0.76)/0.0025 + 25; //Reading the temperature section in complete ref manual
	IntTemp = celsius;
	return celsius;
}

/**
  * @brief Read ADC input value.
  * @param none
  * @retval none
  */
uint16_t Adc_Read()
{
	uint32_t adcData;
	double voltage;

	//while(!(ADC2->SR & 2)){}
	adcData = ADC2->DR;
	//voltage = (double)adcData/4095*2.966; //Discovery uses 3V for VDD (2.966V is measured)

	return adcData; //voltage; - until no friction handling in Raspberry Pi
}

/**
  * @brief Get Internal Temperature
  * @note This function only provides the last read temperature, but do not trigger a new read
  * @param none
  * @retval none
  */
double Adc_GetInternalTemp()
{
	return IntTemp;
}
