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
* @file FTM_EPWM_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief Demonstrate EPWM, output pwm from FTM2 Ch1, 
         LED RED on FRDM-KE02 board will blink as pwm pulse width changing..
*
*******************************************************************************/

#include "common.h"
#include "ics.h"
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
******************************************************************************/;
void FTM2_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/

/********************************************************************/
int main (void)
{     
    /* Perform processor initialization */
    sysinit();
    
    SIM_RemapFTM2CH1Pin();
    SIM_RemapFTM2CH0Pin();

    /* FTM2 is set as edge aligned pwm mode, high true pulse */
    FTM_PWMInit(FTM2, FTM_PWMMODE_EDGEALLIGNED, FTM_PWM_HIGHTRUEPULSE); 
    /* FTMEN enable */
    FTM_SetFTMEnhanced(FTM2);
    /* update MOD value */
    FTM_SetModValue(FTM2, 9999);
    /* set clock source, start counter */
    FTM_ClockSet(FTM2, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV1);  
    /* enable FTM2 interrupt in NVIC */
    NVIC_EnableIRQ(FTM2_IRQn);
    /* setup call back function for interrupt */
    FTM_SetCallback(FTM2, FTM2_Task);
    /* enable FTM2 overflow interrupt */
    FTM_EnableOverflowInt(FTM2);

	while(1)
	{} 
}

/*****************************************************************************//*!
*
* @brief callback routine of FTM2 driver. FTM2 CH5 will change the duty cycle
*        periodically. 
*        
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
uint16_t u16ChV_old, u16ChV_new;
void FTM2_Task(void)
{
    static uint16_t u16count;
    static uint8_t u8DirMark;
 
    
    /* clear the flag */
    FTM_ClrOverFlowFlag(FTM2);
          
    if(100 == u16count)
    {
        u16count = 0;
        u16ChV_old = FTM2->CONTROLS[1].CnV;
        if(!u8DirMark)
        {
            u16ChV_new = u16ChV_old + 200;
            if(u16ChV_new >= FTM2->MOD)
            {
                u16ChV_new = FTM2->MOD - 200; 
                u8DirMark  = 1;
            }
            else
            {
            }
        }
        else
        {
            u16ChV_new = u16ChV_old - 200;
            if(u16ChV_new <  200)
            {
                u16ChV_new = 200; 
                u8DirMark  = 0;
            }
            else
            {  
            }
        }
        /* update the channel value */
        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL1, u16ChV_new);
    }
    else
    {
        u16count++;
    }
}
/********************************************************************/