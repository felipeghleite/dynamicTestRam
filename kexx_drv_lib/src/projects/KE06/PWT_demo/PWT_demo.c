
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
* @file PWT_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief providing framework of demo cases for MCU. 
*
*******************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "pwt.h"
#include "sysinit.h"

/******************************************************************************
* Global variables
******************************************************************************/

volatile uint32_t u32PositivePulse[64], u32NegativePulse[64], u32Period;
volatile uint32_t u32SampleCycle = 0,  u32Inc;
volatile uint32_t u32IntervalTime=0;
/******************************************************************************
* Constants and macros
******************************************************************************/

#define DIVIDER_RATIO 128
#define TIMER_CLK     40000000L  

#define PTD0_Init()				GPIOA->PDDR |= (1<<24) 
#define PTD0_Toggle()		    GPIOA->PTOR = (1<<24)
#define PTD0_On()		        GPIOA->PCOR = (1<<24)
#define PTD0_Off()		        GPIOA->PSOR = (1<<24)

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
void PWT_Task(void);
void Pulse_Calculation(void);

/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{

    PWT_ConfigType  sPWTConfig = {0};
    PWT_ConfigType  *pPWTConfig = &sPWTConfig;     
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;       
    
    /* Perform processor initialization */
    sysinit();
  	printf("\nRunning the PWT_demo project.\n");    
   
    /* the code must be runned in FEE mode with exteral 8MHz crystal, and TIMER_CLK is 40MHz */
    /* the code is for input clock is from 100Hz~10KHz*/
 
    
    /*inital PTD0 near PWT_IN0 input(PTD5) on freedom board*/
    PTD0_Init();  

    /* configure RTC to 0.5KHz interrupt frequency */
    pRTCConfig->u16ModuloValue = 4999;                                      

    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
    pRTCConfig->bClockSource   = RTC_CLKSRC_EXTERNAL;      /*clock source is external 8MHz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_1024;   /*prescaler is 8*/
    RTC_SetCallback(RTC_Task);
	RTC_Init(pRTCConfig);

    pPWTConfig->bOverflowIntEn      = TRUE;    
    pPWTConfig->bReadyIntEn         = TRUE;                     /*enable data reday interrupt*/
    pPWTConfig->bPWTIntEn           = TRUE;                     /*enable PWT module interrupt*/
    pPWTConfig->bPWTEn              = TRUE;                     /*enable PWT module */
    pPWTConfig->bClockPrescaler     = PWT_CLK_PRESCALER_128;    /*PWT clock source presclaer is 128 */
    pPWTConfig->bEdgeSensitivity    = RISING_START_CAPTURE_ALL; /*start to measure pulse from first rising-edge and all dege */
    pPWTConfig->bPinSelect          = PWT_PINSEL_PTC4;          /*PWT input clock pin is PTD5 */        
    pPWTConfig->bClockSource        = PWT_PCLKS_BUS;            /*PWT clock source is bus clock */
    PWT_SetCallback(PWT_Task);
    PWT_Init(pPWTConfig);

    while(1)
    {        
        Pulse_Calculation(); 
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

    /* toggle PTD0 to produce 1KHz frequency waveform */
    PTD0_Toggle();
    u32IntervalTime++;
}


/*****************************************************************************//*!
*
* @brief callback routine of PWT driver 
*        
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void PWT_Task(void)
{            
    if(PWT_GetOverflowFlag())
    {
        PWT_ClrOverflowFlag();
    }
     
    if(PWT_GetReadyFlag())
    {   
        PWT_ClrReadyFlag();
        if(u32SampleCycle<64)
        {   
            u32PositivePulse[u32SampleCycle] = ((PWT->R1>>16)&0xFFFF);   /*put the positive pulse width data to buffer */      
            u32NegativePulse[u32SampleCycle] = (PWT->R2&0xFFFF);         /*put the negative pulse width data to buffer */
            PWT_ClrReadyFlag();
            u32SampleCycle++;
        }
        if(u32SampleCycle == 64)                                     /*only sample 64 times */ 
        {   
            u32SampleCycle = 0;  
        }
    }
}

/*****************************************************************************//*!
*
* @brief calculate the period of input pulse 
*        
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void Pulse_Calculation(void)
{
    while(u32IntervalTime<=2000);    /*calculate pluse width each 1s */ 
    for(u32Inc=0;u32Inc<64;u32Inc++)
    {    
        if(u32PositivePulse[u32Inc] && u32NegativePulse[u32Inc])
        {
            u32Period = TIMER_CLK/((u32PositivePulse[u32Inc] + u32NegativePulse[u32Inc])*DIVIDER_RATIO);   /*calculate pluse width each 1s */
            printf("Positive Pulse Width =%d, Negative Pulse Width =%d, Period = %dHz\n", u32PositivePulse[u32Inc], u32NegativePulse[u32Inc], u32Period);
        }
    }

    for(u32Inc=0;u32Inc<64;u32Inc++)
    {
        u32PositivePulse[u32Inc] =0;
        u32NegativePulse[u32Inc] =0;
    }
    
    u32IntervalTime = 0;

}
