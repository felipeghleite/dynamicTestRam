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
#include "uart_app.h"
#include "sysinit.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define SEND_BUF_LEN    1
#define RECEIVE_BUF_LEN 1
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t send_buf[SEND_BUF_LEN] = {'L'};
uint8_t receive_buf[RECEIVE_BUF_LEN] = {0};

/******************************************************************************
* Global functions
******************************************************************************/
int main (void)
{	
    UART_ConfigType sConfig;
    
    /* Perform processor initialization */
    sysinit();
    
    sConfig.u32SysClkHz = BUS_CLK_HZ;
    sConfig.u32Baudrate = 115200;

	LED0_Init();
    printf("\nRunning the UART_Loopback_demo project.\n");  
    printf("\nEnter any character to echo...\n");
    UART_WaitTxComplete(UART1);
    
    UART_Init(UART0,&sConfig);
  	UART_EnableLoopback(UART0);
	UART_SetCallback(UART_HandleInt);	

    /* disable UART1 rx/tx interrupt */
    UART_DisableInterrupt(TERM_PORT, UART_RxBuffFullInt);
    UART_DisableInterrupt(TERM_PORT, UART_TxBuffEmptyInt);
	/* enable UART1 receive overrun interrupt */  
 	UART_EnableInterrupt(UART1, UART_RxOverrunInt);
    NVIC_EnableIRQ(UART1_IRQn);
     
    while (1)
    {
        send_buf[0] = UART_GetChar(TERM_PORT);
        UART_SendWait(UART0, send_buf, 1); 
        UART_ReceiveWait(UART0, receive_buf, 1);
        UART_PutChar(TERM_PORT, receive_buf[0]);    
    }
}

