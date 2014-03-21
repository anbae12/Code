
/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: keypad.c
*
* PROJECT....: EMP Assignments
*
* DESCRIPTION: Driver for a 3x4 keypad
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140307  MSC   Module created.
*
*****************************************************************************
* TODO: 
*   
****************************************************************************/

/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "emp_type.h"

/*****************************    Defines    *******************************/

//*****************************************************************************
// Defines for Keypads 3x4 bit interface pins:
//*****************************************************************************

#define KEYPAD_X_PINS           0b00011100  //            (PORT B)
#define KEYPAD_X1               0b00000100  //            (PORT B)
#define KEYPAD_X2               0b00001000  //            (PORT B)
#define KEYPAD_X3               0b00010000  //            (PORT B)

#define KEYPAD_Y_PINS           0b00111100  //            (PORT D)
#define KEYPAD_Y1               0b00100000  //            (PORT D)
#define KEYPAD_Y2               0b00010000  //            (PORT D)
#define KEYPAD_Y3               0b00001000  //            (PORT D)
#define KEYPAD_Y4               0b00000100  //            (PORT D)




/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/
extern void config_output();
extern INT8U keypad_task();

/****************************** End Of Module *****************************/

// 1 ms: 500 00
// 1 us: 50