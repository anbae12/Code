#include "emp_type.h"
#include "lm3s6965.h"
#include "systick.h"
#include "keypad.h"
#include "spi.h"




int main(void)
{
	init_clk_system();
	enable_global_int();
	systick_init();
	spi_init();

	spi_buffer_push(0xCC);
	spi_buffer_push(0xCC);
	spi_buffer_push(0xCC);
	spi_buffer_push(0xCC);
	spi_buffer_push(0xCC);
    while(1)
    {

    	while (! ticks );
    	ticks--;
    	//spi_buffer_push(0b10101010);

    	keypad_task();
    }
}
