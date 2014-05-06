/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (ECP)
*
* MODULENAME.: queue_ini.c
*
* PROJECT....: Pan and tilt
*
* DESCRIPTION:
* This module is used to initialize freeRTOS queues. 
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
#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "inc/binary.h"

#include "FRT_Library/FreeRTOS/Source/include/FreeRTOS.h"
#include "FRT_Library/FreeRTOS/Source/include/task.h"

#include "FRT_Library/FreeRTOS/Source/include/queue.h"
#include "FRT_Library/FreeRTOS/Source/include/semphr.h"
#include "queue_ini.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

xQueueHandle enc_queue[2];

/*****************************   Variables   *******************************/
void init_spi_queue( void )
{

	enc_queue[0] = xQueueCreate( ENC_QUEUE_LENGTH, ENC_QUEUE_DATA_SIZE );
	enc_queue[1] = xQueueCreate( ENC_QUEUE_LENGTH, ENC_QUEUE_DATA_SIZE );

	//while(1);

}
void add_to_enc_queue(INT8U queue_id, INT16U data)
{
	if ( enc_queue[queue_id] != 0 )
	{
		if (xQueueSendToFront(enc_queue[queue_id], &data, 100))
		{
			// ERROR!
		}
	}
}


/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/

