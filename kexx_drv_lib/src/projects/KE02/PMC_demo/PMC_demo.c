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
* @file PMC_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief providing framework of demo cases for MCU. How enter lowpower mode is shown.
*
*******************************************************************************/

#include "common.h"
#include "rtc.h"
#include "pmc.h"
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
    PMC_ConfigType  PMC_Config={0};
    RTC_ConfigType  RTC_Config={0};

    /* Perform processor initialization */
	  sysinit();
  	printf("\nRunning the PMC_demo project.\n");
    
    LED0_Init();  
    LED2_Init(); 
    
    PMC_Config.sCtrlstatus.bits.bBandgapEn = 1;
    PMC_Config.sCtrlstatus.bits.bLvdStopEn = 0;
    PMC_Config.sCtrlstatus.bits.bLvdRstEn = 0;
    PMC_Init(PMC, &PMC_Config);
    PMC_DisableLVWInterrupt(PMC);
    u8Ch = PMC_GetLVWFlag(PMC);
    
    RTC_Config.u16ModuloValue = 3;
    RTC_Config.bClockSource   = RTC_CLKSRC_1KHZ;
    RTC_Config.bClockPresaler = RTC_CLK_PRESCALER_1000;
    RTC_SetCallback(RTC_Task);
    RTC_Config.bInterruptEn   = 1;
    RTC_Init(&RTC_Config);
   
    printf("\nEnter stop mode and will be woken up in about 4s by RTC Irq.\n");
    PMC_SetMode(PMC,PmcModeStop3);
    printf("\nWake up now.\n");
	
 
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
/********************************************************************/
