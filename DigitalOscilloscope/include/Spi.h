/*
 * Spi.h
 *
 *  Created on: May 8, 2020
 *      Author: David Imre
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx.h"

void Spi_Init();
void Spi_WriteData();
uint32_t Spi_ReadData();


#endif /* SPI_H_ */
