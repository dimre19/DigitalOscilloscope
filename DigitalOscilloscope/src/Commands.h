/*
 * Commands.h
 *
 *  Created on: Jun 15, 2020
 *      Author: David Imre
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "stm32f4xx.h"

typedef enum UsartCommand
{
	NOT_VALID,
	FG_SET_WAVEFORM,
	FG_UPDATE_FREQ,
	FG_UPDATE_AMP,
	TURN_OFF_LEDS
} UsartCommand;

UsartCommand GetCommand(uint8_t* rxBuff, uint8_t rxBuffLength);
UsartCommand FindCommand(uint8_t* buff, uint16_t size);
void CommandHandler();
uint32_t GetFrequency();
#endif /* COMMANDS_H_ */
