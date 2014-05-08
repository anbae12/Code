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

/*****************************    Defines    *******************************/
#define LIST_SIZE 100
#define MATLAB_LIST_SIZE 15 //3 times LIST_SIZE

typedef struct coordinate_type {
  FP32 x;
  FP32 y;
  FP32 z;
} coordinate_type;

/*****************************   Constants   *******************************/
static coordinate_type invalid_coordinate = {.y = 1000, .x = 1000, .z = 1000};
/******************************** Variables *********************************/
extern coordinate_type interface_coordinate;
extern coordinate_type target_var;

/*****************************   Functions   *******************************/

extern void read_pos_task(void *pvParameters);
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
extern void init_list( coordinate_type final_list[LIST_SIZE] );
/*****************************************************************************
 * Initializes the list.
 * The matlab generated list should written into this function.
 ****************************************************************************/
/****************************** End Of Module *******************************/
