/***************************************************************************** 
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: koordinattransformation.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140507   ALB    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "inc/emp_type.h"
#include <math.h>
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/
FP64 PI=3.14159;

/*****************************   Variables   *******************************/
FP64 Srho=0;
FP64 Sphi=0;
FP64 Stheta=0;

/*****************************   Functions   *******************************/


void koordinattransformation(FP64 Cx, FP64 Cy, FP64 Cz)
/*****************************************************************************
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{
    Srho =sqrt(pow(Cx,2)+pow(Cy,2)+pow(Cz,2));
    Sphi = atan((sqrt(pow(Cx,2)+pow(Cy,2)))/Cz)*180/PI;
    Stheta = atan(Cy/Cx)*180/PI;
}

/****************************** End Of Module *******************************/


