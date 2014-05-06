/*****************************************************************************
* Odense University College of Engineering
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

/*****************************    Defines    *******************************/

#define ENC_QUEUE_LENGTH                   8
#define ENC_QUEUE_DATA_SIZE                2
/*****************************   Constants   *******************************/

extern xQueueHandle enc_queue[2];

/*****************************   Variables   *******************************/
extern void init_spi_queue( void );
extern void add_to_enc_queue(INT8U queue_id, INT16U data);
/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/

