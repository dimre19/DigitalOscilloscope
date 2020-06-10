/*
 * Led.h
 *
 *  Created on: May 2, 2020
 *      Author: David Imre
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f4xx.h"

#define GREEN_LED 1
#define ORANGE_LED 2
#define RED_LED 3
#define BLUE_LED 4
void Led_Init();
void Led_Toggle(uint8_t led);

#endif /* LED_H_ */
