/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: spi.h
*
* PROJECT....: Pan and tilt project
*
* DESCRIPTION: Implements hardware SPI/SSI on the LM3S6965. 
               Uses the TI SSI Frame Format (p. 476, datasheet)
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140328  MSC   Created
*
*****************************************************************************
* TODO:
*   Initialize function
****************************************************************************/

/***************************** Include files *******************************/
//#include "fifo.h"
#include "lm3s6965.h"
#include "emp_type.h"
#include "spi.h"



/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void spi_init(void) 
/*****************************************************************************
*   Input    :
*   Output   :
*   Function : Initializes SPI
*   		   Based on p. 483 in datasheet
******************************************************************************/
{
  INT8U count = 0;
  // SSI_CLK = PA2
  // SSI_SS  = PA3
  // SSI_TX  = PA4
  // SSI_RX  = PA5
  // bitmask SPI_PINS
  // Enable portA
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
  for(count = 0; count < 100; count++);

  
  // Writes I/O registers according to table 8.6, p. 294
  GPIO_PORTA_AFSEL_R |= SPI_PINS;
  GPIO_PORTA_ODR_R &= ~(SPI_PINS);
  GPIO_PORTA_DEN_R |= SPI_PINS;
  GPIO_PORTA_DR2R_R |= SPI_PINS;

	// Enable clock for SPI
	SYSCTL_RCGC1_R |= (SYSCTL_RCGC1_SSI0);

	// Disable SPI
	SSI0_CR1_R &= ~(SSI_CR1_SSE);

	// Set µC as master
	SSI0_CR1_R = SPI_MODE_MASTER;

	// Set prescaler (later: change this to preserver bits (31:8)
	SSI0_CPSR_R = SPI_PRESCALE_VALUE;



	// The SSI CR0 contains clk rate, protocol mode, data size:
	// Clear register
	SSI0_CR0_R = SPI_CLEAR_REGISTER;

	// Set clockrate
	SSI0_CR0_R |= (SPI_CLOCK_RATE << 8);

	// Set protocol mode = TI SSI Frame Format
	SSI0_CR0_R |= (SPI_PROTOCOL_MODE << 4);

	// Set data size
	SSI0_CR0_R |= (SPI_DATA_SIZE);


	// Enable SPI
	SSI0_CR1_R |= (SSI_CR1_SSE);


}

void spi_buffer_push( INT8U data ) 
{
  // Convert data to 32bit
  INT32U data32 = 0x0;
  data32 |= data;
  SSI0_DR_R = data32;
}
