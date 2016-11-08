/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file Angle_cal.c
*
* @author B37811
*
* @version 0.0.1
*
* @date Aug 16, 2013
*
* @brief 		accelerometer apps 
*			    Process information from accelerometer
*			    and calculates tilt angle
*******************************************************************************
*
* 
******************************************************************************/
#include "common.h"
#include "sqrt16.h"
#include "median.h"
#include "asin.h"


/******************************************************************************
* Local Macro
******************************************************************************/
/*Fall detection*/
#define FALL_LIMIT  45//50

/******************************************************************************
* Local function
******************************************************************************/
void Angle_Calculation(void);
void FallDetection(void);

/******************************************************************************
* Local variables
******************************************************************************/
extern uint16_t u16Result_x, u16Result_y, u16Result_z;

/*Acceleration	RAM */
signed int i16Acc_x;
signed int i16Acc_y;
signed int i16Acc_z;

unsigned int u16Mag_xy;
unsigned int u16Mag_xz;
unsigned int u16Mag_yz;

signed	int i16Angle_xy;
signed	int i16Angle_xz;
signed	int i16Angle_yz;


struct TIPO_MEDIANA sMediannaArry[3];
//unsigned char ADC_buffer[3];
unsigned int u16Cat;
unsigned int u16Offset;
unsigned char u16FallInput;
unsigned int u16TiltFall;

/******************************************************************************
* Local function prototypes
******************************************************************************/
/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: Angle_Calculation

	The angle caluclation 

	mag = sqrt(X^2 + Y^2)

	sin(alpha) = CO/hip;

	alpha = asin(CO/hip);

	where CO  u16Cateto opuesto
	hip = hipotenusa= magnitud
	the other way can be

	// to check what is better
	atan = y/x

	where x/y can take values from -inf to + inf
	 and for´
     
   Note: Direct calculos based on  angle = asin(x/g) 
   is possible but is only valid when plane XY is the in the same line of G
   
   no accept small tilt of the board.
   *
   * @brief calculate the angle 
   *        
   * @param none
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void Angle_Calculation(void)
{
	signed int nv, x2, y2, z2;
	nv = (signed char)(u16Result_x);
	i16Acc_x = median(nv, &sMediannaArry[0]);


	nv = (signed char)(u16Result_y);
	i16Acc_y = median(nv, &sMediannaArry[1]);

	nv = (signed char)(u16Result_z);  
	i16Acc_z = median(nv, &sMediannaArry[2]);

	x2 = i16Acc_x*i16Acc_x;
	y2 = i16Acc_y*i16Acc_y;

	u16Mag_xy   = sqrt_16(x2 + y2);

	if (i16Acc_y<0) u16Cat = -i16Acc_y; else u16Cat = i16Acc_y;

	u16Offset = (unsigned int)(u16Cat<<7)/(unsigned int)u16Mag_xy;
	if (u16Offset>127) u16Offset = 127;    
	i16Angle_xy = asin[u16Offset];

	if (i16Acc_y>0)  i16Angle_xy = -i16Angle_xy;
	 
	z2 = i16Acc_z*i16Acc_z;   
	u16Mag_xz    = sqrt_16(x2 + z2);
	if (i16Acc_x<0) u16Cat = -i16Acc_x; else u16Cat = i16Acc_x;
	u16Offset = (unsigned int)(u16Cat<<7)/(unsigned int)u16Mag_xz;
	if (u16Offset>127) u16Offset = 127;    
	i16Angle_xz = asin[u16Offset];

	if (i16Acc_x>0)  i16Angle_xz = -i16Angle_xz;


	u16Mag_yz  = sqrt_16(y2 + z2);
	if (i16Acc_y<0) u16Cat = -i16Acc_y; else u16Cat = i16Acc_y;
	u16Offset = (unsigned int)(u16Cat<<7)/(unsigned int)u16Mag_yz;
	if (u16Offset>127) u16Offset = 127;    
	i16Angle_yz = asin[u16Offset];
	if (i16Acc_y>0)  i16Angle_yz = -i16Angle_yz;
    
}

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: ADC_GetResult
   *
   * @brief start a conversion and get conversion result
   *        
   * @param  none 
   *
   * @return ADC register value
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void FallDetection(void)
{
   if (u16Mag_xy<FALL_LIMIT && u16Mag_xz < FALL_LIMIT && u16Mag_yz < FALL_LIMIT  && !u16TiltFall) 
    u16FallInput=1;
   else 
   {
     u16FallInput = 0;
     u16TiltFall = 10;
    }
   
}
