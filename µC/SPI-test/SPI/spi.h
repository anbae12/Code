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
               All page references is from the datasheet.
*
* Change Log: See the .c file
*****************************************************************************
* TODO: See the .c file
*   
****************************************************************************/
#ifndef _SPI_H
  #define _SPI_H


/***************************** Include files *******************************/
#include "emp_type.h"


/*****************************    Defines    *******************************/

#define SPI_MODE_MASTER                0x00000000
#define SPI_PRESCALE_VALUE             0x0000000A  // = 10
#define SPI_CLEAR_REGISTER             0x00000000

#define SPI_PINS                       0b00111100  // Bit mask for SPI pins

#define SPI_CLOCK_RATE                 0b00000101  // = 10
#define SPI_PROTOCOL_MODE              0b01        // TI SSI Frame format (p. 476)
#define SPI_DATA_SIZE                  0b0111      // 8 bit data (p. 487)
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

// extern void ();
extern void spi_init(); 
extern void spi_buffer_push( INT8U );


/****************************** End Of Module *******************************/
#endif
