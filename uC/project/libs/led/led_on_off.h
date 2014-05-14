/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: led_on_off.h
*
* PROJECT....: Traffic light
*
* DESCRIPTION: LED on/offs
*
* Change Log: See the .c file
*****************************************************************************
* TODO: See the .c file
*   
****************************************************************************/

/***************************** Include files *******************************/
#include "inc/emp_type.h"
#include "inc/glob_def.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

extern void turn_on_red();
extern void turn_off_red();
extern void turn_on_yellow();
extern void turn_off_yellow();
extern void turn_on_green();
extern void turn_off_green();
extern void led_ryg(BOOLEAN, BOOLEAN, BOOLEAN);
