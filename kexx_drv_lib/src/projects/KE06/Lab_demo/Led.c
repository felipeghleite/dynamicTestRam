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
#include "ftm.h"

#define LED_LIGHT_CHANGE_RATE     600
#define LED_LIGHT_MAX_VALUE       9999
/******************************************************************************
* External objects
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/
volatile uint16_t u16LedBluePwmValue = 0;
volatile uint16_t u16LedGreenPwmValue = 0;
volatile uint16_t u16LedRedPwmValue = 0;


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



/*****************************************************************************//*!
*
* @brief callback routine of FTM2 driver. FTM2 CH5 will change the duty cycle
*        periodically. 
*        
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void FTM2_Task(void)
{
    static uint16_t u16count;
    static uint8_t u8DirMark;

    DisableInterrupts;
    
    /* clear the flag */
    FTM_ClrOverFlowFlag(FTM2);
          
    if(100 == u16count)
    {
        /* update the channel value */
        //FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL5, u16LedBluePwmValue);
    }
    else
    {
        u16count++;
    }

    EnableInterrupts;
}

void LED_Increase_Light(LED_Type LEDType)
{
    if (LEDType == LED_Blue)
    {
        if (u16LedBluePwmValue <= LED_LIGHT_CHANGE_RATE)
        {
            u16LedBluePwmValue = 0;
        }
        else
        {
            u16LedBluePwmValue -= LED_LIGHT_CHANGE_RATE;
        }

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL5, u16LedBluePwmValue);
    }
    else if (LEDType == LED_Green)
    {
        if (u16LedGreenPwmValue <= LED_LIGHT_CHANGE_RATE)
        {
            u16LedGreenPwmValue = 0;
        }
        else
        {
            u16LedGreenPwmValue -= LED_LIGHT_CHANGE_RATE;
        }

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL4, u16LedGreenPwmValue);
    }
    else if (LEDType == LED_Red)
    {
        if (u16LedRedPwmValue <= LED_LIGHT_CHANGE_RATE)
        {
            u16LedRedPwmValue = 0;
        }
        else
        {
            u16LedRedPwmValue -= LED_LIGHT_CHANGE_RATE;
        }

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL3, u16LedRedPwmValue);
    }
    else
    {
        //default case
    }
}

void LED_Decrease_Light(LED_Type LEDType)
{
    if (LEDType == LED_Blue)
    {
        if (u16LedBluePwmValue >= LED_LIGHT_MAX_VALUE)
        {
            u16LedBluePwmValue = LED_LIGHT_MAX_VALUE;
        }
        else
        {
            u16LedBluePwmValue += LED_LIGHT_CHANGE_RATE;
        }

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL5, u16LedBluePwmValue);
    }
    else if (LEDType == LED_Green)
    {
        if (u16LedGreenPwmValue >= LED_LIGHT_MAX_VALUE)
        {
            u16LedGreenPwmValue = LED_LIGHT_MAX_VALUE;
        }
        else
        {
            u16LedGreenPwmValue += LED_LIGHT_CHANGE_RATE;
        }

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL4, u16LedGreenPwmValue);
    }
    else if (LEDType == LED_Red)
    {
        if (u16LedRedPwmValue >= LED_LIGHT_MAX_VALUE)
        {
            u16LedRedPwmValue = LED_LIGHT_MAX_VALUE;
        }
        else
        {
            u16LedRedPwmValue += LED_LIGHT_CHANGE_RATE;
        }

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL3, u16LedRedPwmValue);
    }
    else
    {
        //default case
    }
}

void LED_Set_Light(LED_Type LEDType, uint16 u16LightPercent)
{
    //u16LightPercent ranges from 0 - 100
    uint32_t temp = 0;

    if (LEDType == LED_Blue)
    {
        if (u16LightPercent > 100)
        {
            u16LightPercent = 100;
        }
        
        temp = (100 - u16LightPercent) * LED_LIGHT_MAX_VALUE /100 ;
        u16LedBluePwmValue = temp;

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL5, u16LedBluePwmValue);
    }
    else if (LEDType == LED_Green)
    {
        if (u16LightPercent > 100)
        {
            u16LightPercent = 100;
        }
        
        temp = (100 - u16LightPercent) * LED_LIGHT_MAX_VALUE /100 ;
        u16LedGreenPwmValue = temp;

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL4, u16LedGreenPwmValue);
    }
    else if (LEDType == LED_Red)
    {
        if (u16LightPercent > 100)
        {
            u16LightPercent = 100;
        }
        
        temp = (100 - u16LightPercent) * LED_LIGHT_MAX_VALUE /100 ;
        u16LedRedPwmValue = temp;

        FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL3, u16LedRedPwmValue);
    }
}
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
    //Select FTM2Ch5 on PTG7, Blue LED
    SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS5_MASK;

    //Select FTM2Ch3 on PTG5, Red LED
    SIM->PINSEL1 = (SIM->PINSEL1 & (~SIM_PINSEL1_FTM2PS3_MASK)) | SIM_PINSEL1_FTM2PS3(2);

    /* FTM2 is set as edge aligned pwm mode, high true pulse */
    FTM_PWMInit(FTM2, FTM_PWMMODE_EDGEALLIGNED, FTM_PWM_HIGHTRUEPULSE); 
    /* FTMEN enable */
    FTM_SetFTMEnhanced(FTM2);
    /* update MOD value */
    FTM_SetModValue(FTM2, LED_LIGHT_MAX_VALUE);
    /* set clock source, start counter */
    FTM_ClockSet(FTM2, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV1);  
    /* enable FTM2 interrupt in NVIC */
    NVIC_EnableIRQ(FTM2_IRQn);
    /* setup call back function for interrupt */
    FTM_SetCallback(FTM2, FTM2_Task);
    /* enable FTM2 overflow interrupt */
    FTM_EnableOverflowInt(FTM2);

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



