/**
@page Lab_demo_readme   Lab Demo
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
This example provides a description of how demo the lab running on the board,from the 
start on, it will print messsage to show all Lab message and indicate how to switch lab demo by slider
on boards.
1. TSS Alisder
	Touch slider on the boards to switch Lab demo.
	This demo implement the touch sensor function by GPIO simulation.
2. Accelerometer demo
	Read the data from accelerometer sensor part(MMA8451) through I2C bus and calculate angle change,
	then drive LED to change brightness by Flextimer.
	when you rotate board, blue and green light will change.
3. IrDA
	Uart0_TX output is modulated by FTM0 channel 0( frequency:38khz),then drive infrared transmiting LED.
	UART0_RX input signal is filtered by ACMP in1, then injected to UART0.
	The UART0 is configured as follow: 
	- Word Length = 7 Bits
  - One Stop Bit
  - No parity
  - BaudRate = 4800 baud
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  Code is implemented to transmit 16 byte, then check the data received, if success, green light blink,
  otherwise,blue light blink
  when run in IrDA demo, will print mess information to terminals.
5. RTC 
	In RTC demo mode,blue light blink with 2.5Hz frequency, and green light blink with 0.5hz frequency.
6. Idle mode
	In this mode,it don't run others Lab, only Slider is active,blue light bink with cycle of 100mS on and 500mS off.
	
@par Code configuration
   ===================
Make sure the following macros are defined in ke04_config.h:
   USE_FEE and \n
   EXT_CLK_FREQ_KHZ macro is defined to 8000(8Mhz).
	 
@par Directory contents
     ==================
	projects/Lab_demo      


@par Hardware environment
     ====================
 - Board: freedom FRDM-KE04Z, revA \n
  	 	Requirement for demo: \n
	
 - Connect a null-modem male/female RS232 cable between the J6 connector 
   and PC serial port.  

 - Hyperterminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 
  
  
@par How to use it
     =============
In order to make the program work, you must do the following :
- Rebuild all project files and load demo code into the device
- Connect J6 USB port to the computer that will find serial terminal port
- Open the serial terminal
- Configure the serial terminal to protocol: 8-N-1, 115200bps, and flow control: none
- Run the example
- Touch slider to switch to next demo.


@verbatim
************************END OF FILE********************************************
@endverbatim

*/
