/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: read_pos_upsampling.c
 *
 * PROJECT....: SemesterprojektF14
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140408  NI   Module created.
 * 140514  MSC  Module changed from task to function.
 *****************************************************************************/

#pragma once
/***************************** Include files *******************************/
#include "inc/emp_type.h"
#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"

/*****************************    Defines    *******************************/
// Disse to bliver ikke brugt længere. 
#define LIST_SIZE 92
#define MATLAB_LIST_SIZE (LIST_SIZE*3)

#define UPSAMPLING_FACTOR          10

#define MATLAB_COOR_LIST_SIZE      92

typedef struct coordinate_type {
  FP32 x;
  FP32 y;
  FP32 z;
} coordinate_type;


/******************************** Variables *********************************/
coordinate_type interface_coordinate;
coordinate_type target_var;

/*****************************   Functions   *******************************/

extern void read_pos_debug(coordinate_type coordinate); //strictly for debugging purposes... This is why it is not mentioned anywhere else...

extern coordinat_type read_pos(INT8U reset);
extern coordinate_type fir_filter(coordinate_type new_sample);

extern void init_list( coordinate_type final_list[LIST_SIZE] );



