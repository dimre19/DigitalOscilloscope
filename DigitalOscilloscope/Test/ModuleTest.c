#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "Led.h"
#include "Timing.h"

#if MODULE_TEST_ON //Content is uncommented in order to no complain for 2 main at switching
				//--> TODO: at switch, deleting .elf or other Debug files shall also solve this issue.

void ModuleTest_SysTickFunctions();
int32_t ModuleTest_SysTickTickStopPart(int32_t _lastTick, int32_t _startingTick);

int main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.
	ModuleTest_SysTickFunctions();
	//TODO: use Green and Red LED if test succeeded or failed.
	return 0;

}

int32_t ModuleTest_SysTickTickStopPart(int32_t _lastTick, int32_t _startingTick)
{
	int32_t lastTick, elapsedTick, startingTick;

	lastTick = _lastTick;
	startingTick = _startingTick;

	elapsedTick = (int32_t)(startingTick)-lastTick; //TODO: test if substraction of to uint32_t how results int32_t

	//Handle negative output (in case of down-counting reached 0 during tick measurement)
	if(elapsedTick < 0)
	{
		elapsedTick = SysTick->LOAD + elapsedTick; //Add the load value when SysTick turned 0->LOAD at the end of downcounting
	}

	return elapsedTick;
}

void ModuleTest_SysTickFunctions()
{
	uint8_t isSysTickEnabled = 0;
	uint32_t elapsedTime[7];
	double elapsedTimeInSec[3];
	int32_t SysTickTickStopPart[7];

	Led_Init();
	SysTick_Enable();
	isSysTickEnabled = SysTick_IsEnabled();
	elapsedTime[0] = SysTick_GetElapsedTimeSinceStartInMs();
	elapsedTimeInSec[0] = SysTick_GetElapsedTimeSinceStartInSec();

	SysTick_DelayInMs(1); //prevent that SysTick_TimerStart() saves 0ms (stop takes it as not started)
	SysTick_TimerStart();
	SysTick_TickStart();
	Led_Toggle();
	elapsedTime[1] = SysTick_TickStop();
	elapsedTime[2] = SysTick_TickStop();
	SysTick_DelayInMs(300);
	elapsedTime[3] = SysTick_TimerStop();
	elapsedTime[4] = SysTick_TimerStop();
	elapsedTime[5] = SysTick_GetElapsedTimeSinceStartInMs();
	elapsedTimeInSec[1] = SysTick_GetElapsedTimeSinceStartInSec();

	//manual test of SysTick in SysTick_TickStop()
	SysTickTickStopPart[0] = ModuleTest_SysTickTickStopPart(1,0);
	SysTickTickStopPart[1] = ModuleTest_SysTickTickStopPart(0,1);
	SysTickTickStopPart[2] = ModuleTest_SysTickTickStopPart(1,1);
	SysTickTickStopPart[3] = ModuleTest_SysTickTickStopPart(1,SysTick->LOAD);
	SysTickTickStopPart[4] = ModuleTest_SysTickTickStopPart(1,SysTick->LOAD-1000);
	SysTickTickStopPart[5] = ModuleTest_SysTickTickStopPart(SysTick->LOAD,1);
	SysTickTickStopPart[6] = ModuleTest_SysTickTickStopPart(SysTick->LOAD-1000,1);

	while (1)
	{
	 Led_Toggle();
	 SysTick_DelayInMs(300);
	 elapsedTimeInSec[2] = SysTick_GetElapsedTimeSinceStartInSec();
	 elapsedTime[6] = SysTick_GetElapsedTimeSinceStartInMs();
	}
}

void ModuleTest_Usart2()
{
	uint32_t txBuffer = 65;//'*';
		uint32_t rxBuffer = 2;
		USART2_Init();
		Led_Init();
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0); //TODO: update priority
		HAL_NVIC_EnableIRQ(USART2_IRQn);

		while(1)
		{
			//USART2_Send(txBuffer++);
	//		rxBuffer = USART2_Receive(); //comment out if IRQ is applied
			SysTick_DelayInMs(1000);
			Led_Toggle(BLUE_LED);
		}
}
#endif
