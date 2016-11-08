
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
* @file spi_app.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun 25, 2013
*
* @brief SPI application driver, provide some high level driver and interrupt 
*        service routine. 
*
******************************************************************************/
#include "common.h"
#include "spi.h"
#include "spi_app.h"

/******************************************************************************
* Local variables
******************************************************************************/

static SPI_WidthType *pSPI_RxBuf[MAX_SPI_NO] = {NULL};           /*  pointer to RxBuf */
static SPI_WidthType *pSPI_TxBuf[MAX_SPI_NO] = {NULL};           /*  pointer to TxBuf */
static uint16_t u16SPI_RxBuffWrPointer[MAX_SPI_NO] = {0};                   /* write pointer to RxBuf */
static uint16_t u16SPI_TxBuffRdPointer[MAX_SPI_NO] = {0};                   /* read pointer to TxBuf */
static uint32_t gu32SPI_BuffSize[MAX_SPI_NO] = {0};                   /* buf size*/
static uint8_t u8SPI_Status[MAX_SPI_NO];

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local functions
*****************************************************************************/
void SPI0_MasterCallback(void);
void SPI1_MasterCallback(void);
/******************************************************************************
* Global functions
******************************************************************************/


/*****************************************************************************//*!
   *
   * @brief initialize SPI as per params.
   *        
   * @param[in]  pSPI   point to SPI module type.
   * @param[in]  pConfig point to configuration parameters.
   *
   * @return none.
   *
   * @ Pass/ Fail criteria: none.
   *****************************************************************************/
void SPI_InitGlobalVariable( void )
{
    pSPI_RxBuf[0] = NULL;
    pSPI_TxBuf[0] = NULL;
    u16SPI_RxBuffWrPointer[0] = 0;
    u16SPI_TxBuffRdPointer[0] = 0;
    u8SPI_Status[0] = SPI_STATUS_IDLE;
    SPI_SetCallback(SPI0,SPI0_MasterCallback);

    pSPI_RxBuf[1] = NULL;
    pSPI_TxBuf[1] = NULL;
    u16SPI_RxBuffWrPointer[1] = 0;
    u16SPI_TxBuffRdPointer[1] = 0;
    u8SPI_Status[1] = SPI_STATUS_IDLE;
    SPI_SetCallback(SPI1,SPI1_MasterCallback);
}

/*****************************************************************************//*!
   *
   * @brief implement master write data to SPI.
   *        
   * @param[in]   pSPI  pointer to SPI module type.
   * @param[in]   pWrBuff -- write data buffer pointer. 
   * @param[in]   uiLength -- read/write data length.
   * @param[out]   pRdBuff -- read data buffer pointer. 
   * 
   * @return  if <0, means error, 0: success.
   *
   * @ Pass/ Fail criteria: none.
   *****************************************************************************/
ResultType SPI_Transfer(SPI_Type *pSPI, SPI_WidthType* pRdBuff, SPI_WidthType * pWrBuff,uint32 u32Length)
{
    uint8_t  iSPI = ((uint32_t)pSPI-(uint32_t)SPI0)>>12;
    ResultType err = SPI_ERR_SUCCESS;
    
    if(!u32Length)
    {
        return (err);
    }
    /* record the buffer pointers and buf size */
    gu32SPI_BuffSize[iSPI] = u32Length;
    pSPI_RxBuf[iSPI] = pRdBuff;
    pSPI_TxBuf[iSPI] = pWrBuff;
    u16SPI_RxBuffWrPointer[iSPI] = 0;
    u16SPI_TxBuffRdPointer[iSPI] = 0;
    u8SPI_Status[iSPI] = SPI_STATUS_BUSY;
                
    SPI_TxIntEnable(pSPI);         /* enable tx and rx interrupt */                
	SPI_IntEnable(pSPI); 
    return (err);        
}

/*****************************************************************************//*!
   *
   * @brief  SPI0 interrupt service.
   *        
   * @param[in]   none.
   *
   * @return  none.
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/

void SPI0_MasterCallback(void)
{
    /* check receiver */
    if(SPI_IsSPRF(SPI0))       
    {
    	if( u16SPI_RxBuffWrPointer[0] < gu32SPI_BuffSize[0] )
    	{
       		pSPI_RxBuf[0][u16SPI_RxBuffWrPointer[0]++] = SPI_ReadDataReg(SPI0);  /* read data to rx buffer */
        }
       	if (u16SPI_RxBuffWrPointer[0] >= gu32SPI_BuffSize[0])
       	{
       		SPI_ReadDataReg(SPI0);
       		u8SPI_Status[0] |= SPI_STATUS_RX_OVER;
       	}
    }
    /* check transmitter */
    if(SPI_IsSPTEF(SPI0))
    {
        if(u16SPI_TxBuffRdPointer[0] < gu32SPI_BuffSize[0])            /* tx buf not empty? */
        {
            SPI_WriteDataReg(SPI0,pSPI_TxBuf[0][u16SPI_TxBuffRdPointer[0]++]);      
        }  
        else
        {
			SPI_TxIntDisable(SPI0);
			u8SPI_Status[0] |= SPI_STATUS_TX_OVER;
        }
    }
    /* check other flags */
}
/*****************************************************************************//*!
   *
   * @brief  SPI1 interrupt service.
   *        
   * @param[in]   none.
   *
   * @return  if <0, means error, 0: success.
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/

void SPI1_MasterCallback(void)
{
    /* check receiver */
    if(SPI_IsSPRF(SPI1))       
    {
    	if( u16SPI_RxBuffWrPointer[1] < gu32SPI_BuffSize[1] )
    	{
       		pSPI_RxBuf[1][u16SPI_RxBuffWrPointer[1]++] = SPI_ReadDataReg(SPI1);  /* read data to rx buffer */
       	}
       	if( u16SPI_RxBuffWrPointer[1] >= gu32SPI_BuffSize[1] )
       	{
       		SPI_ReadDataReg(SPI1);
      		u8SPI_Status[1] |= SPI_STATUS_RX_OVER;
       	}
    }
    /* check transmitter */
    if(SPI_IsSPTEF(SPI1))
    {
        if(u16SPI_TxBuffRdPointer[1] < gu32SPI_BuffSize[1])            /* tx buf not empty? */
        {
            SPI_WriteDataReg(SPI1,pSPI_TxBuf[1][u16SPI_TxBuffRdPointer[1]++]);      
        }  
        else
        {
			SPI_TxIntDisable(SPI1);
			u8SPI_Status[1] |= SPI_STATUS_TX_OVER;
        }
    }
    /* check other flags */
}
/*****************************************************************************//*!
   *
   * @brief  get the transfer status.
   *        
   * @param[in]   pSPI  pointer to SPI module type.
   *
   * @return  transfer status.
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
uint8_t SPI_GetTransferStatus( SPI_Type *pSPI )
{
	if( pSPI == SPI0 )
    {
        return u8SPI_Status[0];
    }
    else
    {
        return u8SPI_Status[1];
    }
}

/*****************************************************************************//*!
   *
   * @brief  reset the transfer status.
   *        
   * @param[in]   pSPI  pointer to SPI module type.
   *
   * @return  none.
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void SPI_ResetTransferStatus( SPI_Type *pSPI )
{
    if( pSPI == SPI0 )
    {
        u8SPI_Status[0] = SPI_STATUS_IDLE;
    }
    else
    {
        u8SPI_Status[1] = SPI_STATUS_IDLE;
    }
}





