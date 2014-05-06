/*****************************************************************************
 * Mikkel, Åse & Mikael
 * MODULENAME:  uartprintf
 * DESCRIPTION:
 ****************************************************************************/

/***************************** Include files *******************************/
#include <stdarg.h>
#include "uartprintf.h"
#include "uart.h"
#include "configs/project_settings.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/
// A mapping from an integer between 0 and 15 to its ASCII character
// equivalent.
static const char * const g_pcHex = "0123456789abcdef";

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void UARTCharPut(unsigned long ulBase, unsigned char ucData)
/*****************************************************************************
 * Function:    See header file.
 ****************************************************************************/
{
  PUTCHAR(ucData);
}

int UARTwrite(const char *pcBuf, unsigned long ulLen)
/*****************************************************************************
 * Function:    See header file.
 ****************************************************************************/
{
    unsigned int uIdx;
    unsigned long g_ulBase = 0;
    //
    // Check for valid UART base address, and valid arguments.
    //
    // ASSERT(g_ulBase != 0);
    // ASSERT(pcBuf != 0);

    //
    // Send the characters
    //
    for(uIdx = 0; uIdx < ulLen; uIdx++)
    {
      //
      // If the character to the UART is \n, then add a \r before it so that
      // \n is translated to \n\r in the output.
      //
      if(pcBuf[uIdx] == '\n')
      {
        UARTCharPut(0, '\r');
      }
      if(pcBuf[uIdx] == '\r')
        UARTCharPut(0, '\n');

      //
      // Send the character to the UART output.
      //
      UARTCharPut(g_ulBase, pcBuf[uIdx]);
    }

    //
    // Return the number of characters written.
    //
    return(uIdx);


  return 0;
}

