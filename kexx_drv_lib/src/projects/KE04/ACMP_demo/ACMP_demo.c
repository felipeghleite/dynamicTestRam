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
* @file ACMP_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief providing framework of demo cases for MCU. Changing the voltage level 
*        of ACMP external pin0 between GND and VDD, ACMP ISR happens.
*
*******************************************************************************/

#include "common.h"
#include "acmp.h"
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
void FunForCallback(void);

/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{
    ACMP_ConfigType ACMP_Config;  	
    /* Perform processor initialization */
    sysinit();    
    printf("\nRunning the ACMP_demo project.\n");
    
    /* init ACMP config parameter */
    ACMP_Config.sCtrlStatus.bits.bIntEn = TRUE; /* enable interrupt */
    ACMP_Config.sCtrlStatus.bits.bOutEn = 1;    /* enable output */
    ACMP_Config.sPinSelect.bits.bNegPin = 0x3;  /* negative pin:DAC */
    ACMP_Config.sPinSelect.bits.bPosPin = 0;    /* positive pin:ex0 */
    ACMP_Config.sDacSet.bits.bEn        = TRUE; /* enable DAC */    
    ACMP_Config.sDacSet.bits.bRef       = DAC_REF_VDDA;    /* reference:Vdda */
    ACMP_Config.sDacSet.bits.bVal       = 0x1F; /* half DAC out */
    ACMP_Config.sPinEnable.bits.bEn     = TRUE; /* enable ex0 */
    
    ACMP_SetCallback(ACMP0,FunForCallback);     /* install callback function */
    
    ACMP_Init(ACMP0, &ACMP_Config);             /* init ACMP moudle */
    
    ACMP_Enable(ACMP0);                         /* enable ACMP */
   
	while(1) 
	{      
	} 
}

/*****************************************************************************//*!
*
* @brief  call back function.
*        
* @param none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/
void FunForCallback(void)
{
    if(ACMP_GetFlag(ACMP0))
    {    
        ACMP_ClrFlag(ACMP0);  
    }
    
    printf("\nCallback happens.");
}