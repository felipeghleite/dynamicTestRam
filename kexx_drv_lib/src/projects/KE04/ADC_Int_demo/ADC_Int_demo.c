
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
* @file ADC_Int_demo.c
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
uint16_t u16ADC_ConversionBuff[16];
uint16_t u16ADC_ConversionCount = 0;
volatile uint8_t  u8ADC_ConversionFlag = 0;
/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void ADC_CallBack( void );
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    uint8_t         u8Ch;
    ADC_ConfigType  sADC_Config = {0};
    
    /* Perform processor initialization */
	  sysinit();
  	printf("\nRunning the ADC_Int_demo project.\r\n");
  
    UART_WaitTxComplete(TERM_PORT);   
    
    /* initiaze ADC module */
    sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_4;
    sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;
    sADC_Config.u8Mode = ADC_MODE_12BIT;
    sADC_Config.sSetting.bIntEn = 1;

    ADC_SetCallBack(ADC_CallBack);
    ADC_Init( ADC, &sADC_Config);
    /* echo chars received from terminal */
	while(1)
	{
	    /* set channel to start a new conversion */
	    u8ADC_ConversionFlag = 0;
        ADC_SetChannel(ADC,ADC_CHANNEL_AD22_TEMPSENSOR);
        /* wait conversion complete */
        while( !u8ADC_ConversionFlag);

        /* set channel to start a new conversion */
	    u8ADC_ConversionFlag = 0;
        ADC_SetChannel(ADC,ADC_CHANNEL_AD29_VREFH);
        /* wait conversion complete */
        while( !u8ADC_ConversionFlag);

        /* set channel to start a new conversion */
	    u8ADC_ConversionFlag = 0;
        ADC_SetChannel(ADC,ADC_CHANNEL_AD30_VREFL);
        /* wait conversion complete */
        while( !u8ADC_ConversionFlag);

        printf("ADC conversion result as below:\r\n");
        printf("\r\n \r\n");
        for( u8Ch =0 ;u8Ch< u16ADC_ConversionCount; u8Ch ++)
        {

            printf("0x%x,",u16ADC_ConversionBuff[u8Ch]);
        }
        printf("\r\n \r\n");
        printf("\r\n input any character to start a new conversion!\r\n");
        u8Ch = UART_GetChar(TERM_PORT);
        u16ADC_ConversionCount = 0;
	} 

}


/***************************************************************************
+FUNCTION----------------------------------------------------------------
* @function name: ADC_CallBack
*
* @brief callback routine of ADC driver which does what you want to do at 
*        every ADC interrupt.
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void ADC_CallBack( void )
{
    if( u16ADC_ConversionCount < 16 )
    {
        u16ADC_ConversionBuff[u16ADC_ConversionCount++] = ADC_ReadResultReg(ADC);
    }

     u8ADC_ConversionFlag = 1;
}
/********************************************************************/
