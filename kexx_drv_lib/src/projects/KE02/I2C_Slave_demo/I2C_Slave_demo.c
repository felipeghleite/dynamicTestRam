
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
* @file I2C_Slave_demo.c
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
#include "i2c.h"
#include "i2C_app.h"
#include "sysinit.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define I2C_SLAVE_ADDRESS       0x50<<1
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t u8I2C_SendBuff[64];
uint8_t u8I2C_ReceiveBuff[64];
/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void RTC_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    uint8_t         u8I2C_ReceiveLength;
    uint32_t        i;

    I2C_ConfigType  sI2C_Config = {0};
     
    sysinit();
  	printf("\nRunning the I2C_Slave_demo project.\n");
    LED0_Init();  
    LED2_Init();

    UART_WaitTxComplete(TERM_PORT);   
    
    /* initialize I2C global variable and call back function*/
    I2C_InitGlobalVariable(  );
 
    
    sI2C_Config.u16Slt = 0;
    sI2C_Config.u16F = 0x1F;
    sI2C_Config.u16OwnA1 = I2C_SLAVE_ADDRESS;
    sI2C_Config.sSetting.bIntEn = 1;
    sI2C_Config.sSetting.bI2CEn = 1;

    I2C_Init(I2C0,&sI2C_Config);

    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;
    }
    u8I2C_SendBuff[0] = 0xa0;
    I2C_SlaveSend(u8I2C_SendBuff,64);
    
    /* echo chars received from terminal */
	while(1)
	{
        u8I2C_ReceiveLength = I2C_SlaveReceive(&u8I2C_ReceiveBuff[0]);
        if( u8I2C_ReceiveLength < 64 )
        {
            /* send the old data to master */
            I2C_SlaveSend(&u8I2C_SendBuff[0],64);
        }
        else
        {
            /* send the newest data received from master */
            I2C_SlaveSend(&u8I2C_ReceiveBuff[0],64);
        }
        if( u8I2C_ReceiveLength )
        {
            printf("I2C received data:\n");
            for(i=0;i<u8I2C_ReceiveLength;i++)
            {
            		if( (i%8) == 0 )
                {
                   printf("\n"); 
                }
                printf("0x%x,", u8I2C_ReceiveBuff[i]);
            }
            printf("\n");
        }
        //delay
        for(i=0;i<0xfffff;i++);
	} 
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: RTC_Task
*
* @brief callback routine of RTC driver which does what you want to do at 
*        every RTC period.
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void RTC_Task(void)
{
    /* toggle LED1 */
    LED0_Toggle();
}
/********************************************************************/
