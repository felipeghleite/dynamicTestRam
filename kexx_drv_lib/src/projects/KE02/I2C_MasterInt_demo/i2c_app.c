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
* @file i2c_app.c
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
#include "i2c.h"
#include "i2c_app.h"

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

static I2C_MasterSendType sI2C_MasterSendInfo = {0};
static I2C_MasterReceiveType sI2C_MasterReceiveInfo = {0};
static I2C_SlaveSendType sI2C_SlaveSendInfo = {0};
static I2C_SlaveReceiveType sI2C_SlaveReceiveInfo = {0};
//static uint8_t u8I2C_BusStatus = I2C_BUS_NORMAL;

/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/


/*****************************************************************************//*!
   *
   * @brief call back function for I2C module.
   *        
   * @param  none
   *
   * @return none
   *
   * @ Pass/ Fail criteria:  none
*****************************************************************************/

void I2C_CallBack( void )
{
    I2C_ClearStatus(I2C0,I2C_S_IICIF_MASK);
    if( I2C_GetStatus(I2C0) & I2C_S_ARBL_MASK )
    {
        I2C_ClearStatus(I2C0,I2C_S_ARBL_MASK);
        if( !(I2C_GetStatus(I2C0) & I2C_S_IAAS_MASK) )
        {
            // IIAAS is 0
            return;
        }
    }
	if( I2C_IsSMB_SLTF(I2C0) )
	{
		/* clear this flag */
		I2C_ClearSLTF(I2C0);

		/* Scl holding low over specified counter, add your code in here */
		//u8I2C_BusStatus |= I2C_BUS_SLTF;
	}

	if( I2C_IsSMB_SHTF2(I2C0) )
	{
		/* clear this flag */
		I2C_ClearSHTF2(I2C0);

		/* Scl holding low over specified counter, add your code in here */
		//u8I2C_BusStatus |= I2C_BUS_SHTF2;
	}
	
    if( I2C_IsMasterMode(I2C0) )
    {
        I2C_MasterIntProcessing();
    }
    else
    {
        I2C_SlaveIntProcessing();
    }

}

/*****************************************************************************//*!
   *
   * @brief initialize the global variable in application file.
   *        
   * @param[in] none.
   *
   * @return none.
   *
   * @ Pass/ Fail criteria:  none.
*****************************************************************************/

void I2C_InitGlobalVariable( void )
{
	
	sI2C_SlaveReceiveInfo.u32FreeLength = I2C_SLAVE_RECEIVE_BUFFER_LENGTH;

	/* initialize call back function */
    I2C0_SetCallBack(I2C_CallBack);
}


/*****************************************************************************//*!
   *
   * @brief I2C0 master interrupt service routine.
   *        
   * @param  none.
   *
   * @return none.
   *
   * @ Pass/ Fail criteria:  none.
*****************************************************************************/
void I2C_MasterIntProcessing( void )
{
    if( I2C_IsTxMode(I2C0) )
    {
        if( sI2C_MasterSendInfo.u32Pointer < sI2C_MasterSendInfo.u32Length )
        {
            I2C_WriteDataReg(I2C0,sI2C_MasterSendInfo.u8SendBuff[sI2C_MasterSendInfo.u32Pointer++]);
        }
        else
        {
            I2C_Stop(I2C0);//I2C0->C1 &= ~I2C_C1_MST_MASK;
        }
    }
    else
    {
        /* Master receive data from slave */
        
        
        if( sI2C_MasterReceiveInfo.u32Pointer < ( sI2C_MasterReceiveInfo.u32Length -1) )
        {
            I2C_SendAck(I2C0);
            sI2C_MasterReceiveInfo.u8ReceiveBuff[sI2C_MasterReceiveInfo.u32Pointer++] =
                    I2C_ReadDataReg(I2C0);  
        }
        else if( sI2C_MasterReceiveInfo.u32Pointer == (sI2C_MasterReceiveInfo.u32Length -1) )
        {
            I2C_SendNack(I2C0);
            sI2C_MasterReceiveInfo.u8ReceiveBuff[sI2C_MasterReceiveInfo.u32Pointer++] =
                    I2C_ReadDataReg(I2C0);  
        }
        else
        {
            I2C_Stop(I2C0);
            I2C_ReadDataReg(I2C0); 
        }
         
    }
}

