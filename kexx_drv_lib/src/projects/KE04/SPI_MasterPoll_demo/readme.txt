/**
@page SPI_MasterPoll_demo_readme   SPI Master Poll Demo
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
This example provides a template for SPI master transfer with poll mode.
it cycle to transfer data to slave device and varify read result, and print the value of wrong 
counter.\n
PTB2 - SPI0_SCK \n
PTB3 - SPI0_MOSI \n
PTB4 - SPI0_MISO \n
PTB5 - SPI0_SS \n
please move resistor R16 to R17.
it need to connect these signals to the other board programming SPI_Slave_demo code for test.
because PTB4 default to be NMI function, so need to define MACRO "DISABLE_NMI" in ke04_config.h.
@par Code configuration
     ===================
Make sure the following macros are defined in ke04_config.h: \n
   USE_FEE and \n
   EXT_CLK_FREQ_KHZ macro is defined to 8000(8Mhz) and \n
   DISABLE_NMI.


@par Directory contents
     ==================
- platforms/ke04_config.h               library and board Configuration file
- projects/ke04/isr.h                   Interrupt handler installation 
- projects/ke04/SPI_MasterPoll_demo.c   Main program


@par Hardware environment
     ====================
 - Board: freedom FRDM-KE04Z, revA \n
  	 	Requirement for demo: \n
        1. connect two boards with J2  	12-12(SPI0_SCK), \n
                                        10-10(SPI0_MOSI), \n
                                        8-8(SPI0_MISO),  \n
                                        6-6(SPI0_SS), \n
                                        14-14(GND)
        2. remove r16 and populate r17.
  
@par How to use it
     =============
In order to make the program work, you must do the following :
- Rebuild all project files and load demo code into the device
- Connect J6 USB port to the computer that will find serial terminal port
- Open the serial terminal
- Configure the serial terminal to protocol: 8-N-1, 115200bps, and flow control: none
- Run the example
- Type any character which will echo to the terminal window

@verbatim
************************END OF FILE********************************************
@endverbatim

*/
