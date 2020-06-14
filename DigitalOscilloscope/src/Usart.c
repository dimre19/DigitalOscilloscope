/*
 * Usart.c
 *
 *  Created on: Apr 14, 2020
 *      Author: David Imre
 */

#include "Usart.h"


uint8_t rxBuffer[32]; //maximum length is 32;
/**
  * @brief Initalizes the USART module
  * @note  Asynchronous mode
  * @param none
  * @retval none
  */
void USART2_Init(void)
{
	// Clock init
	RCC->AHB1ENR |= 1; //Enable clock for GPIOA
	RCC->APB1ENR |= 0X20000; //Enable clock for USART2

	// Rx Init
	// Configure PA3 as USART2 Rx
	GPIOA->AFR[0] |= 0x7000; //Alternate function 7 on PA3 is USART2 Rx
	GPIOA->MODER |= 0x0080;  //Enable alternate function at PA3

	USART2->BRR = 4375;//TODO: calculate it from APB2! 42Mhz(APB2)/9600bps = 4375

	USART2->CR1 |= 0X0020;//Interrupt enable for Rx //TODO: disable/enable irq-s needed?

	USART2->CR1 |= 0x0004; //enable Rx

	// Tx Init
	// Configure PA2 as USART2 Tx
	GPIOA->AFR[0] |= 0x700; //Alternate function 7 on PA2 is USART2 Tx
	GPIOA->MODER |= 0x0020;  //Enable alternate function at PA2

	USART2->CR1 |= 0x0008; //enable Tx

	// Enable USART2
	USART2->CR1 |= 0x2000;

}


/**
  * @brief Read incoming USART message
  * @param none
  * @retval received data
  */
uint32_t USART2_Receive() //TODO: 1. printf from Udemy 2. update to uint8_t or uint16_t (depends on settings)
{
	while(!(USART2->SR & 0x0020)) {}
	return USART2->DR;
}

/**
  * @brief Send USART message
  * @param none
  * @retval received data
  */
void USART2_Send(uint32_t txBuffer) //TODO: update to uint8_t or uint16_t (depends on settings)
{
	while(!(USART2->SR & 0x0080)) {}
	USART2->DR = txBuffer;
	while(USART2->SR & 0x0040) {}
}

void USART2_UpdateRxBuffer(uint8_t newData)
{
	static uint8_t rxIndex = 0;
	rxBuffer[rxIndex++] = newData;
	if(rxIndex == 32 || newData == 13)
		rxIndex = 0; //TODO: make some overflow alert

	//TODO: pend bit if a command is identified. Note: clear remaining bits from TxBuffer once command is identified. + reset the index.
	//shall it the string compare happen in main? --> time analysis!
}

void USART2_ReadRxBuffer(uint8_t* rxBuff)
{
//	if(sizeof(RxBuff) != 32) //TODO: alert or return value
//		return;

	for(int i = 0; i < 32; i++)
	{
		rxBuff[i] = rxBuffer[i];
	}
}
