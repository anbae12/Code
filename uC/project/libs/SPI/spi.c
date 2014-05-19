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

#include "configs/project_settings.h"

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
  // The received data is returned as INT16U
  // If data is not available, 0xFFFF is returned. 

  
  INT16U return_value = 0xFFFF;
  
  // Check if data is available
  // By reading "Receive not empty" in the status register (p. 491)
  if (SSI0_SR_R & SSI_SR_RNE)
  {
    return_value = (SSI0_DR_R & 0x0000FFFF); // Only 16 lowest bits is valie (p. 490)
  }
  return return_value;
}

motor_pos spi_read_encoders()
// This function reads the position of the PTS, 
// and returns a motor_pos struct (defined in spi.h)
{
  motor_pos return_value;
  INT16U data_in;

  INT8U i;
  INT32U count;

  //empty spi buffer
  do
  {
    data_in = spi_receive();
  }
  while( data_in != 0xFFFF );

// motorbit | retningsbit | pwmbit| 2 ignore | 11 pwm
  
  for(i = 0; i < 2; i++)
  {
    spi_buffer_push(0x0000); // Send a pwm, with ignorebit set.
    for(count = 0; count < SPI_WAIT; count++); // If baudrate = 100k
    data_in = spi_receive();
    if (data_in != 0xFFFF) // Check if data is valid
    {
      if( !(data_in & SPI_MOTOR_SEL_MASK) ) 
      {
        return_value.positionA = (FP32) (data_in & SPI_MOTOR_POS_MASK);
      }
      else 
      {
        return_value.positionB = (FP32) (data_in & SPI_MOTOR_POS_MASK);
      }
    }
  }
  return return_value;
}

void spi_send_pwm(pwm_duty_cycle_type pwm)
{
  INT16U data_in;
  INT16U count;
  //create message

  INT16U messageA = 0;
  INT16U messageB = 0;
  // motorbit | retningsbit | pwmbit| 2 ignore | 11 pwm
  BOOLEAN motorA_direction = 1;
  BOOLEAN motorB_direction = 1;

  //check direction
  if(pwm.motorA < 0)
  {
    motorA_direction = 0;
    pwm.motorA = pwm.motorA * (- 1);
  }
  if(pwm.motorB < 0)
  {
    motorB_direction = 0;
    pwm.motorB = pwm.motorB * (-1);
  }

  //Saturate pwm
  if(pwm.motorA > 2047)
  {
    pwm.motorA = 2047;
  }
  if(pwm.motorB > 2047)
  {
    pwm.motorB = 2047;
  }


  messageA = (motorA_direction & 1) << SPI_DIRECTION_BIT_POS;
  messageA |= (INT16U) pwm.motorA; 
  messageA |= 1 << SPI_PWM_BIT_POS;
  messageA |= 1 << SPI_MOTOR_BIT_POS;

  messageB = (motorB_direction & 1) << SPI_DIRECTION_BIT_POS;
  messageB |= (INT16U) pwm.motorB;
  messageB |= 1 << SPI_PWM_BIT_POS;
  messageB &= ~(1 << SPI_MOTOR_BIT_POS);

  static INT8U qwerty = 0;
  if( UART_MSG_OUT_DEBUG )
  {
    qwerty++;
    if(qwerty == 100){
    qwerty = 0;
    PRINTF("MESSAGEA = 0x%x\t",messageA);
    PRINTF("MESSAGEB = 0x%x\n",messageB);
    }}
  spi_buffer_push(messageA);
  for(count = 0; count < SPI_WAIT; count++);
  data_in = spi_receive(); // unused

  spi_buffer_push(messageB);
  for(count = 0; count < SPI_WAIT; count++);
  data_in = spi_receive(); // unused
}

