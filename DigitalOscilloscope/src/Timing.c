/*
 * Timing.c
 *
 *  Created on: May 2, 2020
 *      Author: David Imre
 */

#include "Timing.h"

void SysTick_DelayInMs(uint32_t ms)
{

		//Configure SysTick
		SysTick->LOAD = 168000; //168Mz configuration
		SysTick->VAL = 0;
		SysTick->CTRL = 5;

		for(int i = 0; i < ms ; i++)
		{
		  // Infinite loop
		  while ((SysTick->CTRL & 0x10000) == 0)
		  {
			  //do nothing
		  }
		}
		SysTick->CTRL = 0;

}
