/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (ECP)
*
* MODULENAME.: lifo.c
*
* PROJECT....: ECP
*
* DESCRIPTION:
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "emp_type.h"
#include "binary.h"
#include "lifo.h"

/*****************************    Defines    *******************************/
typedef struct lifo_buffer
{
  //INT8U current_in;
  //INT8U current_out;
  INT16U data;
} lifo_buffer;
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
lifo_buffer test; 
/*****************************   Functions   *******************************/
INT16U read_data(void)
{
  return test.data;
}
void write_data(INT16U data)
{
  test.data = data; 
}
/****************************** End Of Module *******************************/












