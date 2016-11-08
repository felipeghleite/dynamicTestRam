
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
*        it is recommended that high level optimization for speed be used.
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
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define GPIO_ALIAS_OFFSET       0x000F0000L                                          /*!< alias GPIO base address to 0x4000F000 which can be used for BME BFI/UBFX function  */ 
#define GPIOB_PDOR_ALIAS        (((uint32_t)&GPIOB->PDOR)-GPIO_ALIAS_OFFSET)         /*!< alias GPIO base address to 0x4000F000 which can be used for BME BFI/UBFX function  */ 

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
* Global Definition
******************************************************************************/
#ifdef __GNUC__
#define __ramfunc __attribute__((section (".data")))
#endif

/******************************************************************************
* Global functions
******************************************************************************/

/********************************************************************/
int main (void)
{
    /* Perform processor initialization */
    sysinit();
    cpu_identify();

    ICS_ConfigType  sICSConfig;
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;       
    UART_ConfigType sConfig;

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

    printf("\nIt is in FEE mode now,");
    UART_WaitTxComplete(TERM_PORT);   

    /* switch clock mode from FEE to FEI */ 
    sICSConfig.u32ClkFreq = 32;
    ICS_SwitchMode(FEE,FEI, &sICSConfig);

    /* reinitialize the UART for terminal due to bus clock changed to 24MHz*/
    sConfig.u32SysClkHz = 24000000L;
    sConfig.u32Baudrate  = UART_PRINT_BITRATE;

    UART_Init (TERM_PORT, &sConfig);    

    printf("switch to FEI mode.\n");

     /* start to test bit operations with normal C and BME */

    printf("Logic operation in C takes %d ticks!\r\n", BME_LogicOPwithC());
    printf("Logic operation with BME takes %d ticks!\r\n", BME_LogicOPwithBME());
    /*! 
     * configure PTG0 to PTG3 as output pin
     */
    GPIOB->PDDR |= (0xF << 16);             /* configure PTG0 to PTG3 as output pins */
    GPIOB->PIDR &= ~(0xF << 16);            /* Note: must clear the PIDR bit in order to read correct data from the pin */

       
    printf("Bit field operation in C takes %d ticks!\r\n", BME_BFIwithC((uint32_t*)&GPIOB->PDOR,16,4-1, 5<<16));  /*!< bit width -1 as 3rd parameter */
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
* @brief perform bit field operation with normal C code .
*       
* @param[in] pAddr          pointer to destination address of a 32-bit data. 
* @param[in] u8BitPos       bit field position of a 32-bit data to be operated. 
* @param[in] u8FieldWidth   bit field width-1 of a 32-bit data to be replaced.
* @param[in] u32Data        32-bit data containing the bit field to be inserted to 
*                           the corresponding bit field of 32-bit data. 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
#if (defined(__ICCARM__) || defined(__GNUC__))
__ramfunc uint32_t BME_BFIwithC(uint32_t *pAddr, uint8_t u8BitPos, uint8_t u8FieldWidth, uint32_t u32Data)
#elif defined(__CC_ARM)
uint32_t BME_BFIwithC(uint32_t *pAddr, uint8_t u8BitPos, uint8_t u8FieldWidth, uint32_t u32Data)
#endif
{
    uint32_t    u32RegVal;
    uint32_t    u32Mask;  
    uint32_t    u32LogicOPTicks;

    /* initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/* clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */  
    u32RegVal = *pAddr;   
    u32Mask = ((1 << (u8FieldWidth+1)) - 1) << u8BitPos;
    u32RegVal = (u32RegVal & ~u32Mask)|((u32Data) & u32Mask);
    *pAddr = u32RegVal;  
    /* measure ticks */    
    u32LogicOPTicks = SysTick->VAL;
    return (SysTick->LOAD - u32LogicOPTicks); 
}


/*****************************************************************************//*!
*
* @brief perform bit field operation with BME.
*       Perform bit field operation and emulate GPIO mask.
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
#if (defined(__ICCARM__) || defined(__GNUC__))
__ramfunc uint32_t BME_BFIwithBME(void)
#elif defined(__CC_ARM)
uint32_t BME_BFIwithBME(void) 
#endif
{
      
    uint32_t    u32LogicOPTicks;
    uint32_t    u32Data = (0x5 << 16);
    uint32_t    u32Addr = GPIOB_PDOR_ALIAS;
     /* configure PTG0 to PTG3 as output pin */
    GPIOB->PDDR |= (0xF << 16);             /* configure PTG0 to PTG3 as output pins */
    GPIOB->PDOR =  0;                       /* output 0 to PTG0 to PTG3  */
    GPIOB->PIDR &= ~(0xF << 16);            /* Note: must clear the PIDR bit in order to read correct data from the pin */
    
    /* initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/* clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */
    BME_BITFIELD_INSERT(u32Addr,16,4) =  u32Data;  /* write 5 to bit 19..16 */
    /* measure ticks */    
    u32LogicOPTicks = SysTick->VAL;

    /* check if the data field is inserted correctly (canceled)*/
    /*
    if(u32Data != GPIOB->PDIR)
    {
        printf("Error: bitfield insertion with BME failed!\n");
    }
    */
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
