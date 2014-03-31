#include "emp_type.h"
#include "fifo.h"

#define MAX_BUFFERS 8
#define MAX_DATA 16

typedef struct fifo_buffer
{
  INT8U current_in;
  INT8U current_out;
  INT8U data[MAX_DATA];
} fifo_buffer;
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

fifo_buffer buffer[MAX_BUFFERS];  // create some buffers

/*****************************   Functions   *******************************/
BOOLEAN buffer_empty( INT8U id )
{
//if ( buffer[id].current_in == buffer[id].current_out && buffer[id].data[ buffer[id].current_in ] == '\0') EARLY BEARLY VERSION!
  if ( buffer[id].data[ buffer[id].current_out ] == '\0') 
  {
    return TRUE;
  }
  else return FALSE;
}
INT8U buffer_pop( INT8U id )
{
  INT8U return_val;
  return_val = buffer[id].data[ buffer[id].current_out ];
  buffer[id].data[buffer[id].current_out] = '\0';
  if(buffer[id].current_out < MAX_DATA)
  {
    buffer[id].current_out++;
  }
  else buffer[id].current_out = 0;
  return return_val;
}

void buffer_push( INT8U id , INT8U new_value)
{
  buffer[id].data[buffer[id].current_in] = new_value;
  if(buffer[id].current_in < MAX_DATA)
  {
    buffer[id].current_in++;
  }
  else buffer[id].current_in = 0;
}

INT8U buffer_read( INT8U id )
{
  return( buffer[id].data[buffer[id].current_out] );
}

init_buffer( INT8U id )
{
  if( id < MAX_BUFFERS )
  {
    buffer[id].current_in  = 0;
    buffer[id].current_out = 0;
    INT8U i;
    for( i = 0; i < MAX_DATA; i++)
    {
      buffer[id].data[i] = '\0';
    }
  }
}