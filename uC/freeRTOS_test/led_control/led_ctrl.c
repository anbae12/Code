
/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: led_ctrl.c
*
* PROJECT....: EMP Assignments
*
* DESCRIPTION: Initializes and names the different switches, leds and peripherals. 
Special freeRTOS edition.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140220  MSC   Module created.
*
*****************************************************************************
* TODO: 
*   Add support for the rest of the switches + other peripherals.
****************************************************************************/

/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "emp_type.h"
#include "led_ctrl.h"
#include "FreeRTOS.h"
#include "task.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void gpio_ini(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{

  //Enable GPIO ports D, F & G
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
  
  INT32U dummy2;

  //Dummy read to insert a few cycles after enabling the peripheral
  //Necessary? 
  dummy2 = SYSCTL_RCGC2_R;


  //Set output pins
  GPIO_PORTF_DIR_R = PIN_LED_STATUS; //Status LED
  GPIO_PORTD_DIR_R = PIN_LED_RED; //red LED
  GPIO_PORTG_DIR_R |= PIN_LED_YELLOW; //yellow LED
  GPIO_PORTG_DIR_R |= PIN_LED_GREEN; //green LED

  //Enable GPIO ports
  GPIO_PORTD_DEN_R = PIN_LED_RED; //PORT D
  GPIO_PORTF_DEN_R = (PIN_LED_STATUS | PIN_SWITCH_SELECT); //PORT F
  GPIO_PORTG_DEN_R = (PIN_LED_YELLOW | PIN_LED_GREEN); //PORT G

  //Enable internal pull-up for select switch
  GPIO_PORTF_PUR_R = PIN_SWITCH_SELECT;

  //Turn off all LED's
  GPIO_PORTD_DATA_R |= (PIN_LED_RED);
  GPIO_PORTG_DATA_R |= (PIN_LED_YELLOW);
  GPIO_PORTG_DATA_R |= (PIN_LED_GREEN);
  
}

void status_led_alive(void *pvParameters)
// I'm alive! Flashes status led at 1Hz.
{
  while(1)
  {
    GPIO_PORTF_DATA_R ^= PIN_LED_STATUS;  // Toggle
    vTaskDelay(50 / portTICK_RATE_MS);   // Every 500 ms
  }
}

void toggle_yellow(void *pvParameters)
{
  //gpio_ini();
  while(1)
  {
    GPIO_PORTG_DATA_R ^= PIN_LED_YELLOW;
    vTaskDelay(2000 / portTICK_RATE_MS); // wait 2000 ms.
  }
}
void toggle_green(void *pvParameters)
{
  //gpio_ini();
  while(1)
  {
  GPIO_PORTG_DATA_R ^= PIN_LED_GREEN;
  vTaskDelay(3000 / portTICK_RATE_MS); // wait 3000 ms.
  }
}
/****************************** End Of Module *****************************/

