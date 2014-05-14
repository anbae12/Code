/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: gpio_ini.c
*
* PROJECT....: EMP Assignments
*
* DESCRIPTION: Initializes and names the different switches, leds and peripherals. 
*
* Change Log: See the .c file
*****************************************************************************
*   
****************************************************************************/

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

//*****************************************************************************
// Defines for LED's
//*****************************************************************************
#define PIN_LED_STATUS          0b00000001  // Status led (PORT F)
#define PIN_LED_RED             0b01000000  // Red led    (PORT D)
#define PIN_LED_YELLOW          0b00000001  // Yellow led (PORT G)
#define PIN_LED_GREEN           0b00000010  // Green led  (PORT G)

//*****************************************************************************
// Defines for switches
//*****************************************************************************
#define PIN_SWITCH_SELECT       0b00000010  // Sel sw.    (PORT F)

#define PIN_SWITCH_UP           0b00000001  // Up sw.     (PORT E) 
#define PIN_SWITCH_DOWN         0b00000010  // Down sw.   (PORT E)
#define PIN_SWITCH_LEFT         0b00000100  // Left sw.   (PORT E)
#define PIN_SWITCH_RIGHT        0b00001000  // Rigth sw.  (PORT E)



/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/



/****************************** End Of Module *****************************/