/*****************************************************************************//*!
   *
   * @brief I2C0 slave interrupt service routine.
   *        
   * @param  none.
   *
   * @return none.
   *
   * @ Pass/ Fail criteria:  none.
*****************************************************************************/
void I2C_SlaveIntProcessing( void )
{
    if( I2C_GetStatus(I2C0) & I2C_S_IAAS_MASK )
    {
        I2C_SendAck(I2C0);

        if( I2C_GetStatus(I2C0)& I2C_S_SRW_MASK )
        {
            // slave send data
            I2C_TxEnable(I2C0);
            sI2C_SlaveSendInfo.u32Pointer = 0;
            I2C_WriteDataReg(I2C0,sI2C_SlaveSendInfo.u8SendBuff[sI2C_SlaveSendInfo.u32Pointer++]);
           
        }
        else
        {
            I2C_RxEnable(I2C0);
            I2C_ReadDataReg(I2C0);
        }
    }
    else
    {
        if( I2C0->S & I2C_S_SRW_MASK )
        {
            // if require ACK from master
            if(I2C_IsReceivedAck(I2C0))
            {
                // no receive the ACK
                I2C_RxEnable(I2C0);
                I2C_ReadDataReg(I2C0);
                // switch to RX
            }
            else
            {
                if( sI2C_SlaveSendInfo.u32Pointer < sI2C_SlaveSendInfo.u32Length )
                {
                    I2C_WriteDataReg(I2C0,sI2C_SlaveSendInfo.u8SendBuff[sI2C_SlaveSendInfo.u32Pointer++]);
                }
                else
                {
                    /* here do nothing, clock  streching
                       or send a 0xff to master.
                                    */
                    I2C_WriteDataReg( I2C0, 0xff );
                }
            }
        }
        else
        {

            if( sI2C_SlaveReceiveInfo.u32FreeLength != 0 )
            {
                sI2C_SlaveReceiveInfo.u8ReceiveBuff[sI2C_SlaveReceiveInfo.u32Tail++] = I2C_ReadDataReg(I2C0);
                if( sI2C_SlaveReceiveInfo.u32Tail >= I2C_SLAVE_RECEIVE_BUFFER_LENGTH )
                {
                    sI2C_SlaveReceiveInfo.u32Tail = 0;
                }
                sI2C_SlaveReceiveInfo.u32FreeLength --;

            }
            else
            {
                /* received buff is full,discard the received data */
                I2C_ReadDataReg(I2C0);
            }
       }
    }
}

/*****************************************************************************//*!
   *
   * @brief I2C0 slave receive data with interrupt. 
   *        
   * @param[out] pRdBuff point the first address of receiving buffer.  
   *
   * @return the length of received data.
   *
   * @ Pass/ Fail criteria:  none.
*****************************************************************************/
uint32_t I2C_SlaveReceive( uint8_t *pRdBuff )
{
    uint32_t i;

    I2C_IntDisable(I2C0);
    i = 0;
    while( sI2C_SlaveReceiveInfo.u32FreeLength < I2C_SLAVE_RECEIVE_BUFFER_LENGTH )
    {
        if( sI2C_SlaveReceiveInfo.u32Head != sI2C_SlaveReceiveInfo.u32Tail )
        {
            pRdBuff[i++] = sI2C_SlaveReceiveInfo.u8ReceiveBuff[sI2C_SlaveReceiveInfo.u32Head++];

            if( sI2C_SlaveReceiveInfo.u32Head >= I2C_SLAVE_RECEIVE_BUFFER_LENGTH )
            {
                sI2C_SlaveReceiveInfo.u32Head = 0;
            }
            
            sI2C_SlaveReceiveInfo.u32FreeLength ++;
        }
    }
    I2C_IntEnable(I2C0);
    return i;
}

