/*****************************************************************************
* Odense University College of Enginerring
* Embedded Programming (EMP)
*
* MODULENAME.: 
*
* PROJECT....: EMP
*
* DESCRIPTION:
* This module creates specific semaphores for this project.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140422 ALB    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#include "semaphore_project.h"



/****************************** Global variables that needs protection **********************/
	



void semaphore_port2_init(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
//     // LCD API    
// 	lcd_queue_sem = xSemaphoreCreateMutex();
// 	
// 	lcd_queue = xQueueCreate(LCD_MAX_INPUT,1);
// 	
//     // keypad
//     keypad_queue_sem = xSemaphoreCreateMutex();
// 	input_queue = xQueueCreate(MAX_INPUT_CHAR,1);
// 	input_queue_sem = xSemaphoreCreateMutex();
//     
//     // uart
// 	uart_tx_sem = xSemaphoreCreateMutex();
// 	uart_tx = xQueueCreate(204,1); 
// 	uart_rx_sem = xSemaphoreCreateMutex();
// 	uart_rx = xQueueCreate(MAX_INPUT_CHAR,1);
// 
//     
//     // interface
//     interface_sem = xSemaphoreCreateMutex();
// 	
// 	global_variables_sem = xSemaphoreCreateMutex();
// 
// 	//log
// 	log_queue_sem = xSemaphoreCreateMutex();
// 	
// 	number_of_sales = 0;
}

