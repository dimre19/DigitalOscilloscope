/*
 * Timer.c
 *
 *  Created on: May 12, 2020
 *      Author: David Imre
 */

#include "Timer.h"

/**
  * @brief Initializing Timer 2 for time-triggered ADC sampling
  * @param none
  * @retval none
  */
void TIM2_Init()
{

	RCC->APB1ENR |= 1; //Enable TIM2 clock

	// Set up PWM. PWM is used to have one rising edge in each cycle (trigger)
	// Other option is simple CC and set ADC to be sensitive both falling and rising edges. Many options. :)
	TIM2->PSC  =  ( 0x0000 );
	TIM2->ARR  =  ( SystemCoreClock / ( 440 * 32 ) ); //syncronizing with DAC frequency
	TIM2->CNT = 0;
	TIM2->CCMR1 = 0x6800; //PWM mode
	TIM2->CCER = 0x0010; //enable output compare 2 preload
	TIM2->CCR2 = 50-1; //pulse-width in ticks. It shall be smaller then ARR register value.
	TIM2->CR1 |= 1; //start the timer.
}

/**
  * @brief Initializing Timer 6 for time-triggered DAC update
  * @param none
  * @retval none
  */
void TIM6_Init()
{

	 RCC->APB1ENR  |= RCC_APB1ENR_TIM6EN; //Enable TIM6 clock

	// TIM6 configuration.
    // Set prescaler and autoreload for a 440Hz sine wave.
    TIM6->PSC = 0x0000;
    TIM6->ARR = SystemCoreClock / ( 440 * 32 ); //DAC frequency: 440Hz, 32 sine-samples

    // Enable trigger output on timer update events.
    TIM6->CR2 &= ~TIM_CR2_MMS;
    TIM6->CR2 |= TIM_TRGO_UPDATE;

    TIM6->CR1 |= TIM_CR1_CEN; //start the timer.
}
