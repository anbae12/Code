/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME.: read_pos_upsampling.c
 *
 * PROJECT....: SemesterprojektF14
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140408  NI   Module created.
 * 140514  MSC  Module changed from task to function.
 *****************************************************************************/

/***************************** Include files *******************************/

#include "read_pos_upsampling.h"
#include "queue/queue_ini.h"
#include "configs/project_settings.h"
#include "matlab_coor_list.h"


/********************************* Defines **********************************/
#define Ntap           26

/******************************** Variables *********************************/
coordinate_type interface_coordinate;
coordinate_type target_var;

/******************************** Constants *********************************/
static const FP32 FIRCoef[Ntap] = {
                0.00307400318384166,
                0.00415195519710886,
                0.00675738846385896,
                0.0111662700703571,
                0.0174565654067050,
                0.0254791432494250,
                0.0348562936854787,
                0.0450092416007514,
                0.0552122175937347,
                0.0646670647463716,
                0.0725895105802068,
                0.0782965208675624,
                0.0812838253545979,
                0.0812838253545979,
                0.0782965208675624,
                0.0725895105802068,
                0.0646670647463716,
                0.0552122175937347,
                0.0450092416007514,
                0.0348562936854787,
                0.0254791432494250,
                0.0174565654067050,
                0.0111662700703571,
                0.00675738846385896,
                0.00415195519710886,
                0.00307400318384166
};

/*****************************   Functions   *******************************/

void read_pos_debug2(coordinate_type coordinate) //strictly for debugging purposes... This is why it is not mentioned anywhere else...
{
  if( READ_POS_DEBUG )
  {
    INT16S conv;
    conv = (INT16S) (coordinate.x * 1000);
    PRINTF("Coordinate x: %d\t",conv);
    conv = (INT16S) (coordinate.y * 1000);
    PRINTF("Coordinate y: %d\t",conv);
    conv = (INT16S) (coordinate.z * 1000);
    PRINTF("Coordinate z: %d\n",conv);
  }
}



coordinate_type read_pos_kart(INT8U reset)
// Gets camera input from "read_pos" and upsamples. 
// If reset is set, the index for read_pos is set to 0. 
{
  static INT16U array_index = 0;
  static INT8U upsample_count = 0;
  coordinate_type coordinate = {.x = 0, .y = 0, .z = 0};
  //coordinate_type list[LIST_SIZE];
  coordinate_type output;

  if(reset) // Reset index
  {
    array_index = 0;
    upsample_count = 0;
  }
  
  upsample_count++;
  // Read new value
  if (upsample_count == 1 )
  {
    // Read samples from list at 60 Hz
    if( array_index < LIST_SIZE - 0 )
    {
      coordinate = coor_list[array_index++];
    }
    else
    {
      coordinate.x = 0;
      coordinate.y = 0;
      coordinate.z = 0;
    }
  }
  else
  {
    if (upsample_count == UPSAMPLING_FACTOR)
    {
      upsample_count = 0;
    }
	coordinate.x = 0;
	coordinate.y = 0;
	coordinate.z = 0;
  }

  // Run the FIR filter from winfilter.
  output = fir_filter(coordinate);

  // Debug
  //read_pos_debug2(output);

  //if interface sends a coordinate
  if( xSemaphoreTake(position_ctrl_mutex, 0) )
  {
    if( interface_coordinate.x != invalid_coordinate.x &&
        interface_coordinate.y != invalid_coordinate.y &&
        interface_coordinate.z != invalid_coordinate.z )
    {
      output = interface_coordinate;
      array_index = 0;
    }
    xSemaphoreGive(position_ctrl_mutex);
  }
  INT16S corx, cory, corz;
  corx = (INT16S) (output.x * 100);
  cory = (INT16S) (output.y * 100);
  corz = (INT16S) (output.z * 100);
//  PRINTF("x: %d\t y: %d\t z: %d\n",corx,cory,corz);
  return output;
}


coordinate_type fir_filter(coordinate_type new_sample)
// FIR filter using coordinate type. 
{
  static coordinate_type input[Ntap] = {{0,0,0}}; //input samples
  coordinate_type output = {0,0,0};          //output sample
  INT8U n;
  
  //shift the old samples
  for(n=Ntap-1; n>0; n--)
     input[n] = input[n-1];
  
  input[0] = new_sample;

  //Calculate the new output
  for(n=0; n<Ntap; n++)
  {
    output.x += FIRCoef[n] * input[n].x;
    output.y += FIRCoef[n] * input[n].y;
    output.z += FIRCoef[n] * input[n].z;
  }

  return output;
}
