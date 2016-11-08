/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013-2014 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file Median.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Apr. 16, 2013
*
* @brief  Calculates the median of 8 integer samples 
*
******************************************************************************/
#include "common.h"
#include "median.h"

/******************************************************************************
* Local variables
******************************************************************************/


/******************************************************************************
* Local functions
*****************************************************************************/


/******************************************************************************
* Local function prototypes
******************************************************************************/

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: median
   *
   * @brief implement Calculates the median of 8 integer samples
   *        
   * @param
   *         
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
int median(int new_value, struct TIPO_MEDIANA *medicion)
{
	int i16BufferOrd[NUM_MUESTRAS_MEDIANA];
  	int i;
  	int temp1;
  	int bandera;
  
  	medicion->ap_muestras++;
  	medicion->ap_muestras &= 0x7;  //de 0 a 7  
  	medicion->buffer_muestras[medicion->ap_muestras]=new_value;
	/*copia el buffer en temporal  */
  	for(i=0;i<8;i++) i16BufferOrd[i]=medicion->buffer_muestras[i];  	
  	/*ordena el buffer temporal    */
  	do
  	{
   		bandera=0; 
   		for (i=0;i<7;i++)  
   		{
    		if (i16BufferOrd[i] > i16BufferOrd[i+1])
     		{
      			temp1 = i16BufferOrd[i];
      			i16BufferOrd[i] = i16BufferOrd[i+1];
      			i16BufferOrd[i+1] = temp1;
      			bandera=1;  	
     		}	
    	}
  	}
  	
  	while (bandera);   
    temp1= i16BufferOrd[3];
  	return temp1;
 }
 