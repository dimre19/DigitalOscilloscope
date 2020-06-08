/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "Led.h"
#include "Timing.h"
#include "Spi.h"
#include "I2c.h"
#include "Adc.h"
// ----------------------------------------------------------------------------
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// 32-sample sine wave.
#define _AMP(x) ( x / 8 ) + 100
const size_t SINE_SAMPLES = 32;
const uint16_t SINE_WAVE[] = {
  _AMP(2048), _AMP(2447), _AMP(2831), _AMP(3185),
  _AMP(3495), _AMP(3750), _AMP(3939), _AMP(4056),
  _AMP(4095), _AMP(4056), _AMP(3939), _AMP(3750),
  _AMP(3495), _AMP(3185), _AMP(2831), _AMP(2447),
  _AMP(2048), _AMP(1649), _AMP(1265), _AMP(911),
  _AMP(601),  _AMP(346),  _AMP(157),  _AMP(40),
  _AMP(0),    _AMP(40),   _AMP(157),  _AMP(346),
  _AMP(601),  _AMP(911),  _AMP(1265), _AMP(1649)
};

uint16_t results[32*5];
// Simple imprecise delay method.
void __attribute__( ( optimize( "O0" ) ) )
delay_cycles( uint32_t cyc ) {
  for ( uint32_t d_i = 0; d_i < cyc; ++d_i ) { asm( "NOP" ); }
}

#if  !MODULE_TEST_ON
int main(int argc, char* argv[])
{
//  // At this stage the system clock should have already been configured
//  // at high speed.
//	//uint32_t spiRxBuff;
//	//TODO: manual test of SysTick in SysTick_TickStop()
	/*uint8_t*/uint16_t tempSensorVal, adcResult; //TODO: update to double --> how to handle in I2C communication?
	double celsius;
	int i = 0;
//	Led_Init();
//	I2c_Init();
	TIM2_Init(); //for ADC Internal Temp Read
	Adc_Init();
//
//	while (1)
//	{
//		//SPI test
////		Spi_WriteData();
////		spiRxBuff = Spi_ReadData();
////		celsius = Adc_ReadIntTemp(); //1sec period
////		tempSensorVal = (uint8_t)celsius;
//		//at start generation this function shall be already called.
//		adcResult = Adc_Read()/16;
//		I2c_SlaveTransmit(adcResult); //period = elapsed time between two read request (from Raspberry Pi)
//
//		Led_Toggle();
//		SysTick_DelayInMs(30);
//	}

  RCC->AHB1ENR  |= ( RCC_AHB1ENR_GPIOAEN |
					 RCC_AHB1ENR_DMA1EN );
  RCC->APB1ENR  |= ( RCC_APB1ENR_DACEN |
					 RCC_APB1ENR_TIM6EN );

  // Pin A4 output type: Analog.
  GPIOA->MODER    &= ~( 0x3 << ( 4 * 2 ) );
  GPIOA->MODER    |=  ( 0x3 << ( 4 * 2 ) );

  // DMA configuration (channel 7 / stream 5).
  // SxCR register:
  // - Memory-to-peripheral
  // - Circular mode enabled.
  // - Increment memory ptr, don't increment periph ptr.
  // - 16-bit data size for both source and destination.
  // - High priority (2/3).
  DMA1_Stream5->CR &= ~( DMA_SxCR_CHSEL |
						 DMA_SxCR_PL |
						 DMA_SxCR_MSIZE |
						 DMA_SxCR_PSIZE |
						 DMA_SxCR_PINC |
						 DMA_SxCR_EN );
  DMA1_Stream5->CR |=  ( ( DMA_PRIORITY_HIGH/*0x2 << DMA_SxCR_PL_Pos*/ ) |
						 ( DMA_MDATAALIGN_HALFWORD/*0x1 << DMA_SxCR_MSIZE_Pos */) |
						 ( DMA_PDATAALIGN_HALFWORD/*0x1 << DMA_SxCR_PSIZE_Pos*/ ) |
						 ( DMA_CHANNEL_7/*0x7 << DMA_SxCR_CHSEL_Pos*/ ) |
						 DMA_MINC_ENABLE /*DMA_SxCR_MINC*/ |
						 DMA_CIRCULAR/*DMA_SxCR_CIRC*/ |
						 ( DMA_MEMORY_TO_PERIPH/*0x1 << DMA_SxCR_DIR_Pos*/ ) );
  // Set DMA source and destination addresses.
  // Source: Address of the sine wave buffer in memory.
  DMA1_Stream5->M0AR  = ( uint32_t )&SINE_WAVE;
  // Dest.: DAC1 Ch1 '12-bit right-aligned data' register.
  DMA1_Stream5->PAR   = ( uint32_t )&( DAC->DHR12R1 );
  // Set DMA data transfer length (# of sine wave samples).
  DMA1_Stream5->NDTR  = ( uint16_t )SINE_SAMPLES;
  // Enable DMA1 Stream 5.
  DMA1_Stream5->CR   |= ( DMA_SxCR_EN );

  // TIM6 configuration.
  // Set prescaler and autoreload for a 440Hz sine wave.
  TIM6->PSC  =  ( 0x0000 );
  TIM6->ARR  =  ( SystemCoreClock / ( 440 * SINE_SAMPLES ) );
  // Enable trigger output on timer update events.
  TIM6->CR2 &= ~( TIM_CR2_MMS );
  TIM6->CR2 |=  ( TIM_TRGO_UPDATE /* 0x2 << TIM_CR2_MMS_Pos*/ );
  // Start the timer.
  TIM6->CR1 |=  ( TIM_CR1_CEN );

  // DAC configuration.
  // Set trigger sources to TIM6 TRGO.
  DAC->CR  &= ~( DAC_CR_TSEL1 );
  // Enable DAC DMA requests.
  DAC->CR  |=  ( DAC_CR_DMAEN1 );
  // Enable DAC Channels.
  DAC->CR  |=  ( DAC_CR_EN1 );
  // Delay briefly to allow sampling to stabilize (?)
  delay_cycles( 1000 );
  // Enable DAC channel trigger.
  DAC->CR  |=  ( DAC_CR_TEN1 );

  // Done; a low-res 440Hz sine wave should be playing on PA4.
  while (1) {
	  adcResult = Adc_Read();
	  results[i++ % (32*5)] = adcResult;
	  }
}

#endif
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
