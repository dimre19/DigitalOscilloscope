/*
 * Adc.h
 *
 *  Created on: May 12, 2020
 *      Author: David Imre
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"


void Adc_Init();
uint16_t Adc_Read();
double Adc_ReadIntTemp();

#endif /* ADC_H_ */
