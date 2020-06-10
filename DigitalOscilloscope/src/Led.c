/*
 * Led.c
 *
 *  Created on: May 2, 2020
 *      Author: David Imre
 */
#include "Led.h"

/**
  * @brief Initalizes the LED module
  * @note  Blue LED is used on DiscoveryKit
  * @param none
  * @retval none
  */
void Led_Init()
{
	RCC->AHB1ENR |= 8; //Enable clock for GPIOD
	GPIOD->MODER |= 0x55000000; //Activate all LEDs
	//GPIOD->ODR =0xF000;	//All LED ON
}

/**
  * @brief Toggle LED
  * @param none
  * @retval none
  */
void Led_Toggle(uint8_t led)
{
	if(led == GREEN_LED) //TODO: update to struct instead of define
		GPIOD->ODR ^= 0x1000;	//PD12 Green LED
	else if(led == ORANGE_LED)
		GPIOD->ODR ^= 0x2000;	//PD13 Orange LED
	else if(led == RED_LED)
		GPIOD->ODR ^= 0x4000;	//PD14 Red LED
	else if(led == BLUE_LED)
		GPIOD->ODR ^= 0x8000;	//PD15 Blue LED
}
