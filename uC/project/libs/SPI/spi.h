/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: spi.h
*
* PROJECT....: Pan and tilt project
*
* DESCRIPTION: Implements hardware SPI/SSI on the LM3S6965. 
               Uses the TI SSI Frame Format (p. 476, data sheet)
               All page references is from the data sheet.
*
* Change Log: See the .c file
*****************************************************************************
* TODO: See the .c file
*   
****************************************************************************/
#ifndef _SPI_H
  #define _SPI_H


/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

// motorbit | retningsbit | pwmbit| 2 ignore | 11 pwm
#define SPI_MOTOR_BIT_POS     15
#define SPI_DIRECTION_BIT_POS 14
#define SPI_PWM_BIT_POS       13
#define SPI_PWM_MASK          0x07FF




#define SPI_MODE_MASTER                0x00000000
#define SPI_PRESCALE_VALUE             0x0000000A  // = 10
#define SPI_CLEAR_REGISTER             0x00000000

#define SPI_PINS                       0b00111100  // Bit mask for SPI pins

#define SPI_CLOCK_RATE                 0b00110010  // = 50
#define SPI_PROTOCOL_MODE              0b01        // TI SSI Frame format (p. 476)
#define SPI_DATA_SIZE                  0b1111      // 16 bit data (p. 487)

#define SPI_MOTOR_SEL_MASK             0x00008000  // Bit mask for reading the motor bit
#define SPI_MOTOR_POS_MASK             0x000007FF  // Bit mask for reading motor position

#define SPI_MOTOR_SEL_BIT_POS          15
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
typedef struct motor_pos {
  FP32 positionA;
  FP32 positionB;
} motor_pos;

typedef struct pwm_duty_cycle_type {
  INT16U motorA;
  INT16U motorB;
  BOOLEAN directionA;
  BOOLEAN directionB;
} pwm_duty_cycle_type;

/*****************************   Functions   *******************************/
extern motor_pos spi_read_encoders();
extern void spi_init(); 
extern void spi_buffer_push( INT16U );
extern INT16U spi_receive( void );
extern void spi_receive_task( void *pvParameters );
extern void spi_test_task( void *pvParameters );
/****************************** End Of Module *******************************/
#endif
