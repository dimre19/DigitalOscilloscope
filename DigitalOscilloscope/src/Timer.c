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
//	TIM2->PSC = 84-1; //8400-1;
//	TIM2->ARR = 71*2-1; // 70 us 1000-1; //100ms
	TIM2->PSC  =  ( 0x0000 );
	TIM2->ARR  =  ( SystemCoreClock / ( 440 * 32 ) );
	TIM2->CNT = 0;
	TIM2->CCMR1 = 0x6800;
	TIM2->CCER = 0x0010;
	TIM2->CCR2 = 50-1;
	TIM2->CR1 |= 1;
}

void TIM6_Init()
{
	// TIM6 configuration.
    // Set prescaler and autoreload for a 440Hz sine wave.
    TIM6->PSC  =  ( 0x0000 );
    TIM6->ARR  =  ( SystemCoreClock / ( 440 * 32 ) ); //TODO: update 32 to parameter

    // Enable trigger output on timer update events.
    TIM6->CR2 &= ~( TIM_CR2_MMS );
    TIM6->CR2 |=  ( TIM_TRGO_UPDATE /* 0x2 << TIM_CR2_MMS_Pos*/ );

    // Start the timer.
    TIM6->CR1 |=  ( TIM_CR1_CEN );
}