/*****************************************************************************//*!
   *
   * @brief I2C0 slave send data with interrupt. 
   *        
   * @param[in] pRdBuff point the first address of transmitting buffer. 
   * @param[in] uiLength the length of transferring data.   
   *
   * @return transferring status
   *
   * @ Pass/ Fail criteria:  none.
*****************************************************************************/
uint8_t I2C_SlaveSend( uint8_t *pWrBuff, uint32_t uiLength )
{
    if( uiLength > I2C_SLAVE_SEND_BUFFER_LENGTH )
    {
        return FALSE;
    }
    I2C_IntDisable(I2C0);
    sI2C_SlaveSendInfo.u32Length = uiLength;
    memcpy((void *)&sI2C_SlaveSendInfo.u8SendBuff[0],(void *)pWrBuff,uiLength);
    I2C_IntEnable(I2C0);

    return TRUE;
}

/*****************************************************************************//*!
   *
   * @brief check how many data I2C receive, copy received data to data buffer. 
   *        
   * @param[out] pRdBuff point the first address of receiving buffer. 
   *
   * @return the length of receiving data.
   *
   * @ Pass/ Fail criteria:  none.
*****************************************************************************/
uint32_t I2C_MasterCheckRead( uint8_t *pRdBuff )
{
    uint32_t uiPointer;
    
    if( sI2C_MasterReceiveInfo.u32Pointer !=0 )
    {
        memcpy((void *) pRdBuff,
        (void *)&sI2C_MasterReceiveInfo.u8ReceiveBuff[0],
        sI2C_MasterReceiveInfo.u32Pointer );
        uiPointer = sI2C_MasterReceiveInfo.u32Pointer;
        sI2C_MasterReceiveInfo.u32Pointer = 0;
        return uiPointer;
    }
    else
    {
        /* don't receive all required data */
        return 0;
    }
}

/*****************************************************************************//*!
   *
   * @brief I2C0 master receive data with interrupt. 
   *        
   * @param[in] u16Address I2C slave address. 
   * @param[in] u32Length the length of reading data to slave I2C.  
   *
   * @return error status.
   *
   * @ Pass/ Fail criteria:  none.
*****************************************************************************/
uint8_t I2C_MasterRead( uint16_t u16Address,uint32_t u32Length )
{
    
    uint8_t u8Status;
    
   I2C_IntDisable(I2C0);
   
   /* send out start on bus */
   u8Status = I2C_Start(I2C0);

   if( u8Status == I2C_ERROR_NULL )
   {
       u8Status = I2C_WriteOneByte(I2C0,(uint8)((u16Address<<1)|0x01));
       if( u8Status == I2C_ERROR_NULL )
       {
           
            /* change to Rx mode */
            sI2C_MasterReceiveInfo.u32Length = u32Length;
            sI2C_MasterReceiveInfo.u32Pointer = 0;
            I2C_SendAck(I2C0);
            I2C_RxEnable(I2C0);
            I2C_ReadDataReg(I2C0);
       }
   }

   I2C_IntEnable(I2C0);
   
   return u8Status;
}

/*****************************************************************************//*!
   *
   * @brief I2C0 master send data with interrupt. 
   *        
   * @param[in] u16Address I2C slave address. 
   * @pWrBuff[in] the first address of sending to I2C slave device. 
   * @param[in] u32Length the length of reading data to slave I2C.  
   *
   * @return error status
   *
   * @ Pass/ Fail criteria:  read data
*****************************************************************************/
uint8_t I2C_MasterSend( uint16_t Address,uint8_t *pWrBuff, uint32_t u32Length )
{
    uint8_t u8Status;

    if( I2C_IsBusy(I2C0) )
    {
        return I2C_ERROR_BUS_BUSY;
    }

    I2C_IntDisable(I2C0);
    
    /* send out start on bus */
    u8Status = I2C_Start(I2C0);
    I2C_TxEnable(I2C0);
    if( u8Status == I2C_ERROR_NULL )
    {
        u8Status = I2C_WriteOneByte(I2C0,(uint8)((Address<<1)&0xfe));
        if( u8Status == I2C_ERROR_NULL )
        {
            memcpy((void *)&sI2C_MasterSendInfo.u8SendBuff[0],
                    (void *)pWrBuff,u32Length); 
   
        }
    }
    sI2C_MasterSendInfo.u32Pointer = 0;
    sI2C_MasterSendInfo.u32Length = u32Length;
    I2C_IntEnable(I2C0);
    
    I2C_WriteDataReg(I2C0, sI2C_MasterSendInfo.u8SendBuff[sI2C_MasterSendInfo.u32Pointer++] );

    return u8Status;

}



