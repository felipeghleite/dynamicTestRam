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
* @file Key.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Oct. 25, 2013
*
* @brief providing framework of demo cases for MCU. 
*
*******************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "kbi.h"
#include "led.h"

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
    DisableInterrupts;
    
    uint32_t u32KeyValue;
    
    u32KeyValue = KBI_GetSP(KBI1);

    if ((DemoState != DemoKBI) && ((u32KeyValue == 0x8000000) || (u32KeyValue == 0x10000000)))
    {
        //the first key input        
        //In KBI Demo, turn off RED/Green LED
        DemoState = DemoKBI;
        LED_Set_Light(LED_Blue, 0);  
        LED1_Off();
        
        printf("\r\nNow is KBI/FTM Demo, the lightness of red LED is changed according to the key input. \r\n");
    }

    LED_Set_Light(LED_Blue, 0);  
    LED1_Off();

    if(u32KeyValue == 0x8000000)
    {
        //Key sw3 pushed, increase LED light
        LED_Increase_Light(LED_Red);
    }
	else if(u32KeyValue == 0x10000000)
    {
        //Key sw2 pushed, decrease LED light
        LED_Decrease_Light(LED_Red);
    }
    else
    {
        //default
    }

    //reset KBI value
    KBI_RstSP(KBI1);

    EnableInterrupts;
}

void Key_Init(void)
{
	uint32_t	i;

    KBI_ConfigType  sKBIConfig;

    //LED2_Init();

    /* Initiate all the KBI pins disabled */
    for (i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
    {
        sKBIConfig.sPin[i].bEn	 = 0;
    }

    sKBIConfig.sBits.bRstKbsp   = 1; /*Writing a 1 to RSTKBSP is to clear the KBIxSP Register*/
    sKBIConfig.sBits.bKbspEn   = 1;  /*The latched value in KBxSP register while interrupt flag occur to be read.*/
    sKBIConfig.sBits.bMode   = KBI_MODE_EDGE_ONLY;
    sKBIConfig.sBits.bIntEn  = 1;

    /*Falling edge/low level select; KBI1_P27 channel enable(SW3 on FRDM-KE06 board) */
    sKBIConfig.sPin[27].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
    sKBIConfig.sPin[27].bEn   = 1;

    /*Falling edge/low level select; KBI1_P28 channel enable(SW2 on FRDM-KE06 board) */
    sKBIConfig.sPin[28].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
    sKBIConfig.sPin[28].bEn   = 1;


    KBI_Init(KBI1, &sKBIConfig);
    
    //reset KBI value
    KBI_RstSP(KBI1);

    KBI_SetCallback(KBI1, &KBI1_Task);
}


