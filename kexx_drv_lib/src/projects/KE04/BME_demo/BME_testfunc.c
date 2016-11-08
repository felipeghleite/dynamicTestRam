
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
* @file BME_testfunc.c
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
*        for KE04 this is no enough RAM space to load program. So, some test result will be affected.
*******************************************************************************/
#include "common.h"
#include "systick.h"
#include "bme.h"

/******************************************************************************
* Global defination
******************************************************************************/
#ifdef __GNUC__
#define __ramfunc __attribute__((section (".data")))
#endif

/******************************************************************************
* Constants and macros
******************************************************************************/
#define GPIO_ALIAS_OFFSET       0x000F0000L                                          /*!< alias GPIO base address to 0x4000F000 which can be used for BME BFI/UBFX function  */ 

#if defined(CPU_KE04)
    #define GPIOA_PDOR_ALIAS        (((uint32_t)&GPIOA->PDOR)-GPIO_ALIAS_OFFSET)         /*!< alias GPIO base address to 0x4000F000 which can be used for BME BFI/UBFX function  */ 
#else
    #define GPIOB_PDOR_ALIAS        (((uint32_t)&GPIOB->PDOR)-GPIO_ALIAS_OFFSET)         /*!< alias GPIO base address to 0x4000F000 which can be used for BME BFI/UBFX function  */ 
#endif

/******************************************************************************
* Global functions
******************************************************************************/

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
    uint32_t    u32Addr = GPIOA_PDOR_ALIAS;
    /*! 
     * configure PTC0 to PTC3 as output pin
     */
    GPIOA->PDDR |= (0xF << 16);             /* configure PTC0 to PTC3 as output pins */
    GPIOA->PDOR =  0;                       /* output 0 to PTC0 to PTC3  */
    GPIOA->PIDR &= ~(0xF << 16);            /* Note: must clear the PIDR bit in order to read correct data from the pin */
    
    /* initialize the system tick and count the time of operations */
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk); 
    SysTick->VAL = 0x0;	/* clear current timer value */
    SysTick->LOAD = 0x00FFFFFF;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    /* exe: */
    BME_BITFIELD_INSERT(u32Addr,16,4) =  u32Data;  /* write 5 to bit 19..16 */
    /* measure ticks */    
    u32LogicOPTicks = SysTick->VAL;

    return (SysTick->LOAD - u32LogicOPTicks);   
}
