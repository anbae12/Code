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

#include "binary.h"
#include "spi.h"

#include "led_ctrl.h"
#include "cpu.h"
#include "lifo.h"

#include "queue_ini.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO	0
#define LOW_PRIO	1
#define	MED_PRIO	2
#define	HIGH_PRIO 3

#define PF0		0		// Bit 0


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
  gpio_ini();
  init_queue();
}

int main(void)
{
  portBASE_TYPE return_value = pdTRUE;

  setupHardware();

  /*
   * Start the tasks defined within this file/specific to this demo.
   */
  xTaskCreate( status_led_alive, "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );


  //xTaskCreate( init_queue_task, "Queue_init", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);

  // Create toggle yellow task:

  xTaskCreate( spi_receive_task, "Spi_receive", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);
  xTaskCreate( spi_test_task, "SPI_test", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);

  xTaskCreate( toggle_green, "Green_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( buffer_full_debug_task, "Yellow_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

  /*
   * Start the scheduler.
   */
  vTaskStartScheduler();

  /*
   * Will only get here if there was insufficient memory to create the idle task.
   */
  return 1;

}
