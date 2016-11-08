/**
@page I2C_MasterInt_demo_readme   IIC Mater Interrupt Demo
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
This example provides a template for I2C master with interrupt mode.
It cycle to transfer a frame data(64 bytes) and show it on terminal.
It also provide a template for how to initialize I2C module.
It need to connect the other board programming I2C slave code. 
also can enable the MACRO I2C0_4WIRE_ENABLE, so that user chose the bus voltage 
level they want and the current he need for strong bus noise immunity.

@par Code configuration
     ===================
Make sure the following macros are defined in ke06_config.h:\n
   USE_FEE and \n
   EXT_CLK_FREQ_KHZ macro is defined to 8000(8Mhz).

@par Directory contents
     ==================
- platforms/ke06_config.h               library and board Configuration file
- projects/ke06/isr.h                   Interrupt handler installation 
- projects/ke06/I2C_MasterInt_demo.c    Main program
- projects/ke06/i2c_app.c               I2C application drivers
- projects/ke06/i2c_app.h               header file of I2C application drivers

@par Hardware environment
     ====================
 - Board: freedom FRDM-KE06Z, revA \n
	 - Requirement for demo: \n
                1. connect two boards with J2 18-18(SDA),20-20(SCL), 14-14(GND)
                2. populate the R47 and R39.
@par How to use it
     =============
In order to make the program work, you must do the following :
- Rebuild all project files and load demo code into the device
- Connect J6 USB port to the computer that will find serial terminal port
- Open the serial terminal
- Configure the serial terminal to protocol: 8-N-1, 115200bps, and flow control: none
- Run the example


@verbatim
************************END OF FILE********************************************
@endverbatim

*/
