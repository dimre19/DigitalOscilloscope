/*
 * Commands.c
 *
 *  Created on: Jun 15, 2020
 *      Author: David Imre
 */

#include "Commands.h"
#include  <string.h>

#define NUMBER_OF_COMMANDS 4
#define MAX_STRING_SIZE 20
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
	UsartCommand retval;
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
