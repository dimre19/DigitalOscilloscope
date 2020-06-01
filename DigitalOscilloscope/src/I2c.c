/*
 * I2c.c
 *
 *  Created on: May 9, 2020
 *      Author: David Imre
 */


#include "I2C.h"
#include "stm32f4xx.h"


/**
  * @brief Initalizes the I2C module
  * @note  Slave mode
  * @param none
  * @retval none
  */
void I2c_Init()
{
	//Clock settings
	RCC->APB1ENR |= 0x00800000; //enable clock for I2C3
	RCC->AHB1ENR |= 0x5;// equal to: 0b101; //enable clock for GPIOA and GPIOC (I2C3 is on PA8 and PC9)

	//GPIO settings
	GPIOA->MODER &= ~0x00030000; //clear PA8 pin settings
	GPIOA->MODER |= 0x00020000; //PA8 as alternate function (SCL)
	GPIOC->MODER &= ~0x000C0000; //clear PC9 pin settings
	GPIOC->MODER |= 0x00080000; //PC9 as alternate function (SDA)

	GPIOA->AFR[1] &= ~0xF;		//clear PA8 AF selection
	GPIOA->AFR[1] |= 0x4; 		//PA8 AF: I2C3_SCL
	GPIOC->AFR[1] &= ~0xF0;		//clear PA8 AF selection
	GPIOC->AFR[1] |= 0x40; 		//PC9 AF: I2C3_SDA

	GPIOA->OTYPER |= 0x100; 	//PA8 open-drain
	GPIOC->OTYPER |= 0x200; 	//PC9 open-drain

	GPIOA->PUPDR &= ~0x00030000; //clear PA8 pull-up/pull-down register settings
	GPIOA->PUPDR |=	0x00010000;  //set pull-up resistor for PA8
	GPIOC->PUPDR &= ~0x000C0000; //clear PC9 pull-up/pull-down register settings
	GPIOC->PUPDR |=	0x00040000;  //set pull-up resistor

	//I2C settings
	I2C3->CR1 =  0x8000; //I2C3 reset
	I2C3->CR1 &=  ~0x8000; //I2C3 release reset
	I2C3->CR2 = 42; //APB1 is 42MHz
//	I2C3->CCR = 210; //100kHz Sm mode Don't care until I2C is in slave mode
//	I2C3->TRISE = 43; //42+1, see ref man. Don't care until I2C is in slave mode
	I2C3->OAR1 = 0x4000 | (0xFF << 1); //adress is 0x45 (random selected) on ADD[7:1] bits. 0x4000 not needed (only STM32 HAL use this bit)

	I2C3->CR1 |= 1; //enable I2C3
	I2C3->CR1 |= 0x400; //Bit 10: ACK enable. This bit shall be set after enabling I2C3

	I2cMasterModeSelected = 0;

}

/**
  * @brief Transmit data in slave mode
  * @note  This function is currently hard coded to 1 write 1 read section process. TODO: separation
  * @note The communication works, but this function is not yet done. TODO: logic decision what to send based on command (e.g. temperature value), clear code, timeout for while etc...
  * @param none
  * @retval none
  */
void I2c_SlaveTransmit(uint8_t data) //TODO: update to RxBuff
{
	uint32_t tmp, command;

	if(I2cMasterModeSelected)
			return;

	while(I2C3->SR2 & 2) {} //make sure bus is not busy  //TODO: busy condition should be a trigger instead of precheck

	while(!(I2C3->SR1 & 2)) {}; //wait until address flag is set
	//ack test
	//SysTick_DelayInMs(2);
	tmp = I2C3->SR2; //clear ADDR bit (SR1 bit 1)

	//while(!(I2C3->SR1 & 0x80)) {}; //wait until test previous data is sent

	//next address (for read request)
	while(!(I2C3->SR1 & 2)) {}; //wait until address flag is set
		//ack test
		//SysTick_DelayInMs(2);
		tmp = I2C3->SR2; //clear ADDR bit (SR1 bit 1)

	//check command
	while(!(I2C3->SR1 & 0x40)) {}
	command = I2C3->DR;

	//send data
	while(!(I2C3->SR2 & 0x4)) {}; //Read request

	switch(command) {

	case READ_INT_TEMP:
		I2C3->DR = data; //send data
		break;

	case TEST_DATA:
		I2C3->DR = 2;
		break;

	default:
		I2C3->DR = 3;
		break;
	}

	while(!(I2C3->SR1 & 0x80)) {}; //wait until test data is sent
	//TODO: send 2. byte (fraction). Do not forget to prepare Raspberry for it

	//TODO:remove while from acknowledge check
	//uncommented because Raspberry does not send acknowledge signal for some reason
	//while(I2C3->SR1 & 0X400) {} //note: in HAL: sr1itflags & I2C_FLAG_RXNE) != RESET), Where 0x00010040U 16.bit is 1. Why??

	//Generate stop condition
	I2C3->CR1 |=0x200;
}
