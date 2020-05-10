/*
 * I2c.h
 *
 *  Created on: May 9, 2020
 *      Author: David Imre
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx.h"

uint8_t I2cMasterModeSelected;
void I2c_Init();
void I2c_MasterTransmit();
void I2c_SlaveTransmit();

#endif /* I2C_H_ */
