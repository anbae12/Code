/*****************************************************************************
 * Nikolaj
 * MODULENAME:  read_pos.h
 * DESCRIPTION:
 ****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include "inc/emp_type.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"
#include "spi/spi.h"

/*****************************    Defines    *******************************/
#define PWM_LIST_SIZE 101
#define MATLAB_PWM_LIST_SIZE (PWM_LIST_SIZE*2) //2 times LIST_SIZE

/*****************************   Constants   *******************************/
static pwm_duty_cycle_type invalid_pwm = {.motorA = 1981, .motorB = 1981};
/******************************** Variables *********************************/
extern pwm_duty_cycle_type interface_pwm;
extern pwm_duty_cycle_type target_pwm;

/*****************************   Functions   *******************************/

extern void read_pwm_task(void *pvParameters);
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:    
 *              Reads the pos_ctrl_queue for input
 *              If there is none, it puts the next coordinate into target_pos_queue
 *              If the queue is full it means that the control loop isn't taking coordinates. 
 *              In that case it should just wait. 
 *              If nothing has been taken in 15 seconds the list index is resat. 
 ****************************************************************************/
extern void init_pwm_list( pwm_duty_cycle_type final_list[PWM_LIST_SIZE] );
/*****************************************************************************
 * Initializes the list.
 * The matlab generated list should written into this function.
 ****************************************************************************/
/****************************** End Of Module *******************************/
