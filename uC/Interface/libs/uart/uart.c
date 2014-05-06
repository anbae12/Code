/*****************************************************************************
 * Mikkel, Åse & Mikael
 * MODULENAME:  uart
 * DESCRIPTION:
 ****************************************************************************/

/***************************** Include files *******************************/
#include "string.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "inc/glob_def.h"
#include "inc/binary.h"
#include "configs/uartcnf.h"
#include "uart.h"
#include "configs/tmodel.h"
#include "configs/project_settings.h"

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
  // The start bit is always there.
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

void uart_char_put_blocking(INT8S data)
{
  while(uart_char_put_non_blocking(data)==FALSE);
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

INT8S uart_char_get_blocking()
{
  INT8S retval=-1;
  while(retval==-1)
  {
    retval=uart_char_get_non_blocking();
  }
  return retval;
}




void uart_send_task(void *pvParameters)
{

  INT8S send_char;

  while(1)
  {
    while(xQueueReceive(uart_send_queue,&send_char,portMAX_DELAY)==pdTRUE)
    {
      uart_char_put_blocking(send_char);
    }

    _wait(MILLI_SEC(5));
  }
}

void uart_receive_task(void *pvParameters)
{

  INT8S received_char;

  while(1)
  {
    while(uart_char_avail())
    {
      received_char=uart_char_get_blocking();
      while(xQueueSend(uart_receive_queue,&received_char,portMAX_DELAY)!=pdTRUE);
    }

    _wait(MILLI_SEC(5));
  }
}

void uart_push_string(char *string_to_push)
{
  INT8U i;
  INT8U ch;
  //Enqueue the string.
  while(xSemaphoreTake(uart_outgoing_mutex,portMAX_DELAY)!=pdTRUE);
  for(i = 0; i < strlen(string_to_push)/sizeof(string_to_push[0]); i++)
  {
    if( !(string_to_push[i] == '\0') )
    {
      ch = string_to_push[i];
      while(xQueueSend(uart_send_queue,&ch,portMAX_DELAY)!=pdTRUE);

      switch(ch)
      {
      case '\r':
        ch='\n';
        while(xQueueSend(uart_send_queue,&ch,portMAX_DELAY)!=pdTRUE);
        break;
      case '\n':
        ch='\r';
        while(xQueueSend(uart_send_queue,&ch,portMAX_DELAY)!=pdTRUE);
        break;
      default:
        break;
      }

    }
    else break;
  }
  xSemaphoreGive(uart_outgoing_mutex);
}

void uart_push_char(INT8U char_to_push)
{
  while(xSemaphoreTake(uart_outgoing_mutex,portMAX_DELAY)!=pdTRUE);
  uart_direct_push_char(char_to_push);
  xSemaphoreGive(uart_outgoing_mutex);
}

void uart_direct_push_char(INT8U char_to_push)
{
  while(xQueueSend(uart_send_queue,&char_to_push,portMAX_DELAY)!=pdTRUE);
}

BOOLEAN uart_pop_string_echo(char *string, INT8U string_length, BOOLEAN echo)
{
  BOOLEAN got_string = FALSE;
  BOOLEAN zero_pad = FALSE;
  INT8U i;
  INT8U ch;

  while(xSemaphoreTake(uart_incoming_mutex,portMAX_DELAY)!=pdTRUE);



  if(uxQueueMessagesWaiting(uart_receive_queue) > 0)
  {
    got_string = TRUE;
    for(i=0; i<(string_length-1); i++)
    {
      if(!zero_pad)
      {
        while(xQueueReceive(uart_receive_queue,&ch,portMAX_DELAY)!=pdTRUE);

        if(char_valid(ch))
        {
          if(echo)
            uart_push_char(ch);

          string[i] = ch;
        }
        if(ch=='\r')
        {
          if(echo)
            uart_push_char('\n');
          string[i] = '\0';     //Remove the \n and end the string.
          zero_pad = TRUE;
        }
        else if(ch=='\n')
        {
          if(echo)
            uart_push_char('\r');
          string[i] = '\0';     //Remove the \r and end the string.
          zero_pad = TRUE;
        }

      }
      else
      {
        string[i]='\0';
        break;
      }
    }
    string[string_length-1]='\0'; // No matter what.
  }
  xSemaphoreGive(uart_incoming_mutex);

  return got_string;
}

BOOLEAN uart_pop_string(char *string, INT8U string_max_length)
{
  return uart_pop_string_echo(string,string_max_length,FALSE);
}

/***************************** End Of Module *******************************/












