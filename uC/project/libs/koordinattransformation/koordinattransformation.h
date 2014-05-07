/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: koordinattransformation.h
 *
 * PROJECT....: Semesterprojekt
 *
 *
 * Change Log:
 *****************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140507   ALB    Module created.
 *
 *****************************************************************************/

#ifndef koordinattransformation_h
#define koordinattransformation_h
/***************************** Include files *******************************/
#include "inc/emp_type.h"
#include <math.h>
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

extern void koordinattransformation(FP64 Cx, FP64 Cy, FP64 Cz);
/*****************************************************************************
 *   Input    : Skeet position 3D cartesian coordinates.
 *   Output   : Skeet position 3D spherical coordinates.
 *   Function : Transform cartesian to spherical coordinates.
 ******************************************************************************/


/****************************** End Of Module *******************************/


#endif
