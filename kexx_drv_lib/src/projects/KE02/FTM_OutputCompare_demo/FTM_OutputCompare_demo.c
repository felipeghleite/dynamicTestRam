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
* @file FTM_OutputCompare_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief Demo FTM output compare feature. 
*        FTM0 Ch1 generates toggled waveform on PTB3 which is conntected to J2 Pin8
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
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/********************************************************************/
int main (void)
{
    uint8_t u8Ch;
    
    /* Perform processor initialization */
    sysinit();

    /* Ouput FTM0 ch1 to PTB3 which is connected to J2 Pin8 for measure */
    SIM_RemapFTM0CH1Pin();
    /* FTM0 output compare mode, channel 1 toggles when match */
    FTM_OutputCompareInit(FTM0, FTM_CHANNEL_CHANNEL1, FTM_OUTPUT_TOGGLE);
    /* set MOD value */
    FTM_SetModValue(FTM0, 5000);
    /* set C1V value */
    FTM_SetChannelValue(FTM0, FTM_CHANNEL_CHANNEL1, 2000);
    /* set clock source and start the counter */
    FTM_ClockSet(FTM0, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV1);
       
    /* echo chars received from terminal */
	while(1)
	{
        u8Ch = UART_GetChar(TERM_PORT);
        UART_PutChar(TERM_PORT, u8Ch);
	} 

}