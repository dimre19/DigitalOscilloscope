/*
 * I2c.h
 *
 *  Created on: May 9, 2020
 *      Author: David Imre
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx.h"

/******************MACROS****************/
#define READ_INT_TEMP 0x40
#define TEST_DATA 0x30
uint8_t I2cMasterModeSelected;
void I2c_Init();
void I2c_MasterTransmit();
void I2c_SlaveTransmit(uint8_t data);

#endif /* I2C_H_ */
