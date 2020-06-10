/*
 * Timer.c
 *
 *  Created on: May 12, 2020
 *      Author: David Imre
 */

#include "Timer.h"

/**
  * @brief Initializing Timer 2 for time-triggered ADC internal temperature sampling
  * @param none
  * @retval none
  */
void TIM2_Init()
{

	RCC->APB1ENR |= 1; //Enable TIM2 clock

	// Set up PWM. PWM is used to have one rising edge in each cycle (trigger)
	// Other option is simple CC and set ADC to be sensitive both falling and rising edges. Many options. :)
	TIM2->PSC  =  8400-1; //prescale to 10kHz
	TIM2->ARR  =  10000-1; //1kHz signal
	TIM2->CNT = 0;
	TIM2->CCMR1 = 0x6800; //PWM mode
	TIM2->CCER = 0x0010; //enable output compare 2 preload
	TIM2->CCR2 = 50-1; //pulse-width in ticks. It shall be smaller then ARR register value.
	TIM2->CR1 |= 1; //start the timer.
}

/**
  * @brief Initializing Timer 5 for time-triggered ADC input signal measurement
  * @param none
  * @retval none
  */
void TIM5_Init()
{

	RCC->APB1ENR |= 8; //Enable TIM5 clock

	// Set up PWM. PWM is used to have one rising edge in each cycle (trigger)
	// Other option is simple CC and set ADC to be sensitive both falling and rising edges. Many options. :)
	TIM5->PSC  =  0x0000;
	TIM5->ARR  =  SystemCoreClock / ( 440 * 32 ); //syncronize with DAC frequency
	TIM5->CNT = 0;
	TIM5->CCMR1 = 0x6800; //PWM mode
	TIM5->CCER = 0x0010; //enable output compare 2 preload
	TIM5->CCR2 = 50-1; //pulse-width in ticks. It shall be smaller then ARR register value.
	TIM5->CR1 |= 1; //start the timer.
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
