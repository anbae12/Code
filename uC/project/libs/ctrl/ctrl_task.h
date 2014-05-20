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
#include "SPI/spi.h"
#include "read_pos/read_pos_upsampling.h"

/*****************************    Defines    *******************************/
#define UART_QUEUE_LEN  50


//interface_to_control_byte
#define stop_bit_location 7
#define pos_bit_location  6
#define pwm_bit_location  5

//control_states
#define SINGLE_ANGLE_POSITION 1          //A aaaa.bbbb
#define SINGLE_PWM_MODE 2                //PA+99
#define SINGLE_CARTESIAN_POSITION 3      //C xxx.yyy.zzz
#define OPEN_LOOP_TEST 4                 //open
#define SKEET_SHOOT_DEMO 5               //start
#define STOP_MOTORS_NOW  6               //stop

typedef struct message_user_interface_type {
  INT8U state;
  void *msg;
} message_user_interface_type;



/*****************************   Constants   *******************************/

/******************************** Variables *********************************/
//---------------- Tasks ------------------
extern xTaskHandle      uart_send_task_handle;          //Create elsewhere.
extern xTaskHandle      uart_receive_task_handle;       //Create elsewhere.
xQueueHandle     interface_to_control_queue;

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
extern motor_pos coordinate_transform(coordinate_type coord);
extern pwm_duty_cycle_type get_target_pwm();

extern message_user_interface_type control_get_state(void );
extern void control_set_state( INT8U state, void *msg );

/****************************** End Of Module *******************************/
