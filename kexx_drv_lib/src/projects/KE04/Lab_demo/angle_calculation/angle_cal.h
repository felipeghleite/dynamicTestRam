/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file Angle_cal.h
*
* @author 
*
* @version 0.0.1
*
* @date Aug 16, 2013
*
* @brief 		
*******************************************************************************
*
* 
******************************************************************************/
#ifndef _ANGLE_CAL_H_
#define _ANGLE_CAL_H_

/******************************************************************************
* Local variables
******************************************************************************/
extern signed int i16Acc_x;
extern signed int i16Acc_y;
extern signed int i16Acc_z;

extern unsigned int u16Mag_xy;
extern unsigned int u16Mag_xz;
extern unsigned int u16Mag_yz;

extern signed  int i16Angle_xy;
extern signed  int i16Angle_xz;
extern signed  int i16Angle_yz;

extern unsigned char u16FallInput;
/******************************************************************************
* Local function
******************************************************************************/
extern void Angle_Calculation(void);
extern void FallDetection(void);

#endif

