 /*****************************************************************************
 * Mikkel, �se & Mikael
 * MODULENAME: manager_interface
 * DESCRIPTION:
 ****************************************************************************/
#pragma once
/****************************** Include files *******************************/
#include "inc/emp_type.h"

/********************************* Defines **********************************/
#define         GAS_PRICE_O92   10
#define         GAS_PRICE_O95   11
#define         GAS_PRICE_E10   19

#define         GAS_TYPE_O92    0
#define         GAS_TYPE_O95    1
#define         GAS_TYPE_E10    2

/******************************** Constants *********************************/

/******************************** Variables *********************************/

/******************************** Functions *********************************/

void interface_task(void *pvParameters);
/*****************************************************************************
 * Input:       pvParameters:   Unused.
 * Output:      -
 * Function:    See application description.
 ****************************************************************************/

void interface_display_commands();
/*****************************************************************************
 * Input:       -
 * Output:      -
 * Function:	Display the commands, that the manager can use.
 ****************************************************************************/



/****************************** END OF MODULE *******************************/