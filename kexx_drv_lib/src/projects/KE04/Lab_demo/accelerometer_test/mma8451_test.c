/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file pmc.c
*
* @author 
*
* @version 0.0.1
*
* @date Jul 17, 2011
*
* @brief 
*
*******************************************************************************
*
******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
	
#include "common.h"
#include "mma8451_test.h"
#include "hal_dev_mma8451.h"
#include "angle_cal.h"
#include "led.h"
/******************************************************************************
* Constants
******************************************************************************/
/******************************************************************************
* Macros
******************************************************************************/
	
#define T_DELAY_LED  10

/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/
uint32_t u16Result_x, u16Result_y, u16Result_z;
uint16_t u16Accel_x,u16Accel_y,u16Accel_z;
uint16_t u16TiltDelay = 0;
/******************************************************************************
* Global functions
******************************************************************************/
void AccelInit(void);
void AccelRead(void);
void AccelDemo(void);

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: AccelInit
*
* @brief initialize accelerometer
*		 
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void AccelInit(void)
{
    unsigned char tmp;
    printf("****Accelerometor init ****\n");
   //Configure MMA8451 sensor //MMA8451Q
    HAl_DevMma8451Init();      //Initialize I2C modules
    tmp = HAL_DevMma8451ReadReg(0x2a);
    HAL_DevMma8451WriteReg(0x2a,tmp|0x01);
}
/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: AccelRead
*
* @brief    read accelerometer sensor
*		 
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void AccelRead(void)
{
    if((HAL_DevMma8451ReadReg(0x00)&0xf) != 0)
    {

        u16Accel_x   = HAL_DevMma8451ReadReg(0x01)<<8;
        u16Accel_x  |= HAL_DevMma8451ReadReg(0x02);
        u16Accel_x >>= 2;

        u16Accel_y   = HAL_DevMma8451ReadReg(0x03)<<8;
        u16Accel_y  |= HAL_DevMma8451ReadReg(0x04);
        u16Accel_y >>= 2;

        u16Accel_z   = HAL_DevMma8451ReadReg(0x05)<<8;
        u16Accel_z  |= HAL_DevMma8451ReadReg(0x06);
        u16Accel_z >>= 2;

        u16Result_x   = HAL_DevMma8451ReadReg(0x01)<<8;
        u16Result_x  |= HAL_DevMma8451ReadReg(0x02);
        u16Result_x >>= 8;

        u16Result_y   = HAL_DevMma8451ReadReg(0x03)<<8;
        u16Result_y  |= HAL_DevMma8451ReadReg(0x04);
        u16Result_y >>= 8;

        u16Result_z   = HAL_DevMma8451ReadReg(0x05)<<8;
        u16Result_z  |= HAL_DevMma8451ReadReg(0x06);
        u16Result_z >>= 8;
        
        
        Angle_Calculation(); //-900  to  900            
        FallDetection();
    }

}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: AccelDemo
*
* @brief    
*		 
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void AccelDemo(void)
{
	static int note,notez;
	static int note1,notez1;
	static int note2,notez2;

	AccelRead(); 
	note  = i16Angle_yz/100;  //9-9
    note1 = i16Angle_xy/100;  //9-9
    note2 = i16Angle_xz/100;  //9-9
    
	if (note != notez && u16Mag_yz > 20 && !u16TiltDelay)   //yz
	{
		if (note <-9) note=-9;
		if (note >9)  note =9;
		if (notez < note) notez++;
		if (notez > note) notez--;
		SET_LED_GREEN(notez*55+500);
		u16TiltDelay = T_DELAY_LED;
	}
    else{
       SET_LED_GREEN(0);
    } 
	 
	if (note1 != notez1 && u16Mag_xy > 20 && !u16TiltDelay)
	{
		if (note1 <-9) note1=-9;
		if (note1 >9)  note1 =9;
		if (notez1 < note1) notez1++;
		if (notez1 > note1) notez1--;
		SET_LED_RED(notez1*55+500);
		u16TiltDelay = T_DELAY_LED;
	} 
    else
    {
        SET_LED_RED(0);
    }


	if (note2 != notez2 && u16Mag_xz > 20 && !u16TiltDelay)
	{
		if (note2 <-9) note2=-9;
		if (note2 >9)  note2 =9;
		if (notez2 < note2) notez2++;
		if (notez2 > note2) notez2--;
		SET_LED_RED(notez2*55+500);
		u16TiltDelay = T_DELAY_LED;
	}
    else{
        SET_LED_RED(0);
    }
	
}

