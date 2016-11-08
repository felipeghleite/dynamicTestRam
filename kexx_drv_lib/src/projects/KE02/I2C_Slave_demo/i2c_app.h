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
* @file I2C.h
*
* @author b37531
*
* @version 0.0.1
*
* @date Jun 4, 2013
*
* @brief 
*
*******************************************************************************
*
******************************************************************************/
#ifndef _I2C_APP_H__
#define _I2C_APP_H__

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

#define I2C_MASTER_SEND_BUFFER_LENGTH           64
#define I2C_MASTER_RECEIVE_BUFFER_LENGTH        64
#define I2C_SLAVE_SEND_BUFFER_LENGTH            64
#define I2C_SLAVE_RECEIVE_BUFFER_LENGTH         65

/*!
 * @brief I2C master send data type.
 *
 */
typedef struct
{
    uint32_t u32Pointer;
    uint32_t u32Length;
    uint8_t u8SendBuff[I2C_MASTER_SEND_BUFFER_LENGTH];
}I2C_MasterSendType;

/*!
 * @brief I2C master receive data type.
 *
 */
typedef struct
{
    uint32_t u32Pointer;
    uint32_t u32Length;
    uint8_t u8ReceiveBuff[I2C_MASTER_RECEIVE_BUFFER_LENGTH];
}I2C_MasterReceiveType;

/*!
 * @brief I2C slave receive data type.
 *
 */
typedef struct
{
    uint32_t u32Head;
    uint32_t u32Tail;
    uint32_t u32FreeLength;
    uint8_t u8ReceiveBuff[I2C_SLAVE_RECEIVE_BUFFER_LENGTH];
}I2C_SlaveReceiveType;
/*!
 * @brief I2C slave send data type.
 *
 */
typedef struct
{
    uint32_t u32Pointer;
    uint32_t u32Length;
    uint8_t u8SendBuff[I2C_SLAVE_SEND_BUFFER_LENGTH];
}I2C_SlaveSendType;

/******************************************************************************
* Global functions
******************************************************************************/
void I2C_CallBack( void );
void I2C_InitGlobalVariable( void );
void I2C_MasterIntProcessing( void );
void I2C_SlaveIntProcessing( void );
uint32_t I2C_SlaveReceive( uint8_t *pRdBuff );
uint8_t I2C_SlaveSend( uint8_t *pWrBuff, uint32_t uiLength );
uint32_t I2C_MasterCheckRead( uint8_t *pRdBuff );
uint8_t I2C_MasterRead( uint16_t u16Address,uint32_t u32Length );
uint8_t I2C_MasterSend( uint16_t Address,uint8_t *pWrBuff, uint32_t u32Length );

#endif //

