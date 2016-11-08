
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
* @file I2C_MasterPoll_demo.c
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
#include "sysinit.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define I2C_SLAVE_ADDRESS1          0x50
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
    I2C_ConfigType  sI2C_Config = {0};
    volatile uint32_t i,j;
    
    sysinit();
    for(i=0;i<0xfff;i++);
    
  	printf("\nRunning the I2C_MasterPoll_demo project.\n");
    LED0_Init();  
    LED2_Init();

    UART_WaitTxComplete(TERM_PORT);   
    
    /* Initialize I2C module with poll mode */
    sI2C_Config.u16F = 0x1F;
    sI2C_Config.sSetting.bIntEn = 0;
    sI2C_Config.sSetting.bI2CEn = 1;

    I2C_Init(I2C0,&sI2C_Config );

    for(i=0;i<64;i++)
    {
        u8I2C_SendBuff[i] = i;
    }
 
	while(1)
	{
        I2C_MasterSendWait(I2C0,I2C_SLAVE_ADDRESS1,&u8I2C_SendBuff[0],64);
        I2C_MasterReadWait(I2C0,I2C_SLAVE_ADDRESS1,&u8I2C_ReceiveBuff[0],64);
        printf("Read data from I2C slave:\n");
        for(i=0;i<8;i++)
        {
            for(j=0;j<8;j++)
            {
                printf("0x%x,", u8I2C_ReceiveBuff[i*8+j]);
            }

            printf("\n");
        }
        u8I2C_SendBuff[0]++;
        for(i=1;i<64;i++)
        {
            u8I2C_SendBuff[i] = i+u8I2C_SendBuff[0];
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
