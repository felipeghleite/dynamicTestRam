/******************************************************************************
* @file SPI.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Apr. 16, 2013
*
* @brief Calculates the square root of the value
******************************************************************************/
#include "common.h"
#include "sqrt16.h"

/******************************************************************************
* Local variables
******************************************************************************/

/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: sqrt_16
   *
   * @brief Calculates the square root of the value
   *        
   * @param
   *			base     -- SPI index  
   *			master   -- if master, set 1; else set 0     
   *         
   *
   * @return (unsigend long)square root
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
unsigned int sqrt_16(unsigned int value)
{
	unsigned char num_iter=0;
	unsigned long result = 1000;
	unsigned long last_result = 1000;
	do
	{
		last_result = result;
		result = (result + value/result)/2;
	}
	while (num_iter++<7 && last_result!= result);
	return (unsigned int)result;
}


