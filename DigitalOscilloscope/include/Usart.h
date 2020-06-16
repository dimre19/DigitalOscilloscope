/*
 * Usart.h
 *
 *  Created on: Apr 14, 2020
 *      Author: David Imre
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f4xx.h"


void USART2_Init(void);
void USART2_Init(void);
uint32_t USART2_Receive(void);
void USART2_Send(uint32_t txBuffer);
void USART2_UpdateRxBuffer(uint8_t newData);
void USART2_ReadRxBuffer(uint8_t* rxBuff);





#endif /* USART_H_ */
