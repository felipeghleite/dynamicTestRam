
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
* @file ADC_Poll_demo.c
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
#include "adc.h"
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
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    ADC_ConfigType  sADC_Config = {0};
    
    sysinit();
  	printf("\nRunning the ADC_Poll_demo project.\n");
    
    UART_WaitTxComplete(TERM_PORT);   
    
    /* initiaze ADC module */
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_4;
    sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;
    sADC_Config.u8Mode = ADC_MODE_12BIT;
    
    ADC_Init( ADC, &sADC_Config);
    /* echo chars received from terminal */
	while(1)
	{
	    printf("Temperature sensor conversion value:0x%x\n",ADC_PollRead(ADC,ADC_CHANNEL_AD22_TEMPSENSOR));
        printf("VREFH conversion value:0x%x\n",ADC_PollRead(ADC,ADC_CHANNEL_AD29_VREFH));
        printf("VREFL conversion value:0x%x\n",ADC_PollRead(ADC,ADC_CHANNEL_AD30_VREFL));
        printf("input any character to start a new conversion!\n");
        UART_GetChar(TERM_PORT);
        
	} 

}

/********************************************************************/
