/**
@page I2C_Slave_demo_readme   IIC Slave Demo
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
This example provides a template for I2C slave mode.
It receive frame and  show it on terminal.
It also provide a template for how to initialize I2C module.
It need to connect the other board programming I2C master code.

@par Code configuration
     ===================
Make sure the following macros are defined in ke02_config.h:\n
   USE_FEE and \n
   EXT_CLK_FREQ_KHZ macro is defined to 10000.


@par Directory contents
     ==================
- platforms/ke02_config.h           library and board Configuration file
- projects/ke02/isr.h               Interrupt handler installation 
- projects/ke02/I2C_Slave_demo.c    Main program
- projects/ke02/i2c_app.c           I2C application drivers
- projects/ke02/i2c_app.h           header file of I2C application drivers


@par Hardware environment
     ====================
 - Board: freedom FRDM-KE02Z, revA
 	 - Requirement for demo:
	 	1. populate R47,R39
		2. connect two boards with J2 18-18(SDA),20-20(SCL), 14-14(GND)
  
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
