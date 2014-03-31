
/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: keypad.c
*
* PROJECT....: EMP Assignments
*
* DESCRIPTION: Driver for a 3x4 keypad
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140307  MSC   Module created.
*
*****************************************************************************
* TODO: 
*   Make separate init function for using the keypad on its own.
****************************************************************************/

/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "emp_type.h"
#include "keypad.h"
#include "spi.h"
#include "fifo.h"
//#include "ass4_modes.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT32U timer_var = 0;

/*****************************   Functions   *******************************/
void pin_enable() 
/*****************************************************************************
*   Input    : 
*   Output   :  
*   Function :  Configures I/O for use with the keypad. 
                This is necessary, due to sharing PortB with lcd
******************************************************************************/
{
  //Enable port B
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

  //Enable port D
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
  
  // Set x pins as output: 
  GPIO_PORTB_DIR_R |= KEYPAD_X_PINS;
  GPIO_PORTB_DEN_R |= KEYPAD_X_PINS;
  
  // Set y pins as input:
  GPIO_PORTD_DIR_R &= ~(KEYPAD_Y_PINS);
  GPIO_PORTD_DEN_R |= KEYPAD_Y_PINS;
  
  
}

void pin_disable()

{
  //Disable y pins
  GPIO_PORTD_DEN_R &= ~(KEYPAD_Y_PINS);

}

INT8U keypad_task()
/*****************************************************************************
*   Input    : 
*   Output   :  
*   Function :  Configures I/O for use with the keypad. 
                This is necessary, due to sharing PortB with lcd
******************************************************************************/
{
  pin_enable();
  static BOOLEAN debounce[3][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};

  INT8U ascii_chars[3][4] = {{'1', '4', '7', '*'},{'2', '5', '8', '0'},{'3', '6', '9', '#'}};
  
  INT8U x_pins[3] = {KEYPAD_X1, KEYPAD_X2, KEYPAD_X3};
  INT8U y_pins[4] = {KEYPAD_Y1, KEYPAD_Y2, KEYPAD_Y3, KEYPAD_Y4};
  INT8U return_char = '\0';
  
  // Looping variables
  INT8U i;
  INT8U j;
  //Outer loop, selects column
  for (i = 0; i < 3; i++)
  {
    // Supply voltage to selected column
    GPIO_PORTB_DATA_R &= ~(KEYPAD_X_PINS);
    GPIO_PORTB_DATA_R |= x_pins[i];

    // Loops through buttons in row i
    for (j = 0; j < 4; j++)
    {
      if (GPIO_PORTD_DATA_R & y_pins[j])
      {
        if (!debounce[i][j])
        {
          return_char = ascii_chars[i][j];
        }
        debounce[i][j] = 1;
      }
      else 
      {
        debounce[i][j] = 0;
      }
    }
  }
  GPIO_PORTB_DATA_R &= ~(KEYPAD_X_PINS);
  
  if (return_char != '\0') 
  {
    spi_buffer_push( return_char );
  }
  
  
  pin_disable();
  return return_char;
  
  
  
}

/****************************** End Of Module *****************************/

