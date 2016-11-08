/**
@page gpio_onecycleaccess_demo_readme GPIO One Cycle Access demo
@verbatim
******************** (C) COPYRIGHT 2013 Freescale ******************************
* File Name          : Readme.txt
* Author             : Freescale
* Date First Issued  : 06/25/2013 : Version 1.0.0
* Description        : Description of the template example.
********************************************************************************
* History:
* 06/25/2013 : Version 1.0.0
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
This example provides a demonstration of Fast GPIO access. Toggle GPIO PTB2
via Fast GPIO register, the toggle frequency can be as fast as 10MHz, while
the core clock (not the bus clock) is 20MHz, indicating the Fast GPIO access
in a single cycle.
NOTE that the optimization option must be "level high" in C compile configuration.

@par Code configuration
     ===================
Make sure the following macros are defined in ke02_config.h:\n
   USE_FEE and \n
   EXT_CLK_FREQ_KHZ macro is defined to 10000.


@par Directory contents
     ==================
- platforms/ke02_config.h                   Library and board Configuration file
- projects/ke02/isr.h                       Interrupt handler installation 
- projects/ke02/GPIO_OneCycleAccess_demo.c  Main program


@par Hardware environment
     ====================
 - Board: freedom FRDM-KE02Z, revA
  
@par How to use it
     =============
In order to make the program work, you must do the following :
- Rebuild all project files and load demo code into the device
- Measure output frequency on PTB2 (J2 Pin12).
- The frequency should be 10 MHz.
@verbatim
************************END OF FILE********************************************
@endverbatim

*/
