/*****************************************************************************
* Odense University College of Enginerring
* Embedded Programming (EMP)
*
* MODULENAME.: 
*
* PROJECT....: My own
*
* DESCRIPTION:
* This module creates specific semaphores for this project.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140422  ALB    created
*
*****************************************************************************/

#ifndef _SEMAPHORE_PROJECT_H
  #define _SEMAPHORE_PROJECT_H

/***************************** Include files *******************************/
#include "inc/emp_type.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"

/*****************************    Defines    *******************************/

#define GAS_TYPE_A  1
#define GAS_TYPE_B  2
#define GAS_TYPE_C  3
#define LOG_SIZE    255

/*****************************   Constants   *******************************/
/******************************** Variables *********************************/
//---------------- Mutexes ----------------
extern xSemaphoreHandle common_pins_mutex;
extern xSemaphoreHandle lcd_image_mutex;
extern xSemaphoreHandle pwm_duty_cycle_mutex;
extern xSemaphoreHandle fan_ticks_mutex;
extern xSemaphoreHandle uart_outgoing_mutex;
extern xSemaphoreHandle uart_incoming_mutex;
extern xSemaphoreHandle money_mutex;
extern xSemaphoreHandle gas_type_mutex;
extern xSemaphoreHandle current_price_pr_litre_mutex;
extern xSemaphoreHandle quantity_mutex;
extern xSemaphoreHandle gas_prices_mutex;
extern xSemaphoreHandle log_mutex;
extern xSemaphoreHandle total_running_time_mutex;

//-------------- Semaphores ---------------
extern xSemaphoreHandle fan_interrupt_semaphore;
extern xSemaphoreHandle rtc_callback_semaphore;

//---------------- Queues -----------------
extern xQueueHandle     user_input_queue;
extern xQueueHandle     uart_send_queue;
extern xQueueHandle     uart_receive_queue;
extern xQueueHandle     control_input_queue;
extern xQueueHandle     log_input_queue;

//---------------- Tasks ------------------
extern xTaskHandle      alive_task_handle;
extern xTaskHandle      buttons_task_handle;
extern xTaskHandle      user_interface_task_handle;
extern xTaskHandle      keyboard_task_handle;
extern xTaskHandle      uart_send_task_handle;
extern xTaskHandle      uart_receive_task_handle;
extern xTaskHandle      manager_interface_task_handle;
extern xTaskHandle      lcd_task_handle;
extern xTaskHandle      fan_receive_task_handle;
extern xTaskHandle      control_task_handle;
extern xTaskHandle      sw_timer_event_task_handle;
extern xTaskHandle      logger_task_handle;
extern xTaskHandle      rtc_task_handle;

//---------------- Timers -----------------
// extern xTimerHandle     SW_TIMERS[MAX_SWTIMERS];
 

/*****************************   Functions   *******************************/

extern void semaphore_port2_init();
/*****************************************************************************
* Init semaphores for this project
*****************************************************************************/
#endif