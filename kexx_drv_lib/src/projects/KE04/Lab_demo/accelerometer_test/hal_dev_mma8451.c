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
* @file Hal_dev_mma8451.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief providing APIs for accessing mma8451. 
*
*******************************************************************************
*
* provide APIs for accessing mma8451.
******************************************************************************/

#include "common.h"
#include "i2c.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
	
#define MMA8451_I2C_ADDRESS (0x1d<<1)

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

/******************************************************************************
* Global functions
******************************************************************************/
/*****************************************************************************//*!
   *
   * @brief initialize IIC module to access MMA8451.
   *		
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/

void HAl_DevMma8451Init(void)
{
	I2C_ConfigType  sI2C_Config = {0};
	/* Initialize I2C module with poll mode */
	sI2C_Config.u16F = 0x14;
	sI2C_Config.sSetting.bIntEn = 0;
	sI2C_Config.sSetting.bI2CEn = 1;

	I2C_Init(I2C0,&sI2C_Config );

}


/*****************************************************************************//*!
   *
   * @brief read mma8451 register.
   *		
   * @param  none 
   *
   * @return read result from MMA8451 register.
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/

uint8_t HAL_DevMma8451ReadReg(uint8 u8Addr)
{
    uint8_t u8Result;

    I2C_Start(I2C0);
    I2C_WriteOneByte(I2C0,MMA8451_I2C_ADDRESS | I2C_WRITE);
    I2C_WriteOneByte(I2C0,u8Addr);

    I2C_RepeatStart(I2C0);
    I2C_WriteOneByte(I2C0,MMA8451_I2C_ADDRESS | I2C_READ);

	/* dummy read */
    I2C_ReadOneByte(I2C0,&u8Result,I2C_SEND_NACK);
    
    I2C_ReadOneByte(I2C0,&u8Result,I2C_SEND_NACK);

    I2C_Stop(I2C0);

    return u8Result;
}
/*****************************************************************************//*!
   *
   * @brief write mma8451 register.
   *		
   * @param[in] - u8Addr: address to write data
   *                  - u8Data: Data to be write
   *
   * @return error status
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
uint8_t HAL_DevMma8451WriteReg(uint8_t u8Addr, uint8_t u8Data)
{
	uint8_t u8Status;
	u8Status = I2C_Start(I2C0);
	u8Status = I2C_WriteOneByte(I2C0,MMA8451_I2C_ADDRESS | I2C_WRITE);
	u8Status = I2C_WriteOneByte(I2C0,u8Addr);

	u8Status = I2C_WriteOneByte(I2C0,u8Data);

    u8Status = I2C_Stop(I2C0);

    return u8Status;
}





