
/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*******************************************************************************
*
* @file Lab_demo.c
*
* @author a13984
*
* @version 0.0.1
*
* @date Jul-15-2011
*
* @brief providing framework of test cases for MCU. 
*
*******************************************************************************/

#include "common.h"
#include "rtc.h"
#include "pit.h"
#include "irda.h"
#include "mma8451_test.h"
#include "led.h"
#include "uart.h"
#include "sysinit.h"
#include "systick.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
enum
{
	DemoIdle = 0,
	DemoIrDa,
	DemoAccelerometer,
	DemoRTC,
	DemoEnd,
}DEMO_STATE;

enum
{
	LED_Auto = 0,
	LED_Manual,
	LED_Rtc,
	LED_Idle,
}LED_ControlMode;

#define ASLIDER_DELAY_TIMER		    50
#define TSI_RELEASE                 0x01
#define TSI_TOUCH                   0x02
#define OVER_TIME_COUNT             3
#define TSI_THRESHOLD               500
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/
void DemoProcessing( void );
/******************************************************************************
* Local variables
******************************************************************************/

uint32_t u32BaseLine;
uint32_t u32DeltaValue;
uint32_t u32Currentvalue;
uint8_t u8TSI_Status;
uint8_t u8TSI_OverTimeCount;
uint8_t u8DemoState = DemoAccelerometer;
uint8_t u8IrDaTestStatus = 0;
uint8_t u8LedControlMode = 0;
uint8_t u8CurrentDemoState;
uint8_t u8DemoModeUpdatedDelay = 0;
int16_t i16Temperature;
/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void RTC_Task(void);
void Pit0CallBack( void );
void TSI_GetBaseLine(void);
uint32_t TSI_Sample(uint8_t u8PadIndex, uint8_t u8Numbers);
void TSI_Detection( void );
void TSI_ReleaseCallback(void);
void TSI_TouchCallback(void);
/******************************************************************************
* Global functions
******************************************************************************/
extern void UART_InitPrint(void);

/*****************************************************************************/

/*****************************************************************************//*!
*
* @brief 
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
int main(void)
{
		RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;   
    PIT_ConfigType  sPITConfig0;    
    PIT_ConfigType  *pPIT_Config0   =&sPITConfig0; 
    
    /* Perform processor initialization */
	  sysinit();
    
		printf("*************** welcome to the MA8 Lab Demo! ***************\r\n\n");
		printf("  Slip up to switch to last demo modes : \r\n");
	  printf("  Slip down to switch to next demo modes : \r\n");
		printf("  1.DemoIrDa \r\n");
		printf("  2.DemoAcceleroMeter \r\n");
		printf("  3.DemoRTC \r\n");
		printf("***************************************************************\r\n\n");

    /* configure RTC to 1Hz interrupt frequency */
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/
    
    RTC_SetCallback(RTC_Task);
		RTC_Init(pRTCConfig);
    
    /* initialize LED by FTM driving */
    LED_DriveByFtm();
    

		/* initialize IIC and MMA8451 for accerometer test */
		AccelInit();
    
    /* initialize PIT for perodic timer - 10mS */
    pPIT_Config0->u32LoadValue      = 240000L;
    pPIT_Config0->bFreeze           = FALSE;    
    pPIT_Config0->bModuleDis        = FALSE;    /*!< enable PIT module */     
    pPIT_Config0->bInterruptEn      = TRUE;
    pPIT_Config0->bChainMode        = FALSE; 
    pPIT_Config0->bTimerEn          = TRUE;

    PIT_Init(PIT_CHANNEL0, pPIT_Config0);  
    PIT_SetCallback(PIT_CHANNEL0, Pit0CallBack);   
    
    TSI_GetBaseLine();

   	for(;;) {	
                 /* process the Lab demo */
                 DemoProcessing();
            }
    return 0;
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
static uint32_t u32CurrentTime = 0;
void RTC_Task(void)
{
    u32CurrentTime ++;
    if( u8LedControlMode == LED_Rtc )
    {
	    if( u32CurrentTime & 0x01 )
	    {
	    	SET_LED_RED(100);
	    }
	    else
	    {
	    	SET_LED_RED(0);
	    }
	    if( (u32CurrentTime & 0x08)  )
	    {
	    	SET_LED_GREEN(100);
	    }
	    else if( (u32CurrentTime&0x04))
	    {
	    	SET_LED_GREEN(0);
	    }
	}
}
/********************************************************************/

