
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
* @file SPI_MasterInt_demo.c
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
#include "spi.h"
#include "spi_app.h"
#include "sysinit.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define SPI0_TX_DATA_SIZE			128
#define SPI_BIT_RATE                1000000     /* ~1Mbps */
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
static SPI_WidthType gu8SPI0_RxBuff[SPI0_TX_DATA_SIZE];
static SPI_WidthType gu8SPI0_TxBuff[SPI0_TX_DATA_SIZE];
static  uint32_t  gu32ErrorCount    = 0; 
static  uint8_t   gu8Pattern      = 0;
static uint32_t   gu32Loop = 0;
volatile uint32_t   gu32RTCCount = 0;
static uint32_t   gu32RTCCurrentCount = 0;
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
	static uint32_t i;
	SPI_ConfigType sSPIConfig = {0};
    RTC_ConfigType  sRTCConfig = {0};
    RTC_ConfigType  *pRTC_Config=&sRTCConfig;
	
	/* Perform processor initialization */
	sysinit();
	
  	printf("\r\nRunning the SPI_MasterInt_demo project.\r\n");
    LED0_Init();  
    LED2_Init();
    
    /* configure RTC to 0.5Hz interrupt frequency */ 
    pRTC_Config->u16ModuloValue = 0x04;
    pRTC_Config->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /*!< enable interrupt */
    pRTC_Config->bClockSource   = RTC_CLKSRC_1KHZ;          /*!< clock source is 1khz */
    pRTC_Config->bClockPresaler = RTC_CLK_PRESCALER_100;    /*!< prescaler is 100 */
    RTC_SetCallback(RTC_Task);
	RTC_Init(pRTC_Config);

    UART_WaitTxComplete(TERM_PORT);   
    
    SPI_InitGlobalVariable();
    
    /* initialize SPI0 as master    */
    sSPIConfig.u32BitRate = SPI_BIT_RATE;
    sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
    sSPIConfig.sSettings.bModuleEn             = 1;
    sSPIConfig.sSettings.bMasterMode           = 1;
    sSPIConfig.sSettings.bClkPhase1            = 0;
    sSPIConfig.sSettings.bMasterAutoDriveSS    = 1;
   
 
    SPI_Init(SPI0, &sSPIConfig);
    
    gu8Pattern = 0x55;                                  /* initialize data patter */
    /* initialize data */
    for(i = 0; i < SPI0_TX_DATA_SIZE; i++)
    {
        gu8SPI0_TxBuff[i] = i+ gu8Pattern;
    }
    
    NVIC_EnableIRQ(SPI0_IRQn);
	while(1)
	{
         /* start to transfer to send and receive date in non-block mode */
        SPI_Transfer(SPI0,gu8SPI0_RxBuff, gu8SPI0_TxBuff, SPI0_TX_DATA_SIZE);  /* master transfer data */
        
        /* wait till transfer is complete */
        while(!(SPI_GetTransferStatus(SPI0) & SPI_STATUS_RX_OVER) );
    
        SPI_ResetTransferStatus(SPI0);

        /* check received data */
        for(i = 0; i < SPI0_TX_DATA_SIZE; i++)
        {
            if(gu8SPI0_RxBuff[i] != gu8SPI0_TxBuff[i])
            {                
                gu32ErrorCount++;
                RED_Init();                     /* lit RED led */
                break;
            }
        } 

        printf("\r\nError counter is %d\r\n",gu32ErrorCount);

        gu32Loop ++;

        printf("\r\nSPI communication counter %d\r\n",gu32Loop);

        while(gu32RTCCount == gu32RTCCurrentCount);
        
        gu32RTCCurrentCount = gu32RTCCount;
	} 

}

/*****************************************************************************//*!
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
    gu32RTCCount ++;
}
/********************************************************************/
