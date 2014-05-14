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
#define READ_POS_TASK_FREQUENCY 6000

/***************************** Include files *******************************/

#include "read_pos_upsampling.h"
#include "queue/queue_ini.h"
#include "configs/project_settings.h"

/******************************** Variables *********************************/
coordinate_type interface_coordinate;
coordinate_type target_var;

/*****************************   Functions   *******************************/

void read_pos_debug(coordinate_type coordinate)//strictly for debugging purposes... This is why it is not mentioned anywhere else...
{
  if( READ_POS_DEBUG )
  {
    INT16U conv;
    conv = (INT16U) coordinate.x;
    PRINTF("Coordinate x: %d\t",conv);
    conv = (INT16U) coordinate.y;
    PRINTF("Coordinate y: %d\t",conv);
    conv = (INT16U) coordinate.z;
    PRINTF("Coordinate z: %d\n",conv);
  }
}

coordinat_type read_pos(INT8U reset)
// Michael: I just changed the read_pos_task into a function, 
// which is called from the downsampling function.
{
  static INT8U index;
  coordinate_type coordinate = {.y = 12, .x = 111, .z = 222};
  coordinate_type list[LIST_SIZE];
  
  if(reset) // Reset index
    index = 0;
 
  //TODO: Init list should be moved to initialize part
  //      of whatever task is using this. 
  init_list(list);
  
 
  return coordinate;
}