/*****************************************************************************//*!
*
* @brief 
*		
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void DemoProcessing( void )
{
	uint8_t bFreshPrintFlag;
	bFreshPrintFlag = FALSE;
	if( u8DemoState != u8CurrentDemoState )
	{
        if(u8CurrentDemoState == DemoIrDa)
        {
            UART_WaitTxComplete(UART0);
            /* initialize UART for printing */
            SIM->SOPT &= ~SIM_SOPT_RXDFE_MASK;
            SIM->SOPT &= ~SIM_SOPT_TXDME_MASK;
            UART_InitPrint();  
        }
        
        u8CurrentDemoState = 	u8DemoState;
		bFreshPrintFlag = TRUE;
        
        /* first to enter into test, turn off all LED */
        SET_LED_RED(0);
        SET_LED_GREEN(0);
	}
	switch( u8CurrentDemoState )
	{
		case DemoIrDa:
			{
				if( bFreshPrintFlag )
				{
					bFreshPrintFlag = FALSE;
					printf("\r\n");
					printf("Start to Demo IrDA function,touch pad to enter into next demo!\n");
					printf("\r\n");
                    UART_WaitTxComplete(UART0);
                    /* initialize Irda interface */
                    IrDA_Init();
                }
				u8LedControlMode = LED_Auto;
                u8IrDaTestStatus = IrDA_Test();
                if( u8IrDaTestStatus == 0 )
                {
                	/* test pass, green light blinking */
                 	LED_GreenControl(30,100);
					LED_RedControl(0,100);
				
               	}
               	else if( (u8IrDaTestStatus&0x01) == 0x01 )
               	{
               		/* test fail, don't receive any data */
                    //  printf("IrDa test fail,receive timeout, don't receive any data!\n");
               		LED_GreenControl(0,100);
					LED_RedControl(10,100);
					
				}
				else
				{
					/* receive data, but data is incorrect!*/
                    //  printf("IrDa test fail,data receieved is incorrect!\n");
					LED_GreenControl(0,100);
					LED_RedControl(80,100);
					
				}
		}
		break;
		case DemoAccelerometer:
			{
				if( bFreshPrintFlag )
				{
					bFreshPrintFlag = FALSE;
					printf("\r\n");
					printf("Start to Demo Accelerometer function,touch pad into enter into RTC demo!\n");
          			printf("\r\n");
            
				}
                u8LedControlMode = LED_Manual;
                AccelDemo();
		}
		break;
	     
		case DemoRTC:
			if( bFreshPrintFlag )
				{
					bFreshPrintFlag = FALSE;
					printf("\r\n");
					printf("Start to Demo RTC function,touch pad to enter into IrDA demo!\n");
					printf("\r\n");
           					
				}
                u8LedControlMode = LED_Rtc;
		break;
		case DemoIdle:
			if( bFreshPrintFlag )
				{
					bFreshPrintFlag = FALSE;
					printf("\r\n");
					printf("Start to idle demo,touch pad to enter into IrDA demo!\n");
					printf("\r\n");
                           
				}
            
                /* demo is in idle status, blink blue light */
                u8LedControlMode = LED_Auto;
                LED_RedControl(10,50);
		break;
		default:
			break;
	}
}

/*****************************************************************************//*!
*
* @brief callback routine of PIT0 driver which does what you want to do at 
*        every PIT0 period.
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void Pit0CallBack( void )
{
	if( u16TiltDelay )
	{
		u16TiltDelay--;
	}

	/* LED control */
	if( u8LedControlMode == LED_Auto )
	{
		LED_CallBack();
	}

	/* determine demo updated speed */
	if( u8DemoModeUpdatedDelay )
	{
		u8DemoModeUpdatedDelay --;
	}
    
    /* run TSS task to check slider */
    TSI_Detection();
    
}