void UARTprintf(char *pcString, ...)
/*****************************************************************************
 * Function:    See header file.
 ****************************************************************************/
{
  unsigned long ulIdx, ulValue, ulPos, ulCount, ulBase, ulNeg;
  char *pcStr, pcBuf[16], cFill;
  va_list vaArgP;

  //
  // Check the arguments.
  //
  //  ASSERT(pcString != 0);

  //
  // Start the varargs processing.
  //
  va_start(vaArgP, pcString);

  //while(xSemaphoreTake(uart_outgoing_mutex,portMAX_DELAY)!=pdTRUE);
  //_MTX_TAKE_BLOCKING(UART_OUTGOING_MUTEX);
  take_uart_output_permission();

  //
  // Loop while there are more characters in the string.
  //
  while(*pcString)
  {
    //
    // Find the first non-% character, or the end of the string.
    //
    for(ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0');
        ulIdx++)
    {
    }

    //
    // Write this portion of the string.
    //
    UARTwrite(pcString, ulIdx);

    //
    // Skip the portion of the string that was written.
    //
    pcString += ulIdx;

    //
    // See if the next character is a %.
    //
    if(*pcString == '%')
    {
      //
      // Skip the %.
      //
      pcString++;

      //
      // Set the digit count to zero, and the fill character to space
      // (i.e. to the defaults).
      //
      ulCount = 0;
      cFill = ' ';

      //
      // It may be necessary to get back here to process more characters.
      // Goto's aren't pretty, but effective.  I feel extremely dirty for
      // using not one but two of the beasts.
      //
      again:

      //
      // Determine how to handle the next character.
      //
      switch(*pcString++)
      {
      //
      // Handle the digit characters.
      //
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
        //
        // If this is a zero, and it is the first digit, then the
        // fill character is a zero instead of a space.
        //
        if((pcString[-1] == '0') && (ulCount == 0))
        {
          cFill = '0';
        }

        //
        // Update the digit count.
        //
        ulCount *= 10;
        ulCount += pcString[-1] - '0';

        //
        // Get the next character.
        //
        goto again;
      }

      //
      // Handle the %c command.
      //
      case 'c':
      {
        //
        // Get the value from the varargs.
        //
        ulValue = va_arg(vaArgP, unsigned long);

        //
        // Print out the character.
        //
        UARTwrite((char *)&ulValue, 1);

        //
        // This command has been handled.
        //
        break;
      }

      //
      // Handle the %d command.
      //
      case 'd':
      {
        //
        // Get the value from the varargs.
        //
        ulValue = va_arg(vaArgP, unsigned long);

        //
        // Reset the buffer position.
        //
        ulPos = 0;

        //
        // If the value is negative, make it positive and indicate
        // that a minus sign is needed.
        //
        if((long)ulValue < 0)
        {
          //
          // Make the value positive.
          //
          ulValue = -(long)ulValue;

          //
          // Indicate that the value is negative.
          //
          ulNeg = 1;
        }
        else
        {
          //
          // Indicate that the value is positive so that a minus
          // sign isn't inserted.
          //
          ulNeg = 0;
        }

        //
        // Set the base to 10.
        //
        ulBase = 10;

        //
        // Convert the value to ASCII.
        //
        goto convert;
      }

      //
      // Handle the %s command.
      //
      case 's':
      {
        //
        // Get the string pointer from the varargs.
        //
        pcStr = va_arg(vaArgP, char *);

        //
        // Determine the length of the string.
        //
        for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)
        {
        }

        //
        // Write the string.
        //
        UARTwrite(pcStr, ulIdx);

        //
        // Write any required padding spaces
        //
        if(ulCount > ulIdx)
        {
          ulCount -= ulIdx;
          while(ulCount--)
          {
            UARTwrite(" ", 1);
          }
        }
        //
        // This command has been handled.
        //
        break;
      }

      //
      // Handle the %u command.
      //
      case 'u':
      {
        //
        // Get the value from the varargs.
        //
        ulValue = va_arg(vaArgP, unsigned long);

        //
        // Reset the buffer position.
        //
        ulPos = 0;

        //
        // Set the base to 10.
        //
        ulBase = 10;

        //
        // Indicate that the value is positive so that a minus sign
        // isn't inserted.
        //
        ulNeg = 0;

        //
        // Convert the value to ASCII.
        //
        goto convert;
      }

      //
      // Handle the %x and %X commands.  Note that they are treated
      // identically; i.e. %X will use lower case letters for a-f
      // instead of the upper case letters is should use.  We also
      // alias %p to %x.
      //
      case 'x':
      case 'X':
      case 'p':
      {
        //
        // Get the value from the varargs.
        //
        ulValue = va_arg(vaArgP, unsigned long);

        //
        // Reset the buffer position.
        //
        ulPos = 0;

        //
        // Set the base to 16.
        //
        ulBase = 16;

        //
        // Indicate that the value is positive so that a minus sign
        // isn't inserted.
        //
        ulNeg = 0;

        //
        // Determine the number of digits in the string version of
        // the value.
        //
        convert:
        for(ulIdx = 1;
            (((ulIdx * ulBase) <= ulValue) &&
                (((ulIdx * ulBase) / ulBase) == ulIdx));
            ulIdx *= ulBase, ulCount--)
        {
        }

        //
        // If the value is negative, reduce the count of padding
        // characters needed.
        //
        if(ulNeg)
        {
          ulCount--;
        }

        //
        // If the value is negative and the value is padded with
        // zeros, then place the minus sign before the padding.
        //
        if(ulNeg && (cFill == '0'))
        {
          //
          // Place the minus sign in the output buffer.
          //
          pcBuf[ulPos++] = '-';

          //
          // The minus sign has been placed, so turn off the
          // negative flag.
          //
          ulNeg = 0;
        }

        //
        // Provide additional padding at the beginning of the
        // string conversion if needed.
        //
        if((ulCount > 1) && (ulCount < 16))
        {
          for(ulCount--; ulCount; ulCount--)
          {
            pcBuf[ulPos++] = cFill;
          }
        }

        //
        // If the value is negative, then place the minus sign
        // before the number.
        //
        if(ulNeg)
        {
          //
          // Place the minus sign in the output buffer.
          //
          pcBuf[ulPos++] = '-';
        }

        //
        // Convert the value into a string.
        //
        for(; ulIdx; ulIdx /= ulBase)
        {
          pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase];
        }

        //
        // Write the string.
        //
        UARTwrite(pcBuf, ulPos);

        //
        // This command has been handled.
        //
        break;
      }

      //
      // Handle the %% command.
      //
      case '%':
      {
        //
        // Simply write a single %.
        //
        UARTwrite(pcString - 1, 1);

        //
        // This command has been handled.
        //
        break;
      }

      //
      // Handle all other commands.
      //
      default:
      {
        //
        // Indicate an error.
        //
        UARTwrite("ERROR", 5);

        //
        // This command has been handled.
        //
        break;
      }
      }
    }
  }

  //xSemaphoreGive(uart_outgoing_mutex);
  //_MTX_GIVE(UART_OUTGOING_MUTEX);
  give_uart_output_permission();

  //
  // End the varargs processing.
  //
  va_end(vaArgP);
}

/***************************** End Of Module *******************************/
