
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
#include "angle_cal.h"
#include "led.h"
#include "uart.h"
#include "key.h"
#include "math.h"
#include "sysinit.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define ASLIDER_DELAY_TIMER		    50
//#define TSI_RELEASE                 0x01
//#define TSI_TOUCH                   0x02
#define OVER_TIME_COUNT             3
//#define TSI_THRESHOLD               500
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
DemoStateType DemoState = DemoRTC;
uint8_t u8LedControlMode = 0;
uint8_t u8CurrentDemoState;
uint8_t u8DemoModeUpdatedDelay = 0;
int16_t i16Temperature;

uint8_t u8OneSecondFlag = 0;
signed int pre_i16Angle_xy = 0;

uint8_t u8ShakeCnt = 0;

uint8_t u8BoardIdleCnt = 0;

/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void RTC_Task(void);
void Pit0CallBack( void );
/******************************************************************************
* Global functions
******************************************************************************/
extern void UART_InitPrint(void);


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
//static uint32_t u32CurrentTime = 0;

void RTC_Task(void)
{
    DisableInterrupts;

    u8OneSecondFlag = 1;

    if (DemoState == DemoRTC)
    {
        //toggle Green Led to indicate Demo mode is DemoRTC now.
        LED1_Toggle();
        LED_Set_Light(LED_Red, 0);
        LED_Set_Light(LED_Blue, 0);

        if ((i16Angle_yz/100 != 0) && (i16Angle_xz/100 != 0))
        {
            DemoState = DemoMMA8451;
            LED1_Off();
            LED_Set_Light(LED_Red, 0);
            LED_Set_Light(LED_Blue, 0);
            printf("\r\nNow is MMA8451 Demo, the lightness of blue LED is changed according to the board position. \r\n");
        }
    }
    else
    {
        //In other Demo mode
    	if ((i16Angle_yz/100 == 0) && (i16Angle_xz/100 == 0))
    	{
            //MMA8451 detect if FRDM board is placed  (no key input && lay flat for 5s)
            u8BoardIdleCnt ++;

            if (u8BoardIdleCnt >=10)
            {
                DemoState = DemoRTC;
                LED_Set_Light(LED_Red, 0);
                LED_Set_Light(LED_Blue, 0);
                u8BoardIdleCnt  = 0;
                printf("\r\nNow is RTC Demo, the Green LED is toggled every 1 second. \r\n");

            }
        }
        else
        {
        	u8BoardIdleCnt = 0;
        }
    }
    
	EnableInterrupts;

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
    //Read mma8451 every 100ms
    //LED0_Toggle();
	AccelRead();    
}

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

    uint8_t u8IrDaTestStatus = 0;

    /* Perform processor initialization */
	sysinit();

    DisableInterrupts;
    
	printf("*************** Welcome to the KE06 Lab Demo! ***************\r\n\n");
	printf("  The default is RTC Demo, the Green LED is toggled every 1 second.\r\n");
	printf("  Move the FRDM board to switch to MMA8451 Demo.\r\n");
    printf("  Push key (SW2 or SW3) to switch to KBI/FTM Demo.\r\n");
    printf("  Lay flat and no key input for 10 seconds to switch to RTC Demo.\r\n");
	printf("  There're 3 Demo: 1.Demo RTC. 2.Demo MMA8451. 3.Demo KBI/FTM \r\n");
	printf("***************************************************************\r\n\n");

    /* configure RTC to 1Hz interrupt frequency */
    pRTCConfig->u16ModuloValue = 9;                                
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/

    RTC_SetCallback(RTC_Task);
	RTC_Init(pRTCConfig);

    /* initialize PIT - 100ms at 20MHz bus clock */
    pPIT_Config0->u32LoadValue      = (2000000L-1);
    pPIT_Config0->bFreeze           = FALSE;    
    pPIT_Config0->bModuleDis        = FALSE;    /*!< enable PIT module */     
    pPIT_Config0->bInterruptEn      = TRUE;
    pPIT_Config0->bChainMode        = FALSE; 
    pPIT_Config0->bTimerEn          = TRUE;

    PIT_Init(PIT_CHANNEL0, pPIT_Config0);  
    PIT_SetCallback(PIT_CHANNEL0, Pit0CallBack);   
    

    // Init RED LED associated with RTC Demo.
    LED1_Init();
    LED1_Off();
    
    //Init Blue/Red LED associated with MMA8451 Demo.
    LED_DriveByFtm();
    LED_Set_Light(LED_Blue, 0);
    LED_Set_Light(LED_Red, 0);
    //Init KBI Module for 2 keys
    Key_Init();
    
    //LED_Set_Light(20);

    
    //LED1_Init();
    
	/* initialize IIC and MMA8451 for accerometer test */
	AccelInit();
	
	IrDA_Init();

	printf("\r\nNow is RTC Demo, the Green LED is toggled every 1 second. \r\n");


	EnableInterrupts;
    
    while(1)
    {
        if (DemoState == DemoMMA8451)
        {
            AccelDemo();
        }
        else
        {

        }
    }   
}

/********************************************************************/
#if 0
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
	if( DemoState != u8CurrentDemoState )
	{
        if(u8CurrentDemoState == DemoIrDa)
        {
            UART_WaitTxComplete(UART0);
            /* initialize UART for printing */
            SIM->SOPT0 &= ~SIM_SOPT0_RXDFE_MASK;
            SIM->SOPT0 &= ~SIM_SOPT0_TXDME_MASK;
            UART_InitPrint();  
        }
        
        u8CurrentDemoState = 	DemoState;
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
		case DemoMMA8451:
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
		default:
			break;
	}
}
#endif


