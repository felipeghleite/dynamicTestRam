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
* @file KBI_demo.c
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
#include "kbi.h"
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
void KBI0_Task(void);
void KBI1_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
	uint32_t	i;
	uint8_t         u8Ch;
	ICS_ConfigType  sICSConfig={0};
	RTC_ConfigType  sRTCConfig={0};
	RTC_ConfigType  *pRTCConfig = &sRTCConfig;       
	KBI_ConfigType  sKBIConfig={0};

	sysinit();
	printf("\nRunning the KBI_demo project.\n");

	LED0_Init(); 
	LED2_Init();

	/* configure RTC to 1Hz interrupt frequency */
	pRTCConfig->u16ModuloValue = 9;                                      
	pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
	pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
	pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/

	RTC_SetCallback(RTC_Task);
	RTC_Init(pRTCConfig);


	/* Disable all the KBI pins */
	for (i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
	{
		sKBIConfig.sPin[i].bEn	 = 0;
	}

	sKBIConfig.sBits.bRstKbsp   = 1;/*Writing a 1 to RSTKBSP is to clear the KBIxSP Register*/
	sKBIConfig.sBits.bKbspEn   = 1;/*The latched value in KBxSP register while interrupt flag occur to be read.*/
	sKBIConfig.sBits.bMode   = KBI_MODE_EDGE_ONLY;
	sKBIConfig.sBits.bIntEn  = 1;

	/*Falling edge/low level select; KBI1_P27 channel enable(SW3 on FRDM board) */
	sKBIConfig.sPin[27].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	sKBIConfig.sPin[27].bEn   = 1;

	/*Falling edge/low level select; KBI1_P28 channel enable(SW2 on FRDM board) */
	sKBIConfig.sPin[28].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	sKBIConfig.sPin[28].bEn   = 1;

	KBI_Init(KBI0, &sKBIConfig);
	KBI_Init(KBI1, &sKBIConfig);
	KBI_SetCallback(KBI0, &KBI0_Task);
	KBI_SetCallback(KBI1, &KBI1_Task);
    
    /* echo chars received from terminal */
	while(1)
	{
		u8Ch = UART_GetChar(TERM_PORT);
		UART_PutChar(TERM_PORT, u8Ch);
	} 

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

/*****************************************************************************//*!
*
* @brief callback routine of KBI driver. 
*        
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void KBI0_Task(void)
{
	LED2_Toggle();
}

/*****************************************************************************//*!
*
* @brief callback routine of KBI driver. 
*        
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void KBI1_Task(void)
{
	LED2_Toggle();

	if(KBI_GetSP(KBI1)== 0x8000000)
	{
		printf("\nKBI1_P27 pin pressed!SW3 pressed!\n");
	}
	else if(KBI_GetSP(KBI1)== 0x10000000)
	{
		printf("\nKBI1_P28 pin pressed!SW2 pressed!\n");
	}

	KBI_RstSP(KBI1);
}

/********************************************************************/
