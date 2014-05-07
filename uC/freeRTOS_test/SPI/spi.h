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
#define SPI_PRESCALE_VALUE             0x00000004  // = 10
#define SPI_CLEAR_REGISTER             0x00000000

#define SPI_PINS                       0b00111100  // Bit mask for SPI pins

#define SPI_CLOCK_RATE                 0b00000001  // = 10
#define SPI_PROTOCOL_MODE              0b01        // TI SSI Frame format (p. 476)
#define SPI_DATA_SIZE                  0b1111      // 16 bit data (p. 487)

#define SPI_MOTOR_SEL_MASK             0x00008000  // Bit mask for reading the motor bit
#define SPI_MOTOR_POS_MASK             0x000007FF  // Bit mask for reading motor position
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

// extern void ();
extern void spi_init(); 
extern void spi_buffer_push( INT16U );
extern void spi_receive_task( void *pvParameters);
extern void spi_test_task( void *pvParameters);
/****************************** End Of Module *******************************/
#endif
