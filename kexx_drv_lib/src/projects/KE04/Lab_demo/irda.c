/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file ir.c
*
* @author B37531
*
* @version 0.0.1
*
* @date Aug 23, 2011
*
* @brief IRDA deriver,to implement IR communicate with ACMP 
*
******************************************************************************/
#include "common.h"
#include "Uart.h"
#include "irda.h"

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
static unsigned char u8IR_TxBuff[16];
static unsigned char u8IR_RxBuff[16];
/******************************************************************************
* Local functions
******************************************************************************/
void FTM0CH0_Init(void);
void ACMP_InitIrDA (void);
void IrDA_Init(void);
/******************************************************************************
* Global functions
******************************************************************************/
uint8_t IrDA_Test( void );
/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: FTM0CH0_Init
   *
   * @brief initialize FTM0CH0, EPWM output mode,frequency is 38khz
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FTM0CH0_Init( void )
{

    SIM->SCGC |= SIM_SCGC_FTM0_MASK;
	// Init FTM0 to PWM output,frequency is 38khz
	FTM0->MOD= FTM0_MOD_VALUE;
	FTM0->CONTROLS[0].CnSC = 0x28;
	FTM0->CONTROLS[0].CnV = FTM0_C0V_VALUE;
	FTM0->SC = 0x08; // bus clock divide by 2
}
/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: ACMP_Init
   *
   * @brief initialize ACMP
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void ACMP_InitIrDA ( void )
{
	SIM->SCGC |= SIM_SCGC_ACMP0_MASK;
	ACMP0->C1 |= ACMP_C1_DACEN_MASK | 
	           ACMP_C1_DACREF_MASK|
	           ACMP_C1_DACVAL(21);    	// enable DAC

	ACMP0->C0 |= ACMP_C0_ACPSEL(0x03)|
				ACMP_C0_ACNSEL(0x01);

	ACMP0->C2 |= ACMP_C2_ACIPE(0x02);  	// enable ACMP1 connect to PIN

	ACMP0->CS |= ACMP_CS_ACE_MASK;		// enable ACMP
             
}
/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: IrDA_Init
   *
   * @brief initialize IR, enable ACMP ,SCI0 , 38khz as carrier
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void IrDA_Init( void )
{
	unsigned char dummy;
	unsigned char i;
	UART_ConfigType sConfig = {0};

	/* reinitialize the UART for terminal due to bus clock changed to 20MHz*/
    sConfig.u32SysClkHz = 48000000L;
    sConfig.u32Baudrate  = 4800;
    UART_Init (TERM_PORT, &sConfig);    
	
	// initialize FTM0CH1 as 38k PWM output
	FTM0CH0_Init();
	// enable ACMP
	ACMP_InitIrDA();

	SIM->SOPT |= SIM_SOPT_RXDFE(1);

	UART0->S2 &= ~UART_S2_RXINV_MASK;

	SIM->SOPT |= SIM_SOPT_TXDME_MASK;

	
	for(i=0;i<16;i++)
	{
		u8IR_TxBuff[i] = i;
	}

}
/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: IrDA_test
   *
   * @brief test IRDA communication
   *             send a byte,then check received data whether correct
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: pass/fail
   *****************************************************************************/
uint8_t IrDA_Test( void )
{
	uint32_t uiCount;
	volatile uint8_t dummy;
	uint8_t  ucResultFlag;
	uint8_t i;
    ucResultFlag = 0;   
	for(i=0;i<16;i++)
	{
		u8IR_TxBuff[i] += i-1;
       	u8IR_RxBuff[i] = 0;
	}
	// if have invalid data in receiving buff,clear flag and discard it 
    dummy = UART0->S1;
	dummy = UART0->D;
	// ready for transmit a frame data and receive it from echo board and verify received data 
	for(i = 0;i<16;i++)
	{
		 UART_PutChar(UART0,u8IR_TxBuff[i]);
       	 uiCount = 0xfffff;
		 while( (( UART0->S1 & UART_S1_RDRF_MASK) != UART_S1_RDRF_MASK)&&
				 ( --uiCount) );
		 
		 if(uiCount == 0)
		 {
			// printf("IR test failed!\n");
			 ucResultFlag |= 0x01;
			 break;
		 }
		 else
		 {
			 dummy = UART0->S1;
			 u8IR_RxBuff[i] = UART0->D;
		 }
	}
	
	if( memcmp(&u8IR_TxBuff[0],&u8IR_RxBuff[0],16) )
	{
		ucResultFlag |= 0x02;
	}
	else
	{
		ucResultFlag = 0;
	}
	
	return ucResultFlag;

}	


