/**
@page FTM_DualEdgeCapture_demo_readme  FTM_DualEdgeCapture 
@verbatim
******************** (C) COPYRIGHT 2013 Freescale ******************************
* File Name          : Readme.txt
* Author             : Freescale
* Date First Issued  : 10/25/2013 : Version 1.0.0
* Description        : Description of the template example.
********************************************************************************
* History:
* 10/25/2013 : Version 1.0.0
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
Demonsrate FTM dual edge capture feature.
First connect FTM0 Ch0 (J2 Pin2) with FTM2 Ch0 (J3 Pin4).  
FTM0 Ch0 generates toggled waveform, the period is 10000, pulse width is 5000, as the input of FTM2 Ch0.
FTM2 Ch0 captures both rising/falling edge of the input waveform, and output the pulse width 5000.

@par Code configuration
     ===================

@par Directory contents
     ==================
- platforms/ke06_config.h               library and board Configuration file
- projects/ke06/isr.h                   Interrupt handler installation 
- projects/ke06/FTM_DualEdgeCapture.c   Main program


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
- Connect FTM0 ch1 to FTM2 ch0, then the dual edge capture ISR happens and display the period


@verbatim
************************END OF FILE********************************************
@endverbatim

*/
