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
#include "UART_app.h"
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
uint8_t send_buf[SEND_BUF_LEN];
volatile uint8_t u8IsSendDone;

/******************************************************************************
* Global functions
******************************************************************************/

void printf_int(int8* str)
{
	uint32 len = 0;

	u8IsSendDone = 0;                   /* send is in process to protect send_buf[] */
    
	while(*str)
	{
		send_buf[len++] = *str;
		str++;
		if (len >= (SEND_BUF_LEN-1))
		{
			send_buf[SEND_BUF_LEN-1] = 0;
			break;
		}
	}


    UART_SendInt(UART0, send_buf, len);

}

void UART_SendDone(void)
{
    u8IsSendDone = 1;
}

int main (void)
{
    UART_ConfigType sConfig;

    /* Perform processor initialization */
    sysinit();

    u8IsSendDone = 1;

    sConfig.u32SysClkHz = BUS_CLK_HZ;
    sConfig.u32Baudrate = UART_PRINT_BITRATE;
    
    UART_Init(UART0,&sConfig);  
    UART_SetTxDoneCallback(UART0, UART_SendDone);
    UART_SetCallback(UART_HandleInt);

    LED0_Init();
    printf("\r\nRunning the UART_Interrupt_demo project.\r\n");
  	
  	/* enable UART0 interrupts */       
    NVIC_EnableIRQ(UART0_IRQn); 

    printf_int("\r\nPrint characters using interrupt mode.\r\n");
    while (!u8IsSendDone);          /* Waiting for send done */
    
    printf_int("\r\n\rrepeat Print characters using interrupt mode.\r\n");
    while (!u8IsSendDone);          /* Waiting for send done */
    
    while (1);
}

