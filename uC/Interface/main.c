/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: FreeRTOS adaptation.
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140408  NI   Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "inc/lm3s6965.h"
#include "configs/project_settings.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"
#include "configs/project_settings.h"
#include "inc/emp_type.h"
#include "inc/glob_def.h"
#include "inc/binary.h"
#include "inc/cpu.h"
#include "uart/uart.h"
#include "interface/interface.h"

/*****************************    Defines    *******************************/

static void init_hardware(void)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  disable_global_int();
  clk_system_init();
  uart0_init();
  enable_global_int();
}
static void init_tasks_presched()
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function : Initialization of application tasks.
 *              Must be run before scheduler starts.
 *              Does not initialize any hardware.
 *****************************************************************************/
{
  init_uart_receive_task();
  init_uart_send_task();
}

int main(void)
{

  init_hardware();
  init_tasks_presched();

  xTaskCreate( uart_send_task, "uart send", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( uart_receive_task, "uart receive", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( interface_task, "interface", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

  vTaskStartScheduler();

  return 1;

}
