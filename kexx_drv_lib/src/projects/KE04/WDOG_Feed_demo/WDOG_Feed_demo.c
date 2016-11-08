
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
* @file wdog_feed_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief watchdog feed/refresh demo. 
*
*   This demo shows how to use WDOG_Feed routine. To run this demo, the
*   ENABLE_WDOG macro must be defined in kexx_config.h
*******************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "wdog.h"
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
void    DelayUS(uint32_t u32TimeUS);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    WDOG_ConfigType sWDOGConfig = {0};    /*! < watchdog configuration structure */

    /* Perform processor initialization */
    sysinit();
		
    sWDOGConfig.sBits.bWaitEnable   = TRUE;
    sWDOGConfig.sBits.bStopEnable   = TRUE;
    sWDOGConfig.sBits.bDbgEnable    = TRUE;
    sWDOGConfig.sBits.bUpdateEnable = FALSE;
    sWDOGConfig.sBits.bDisable      = FALSE;        /* enable WDOG */
    sWDOGConfig.sBits.bClkSrc       = WDOG_CLK_INTERNAL_1KHZ;
    sWDOGConfig.u16TimeOut          = 1000;  /*< 1s */
    sWDOGConfig.u16WinTime          = 0;  

    WDOG_Init(&sWDOGConfig);

    printf("\nRunning the wdog_feed_demo project.\n");

    if(WDOG_IsReset())                      /*!< check if wathdog reset happens */
    {
        /*! watchdog happens. */
        LED0_Init();                        /*!< initialize red light */
        while(1)
        {
            LED0_Toggle();                  /*!< flash red light if watchdog reset occurs */
            WDOG_Feed();            
            DelayUS(50000);                 /*!< delay around 50ms */
        }
    }

    /*! watchdog does not happens, then flash blue led. */
    LED2_Init();                            /*!< initialize blue light */
    while(1)
    {
        LED2_Toggle();                      /*!< flash blue light if no watchdog reset occurs */
        WDOG_Feed();
        DelayUS(50000);                     /*!< delay around 50ms */
    } 

}


/*****************************************************************************//*!
*
* @brief delay certain period of time in microseconds. 
*        
* @param[in]    u32TimeUS   delay value in microseconds.
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void    DelayUS(uint32_t u32TimeUS)
{
    RTC_ConfigType  sRTCConfig, *pRTC_Config;
      
    pRTC_Config = &sRTCConfig;

    /* configure RTC to 1us period */ 
    //pRTC_Config->bRTCOut = TRUE;
    pRTC_Config->u16ModuloValue = u32TimeUS/(100000000/BUS_CLK_HZ)-1;          
    pRTC_Config->bInterruptEn   = FALSE;                    /*!< enable interrupt */
    pRTC_Config->bClockSource   = RTC_CLKSRC_BUS;           /*!< clock source is bus clock */
    pRTC_Config->bClockPresaler = RTC_CLK_PRESCALER_100;    /*!< prescaler is 100 */
    
    RTC_Init(&sRTCConfig);
    while(!RTC_GetFlags())
        ;
    RTC_ClrFlags();
    RTC_DeInit();    
}

/*****************************************************************************//*!
*
* @brief NMI interrupt service routine. 
*
*   It only stays in endless loop so as to generate watchdog reset.        
* @param    none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void NMI_Isr(void)
{
    /*!
     * endless loop to generate watchdog reset
     */
    while(1)
    {
        printf("Running in NMI_Isr()!\n");
    }
}

/********************************************************************/
