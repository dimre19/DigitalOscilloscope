/*
 * Led.c
 *
 *  Created on: May 2, 2020
 *      Author: David Imre
 */
#include "Led.h"


uint8_t LedEnabled = 0;
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
	LedEnabled = 1;
}

/**
  * @brief Toggle LED
  * @param none
  * @retval none
  */
void Led_Toggle(uint8_t led)
{
	if(!Led_IsEnabled())
		return;

	if(led == GREEN_LED) //TODO: update to struct instead of define
		GPIOD->ODR ^= 0x1000;	//PD12 Green LED
	else if(led == ORANGE_LED)
		GPIOD->ODR ^= 0x2000;	//PD13 Orange LED
	else if(led == RED_LED)
		GPIOD->ODR ^= 0x4000;	//PD14 Red LED
	else if(led == BLUE_LED)
		GPIOD->ODR ^= 0x8000;	//PD15 Blue LED
}

uint8_t Led_IsEnabled()
{
	return LedEnabled;
}

void Led_Enable(LedStatus status)
{
	if(status == LED_DISABLE)
		LedEnabled = 0;
	if(status == LED_ENABLE)
		LedEnabled = 1;
}


//note - this function does not disable LEDs, just turns them off.
void Led_TurnOffAll()
{
	GPIOD->ODR &=~ 0xF000;	//Turn off All LED
}
