/*****************************************************************************
 * Mikkel, Åse & Mikael
 * MODULENAME:  uart
 * DESCRIPTION:
 ****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include "inc/emp_type.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"

/*****************************    Defines    *******************************/
#define UART_QUEUE_LEN  50


//interface_to_control_byte
#define stop_bit_location 7
#define pos_bit_location  6
#define pwm_bit_location  5

//control_states
#define CTRL_STOP_MODE    1
#define CTRL_POS_MODE     2
#define CTRL_PWM_MODE     3
/*****************************   Constants   *******************************/

/******************************** Variables *********************************/
//---------------- Tasks ------------------
extern xTaskHandle      uart_send_task_handle;          //Create elsewhere.
extern xTaskHandle      uart_receive_task_handle;       //Create elsewhere.
extern INT8U interface_to_control_byte;

/*****************************   Functions   *******************************/

extern void ctrl_task();
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:    
 *              Reads ctrl bit and chose state
 *              Receive target position
 *              Convert target position
 *              Calculate PWM
 *              Send PWM to SPI
 *              Receives position from SPI
 *              Put position into status
 ****************************************************************************/
extern motor_pos get_target_position();
extern motor_pos coordinate_transform(coordinate_type);
extern pwm_duty_cycle_type get_target_pwm();
/****************************** End Of Module *******************************/
