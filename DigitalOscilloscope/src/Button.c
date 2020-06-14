/*
 * Button.c
 *
 *  Created on: Jun 14, 2020
 *      Author: David Imre
 */

#include "Button.h"

void Button_Init()
{
	RCC->AHB1ENR |= 1; //enable clock for GPIOA

	GPIOA->MODER &= ~0x00000003; //clear PA0 pin settings
	GPIOA->MODER |= 0x0000000; //PA0 as digital input

	GPIOA->PUPDR &= ~0x00000003; //clear PA0 pull-up/pull-down register settings
	GPIOA->PUPDR |=	0x00000002;  //set pull-down resistor for PA0 (no need for it, pull-down resistor is present on HW)

	//EXTI Init for PA0 interrupt
	__disable_irq(); //before configuring irq-s, irq-s must be disabled!
	RCC->APB2ENR |= 0x4000; //Enable clock for Syscfg (which is responsible for EXTI (Complete reference manual: 9 System configuration controller (SYSCFG))

	SYSCFG->EXTICR[0] = 0; //PA0, is default.

	EXTI->IMR |= 0x01; //unmask EXTI0
	EXTI->FTSR |= 0x01; //falling trigger on EXTI0

	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 2);
	NVIC_EnableIRQ(EXTI0_IRQn);
	__enable_irq();

}
