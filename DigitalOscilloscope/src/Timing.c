/*
 * Timing.c
 *
 *  Created on: May 2, 2020
 *      Author: David Imre
 */

#include "Timing.h"

//Private global variables
uint8_t SysTickEnabled = 0; //only used by SysTick function. Use SysTick_IsEnabled() for asking
uint32_t StartingTimeInMs; //starting time for SysTick_TimerStart(), needed for SysTick_TimerStop()
uint32_t StartingTick = 0; //starting tick for tick measurement (Systick current value)

/**
  * @brief Enable Systick time measurement
  * @note 1 cycle is 1ms
  * @param none
  * @retval none
  */
void SysTick_Enable()
{
	#ifndef USE_HAL_DRIVER //TODO: check the dependency when _initialize_hardware() runs. This section shall be chosen in case of manual clock settings
		//Configure SysTick
		SysTick->LOAD = 168000-1; //168Mz configuration: TODO: update to current proc cícle
		SysTick->VAL = 0;
		SysTick->CTRL = 5;
	#endif

	SysTickEnabled = 1; //start measurement
}


/**
  * @brief Disable Systick time measurement
  * @note Do not use it in case of time measuring since system startup.
  * @param none
  * @retval none
  */
void SysTick_Disable()
{
	SysTick->CTRL = 0; //turn off SysTick;

	SysTickEnabled = 0;
}


/**
  * @brief Check if SysTick time measurement is turned on
  * @note Use this function before using SysTick_DelayInMs() or older SysTick based old function which modify SysTick settings.
  * @param none
  * @retval True, if Systick is running
  */
uint8_t SysTick_IsEnabled()
{
	return SysTickEnabled;
}


/**
  * @brief Get the elapsed time since in ms microcontroller is on (precisely: from the point at system startup
  * 	   when SysTick was activated)
  * @note  none
  * @param none
  * @retval Elapsed time in ms since start
  */
uint32_t  SysTick_GetElapsedTimeSinceStartInMs()
{
	return HAL_GetTick(); //TODO: Update to own tick counter once HAL is not used
}


/**
  * @brief Get the elapsed time in sec since microcontroller is on (precisely: from the point at system startup
  * 	   when SysTick was activated)
  * @note  none
  * @param none
  * @retval Elapsed time in seconds since start
  */
double  SysTick_GetElapsedTimeSinceStartInSec()
{
	return (double)HAL_GetTick()/1000; //TODO: Update to own tick counter once HAL is not used
}


/**
  * @brief Start time measuring with SysTick timer
  * @note  Elapsed Time can be calculated from SysTick_TimerStart() and SysTick_TimerStop()
  * @param none
  * @retval start time in ms
  */
void SysTick_TimerStart()
{
	StartingTimeInMs = HAL_GetTick(); //starting time
}


/**
  * @brief Start time measuring with SysTick timer
  * @note  Elapsed Time can be calculated from SysTick_TimerStart() and SysTick_TimerStop()
  * @param none
  * @retval start time in ms
  */
uint32_t SysTick_TimerStop()
{
	uint32_t stopTimeInMs, elapsedTimeInMs;

	if(StartingTimeInMs) //check if time measuring has been started
	{
		stopTimeInMs = HAL_GetTick();
	}
	else
	{
		stopTimeInMs = 0; //SysTick_TimerStart() not called before this function
	}
	elapsedTimeInMs = stopTimeInMs-StartingTimeInMs;

	StartingTimeInMs = 0; //clear starting time before next start;

	return elapsedTimeInMs;
}


/**
  * @brief Save starting tick for tick measuring with SysTick timer
  * @note  This function can be used for elapsed number of ticks calculation with
  *        SysTick_TickStart() and SysTick_TickStop()
  * @note The SysTick_TickStart() and SysTick_TickStop() function can be used only in <<ms measurements! Otherwise result is misleading (modulo of 1ms)
  * @param none
  * @retval none
  */
void SysTick_TickStart()
{
	StartingTick = SysTick->VAL;
}

/**
  * @brief Save latest tick and count the elapsed ticks since last SysTick_TickStart()
  * @note  This function can be used for elapsed number of ticks calculation with
  *        SysTick_TickStart() and SysTick_TickStop()
  * @note The SysTick_TickStart() and SysTick_TickStop() function can be used only in <<ms measurements! Otherwise result is misleading (modulo of 1ms)
  * @note: TODO: provide elapsed tick between start and stop function (measurement delay)
  * @param none
  * @retval tick count during measurement
  */
int32_t SysTick_TickStop()
{
	int32_t lastTick, elapsedTick;

	lastTick = SysTick->VAL; //save tick as soon as possible

	//Post processing
	if(StartingTick == 0) //check if time measuring has been started
	{
		lastTick = 0; //SysTick_TimerStart() not called before this function
	}

	//down-counting: before reaching 0, StartingTick is the higher value.
	elapsedTick = (int32_t)(StartingTick)-lastTick; //TODO: test if substraction of to uint32_t how results int32_t

	//Handle negative output (in case of down-counting reached 0 during tick measurement)
	if(elapsedTick < 0)
	{
		elapsedTick = SysTick->LOAD + elapsedTick; //Add the load value when SysTick turned 0->LOAD at the end of downcounting
	}

	StartingTick = 0; //clear starting tick before next start;

	return elapsedTick;

}


/**
  * @brief Software delay by using SysTick
  * @note own-developed (tested) and HAL driver function is also implemented
  * @param ms: delay in milliseconds
  * @retval none
  */
void SysTick_DelayInMs(uint32_t ms)
{

	#ifndef USE_HAL_DRIVER //TODO: check the dependency when _initialize_hardware() runs. This section shall be chosen in case of manual clock settings
		//Configure SysTick
		SysTick->LOAD = 168000-1; //168Mz configuration TODO: update to clock dependent value
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
		SysTick->CTRL = 0; //turn off SysTick
	#else
		HAL_Delay(ms);
	#endif

}
