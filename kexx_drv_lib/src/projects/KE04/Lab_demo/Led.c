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
* @file     Led.c
*
* @author   Freescale
*
* @version  0.0.1
*
* @date     Jun 25, 2013
*
* @brief    provide driver for LED APIs.
*
******************************************************************************/


#include "common.h"
#include "led.h"
/******************************************************************************
* External objects
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/
static LED_CONTROL sGreenLedControl;
static LED_CONTROL sCurrentGreenLedControlValue;
static LED_CONTROL sBlueLedControl;
static LED_CONTROL sCurrentBlueLedControlValue;
static LED_CONTROL sRedLedControl;
static LED_CONTROL sCurrentRedLedControlValue;


/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/
void LED_DriveByFtm( void );


/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: Button_Init
*
* @brief initialize button switches.
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void LED_DriveByFtm( void )
{

	SIM->SCGC |= SIM_SCGC_FTM2_MASK ;

	SIM->PINSEL |= 	SIM_PINSEL_FTM2PS3_MASK|
					SIM_PINSEL_FTM2PS2_MASK;


	//FTM1_PWMLOAD |= FTM_PWMLOAD_CH1SEL_MASK|
	//                FTM_PWMLOAD_CH0SEL_MASK;
	FTM2->MOD = FTM_MODULE;  /* 0x0063 / 25MHz = 4uS PWM period */
	FTM2->CONTROLS[2].CnSC = FTM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
	FTM2->CONTROLS[3].CnSC = FTM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
	FTM2->CONTROLS[2].CnV  = FTM_INIT_VAL;  /* 90% pulse width */
	FTM2->CONTROLS[3].CnV  = FTM_INIT_VAL;  /* 90% pulse width */
	FTM2->SC   = FTM_SC_CLKS(1) | FTM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: GREEN Control
*
* @brief initialize button switches.
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void LED_GreenControl(unsigned long OnTime,unsigned long OffTime)
{
	sGreenLedControl.OffTime = OffTime;
	sGreenLedControl.OnTime = OnTime;
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: BLUE Control
*
* @brief initialize button switches.
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void LED_BlueControl(unsigned long OnTime,unsigned long OffTime)
{
	sBlueLedControl.OffTime = OffTime;
	sBlueLedControl.OnTime = OnTime;
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: RED Control
*
* @brief initialize button switches.
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void LED_RedControl(unsigned long OnTime,unsigned long OffTime)
{
	sRedLedControl.OffTime = OffTime;
	sRedLedControl.OnTime = OnTime;
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: CheckLEDStatus
*
* @brief initialize button switches.
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: return the light status 0 -- Light 0ff
*                                                               1 -- Light On
*****************************************************************************/
uint8_t CheckLEDStatus(LED_CONTROL * pCurrentState,LED_CONTROL OriginalState)
{
	if( pCurrentState->OnTime )
	{
		pCurrentState->OnTime --;
		return 1;
	}
	else if( pCurrentState->OffTime )
	{
		pCurrentState->OffTime --;
		return 0;
	}
	else
	{
		*pCurrentState = OriginalState;
	}
    
    return 0;
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: LED_CallBack
*
* @brief initialize button switches.
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: return the light status 0 -- Light 0ff
*                                                               1 -- Light On
*****************************************************************************/

void LED_CallBack ( void )
{
	/* check green light */
	if(CheckLEDStatus(&sCurrentGreenLedControlValue,sGreenLedControl))
	{
		SET_LED_GREEN(300);
	}
	else
	{
		SET_LED_GREEN(0);
	}
	
	/* check blue light */
	if(CheckLEDStatus(&sCurrentBlueLedControlValue,sBlueLedControl))
	{
		SET_LED_BLUE(300);
	}
	else
	{
		SET_LED_BLUE(0);
	}
	/* check green light */
	if(CheckLEDStatus(&sCurrentRedLedControlValue,sRedLedControl))
	{
		SET_LED_RED(300);
	}
	else
	{
		SET_LED_RED(0);
	}
}



