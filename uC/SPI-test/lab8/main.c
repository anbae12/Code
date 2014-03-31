/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: FreeRTOS adaption.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140324  MoH	Module created.
*
*****************************************************************************/


/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "FreeRTOS.h"
#include "task.h"
#include "emp_type.h"
//#include "glob_def.h"
#include "binary.h"
#include "gpio_ini.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO	0
#define LOW_PRIO	1
#define	MED_PRIO	2
#define	HIGH_PRIO 3

#define PF0		0		// Bit 0


void status_led_init(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function :
*****************************************************************************/
{
  INT8S dummy;
  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  // Set the direction as output (PF0).
  SET_BIT_HIGH(GPIO_PORTF_DIR_R, PF0);

  // Enable the GPIO pins for digital function (PF0).
  SET_BIT_HIGH(GPIO_PORTF_DEN_R, PF0);

  // Set pin high
  SET_BIT_HIGH(GPIO_PORTF_DATA_R, PF0);
}


void status_led_task(void *pvParameters)
{

  status_led_init();

  while(1)
  {
	// Toggle status led
	TOGGLE_BIT(GPIO_PORTF_DATA_R, PF0);
	vTaskDelay(500 / portTICK_RATE_MS); // wait 500 ms.
  }
}

void clk_system_init()
/*****************************************************************************
*   Input    : 	-
*   Function : Set the system clock to 50 Mhz.
*****************************************************************************/
{
	INT32U dummy;
	// Init clock settings according to datasheet
	// Set Clock speed to 50 Mhz.

	// Step 1
	// Set BYPASS bit
	SYSCTL_RCC_R |= SYSCTL_RCC_BYPASS;
	// Clear USESYS bits
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_USESYSDIV);
	// Enable Main osc.
	SYSCTL_RCC_R |= SYSCTL_RCC_MOSCDIS;

	// Step 2
	// Clear PLL lock flag
	SYSCTL_RIS_R = ~(SYSCTL_RIS_PLLLRIS);
	// Set XTAL value, first clear bits then set
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_XTAL_M);
	SYSCTL_RCC_R |= SYSCTL_RCC_XTAL_8MHZ;
	// Set OSCSRC, first clear bits then set
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_OSCSRC_M);
	SYSCTL_RCC_R |= SYSCTL_RCC_OSCSRC_MAIN;
	// Clear PWRDN
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_PWRDN);

	// delay
	for (dummy = 200000; dummy >0; dummy--);

	// Step 3
	// Set SYSDIV, first clear bits then set
	// Set to 50 Mhz.
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_SYSDIV_M);
	SYSCTL_RCC_R |= SYSCTL_RCC_SYSDIV_3;
	// Set USESYS bit
	SYSCTL_RCC_R |= SYSCTL_RCC_USESYSDIV;

	// Step 4
	// wait for PLL lock
	while(!(SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS));

	// Step 5
	// Clear BYPASS bit
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_BYPASS);

	// delay
	for (dummy = 200000; dummy >0; dummy--);

}



static void setupHardware(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function :
*****************************************************************************/
{
	// TODO: Put hardware configuration and initialization in here

	// Warning: If you do not initialize the hardware clock, the timings will be inaccurate
	clk_system_init();
}

int main(void)
{
  portBASE_TYPE return_value = pdTRUE;

  setupHardware();

  /*
   * Start the tasks defined within this file/specific to this demo.
   */
  xTaskCreate( status_led_task, "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

  // Create toggle yellow task:

  xTaskCreate( toggle_yellow, "Yellow_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( toggle_green, "Green_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  /*
   * Start the scheduler.
   */
  vTaskStartScheduler();

  /*
   * Will only get here if there was insufficient memory to create the idle task.
   */
  return 1;

}
