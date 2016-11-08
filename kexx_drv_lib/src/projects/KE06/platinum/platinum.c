
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
* @file platinum.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Oct. 15, 2013
*
* @brief providing framework of demo cases for MCU. 
*
*******************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "sim.h"
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
int main (void)
{
    unsigned char u8Ch;
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;       

    /* Perform processor initialization */
    sysinit();
    printf("\nRunning the KE06 platinum project.\n");
    LED0_Init();        //red led
    LED1_Init();        //green led
    LED2_Init();        //blue led
    
    LED0_Off();
    LED1_Off();
    LED2_Off();

    /* configure RTC to 1Hz interrupt frequency */
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/

    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);

    printf("Please enter any character which will echo...\n");
    /* echo chars received from terminal */
    while(1)
    {
        u8Ch = UART_GetChar(TERM_PORT);
        UART_PutChar(TERM_PORT, u8Ch);
        LED2_Toggle();
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
    /* toggle LED1 */
    LED0_Toggle();
}
/********************************************************************/
