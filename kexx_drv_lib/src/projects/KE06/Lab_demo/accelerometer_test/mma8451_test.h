/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file rtc.h
*
* @author a13984
*
* @version 0.0.1
*
* @date Sep 5, 2011
*
* @brief appliu16Cation entry point which performs appliu16Cation specific tasks. 
*
*******************************************************************************
*
******************************************************************************/



#ifndef MMA8451_TEST_H_
#define MMA8451_TEST_H_

/******************************************************************************
* Includes
******************************************************************************/
#include "common.h" /* include peripheral declarations */
/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* globe function
******************************************************************************/
extern void AccelInit(void);
extern void AccelRead(void);
extern void AccelDemo(void);

typedef enum
{
    MMA8451_STATUS = 0x00,
    MMA8451_OUT_X_MSB = 0x01,
    MMA8451_OUT_X_LSB = 0x02,
    MMA8451_OUT_Y_MSB = 0x03,
    MMA8451_OUT_Y_LSB = 0x04,
    MMA8451_OUT_Z_MSB = 0x05,
    MMA8451_OUT_Z_LSB = 0x06,
    MMA8451_F_SETUP = 0x09,
    MMA8451_TRIG_CFG = 0x0a,
    MMA8451_SYSMOD = 0x0b,
    MMA8451_INT_SOURCE = 0x0c,
    MMA8451_WHO_AM_I = 0x0d,
    MMA8451_WHO_AM_I_Device_ID = 0x1a,
    MMA8451_XYZ_DATA_CFG = 0x0e,
    MMA8451_CTRL_REG1 = 0x2a,
    MMA8451_CTRL_REG2 = 0x2b,
    MMA8451_CTRL_REG3 = 0x2c,
    MMA8451_CTRL_REG4 = 0x2d,
    MMA8451_CTRL_REG5 = 0x2e
} mma8451_RegType;
/******************************************************************************
* Variable
******************************************************************************/
extern uint16_t u16TiltDelay;
//extern unsigned long ti_task;
#endif
