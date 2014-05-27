/*****************************************************************************
 * Mikkel, Åse & Mikael
 * MODULENAME:  tmodel.h
 * DESCRIPTION: Assignment task model.
 ****************************************************************************/
#pragma once
/****************************** Include files *******************************/
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "configs/FreeRTOSConfig.h"
#include "uart/uartprintf.h" //For PRINTF helper macro define.

/********************************* Defines **********************************/
//------------- Helper Macros -------------
#define _wait(no_of_ticks)                      vTaskDelay(no_of_ticks)
#define MILLI_SEC(ms)                           ((ms*configTICK_RATE_HZ)/1000)
#define CEIL_DIV(x, y)                          ((x) + (y) - 1) / (y)
#define PRINTF                                  UARTprintf
#define PUTCHAR                                 uart_direct_push_char
#define _SEM_TAKE_BLOCKING(sem_id)              while(xSemaphoreTake(sem_id,portMAX_DELAY)!=pdTRUE)
#define _SEM_TAKE_NONBLOCKING(sem_id)           (xSemaphoreTake(sem_id,0)==pdTRUE)
#define _MTX_TAKE_BLOCKING(mutex_id)            _SEM_TAKE_BLOCKING(mutex_id)
#define _MTX_TAKE_NONBLOCKING(mutex_id)         _SEM_TAKE_NONBLOCKING(mutex_id)
#define _SEM_GIVE(sem_id)                       xSemaphoreGive(sem_id)
#define _MTX_GIVE(mutex_id)                     _SEM_GIVE(mutex_id)
#define _QUEUE_PUT_BLOCKING(queue_id,item)      while(xQueueSend(queue_id,&item,portMAX_DELAY)!=pdTRUE)
#define _QUEUE_TAKE_LONGWAIT(queue_id,item_pointer)     (xQueueReceive(queue_id,item_pointer,portMAX_DELAY)==pdTRUE)
#define _QUEUE_TAKE_BLOCKING(queue_id,item_pointer)     while(!(_QUEUE_TAKE_LONGWAIT(queue_id,item_pointer)))
#define _QUEUE_TAKE_NONBLOCKING(queue_id,item_pointer)  (xQueueReceive(queue_id,item_pointer,0)==pdTRUE)
#define _SEM_COUNTING_TAKE_ALL_AVAILABLE(sem_id)        while(_SEM_TAKE_NONBLOCKING(sem_id)==pdTRUE)

#define PRINTABLE_UINT(any_uint)        (INT32U)(any_uint)

//----------- Scheduler Timing ------------
#define UART_SEND_TASK_DELAY         (MILLI_SEC(5))
#define UART_RECEIVE_TASK_DELAY      (MILLI_SEC(5))
#define CTRL_TASK_CYCLE                (1)       //ctrl_task.c
#define INTERFACE_TASK_CYCLE           (30)  //interface.c


#define USERTASK_STACK_SIZE (configMINIMAL_STACK_SIZE)
//#define USERTASK_STACK_SIZE ( ( unsigned short ) 90 )
#define IDLE_PRIO       0
#define LOW_PRIO        1
#define MED_PRIO        2
#define HIGH_PRIO       3

//for at kun køre PWM og POS hvert x'nde gang
#define CTRL_PER_PWM 10 //hvert 10'nde gang.


#define STACK_OVERFLOW_GCC_HANDLER      putChar

//Based on our SPI communication
#define PWM_PERCENT (2048/100)


//debugging
#define READ_PWM_DEBUG 0     //prints the PWM sent over SPI
#define TARGET_PWM_DEBUG 0   //prints the pwm received 
#define READ_POS_DEBUG 0     //prints x y z coordinates
#define CTRL_TARGET_DEBUG 0         //prints the converted theta and phi coordinate
#define CTRL_CURRENT_DEBUG 0         //prints the current position in ticks
#define UART_MSG_OUT_DEBUG 0 //prints message that is sent

/******************************** Constants *********************************/


/****************************** END OF MODULE *******************************/