coordinat_type read_pos(INT8U reset)
// Gets camera input from "read_pos" and upsamples. 
// If reset is set, the index for read_pos is set to 0. 
{
  static INT8U array_index;
  coordinate_type coordinate = {.x = 0, .y = 0, .z = 0};
  coordinate_type list[LIST_SIZE];
  coordinate_type output;
  if(reset) // Reset index
    index = 0;
  
  
  static INT8U upsample_count; 
  
  // Read new value
  if (upsample_count == 0)
  {
    upsample_count++;
    // Read samples from list at 60 Hz
    if( index < LIST_SIZE - 2 )
    {
      coordinate = list[index++];
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
    upsample_count++;
    if (upsample_count == UPSAMPLING_FACTOR)
    {
      upsample_count = 0;
    }
    coordinate.x = 0;
    coordinate.y = 0;
    coordinate.z = 0;
  }
  
  // Debug
  read_pos_debug(coordinate);
  
  // Run the FIR filter from winfilter.
  output = fir_filter(coordinate);
}

#define Ntap           11
coordinate_type fir_filter(coordinate_type new_sample)
// FIR filter using coordinate type. 
{
  float FIRCoef[Ntap] = { 
        0.08190343120328541500,
        0.08717042789250634100,
        0.09141728478629804200,
        0.09453344515685510600,
        0.09643686561441695800,
        0.09707709069327609400,
        0.09643686561441695800,
        0.09453344515685510600,
        0.09141728478629804200,
        0.08717042789250634100,
        0.08190343120328541500
  };
  static coordinate_type input[Ntap]; //input samples
  coordinate_type output = 0;          //output sample
  INT8U n;
  
  //shift the old samples
  for(n=Ntap-1; n>0; n--)
     input[n] = input[n-1];
  

  //Calculate the new output
  input[0] = new_sample;
  for(n=0; n<Ntap; n++)
  {
    output.x += FIRCoef[n] * input[n].x;
    output.y += FIRCoef[n] * input[n].y;
    output.z += FIRCoef[n] * input[n].z;
  }
  return output;
  
  
}


void init_list( coordinate_type final_list[LIST_SIZE] )
{
  INT16U i;
  INT8U j = 0;
  
  FP32 matlab_liste[MATLAB_LIST_SIZE] = {
  //paste matlab generated list here... I can handle FP32
  //if we find that we need another data type we should correct this in the struct definition as well..
      -13.7,-19.3,2.6,
      -13.856,-18.752,2.6899,
      -14.011,-18.205,2.777,
      -14.167,-17.657,2.8614,
      -14.323,-17.11,2.943,
      -14.478,-16.562,3.022,
      -14.634,-16.015,3.0982,
      -14.79,-15.467,3.1717,
      -14.945,-14.92,3.2424,
      -15.101,-14.372,3.3105,
      -15.257,-13.825,3.3758,
      -15.412,-13.277,3.4384,
      -15.568,-12.73,3.4982,
      -15.724,-12.182,3.5553,
      -15.879,-11.635,3.6097,
      -16.035,-11.087,3.6614,
      -16.191,-10.54,3.7103,
      -16.346,-9.9922,3.7565,
      -16.502,-9.4447,3.8,
      -16.658,-8.8972,3.8408,
      -16.813,-8.3497,3.8788,
      -16.969,-7.8022,3.9141,
      -17.125,-7.2546,3.9466,
      -17.28,-6.7071,3.9765,
      -17.436,-6.1596,4.0036,
      -17.592,-5.6121,4.028,
      -17.747,-5.0646,4.0496,
      -17.903,-4.5171,4.0686,
      -18.059,-3.9695,4.0848,
      -18.214,-3.422,4.0983,
      -18.37,-2.8745,4.109,
      -18.526,-2.327,4.117,
      -18.681,-1.7795,4.1223,
      -18.837,-1.232,4.1249,
      -18.993,-0.68443,4.1247,
      -19.148,-0.13692,4.1218,
      -19.304,0.4106,4.1162,
      -19.46,0.95812,4.1079,
      -19.615,1.5056,4.0968,
      -19.771,2.0531,4.083,
      -19.927,2.6007,4.0664,
      -20.082,3.1482,4.0472,
      -20.238,3.6957,4.0252,
      -20.394,4.2432,4.0005,
      -20.549,4.7907,3.973,
      -20.705,5.3382,3.9429,
      -20.861,5.8858,3.91,
      -21.016,6.4333,3.8744,
      -21.172,6.9808,3.836,
      -21.328,7.5283,3.7949,
      -21.483,8.0758,3.7511,
      -21.639,8.6233,3.7046,
      -21.795,9.1709,3.6553,
      -21.95,9.7184,3.6033,
      -22.106,10.266,3.5486,
      -22.262,10.813,3.4912,
      -22.417,11.361,3.431,
      -22.573,11.908,3.3681,
      -22.729,12.456,3.3024,
      -22.884,13.003,3.2341,
      -23.04,13.551,3.163,
      -23.196,14.099,3.0892,
      -23.351,14.646,3.0126,
      -23.507,15.194,2.9334,
      -23.663,15.741,2.8514,
      -23.818,16.289,2.7667,
      -23.974,16.836,2.6792,
      -24.13,17.384,2.589,
      -24.285,17.931,2.4961,
      -24.441,18.479,2.4005,
      -24.597,19.026,2.3021,
      -24.752,19.574,2.201,
      -24.908,20.121,2.0972,
      -25.064,20.669,1.9907,
      -25.219,21.216,1.8814,
      -25.375,21.764,1.7694,
      -25.531,22.311,1.6546,
      -25.686,22.859,1.5372,
      -25.842,23.406,1.417,
      -25.998,23.954,1.2941,
      -26.153,24.501,1.1684,
      -26.309,25.049,1.0401,
      -26.465,25.596,0.90898,
      -26.62,26.144,0.77515,
      -26.776,26.691,0.6386,
      -26.932,27.239,0.49932,
      -27.087,27.786,0.35731,
      -27.243,28.334,0.21257,
      -27.399,28.881,0.065111,
      -27.554,29.429,-0.085081,
      -27.71,29.976,-0.238,
      -27.866,30.524,-0.39365
  };
  
  for( i = 0; i < MATLAB_LIST_SIZE; i++) //TODO this list is not correct...
  {
    if( ( i % 3 ) == 0 ) final_list[j].x = matlab_liste[i];
    if( ( i % 3 ) == 1 ) final_list[j].y = matlab_liste[i];
    if( ( i % 3 ) == 2 ) final_list[j++].z = matlab_liste[i];
  }
}
