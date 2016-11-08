
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
* @file Flash_demo.c
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
#include "rtc.h"
#include "Flash.h"
#include "sysinit.h"
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
	char ch;  
    uint32_t i;
    uint8_t  u8DataBuff[512];

		sysinit();
  	printf("\nRunning the Flash_demo project.\n");
    LED0_Init();  
    LED2_Init();
    FLASH_Init(BUS_CLK_HZ);
    
    /* Erase 99th sector */
    FLASH_EraseSector(100*FLASH_SECTOR_SIZE);
    
    for(i=0;i<512;i++)
    {
        u8DataBuff[i] = (uint8_t)i;
    }
    
    /* write data to erased sector */
    FLASH_Program( 100*FLASH_SECTOR_SIZE,&u8DataBuff[0],512 );
    
    for( i=0;i<512/16;i++ )
    {
        for(ch =0;ch<16;ch++)
        {
            printf("0x%x,",*((uint8_t *)(i*16+ch+100*FLASH_SECTOR_SIZE)));
        }
        printf("\n");
    }
    while(1)
		{
			ch = in_char();
			out_char(ch);
	    
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
