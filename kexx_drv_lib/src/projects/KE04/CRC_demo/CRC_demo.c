
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
* @file     CRC_demo.c
*
* @author   Freescale
*
* @version  0.0.1
*
* @date     Jun 25, 2013
*
* @brief    Cyclic redundancy check (CRC) demo code. 
*
******************************************************************************/

#include "common.h"
#include "crc.h"
#include "uart.h"
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
/******************************************************************************
* Global functions
******************************************************************************/

/********************************************************************/
int main (void)
{   
    uint8_t         u8Ch;
    uint32_t        u32Crc_ConverterResult;
    uint32_t        u32SeedValue;  
    CRC_ConfigType  sCRC_ConfigType = {0};
    CRC_ConfigType  *pCRC_Config=&sCRC_ConfigType;

    uint8_t MessageSource[] = {"123456789"} ;                      /*!< initialize mesasge */  
    /* Perform processor initialization */
    sysinit();
    printf("\nRunning the CRC_demo project.\n");          
    /* inital CRC register works under 16-bit mode */ 
    pCRC_Config->u32PolyData            = 0x1021;                   /*!< initialize CRC-CCITT poly value */ 
    u32SeedValue                        = 0xFFFF;                   /*!< set CRC-CCITT seed value */     
    pCRC_Config->bWidth                 = CRC_WIDTH_16BIT;
    pCRC_Config->bTransposeReadType     = CRC_READ_TRANSPOSE_NONE;  /*!< do not transpose in read */      
    pCRC_Config->bTransposeWriteType    = CRC_WRITE_TRANSPOSE_NONE; /*!< do not transpose in write */ 
    /* begin to CRC-CCITT convertion */    
    CRC_Init(pCRC_Config);                                          /*!< crc module inital as 16-bit mode */  
    
    u32Crc_ConverterResult = CRC_Cal16(u32SeedValue, &MessageSource[0], (sizeof(MessageSource)-1));    
    printf("CRC-CCITT function calculation result = 0x%x @seed = 0x%x .\n", u32Crc_ConverterResult, u32SeedValue );
    
    /* reset CRC register for 32-bit mode calculation*/
    CRC_DeInit();  
    
    /* inital CRC register works under 32-bit mode */
    pCRC_Config->u32PolyData            = 0x04C11DB7;               /*!< set CRC32 poly value */      
    u32SeedValue                        = 0xFFFFFFFF;               /*!< set CRC32 seed value */  
    pCRC_Config->bWidth                 = CRC_WIDTH_32BIT;
    pCRC_Config->bTransposeReadType     = CRC_READ_TRANSPOSE_ALL;   /*!< do not transpose in read */      
    pCRC_Config->bTransposeWriteType    = CRC_WRITE_TRANSPOSE_BIT;  /*!< do not transpose in write */
    pCRC_Config->bFinalXOR              = TRUE;                     /*!< reverse CRC result */
    /* begin to CRC-CCITT convertion */    
    CRC_Init(pCRC_Config);                                          /*!< crc module inital as 32-bit mode */       
    u32Crc_ConverterResult = CRC_Cal32(u32SeedValue, &MessageSource[0], (sizeof(MessageSource)-1));    
    printf("CRC32 function calculation result = 0x%x @seed = 0x%x .\n", u32Crc_ConverterResult, u32SeedValue );

    printf("Please enter any character which will echo...\n");
    
    /* echo chars received from terminal */    
    while(1)
    {		
        u8Ch = UART_GetChar(TERM_PORT);		
        UART_PutChar(TERM_PORT, u8Ch);
    } 
}
