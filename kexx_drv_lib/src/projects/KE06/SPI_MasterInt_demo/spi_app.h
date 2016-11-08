/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
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
* @file SPI_app.h
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun 23, 2013
*
* @brief header file for SPI application driver.
*
******************************************************************************/

#ifndef SPI_APP_H_
#define SPI_APP_H_

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/
/******************************************************************************
* define PTE3 used for SPI0 CS
*
*******************************************************************************/
#define CS0_PIN_MASK ( 1 << 3)
#define CS0_PIN_INPUT() (GPIOB_PDDR &= ~CS0_PIN_MASK)
#define CS0_PIN_Init()	{GPIOB_PDDR |=  CS0_PIN_MASK; GPIOB_PSOR = CS0_PIN_MASK;}
#define CS0_HIGH()		(GPIOB_PSOR = CS0_PIN_MASK)
#define CS0_LOW()		(GPIOB_PCOR = CS0_PIN_MASK)

/******************************************************************************
* Macros
******************************************************************************/
/* maximum number of SPIs */
#define     MAX_SPI_NO              2

/******************************************************************************
* define SPI driver buffer size
*
*******************************************************************************/

#define     SPI0_TX_BUF_MAX     16
#define     SPI0_RX_BUF_MAX     16
#define     SPI1_TX_BUF_MAX     16
#define     SPI1_RX_BUF_MAX     16

/******************************************************************************
* define SPI RX or TX status
*
*******************************************************************************/
#define SPI_STATUS_IDLE			0x00
#define SPI_STATUS_RX_OVER		0x01
#define SPI_STATUS_TX_OVER		0x02
#define SPI_STATUS_BUSY			0x80

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
ResultType SPI_Transfer(SPI_Type *pSPI, SPI_WidthType* pRdBuff, SPI_WidthType *pWrBuff,uint32 uiLength);
void SPI_InitGlobalVariable( void );
uint8_t SPI_GetTransferStatus( SPI_Type *pSPI );
void SPI_ResetTransferStatus( SPI_Type *pSPI );

#endif /* SPI_H_ */
