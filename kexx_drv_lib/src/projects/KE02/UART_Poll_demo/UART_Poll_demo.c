
/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************
*
* @file Uart_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun-20-2013
*
* @brief providing framework of test cases for MCU. 
*
*******************************************************************************/

#include "common.h"
#include "rtc.h"
#include "uart.h"
#include "sysinit.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define SEND_BUF_LEN    50

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t send_buf[SEND_BUF_LEN] = "\nUART send char by polling\n\r";


/******************************************************************************
* Global functions
******************************************************************************/
int main (void)
{
    UART_ConfigType sConfig;
    
    /* Perform processor initialization */
    sysinit();
    
    sConfig.u32SysClkHz = BUS_CLK_HZ;
    sConfig.u32Baudrate = UART_PRINT_BITRATE;
    
    UART_Init(UART1,&sConfig);
	
    LED0_Init();
    printf("\nRunning the UART_Poll_demo project.\n");
  	      
    UART_SendWait(UART1, send_buf, 50);
    
    while (1);
}

