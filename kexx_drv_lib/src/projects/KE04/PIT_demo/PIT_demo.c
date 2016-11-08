
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
* @file PIT_demo.c
*
* @author FREESCALE
*
* @version 0.0.1
*
* @date Jun 25, 2013
*
* @brief PIT demo. 
*
*   This demo sets up PIT timer 1 in chain mode with PIT timer 0 to generate 
*   interrupt each 1s. PIT timer 0 runs 1,000,000 cycle, PIT timer 1 run 10 cycle,
*   total cycle is 10,000,000, bus clock in the code is 10MHz, that's to say, 
*   interrupt period is 1s. 
*   Tn callback routine toggle LED0/red LED.
*******************************************************************************/

#include "common.h"
#include "uart.h"
#include "pit.h"
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
void PIT_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    uint8_t         u8Ch;
    uint32_t        u32LoadValue0, u32LoadValue1;     
    PIT_ConfigType  sPITConfig0, sPITConfig1;    
    PIT_ConfigType  *pPIT_Config1   =&sPITConfig1;
    PIT_ConfigType  *pPIT_Config0   =&sPITConfig0; 
		
    /* Perform processor initialization */
    sysinit();
    
    printf("\nRunning the PIT_demo project.\n");
    
    LED0_Init();

    /* configure PIT module in chain mode */
    /* PIT clock source is bus clock,20MHz */
    /* PIT channel 0 load value = (1000000-1), channel 1 load value = (20-1) */
    u32LoadValue0   = 0xF423F;                  /*!< PIT ch0 timer load value  */ 
    u32LoadValue1   = 0x13;                     /*!< PIT ch1 timer load value  */     
    
    /* configure PIT channel 1 in chain mode, enable interrupt and timer */
    pPIT_Config1->u32LoadValue      = u32LoadValue1;
    pPIT_Config1->bFreeze           = FALSE;
    pPIT_Config1->bModuleDis        = FALSE;    /*!< enable PIT module */ 
    pPIT_Config1->bInterruptEn      = TRUE;
    pPIT_Config1->bChainMode        = TRUE;
    pPIT_Config1->bTimerEn          = TRUE; 
    
    /* configure PIT channel 0, only enable timer */    
    pPIT_Config0->u32LoadValue      = u32LoadValue0;
    pPIT_Config0->bFreeze           = FALSE;    
    pPIT_Config0->bModuleDis        = FALSE;    /*!< enable PIT module */     
    pPIT_Config0->bInterruptEn      = FALSE;
    pPIT_Config0->bChainMode        = FALSE; 
    pPIT_Config0->bTimerEn          = TRUE;

    PIT_Init(PIT_CHANNEL0, pPIT_Config0);               
    PIT_Init(PIT_CHANNEL1, pPIT_Config1);
      
    PIT_SetCallback(PIT_CHANNEL1, PIT_Task);    
    
    /* echo chars received from terminal */
	while(1)  
    
	{
        u8Ch = UART_GetChar(TERM_PORT);
        UART_PutChar(TERM_PORT, u8Ch);
	} 

}

/*****************************************************************************//*!
*
* @brief PIT module task
*        
* @param none  
*
* @return none 
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void PIT_Task(void)
{
    LED0_Toggle();          /*!< toggle LED1 */
}
/********************************************************************/
