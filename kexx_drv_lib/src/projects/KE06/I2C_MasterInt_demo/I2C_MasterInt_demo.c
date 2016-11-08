
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
* @file I2C_MasterInt_demo.c
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
#define I2C_READ_DATA_LENGTH    64
#define I2C_SLAVE_ADDRESS1      0x50
//#define I2C0_4WIRE_ENABLE        
//#define I2C0_4WIRE_OUT_INVERT
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
uint32_t u32I2C_ReceiveLength;
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
    uint8_t         u8I2C_ErrorStatus;
    I2C_ConfigType  sI2C_Config = {0};
    volatile uint32_t i;
    
    /* Perform processor initialization */
    sysinit();
    
    for(i=0;i<0xfff;i++);
    
  	printf("\r\nRunning the I2C_MasterInt_demo project.\r\n");
    LED0_Init();  
    LED2_Init();

    UART_WaitTxComplete(TERM_PORT);   
    
    /* initialize I2C global variable and call back function*/
    I2C0_InitGlobalVariable( );
    
    /* Initialize I2C module with interrupt mode */
    sI2C_Config.u16Slt = 0;
    sI2C_Config.u16F = 0x1F;
    sI2C_Config.sSetting.bIntEn = 1;
    sI2C_Config.sSetting.bI2CEn = 1;

    I2C_Init(I2C0,&sI2C_Config );
    
#ifdef I2C0_4WIRE_ENABLE
    SIM_Enable4WireI2C0();
#endif
    
#ifdef I2C0_4WIRE_OUT_INVERT
    SIM_EnableI2C0OuputInvertion();
#endif
   
    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;
    }

	while(1)
	{
         u32I2C_ReceiveLength = I2C0_MasterCheckRead(&u8I2C_ReceiveBuff[0]);
         if( !I2C_IsBusy(I2C0) )
         {
             /* if I2C is not busy, read data from slave */
             printf("start to read data from slave!\r\n");
             I2C0_MasterRead(I2C_SLAVE_ADDRESS1,I2C_READ_DATA_LENGTH);
         }
	    if( u32I2C_ReceiveLength == 0 )
	    {
            printf("don't receive any data from slave!\r\n");
	    }
	    else
	    {
            for(i=0;i<u32I2C_ReceiveLength;i++)
            {
                if( (i%8) == 0 )
                {
                    printf("\r\n");
                }
                printf("0x%x,",u8I2C_ReceiveBuff[i]);
               
             }
             if( u32I2C_ReceiveLength >= I2C_READ_DATA_LENGTH )
             {
                printf("\r\nreceived all required data!\r\n");
                printf("start to send data to slave!\r\n");
                u8I2C_SendBuff[0]++;
                for(i=1;i<64;i++)
                {
                    u8I2C_SendBuff[i] = i+u8I2C_SendBuff[0];
                } 
            
                /* wait I2C bus idle, */
                u8I2C_ErrorStatus = I2C0_MasterSend(I2C_SLAVE_ADDRESS1,&u8I2C_SendBuff[0],64);
                if(  u8I2C_ErrorStatus != I2C_ERROR_NULL )
                {
                    printf("I2C transfer status:0x%x\r\n",u8I2C_ErrorStatus);
                }
             }
         }
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
