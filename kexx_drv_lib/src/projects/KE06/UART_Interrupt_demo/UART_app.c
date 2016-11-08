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
* @file uart.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date June-6-2013
*
* @brief providing common UART API. 
*
******************************************************************************/
#include "UART_app.h"

static uint8_t *pUART_TxBuff[MAX_UART_NO] = {NULL};           /* pointer to RxBuf */
static uint8_t *pUART_RxBuff[MAX_UART_NO] = {NULL};           /* pointer to TxBuf */
static uint16_t gu16UART_TxBuffPos[MAX_UART_NO] = {0};        /* write position to RxBuf */
static uint16_t gu16UART_RxBuffPos[MAX_UART_NO] = {0};        /* read position to TxBuf */
static uint32_t gu32UART_BuffSize[MAX_UART_NO] = {0};         /* buffer size*/

UART_TxDoneCallbackType UART_TxDoneCallback[MAX_UART_NO] = {NULL};
UART_RxDoneCallbackType UART_RxDoneCallback[MAX_UART_NO] = {NULL};
/*****************************************************************************//*!
*
* @brief send a series of charecters using interrupt mode.
*        
* @param[in] pUART      base of UART port
* @param[in] pSendBuff  pointer of charecters to send
* @param[in] u32Length  number of charecters
*
* @return       none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
void UART_SendInt(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    
    /* save user defined send buffer pointers and size */
    pUART_TxBuff[u8Port]        = pSendBuff;
    gu32UART_BuffSize[u8Port]   = u32Length;
    gu16UART_TxBuffPos[u8Port]  = 0;

    UART_EnableTxBuffEmptyInt(pUART);   /* enable tx interrupt */ 
}
/*****************************************************************************//*!
*
* @brief receive a series of charecters using interrupt mode.
*        
* @param[in] pUART      base of UART port
* @param[in] pSendBuff  pointer of charecters to send
* @param[in] u32Length  number of charecters
*
* @return       none
*
* @ Pass/ Fail criteria:
*****************************************************************************/
void UART_ReceiveInt(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    
    /* save user defined read buffer pointers and size */
    pUART_RxBuff[u8Port]        = pReceiveBuff;
    gu32UART_BuffSize[u8Port]   = u32Length;
    gu16UART_RxBuffPos[u8Port]  = 0;

    UART_EnableRxBuffFullInt(pUART);   /* enable rx interrupt */ 
}
void UART_HandleInt(UART_Type *pUART)
{
    uint8_t   u8Port;
    uint8_t   *pRdBuff;
    uint8_t   *pWrBuff;  
    volatile uint8_t read_temp = 0;
    
    u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    
    /* check overrun flag */
    if(UART_CheckFlag(pUART,UART_FlagOR))
    {
        read_temp = UART_ReadDataReg(pUART);     
    }
    
    /* check receiver */
    else if(UART_IsRxBuffFull(pUART))       
    {
        /* there's data received in rx buffer */
        pRdBuff = pUART_RxBuff[u8Port];                     /* get user read buffer */
        read_temp = UART_ReadDataReg(pUART);
        pRdBuff[gu16UART_RxBuffPos[u8Port]++]= read_temp;   /* save received data */
        //pRdBuff[gu16UART_RxBuffPos[u8Port]++] = UART_ReadDataReg(pUART);
        if(gu16UART_RxBuffPos[u8Port] == gu32UART_BuffSize[u8Port])
        {    
            /* User read buffer is full, end receive */
            UART_DisableRxBuffFullInt(pUART);
            if (UART_RxDoneCallback[u8Port])
			{
			    /* call back function to tell user that rx is done */
			    UART_RxDoneCallback[u8Port]();
			}
        }    
    }
    /* check transmitter */
			else if(UART_IsTxBuffEmpty(pUART))
    {
        if(gu16UART_TxBuffPos[u8Port] != gu32UART_BuffSize[u8Port])
        {
            /* user tx buffer not empty */
            pWrBuff = pUART_TxBuff[u8Port];
            UART_WriteDataReg(pUART, pWrBuff[gu16UART_TxBuffPos[u8Port]++]);     
        }  
        else
        {
			UART_DisableTxBuffEmptyInt(pUART);
			if (UART_TxDoneCallback[u8Port])
			{
			    /* call back function to tell user that tx is done */
			    UART_TxDoneCallback[u8Port]();
			}
        }
    }
    else
    {
        /* default interrupt */
    }
}

void UART_SetTxDoneCallback(UART_Type *pUART, UART_TxDoneCallbackType pfnCallback)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    UART_TxDoneCallback[u8Port] = pfnCallback;
}

void UART_SetRxDoneCallback(UART_Type *pUART, UART_RxDoneCallbackType pfnCallback)
{
    uint8_t u8Port = ((uint32_t)pUART-(uint32_t)UART0)>>12;
    UART_RxDoneCallback[u8Port] = pfnCallback;
}