/*****************************************************************************//*!
*
* @brief smaple touch pad and return back sample value.
*        
* @param[in]  u8PadIndex - the index of sampling touch pad 
* @param[in]  u8Numbers - the number of sampling touch pad 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
uint32_t TSI_Sample(uint8_t u8PadIndex, uint8_t u8Numbers)
{
    uint8_t i;
    uint32_t u32SampleValue;
    u32SampleValue = 0;
    
    if(u8PadIndex == 0)
    {
        for(i=0;i<u8Numbers;i++)
        {
            GPIOA->PDDR |= (1 << 13);    
            GPIOA->PCOR |= (1 << 13);                 /*!< output 0, discharger first */
            systick_init();                           /*!< start the timer */
            GPIOA->PIDR &= ~(1 << 13);
            GPIOA->PDDR &= ~(1 << 13);                /*!< set as input */
            while(!(GPIOA->PDIR & (1 << 13)));        /*!< wait GPIO charged to 1 */
            systick_disable();                    /*!< stop the PIT timer */
            u32SampleValue += (0x00FFFFFF - SysTick->VAL);
        }
    }
    else
    {
        for(i=0;i<u8Numbers;i++)
        {
            GPIOA->PDDR |= (1 << 16);    
            GPIOA->PCOR |= (1 << 16);                 /*!< output 0, discharger first */
            systick_init();                           /*!< start the timer */
            GPIOA->PIDR &= ~(1 << 16);
            GPIOA->PDDR &= ~(1 << 16);                /*!< set as input */
            while(!(GPIOA->PDIR & (1 << 16)));        /*!< wait GPIO charged to 1 */
            systick_disable();                    /*!< stop the PIT timer */
            u32SampleValue += (0x00FFFFFF - SysTick->VAL);
        }
    }
    
    return u32SampleValue;
}

/*****************************************************************************//*!
*
* @brief detect the touch pad status, touch or release.
*        
* @param none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void TSI_Detection( void )
{
    u32Currentvalue = 0;
    u32Currentvalue += TSI_Sample(0,8);
    u32Currentvalue += TSI_Sample(1,8);
    
    if( u32Currentvalue > u32BaseLine )
    {
        u32DeltaValue = u32Currentvalue - u32BaseLine;
    }
    else
    {
        u32DeltaValue = 0;
    }
    
    if(u32DeltaValue > TSI_THRESHOLD)
    {
        if(u8TSI_Status == TSI_RELEASE)
        {
           u8TSI_OverTimeCount ++;
           if(u8TSI_OverTimeCount > OVER_TIME_COUNT)
           {
               u8TSI_OverTimeCount = 0;
               u8TSI_Status = TSI_TOUCH;
               TSI_TouchCallback();
           }
        }
    }
    else
    {
        if(u8TSI_Status == TSI_TOUCH)
        {
           u8TSI_OverTimeCount ++;
           if(u8TSI_OverTimeCount > OVER_TIME_COUNT)
           {
               u8TSI_OverTimeCount = 0;
               u8TSI_Status = TSI_RELEASE;
               TSI_ReleaseCallback();
           }
        }
    }
}
/*****************************************************************************//*!
*
* @brief get the baseline of touch pad.
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void TSI_GetBaseLine(void)
{
    uint8_t i;
    u32BaseLine = 0;
    for(i=0;i<16;i++)
    {
        u32BaseLine += TSI_Sample(0,8);
        u32BaseLine += TSI_Sample(1,8);
    }
    u32BaseLine = u32BaseLine/16;
    u8TSI_Status = TSI_RELEASE;
}
/*****************************************************************************//*!
*
* @brief call back function of touch behavior. 
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void TSI_TouchCallback(void)
{
    u8DemoState ++;
    if(u8DemoState > DemoEnd)
    {
        u8DemoState = DemoIrDa;
    }
}
/*****************************************************************************//*!
*
* @brief call back function of release behavior. 
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void TSI_ReleaseCallback(void)
{
    //LED_RedControl(0,0);
}