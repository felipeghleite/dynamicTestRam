
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
* @file BitBand_demo.c
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
#include "bitband.h"
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
#ifdef IAR
#pragma location = 0x20000100
uint32_t u32BitBandTestVariable;
#elif (defined (KEIL))
uint32_t u32BitBandTestVariable __attribute__((at(0x20000100)));
#elif (defined(__GNUC__))
uint32_t *p32BitBandTestVariable = 0x20000100;
#endif
BIT_BandPtr pBitBandTestVariablePtr;
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

    RTC_ConfigType  sRTCConfig = {0};
    RTC_ConfigType  *pRTCConfig = &sRTCConfig; 
    
    /* Perform processor initialization */
    sysinit();    
    
  	printf("\nRunning the BitBand_demo project.\n");
    LED0_Init();  
    LED2_Init();

    /* configure RTC to 1Hz interrupt frequency */
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/
    
    RTC_SetCallback(RTC_Task);
	RTC_Init(pRTCConfig);
	
  	printf("\nIt is in FEE mode now,");
    UART_WaitTxComplete(TERM_PORT);   
 
    printf("Please enter any character which will echo...\n");
    
    
#if (defined(__GNUC__))
    BIT_BandVariableInit(p32BitBandTestVariable,&pBitBandTestVariablePtr);
    *p32BitBandTestVariable = 0x55555555;
#else
    BIT_BandVariableInit(&u32BitBandTestVariable,&pBitBandTestVariablePtr);
    u32BitBandTestVariable = 0x55555555;
#endif 
#if (defined(__GNUC__))
    /* echo chars received from terminal */
   	while(1)
   	{
           printf("Tested variable is 0x%x!\n",*p32BitBandTestVariable); 
           if(pBitBandTestVariablePtr->bBit0)
           {
               printf("the bit0 is 1!\n");
           }
           else
           {
               printf("the bit0 is 0!\n");
           }
           if(pBitBandTestVariablePtr->bBit1)
           {
               printf("the bit1 is 1!\n");
           }
           else
           {
               printf("the bit1 is 0!\n");
           }
           printf("Clear the bit 31!\n");
           pBitBandTestVariablePtr->bBit31 = 0;
           printf("Tested variable is 0x%x!\n",*p32BitBandTestVariable); 
           printf("Set the bit 31!\n");
           pBitBandTestVariablePtr->bBit31 = 1;
           printf("Tested variable is 0x%x!\n",*p32BitBandTestVariable); 
           printf("input any char to start next test!\n");
           UART_GetChar(TERM_PORT);
           (*p32BitBandTestVariable) ++;
           printf("tested variable add 1!\n");
      
   	} 

    
#else
    /* echo chars received from terminal */
   	while(1)
   	{
           printf("Tested variable is 0x%x!\n",u32BitBandTestVariable); 
           if(pBitBandTestVariablePtr->bBit0)
           {
               printf("the bit0 is 1!\n");
           }
           else
           {
               printf("the bit0 is 0!\n");
           }
           if(pBitBandTestVariablePtr->bBit1)
           {
               printf("the bit1 is 1!\n");
           }
           else
           {
               printf("the bit1 is 0!\n");
           }
           printf("Clear the bit 31!\n");
           pBitBandTestVariablePtr->bBit31 = 0;
           printf("Tested variable is 0x%x!\n",u32BitBandTestVariable); 
           printf("Set the bit 31!\n");
           pBitBandTestVariablePtr->bBit31 = 1;
           printf("Tested variable is 0x%x!\n",u32BitBandTestVariable); 
           printf("input any char to start next test!\n");
           UART_GetChar(TERM_PORT);
           u32BitBandTestVariable ++;
           printf("tested variable add 1!\n");
      
   	} 
 
#endif    

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
