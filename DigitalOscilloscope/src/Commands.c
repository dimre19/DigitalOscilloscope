/*
 * Commands.c
 *
 *  Created on: Jun 15, 2020
 *      Author: David Imre
 */

#include  <string.h>
#include "Commands.h"
#include "Usart.h"
#include "Timer.h"
#include <math.h> //for pow function

#define NUMBER_OF_COMMANDS 4
#define MAX_STRING_SIZE 20

UsartCommand command = NOT_VALID;
uint8_t usartRxIndex = 0;
uint8_t rxBuffer[32];
uint8_t txBuffer[32];
char* message;

// User Inputs
const char UserCommands[NUMBER_OF_COMMANDS][MAX_STRING_SIZE] =
{
	"FG_SET_WAVEFORM",
	"FG_CHANGE_FREQ",
	"FG_CHANGE_AMP",
	"TURN_OFF_LEDS"
};

UsartCommand GetCommand(uint8_t* rxBuff, uint8_t rxBuffLength)
{
	uint8_t index = 0;
	uint8_t tempBuff[rxBuffLength];
	UsartCommand retval = NOT_VALID;

	while(rxBuff[index] != 13 && index != 32) //Carriage return
	{
	 tempBuff[index] = rxBuff[index];
	 index++;
	}

	retval = FindCommand(tempBuff,index);

	return retval;

}

UsartCommand FindCommand(uint8_t* buff, uint16_t size)
{
	int result;

	for(int i = 0; i < NUMBER_OF_COMMANDS; i++)
	{
		result = strncmp((const char*) buff, (const char*) UserCommands[i],size);

		if(result == 0)
			return NOT_VALID + i + 1; //TODO: how to do it nicer
	}

	return NOT_VALID;
}

uint32_t GetFrequency()
{
	uint32_t freq = 0;

	while(rxBuffer[usartRxIndex] != 13 && usartRxIndex != 32) //Carriage return //TODO: handle longer then 32 digit number bug (even if it is not realistic)
	{
	  usartRxIndex++;
	}

	for(uint8_t i = usartRxIndex; i > 0; i--)
	{
		if(rxBuffer[i-1] < 48 || rxBuffer[i-1] > 57) //not a number
		{
			usartRxIndex = 0;
			return 0;
		}


		freq += (rxBuffer[i-1] - 48) * pow(10,(usartRxIndex - i));
	}

	usartRxIndex = 0;

	return freq;

}

void CommandHandler()
{
	uint32_t freq;

	USART2_ReadRxBuffer(rxBuffer);

	if(command == NOT_VALID)
		command = GetCommand(rxBuffer,sizeof(rxBuffer)/sizeof(uint8_t)); //TODO: remove parameters since they are in the same file with function

	switch(command){
	case NOT_VALID:
	  break;
	case FG_SET_WAVEFORM:
	  //TODO: 1. options for waveforms 2. freq...
	  message = "[STM32]: Enter frequency in Hz: ";
	  strcpy(txBuffer,message);
	  USART2_UpdateTxBuffer(txBuffer,sizeof(txBuffer)/sizeof(uint8_t));
	  command = FG_UPDATE_FREQ;
	  break;
	case FG_UPDATE_FREQ:
	  freq = GetFrequency();
	  //TIM6_UpdateFreq(freq); //modify TIM6 frequency for DAC output control
	  if(freq != 0)
		  TIM5_UpdateFreq(freq);

	  command = NOT_VALID; //Switch to default. TODO: printf error message + rename to NO_COMMAND
	  break;

	default:
	  //printf report
	  break;
	}

}
