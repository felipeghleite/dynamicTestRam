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
* @file FTM_DualEdgeCapture.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief Demonsrate FTM dual edge capture feature.
*        First connect FTM0 Ch1 (J2 Pin15) with FTM2 Ch0 (J2 Pin4).  
*        FTM0 Ch1 generates toggled waveform, the period is 10000, pulse width is 5000,
*        as the input of FTM2 Ch0.
*        FTM2 Ch0 captures both rising/falling edge of the input waveform,
*        and output the pulse width 5000.
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
******************************************************************************/
void FTM2_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/

volatile uint16_t u16Ch0Value,u16Ch1Value;
volatile uint8_t u8IntMark;

/********************************************************************/
int main (void)
{
    /* Perform processor initialization */
    sysinit();
    
    printf("\r\n Enter FTM_DualEdgeCapture_demo\r\n");
    
    /* Ouput FTM0 ch1 to PTB3 which is connected to J2 Pin8 */
    //SIM->PINSEL |= SIM_PINSEL_FTM0PS1_MASK;
    /* FTM0 output compare mode, channel 1 toggles when match */
    /* FTM0 output squre wave, period is 10000, pulse width is 5000 */
    FTM_OutputCompareInit(FTM0, FTM_CHANNEL_CHANNEL1, FTM_OUTPUT_TOGGLE);
    /* set MOD value */
    FTM_SetModValue(FTM0, 4999);
    /* set C1V value */
    FTM_SetChannelValue(FTM0, FTM_CHANNEL_CHANNEL1, 2000);
    /* set clock source and start the counter */
    FTM_ClockSet(FTM0, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV1);
   
    /* configure FTM2 ch0 and ch1, to measuer the input wave period */
    FTM_DualEdgeCaptureInit(  FTM2,    
                              FTM_CHANNELPAIR0,                      /* channel pair 0, ch0 and ch1 */
                              FTM_INPUTCAPTURE_DUALEDGE_ONESHOT,     /* one shot mode */
                              FTM_INPUTCAPTURE_DUALEDGE_RISINGEDGE,  /* channel 0,  detect rising edge */
                              FTM_INPUTCAPTURE_DUALEDGE_FALLInGEDGE  /* channel 1,  detect falling edge */
                            );
    FTM_ClockSet(FTM2, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV1);
    
    FTM_SetCallback(FTM2, FTM2_Task);
    NVIC_EnableIRQ(FTM2_IRQn);
    FTM_EnableChannelInt(FTM2, (FTM_CHANNELPAIR0+1));
    
    
    /* echo chars received from terminal */
	while(1)
	{
        if(u8IntMark) 
        {
            u16Ch0Value = FTM2->CONTROLS[0].CnV;
            u16Ch1Value = FTM2->CONTROLS[1].CnV;
            u8IntMark   = 0;
            printf("\r\n Dual edge capture end. The input wave period is %d\n",(uint16_t)(u16Ch1Value - u16Ch0Value));
            
            /* restart the dual edge capture */
            FTM2->COMBINE |=  (FTM_COMBINE_DECAP0_MASK << (FTM_CHANNELPAIR0 * 4)); 
        }
	} 
}

/*****************************************************************************//*!
*
* @brief callback routine of FTM2 driver 
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void FTM2_Task(void)
{
    FTM_ClrChannelFlag(FTM2, FTM_CHANNELPAIR0);
    FTM_ClrChannelFlag(FTM2, FTM_CHANNELPAIR0+1);
    u8IntMark = 1;
}
/********************************************************************/
