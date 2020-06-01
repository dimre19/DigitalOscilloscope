/*
 * Timer.c
 *
 *  Created on: May 12, 2020
 *      Author: David Imre
 */

#include "Timer.h"

void TIM2_Init()
{

	RCC->APB1ENR |= 1;

	//Set up PWM. I guess PWM was selected to have one rising edge in each cycle
	//(other option is simple CC and set ADC to be sensitive both falling and rising edges. Many options. :)
	TIM2->PSC = 8400-1;
	TIM2->ARR = 1000-1; //100ms
	TIM2->CNT = 0;
	TIM2->CCMR1 = 0x6800;
	TIM2->CCER = 0x0010;
	TIM2->CCR2 = 50-1;
	TIM2->CR1 |= 1;
}
