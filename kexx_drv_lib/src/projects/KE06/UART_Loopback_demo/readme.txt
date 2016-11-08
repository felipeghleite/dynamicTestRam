/**
@page UART_Loopback_demo_readme UART Loopback demo
@verbatim
******************** (C) COPYRIGHT 2013 Freescale ******************************
* File Name          : Readme.txt
* Author             : Freescale
* Date First Issued  : 10/28/2013 : Version 1.0.0
* Description        : Description of the template example.
********************************************************************************
* History:
* 10/28/2013 : Version 1.0.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, FREESCALE SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************
@endverbatim

@par Example description
     ===================
This example provides a demo for UART loopback function: the characters sent from Transmit
will loop back to Receive port by internal connection. It demonstrates how to 
send a character then receive the loopback chacrater.

@par Code configuration
     ===================
Make sure the following macros are defined in ke06_config.h: \n
   USE_FEE and \n
   EXT_CLK_FREQ_KHZ macro is defined to 8000.


@par Directory contents
     ==================
- platforms/ke06/ke06_config.h          library and board Configuration file
- projects/ke06/UART_Loopback_demo.c    Main program


@par Hardware environment
     ====================
 - Board: freedom FRDM-ke06Z, revA
  
@par How to use it
     =============
In order to make the program work, you must do the following :
- Rebuild all project files and load demo code into the device
- Connect J6 USB port to the computer that will find serial terminal port
- Open the serial terminal
- Configure the serial terminal to protocol: 8-N-1, 115200bps, and flow control: none
- Run the example
- Check whether there're characters print on the screen.

@verbatim
************************END OF FILE********************************************
@endverbatim

*/
