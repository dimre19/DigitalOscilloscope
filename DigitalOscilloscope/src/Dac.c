/*
 * Dac.c
 *
 *  Created on: Jun 8, 2020
 *      Author: David Imre
 */
#include "Dac.h"

// 32-sample sine wave.
#define _AMP(x) ( x / 8 ) + 100
const size_t SINE_SAMPLES = 32; //resolution of sine-wave
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

void Dac_Init()
{
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
}
