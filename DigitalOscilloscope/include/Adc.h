/*
 * Adc.h
 *
 *  Created on: May 12, 2020
 *      Author: David Imre
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"

static double IntTemp = 0;
void Adc_Init();
double Adc_Read();
double Adc_ReadIntTemp();

#endif /* ADC_H_ */
