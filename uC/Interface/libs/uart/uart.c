/*****************************************************************************
* Odense University College of Enginerring
* Embedded Programming (EMP)
*
* MODULENAME.: uartprintf.c
*
* PROJECT....: EMP
*
* DESCRIPTION:
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 100216  KHA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdarg.h>
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "inc/glob_def.h"
#include "inc/binary.h"
#include "configs/uartcnf.h"
#include "uart.h"
#include "string.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/


void uart0_init(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	
	// Enable UART0 peripherial 
	SYSCTL_RCGC1_R  |= SYSCTL_RCGC1_UART0;
	
	/**
	* Enable the pins used by the UART
	*/
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	
	// PORTA [0:1] to alternative function
	SET_BIT_HIGH(GPIO_PORTA_AFSEL_R,0);
	SET_BIT_HIGH(GPIO_PORTA_AFSEL_R,1);
	
	// Enable digital function
	SET_BIT_HIGH(GPIO_PORTA_DEN_R,0);
	SET_BIT_HIGH(GPIO_PORTA_DEN_R,1);
	
	// Disable UART
	UART0_CTL_R &= ~(UART_CTL_UARTEN);
	
	// Clear UART pending interrupt.
	UART0_ICR_R |= 0x7F0;
	
	// Baud rate setup.
	UART0_IBRD_R = B_DIV_INT;
	UART0_FBRD_R = B_DIV_FRACT;
	
	// UART setup
	// Clear Line control register
	// UART0_LCRH_R = 0;
	// 8 bit data, 1 stop bit, no parity.
	// Enable FIFO
	UART0_LCRH_R |= UART_LCRH_WLEN_8 | UART_LCRH_FEN;
	
	// Uart0 enable transmitter
	UART0_CTL_R |= UART_CTL_TXE | UART_CTL_RXE;
	
	// Enable UART0
	UART0_CTL_R |= UART_CTL_UARTEN;
}



BOOLEAN uart_char_put_non_blocking(INT8S data)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	BOOLEAN return_value = FALSE;

	if(!(UART0_FR_R & UART_FR_TXFF))
	{
    // Write the character to the transmit FIFO.
	  UART0_DR_R = data;
	  return_value = TRUE;
	}
	else
	{
    // There is no space in the transmit FIFO, so return a failure.
	  return_value = FALSE;
	}
  return return_value;
}



BOOLEAN uart_char_avail(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	BOOLEAN return_value = FALSE;
	//
  // Return the availability of characters.
  //
  if(UART0_FR_R & UART_FR_RXFE)
  {
  	// FIFO empty
  	return_value = FALSE;
  }
  else
  {
  	// FIFO not empty
  	return_value = TRUE;
  }
  return return_value;
}



INT8S uart_char_get_non_blocking(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{

	INT8S return_value = -1;

	//
  // See if there are any characters in the receive FIFO.
  //
  if(!(UART0_FR_R & UART_FR_RXFE))
  {
  	//
    // Read and return the next character.
    //
    return_value = UART0_DR_R;
  }
  else
  {
    //
    // There are no characters, so return a failure.
    //
   	return_value = -1;
  }
  return return_value;
}

void uart_string_put(INT8U* input)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  INT8U x;
  for(x = 0; x < strlen(input)/sizeof(input[0]); x++)
  {
    if( !(input[x] == '\0') )
    {
    uart_char_put_non_blocking(input[x]);
    }
  }
}

void uart_number(INT16U input)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  INT8U a;
  INT8U b;
  INT8U c;
  INT8U d;
  
  d = input%10000; 
  c = (input-d)%1000; 
  b = (input-d-c)%100; 
  a = (input-d-c-b)%10; 
  
  uart_char_put_non_blocking(a+48);
  uart_char_put_non_blocking(b+48);
  uart_char_put_non_blocking(c+48);
  uart_char_put_non_blocking(d+48);
}



/***************************** End Of Module *******************************/












