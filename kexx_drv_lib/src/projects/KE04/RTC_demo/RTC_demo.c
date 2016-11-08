
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
***************************************************************************//*!
*
* @file     RTC_demo.c
*
* @author   Freescale
*
* @version  0.0.1
*
* @date     Jun 25, 2013
*
* @brief    Real-time counter (RTC) demo code. 
*
* Note:  RTC produces 1s interrupt with LPO clock souce, then toggle red LED 
*        with the trigger      
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

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void RTC_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    uint8_t u8Ch; 
    uint16_t u16ModuloValue;
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTC_Config=&sRTCConfig;

    /* Perform processor initialization */
    sysinit();
		
  	printf("\nRunning the RTC_demo project.\n");
    LED0_Init();  
    
    /* configure RTC to 1Hz interrupt frequency */ 
    u16ModuloValue = 0x09;
    pRTC_Config->u16ModuloValue = u16ModuloValue;
    pRTC_Config->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /*!< enable interrupt */
    pRTC_Config->bClockSource   = RTC_CLKSRC_1KHZ;          /*!< clock source is 1khz */
    pRTC_Config->bClockPresaler = RTC_CLK_PRESCALER_100;    /*!< prescaler is 100 */
    RTC_SetCallback(RTC_Task);
	RTC_Init(pRTC_Config);
     
    printf("Please enter any character which will echo...\n");
    
    /* echo chars received from terminal */
	while(1)
	{
        u8Ch = UART_GetChar(TERM_PORT);
        UART_PutChar(TERM_PORT, u8Ch);
	}  
}

/*****************************************************************************//*!
*
* @brief callback routine of RTC driver which does what you want to do at 
*        every RTC period.
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void RTC_Task(void)
{

    LED0_Toggle();      /*!< toggle LED1 */
}
/********************************************************************/
