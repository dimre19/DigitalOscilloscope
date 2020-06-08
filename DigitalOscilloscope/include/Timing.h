/*
 * Timing.h
 *
 *  Created on: May 2, 2020
 *      Author: David Imre
 */

#ifndef TIMING_H_
#define TIMING_H_

#include "stm32f4xx.h"

void SysTick_Enable();
void SysTick_Disable();
uint8_t SysTick_IsEnabled();
uint32_t  SysTick_GetElapsedTimeSinceStartInMs();
double  SysTick_GetElapsedTimeSinceStartInSec();
void SysTick_TimerStart();
uint32_t SysTick_TimerStop();
void SysTick_TickStart();
int32_t SysTick_TickStop();
void SysTick_DelayInMs(uint32_t ms);
void delay_cycles(uint32_t cyc);
#endif /* TIMING_H_ */
