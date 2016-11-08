
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
* @file BME_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief providing demo code for BME. 
* @note  the compiler optimization level will affect the code execution time, \
*        also, FLASH and RAM configuration also effect code excution time, \
*        It ¡®s recommend that use highest optimization level in RAM project configuration to get the most accurate result. \
*        for KE04 this is no enough RAM space to load program. Optimization level set to high-balanced.
*******************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "systick.h"
#include "bme.h"

#include "sysinit.h"
#include "start.h"

/******************************************************************************
* Global defination
******************************************************************************/
#ifdef __GNUC__
#define __ramfunc __attribute__((section (".data")))
#endif
/******************************************************************************
* Global variables
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
uint32_t BME_LogicOPwithC(void);
uint32_t BME_LogicOPwithBME(void);
uint32_t BME_BFIwithC(uint32_t *pAddr, uint8_t u8BitPos, uint8_t u8FieldWidth, uint32_t u32Data);
uint32_t BME_BFIwithBME(void);

/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
#if !defined(USE_FEI)    
    ICS_ConfigType  sICSConfig;
    UART_ConfigType sConfig;
#endif
	
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;  
    /* Perform processor initialization */
    sysinit();
    cpu_identify();

    printf("\r\nRunning the BME_demo project.\r\n");
#if defined(__ICCARM__) 
    printf("Build by IAR\r\n");
#elif defined(__GNUC__) 
    printf("Build by GUNC\r\n");
#elif defined(__CC_ARM)
    printf("Build by MDK\r\n");
#else
    printf("Unrecognized compiler!\r\n");
#endif
    LED0_Init();  
    LED2_Init();

    /* configure RTC to 1Hz interrupt frequency */
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/

    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);

#if !defined(USE_FEI)
    printf("\nIt is in FEE mode now,");
    UART_WaitTxComplete(TERM_PORT);   

    /* switch clock mode from FEE to FEI */ 
    sICSConfig.u32ClkFreq = 32;                             /* NOTE: use value 32 for 31.25KHz to 39.0625KHz of internal IRC */
    ICS_SwitchMode(FEE,FEI, &sICSConfig);

    /* reinitialize the UART for terminal due to bus clock changed to 20MHz*/
    sConfig.u32SysClkHz = 24000000L;
    sConfig.u32Baudrate  = UART_PRINT_BITRATE;

    UART_Init (TERM_PORT, &sConfig);    

    printf("switch to FEI mode.\n");

    OSC_Enable();
#endif
    /* start to test bit operations with normal C and BME */
    printf("Logic operation in C takes %d ticks!\r\n", BME_LogicOPwithC());
    printf("Logic operation with BME takes %d ticks!\r\n", BME_LogicOPwithBME());

    /*! 
     * configure PTC0 to PTC3 as output pin
     */
    GPIOA->PDDR |= (0xF << 16);             /* configure PTC0 to PTC3 as output pins */
    GPIOA->PIDR &= ~(0xF << 16);            /* Note: must clear the PIDR bit in order to read correct data from the pin */

    printf("Bit field operation in C takes %d ticks!\r\n", BME_BFIwithC((uint32_t*)&GPIOA->PDOR,16,4-1, 5<<16));  /*!< bit width -1 as 3rd parameter */  
    printf("Bit field operation with BME takes %d ticks!\r\n", BME_BFIwithBME());  
    printf("Test completed!\n");
    while(1)
    {
    } 
}

/*****************************************************************************//*!
*
* @brief perform logic operation in C.
*       
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
#if (defined(__ICCARM__) || defined(__GNUC__))
__ramfunc uint32_t BME_LogicOPwithC(void)
#elif defined(__CC_ARM)
uint32_t BME_LogicOPwithC(void)
#endif
{
    uint32_t    u32PortVal = 0;
    uint32_t    u32LogicOPTicks;    
          
    /*! 
    * configure PTA1 as output pin
    */
    GPIOA->PDDR |= 0x02;
    GPIOA->PIDR &= ~0x02;           /* Note: must clear the PIDR bit in order to read correct data from the pin */

    /* initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/* clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    /*! \
    * normal read modify and write sequence to toggle a GPIO 
    */
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
        
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
        
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
        
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
        
    u32PortVal = GPIOA->PDOR;
    u32PortVal ^= 0x02;
    GPIOA->PDOR = u32PortVal;
    /* measure ticks */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks);
}

/*****************************************************************************//*!
*
* @brief perform logic operation via BME .
*       
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
#if (defined(__ICCARM__) || defined(__GNUC__))
__ramfunc uint32_t BME_LogicOPwithBME(void)
#elif defined(__CC_ARM)
uint32_t BME_LogicOPwithBME(void)
#endif
{
    uint32_t    u32LogicOPTicks;
    /*! 
    * configure PTA1 as output pin
    */
    GPIOA->PDDR |= 0x02;
    GPIOA->PIDR &= ~0x02;           /* Note: must clear the PIDR bit in order to read correct data from the pin */

    /* initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/* clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    BME_XOR(&GPIOA->PDOR) = 0x02;
    BME_XOR(&GPIOA->PDOR) = 0x02;
    BME_XOR(&GPIOA->PDOR) = 0x02;
    BME_XOR(&GPIOA->PDOR) = 0x02;
    BME_XOR(&GPIOA->PDOR) = 0x02;
    /* measure ticks */
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks); 
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
}
/********************************************************************/
