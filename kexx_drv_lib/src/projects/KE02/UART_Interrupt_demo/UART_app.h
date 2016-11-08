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
* @file UART.h
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun 6, 2013
*
* @brief provide commond UART utilities. 
*
*******************************************************************************/
#ifndef _UART_APP_H_
#define _UART_APP_H_


/******************************************************************************
* Includes
******************************************************************************/
#include "common.h"
#include "uart.h"

/* callback types */
typedef void (*UART_TxDoneCallbackType)(void);
typedef void (*UART_RxDoneCallbackType)(void);
/******************************************************************************
* Global functions declaration
******************************************************************************/
void UART_SendInt(UART_Type *pUART, uint8_t *pSendBuff, uint32_t u32Length);
void UART_ReceiveInt(UART_Type *pUART, uint8_t *pReceiveBuff, uint32_t u32Length);
void UART_HandleInt(UART_Type *pUART);
void UART_SetTxDoneCallback(UART_Type *pUART, UART_TxDoneCallbackType pfnCallback);
void UART_SetRxDoneCallback(UART_Type *pUART, UART_RxDoneCallbackType pfnCallback);


#endif /* #ifndef _UART_APP_H_ */


