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
#include "configs/semaphore_project.h"
#include "inc/emp_type.h"
#include "inc/glob_def.h"
#include "inc/binary.h"
#include "inc/cpu.h"
#include "Uart/uart.h"
#include "Uart/uartprintf.h"
#include "interface/manager_interface.h"
// #include "strQ/string_queue.h"
/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO       0
#define LOW_PRIO        1
#define MED_PRIO        2
#define HIGH_PRIO 3

#define PF0             0               // Bit 0

#define GSS_IDLE        1
#define GSS_ID          2
#define GSS_PIN         3
#define GSS_FILLING     5
#define GSS_UPDATE      6


static void setupHardware(void)
/*****************************************************************************
 *   Input    :  -
 *   Output   :  -
 *   Function :
 *****************************************************************************/
{
  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  clk_system_init();
  uart0_init();
}

int main(void)
{

  setupHardware();

  xTaskCreate( uart_send_task, "uart send", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( uart_receive_task, "uart receive", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
  xTaskCreate( interface_task, "interface", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

  vTaskStartScheduler();

  return 1;

  //BS from westermann's code

  //Create mutexes:
  common_pins_mutex = xSemaphoreCreateMutex();
  lcd_image_mutex = xSemaphoreCreateMutex();
  pwm_duty_cycle_mutex = xSemaphoreCreateMutex();
  fan_ticks_mutex = xSemaphoreCreateMutex();
  uart_outgoing_mutex = xSemaphoreCreateMutex();
  uart_incoming_mutex = xSemaphoreCreateMutex();
  money_mutex = xSemaphoreCreateMutex();
  gas_type_mutex = xSemaphoreCreateMutex();
  current_price_pr_litre_mutex = xSemaphoreCreateMutex();
  quantity_mutex = xSemaphoreCreateMutex();
  gas_prices_mutex = xSemaphoreCreateMutex();
  log_mutex = xSemaphoreCreateMutex();
  total_running_time_mutex = xSemaphoreCreateMutex();

  //Create queues:
  user_input_queue = xQueueCreate(QUEUE_LEN,1);
  uart_send_queue = xQueueCreate(UART_QUEUE_LEN,1);
  uart_receive_queue = xQueueCreate(UART_QUEUE_LEN,1);
  control_input_queue = xQueueCreate(QUEUE_LEN,1);

}
