/*
 * Led.c
 *
 *  Created on: May 2, 2020
 *      Author: David Imre
 */
#include "Led.h"

void Led_Init() //TODO: arguments!
{
	RCC->AHB1ENR |= 8; //Enable clock for GPIOD
	GPIOD->MODER |= 0x40000000; //PD15 Blue LED
	GPIOD->ODR =0x8000;	//PD15 Blue LED
}

void Led_Toggle()
{
	GPIOD->ODR ^= 0x8000;	//PD15 Blue LED
}
