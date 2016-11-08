/**
@page FTM_EPWM_demo_readme  FTM_EPWM_demo
@verbatim
******************** (C) COPYRIGHT 2013 Freescale ******************************
* File Name          : Readme.txt
* Author             : Freescale
* Date First Issued  : 09/25/2013 : Version 1.0.0
* Description        : Description of the template example.
********************************************************************************
* History:
* 09/25/2013 : Version 1.0.0
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
This example provides a template for the sample code. It demonstrates how to 
configure FTM module and  how to configure the edge allign pwm mode.

@par Code configuration
     ===================
Make sure the following macros are defined in ke04_config.h:
   USE_FEE and
   EXT_CLK_FREQ_KHZ macro is defined to 8000.


@par Directory contents
     ==================
- platforms/ke04_config.h           library and board Configuration file
- projects/ke04/isr.h               Interrupt handler installation 
- projects/ke04/FTM_EPWM_demo.c     Main program


@par Hardware environment
     ====================
 - Board: freedom FRDM-ke04Z, revA
  
@par How to use it
     =============
In order to make the program work, you must do the following :
- Rebuild all project files and load demo code into the device
- Connect J6 USB port to the computer that will find serial terminal port
- Open the serial terminal
- Configure the serial terminal to protocol: 8-N-1, 115200bps, and flow control: none
- Run the example
- Check the FTM2 Channel 1 waveform, the duty will swing between low and high.

@verbatim
************************END OF FILE********************************************
@endverbatim

*/
