/*
 * Timer.h
 *
 *  Created on: May 12, 2020
 *      Author: David Imre
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f4xx.h"

void TIM2_Init();
void TIM5_Init();
void TIM6_Init();
void TIM5_UpdateFreq(uint32_t freqHz);
void TIM6_UpdateFreq(uint32_t freqHz);

#endif /* TIMER_H_ */
