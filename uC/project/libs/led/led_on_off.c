/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: led_on_off.c
*
* PROJECT....: Traffic light
*
* DESCRIPTION: LED on/offs
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140220  NiV   Created
*
*****************************************************************************
* TODO:
*   
****************************************************************************/

/***************************** Include files *******************************/
#include "inc/lm3s6965.h"
#include "inc/gpio_ini.h"
#include "led_on_off.h"



/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void turn_on_red()        { GPIO_PORTD_DATA_R &= ~(PIN_LED_RED); }
void turn_off_red()       { GPIO_PORTD_DATA_R |= (PIN_LED_RED); }
void turn_on_yellow()        { GPIO_PORTG_DATA_R &= ~(PIN_LED_YELLOW); }
void turn_off_yellow()       { GPIO_PORTG_DATA_R |= (PIN_LED_YELLOW); }
void turn_on_green()        { GPIO_PORTG_DATA_R &= ~(PIN_LED_GREEN); }
void turn_off_green()       { GPIO_PORTG_DATA_R |= (PIN_LED_GREEN); }

void led_ryg(BOOLEAN red, BOOLEAN yellow, BOOLEAN green)
{
  if ( red ) 
  {
    turn_on_red();
  }
  else 
  {
    turn_off_red();
  }
  if ( yellow )
  {
    turn_on_yellow();
  }
  else 
  {
    turn_off_yellow();
  }
  if ( green )
  {
    turn_on_green();
  }
  else 
  { 
    turn_off_green();
  }
}
