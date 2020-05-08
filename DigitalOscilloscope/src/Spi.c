/*
 * Spi.c
 *
 *  Created on: May 8, 2020
 *      Author: David Imre
 */

#include "Spi.h"

/**
  * @brief Initalizes the SPI module
  * @note  Slave mode, slave select internally controlled (default state: selected)  *
  * @param none
  * @retval none
  */
void Spi_Init()
{
	//Clock configuration
	RCC->AHB1ENR |= 2; 		//enable GPIOB
	RCC->APB1ENR |= 0x4000; // enable SPI2

	//Configure GPIO-s: PB12: SPI_NSS OFF (slave mode), PB13: SCK PB14: MISO PB15: MOSI
	GPIOB->MODER &= ~0xFF000000; //Clear PB12,13,14,15
	GPIOB->MODER |= 0xA8000000; //PB12,13,14,15: AF

	GPIOB->AFR[1] &= ~0xFFFF0000; //clear AFR register for 13,14,15
	GPIOB->AFR[1] |= 0x55500000; //Select AF5 for 13,14,15

	//SPI2 config
	SPI2->CR1 = 0x200; //Bit 2 = 0: slave mode, Bit 9: SSM, Bit 8 = 0: chip selected
	SPI2->CR1 |= 0X40; //Enable SPI2
}

/**
  * @brief Send SPI data
  * @note  8bit can be sent at one call (uC supports 16 bit as well, if needed)
  * @param none (TODO: input argument)
  * @retval none
  */
void Spi_WriteData()
{
	while(!(SPI2->SR & 2)) {} //Wait until Tx buffer is empty TODO: update to nonblocking

	//TODO: Mask upper 24bit of input (nice to have)
	SPI2->DR = 0XAB; //Test Data

	while(!(SPI2->SR & 2)) {} //Wait until Tx buffer is empty: TODO: update to nonblocking


}

/**
  * @brief Read SPI data buffer
  * @note  call this function if RXNE (Rx buffer not empty) bit is set
  * @param none
  * @retval content of SPI Rx data register
  */
uint32_t Spi_ReadData() //TODO: call this function based on RXE value
{
	return SPI2->DR;
}

