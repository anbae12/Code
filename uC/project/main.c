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

#include "queue/queue_ini.h"

#include "read_pwm/read_pwm.h"
#include "inc/glob_def.h"
#include "inc/binary.h"
#include "inc/cpu.h"
#include "uart/uart.h"
#include "interface/interface.h"
#include "SPI/spi.h"
#include "ctrl/ctrl_task.h"
#include "ctrl/controller.h"
#include "led/led_on_off.h"

//The new and improved....
#include "logger/logger.h"

#include "inc/gpio_ini.h"

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
  spi_init();
  init_gpio();
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
  init_sem_and_queues();
  init_logger_presched();
  led_ryg(0,0,0);
}

int main(void)
{
  init_hardware();
  init_tasks_presched();
  portBASE_TYPE return_val = pdTRUE;

  return_val &= xTaskCreate( uart_send_task, "uart send", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_val &= xTaskCreate( uart_receive_task, "uart receive", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  return_val &= xTaskCreate( interface_task, "interface", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

  return_val &= xTaskCreate( ctrl_task, "control task", USERTASK_STACK_SIZE, NULL, MED_PRIO, NULL);
  return_val &= xTaskCreate( logger_task, "logger", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);

  if( return_val != pdTRUE )
  {
    uart_char_put_blocking('E');
    uart_char_put_blocking('R');
    uart_char_put_blocking('R');
  }
  vTaskStartScheduler();

  return 1;
}

char putChar(char input)//in case of stack overflow
{
  uart_char_put_blocking(input);
  return 1;
}
