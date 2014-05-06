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
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "spi.h"

#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"

#include "queue/queue_ini.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void spi_init(void) 
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : Initializes SPI
 *   		   Based on p. 483 in data sheet
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

void spi_buffer_push( INT16U data )
{
  // Convert data to 32bit (why am i doing this?)
  INT32U data32 = 0x0;
  data32 |= data;
  SSI0_DR_R = data32;
}

INT16U spi_receive( void )
{
  // Reads the incoming buffer to see if any data is received
  // The received data is added to the LIFO buffer

  // Check if data is available
  // By reading "Receive not empty" in the status register (p. 491)
  INT32U data_from_buffer;
  INT16U motor_position;
  INT8U motor_id; // May not be necessary

  if (SSI0_SR_R & SSI_SR_RNE)
  {
    data_from_buffer = SSI0_DR_R;
    motor_id = ((data_from_buffer & SPI_MOTOR_SEL_MASK) >> SPI_MOTOR_SEL_BIT_POS);
    motor_position = (data_from_buffer & SPI_MOTOR_POS_MASK);

    if (! xQueueSendToFront(enc_queue[motor_id], &motor_position, 100))
    {
      //error
    }



  }
}


void spi_test_task( void *pvParameters)
{
  vTaskDelay(3000 / portTICK_RATE_MS);
  while(1)
  {
    spi_buffer_push(0xF0F0);
    vTaskDelay(1000 / portTICK_RATE_MS);
    spi_buffer_push(0x0F0F);
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

void spi_receive_task( void *pvParameters)
{
  //write_data(0);
  while (1)
  {
    spi_receive();
  }
}
