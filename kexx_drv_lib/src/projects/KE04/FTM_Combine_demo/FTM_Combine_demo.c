
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
* @file FTM_Combine_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief FTM2 Ch0 and adjacent ch1 are combined to generate PWM signal
*        on PTC0 (J2 Pin4) and PTC1 (J2 Pin3) on KE04 FRDM board.
*
*******************************************************************************/
#include "common.h"
#include "rtc.h"
#include "ftm.h"
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
void RTC_Task(void);
void FTM2_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
	uint8_t u8Ch;   

    /* Perform processor initialization */
    sysinit();

  	printf("\nRunning the FTM_demo project.\n");

    LED0_Init();     
    /* set FTM2 as combine mode */
    FTM_PWMInit(FTM2, FTM_PWMMODE_COMBINE, FTM_PWM_LOWTRUEPULSE);
    /* set MOD value */
    FTM_SetModValue(FTM2, 9999);
    /* set clock source and start the counter */
    FTM_ClockSet(FTM2, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV1);
    
    FTM_SetCallback(FTM2, FTM2_Task);
    NVIC_EnableIRQ(FTM2_IRQn);
    FTM_EnableOverflowInt(FTM2);
    
    u8Ch = UART_GetChar(TERM_PORT);
    /* set the duty cycle, note: only fit for combine mode */
    FTM_SetDutyCycleCombine(FTM2, FTM_CHANNEL_CHANNEL1, 50);
    
	while(1)
	{
		u8Ch = UART_GetChar(TERM_PORT);
		UART_PutChar(TERM_PORT,u8Ch);
        
        LED0_Toggle();
	} 
}

/*****************************************************************************//*!
*
* @brief callback routine of RTC driver which does what you want to do at 
*        every RTC period.
*        
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void RTC_Task(void)
{
    /* toggle LED1 */
    LED0_Toggle();
}

/*****************************************************************************//*!
*
* @brief callback routine of FTM driver. 
*        
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void FTM2_Task(void)
{
    static uint32_t count;
    
    FTM_ClrOverFlowFlag(FTM2);
    
    if(count == 2000)
    {
        count = 0;
        UART_PutChar(TERM_PORT,'@');
    }
    else 
    {
        count++;
    }
}
/********************************************************************/

