/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1


/* Example */
/*
#undef  VECTOR_036
#define VECTOR_036 RTC_Isr

// ISR(s) are defined in your project directory.
extern void RTC_Isr(void);
*/

#undef  VECTOR_036
#define VECTOR_036 RTC_Isr

#undef  VECTOR_038
#define VECTOR_038 PIT_Ch0Isr


#undef  VECTOR_035
#define VECTOR_035      FTM2_Isr          /*!< Vector 35 points to FTM2 interrupt service routine */

#undef  VECTOR_040
#define VECTOR_040 KBI0_Isr
#undef  VECTOR_041
#define VECTOR_041 KBI1_Isr



extern void KBI0_Isr(void);
extern void KBI1_Isr(void);
extern void FTM2_Isr(void);
extern void RTC_Isr(void);
extern void PIT_Ch0Isr( void );
//extern void TSS_HWTimerIsr(void);

#endif  //__ISR_H

/* End of "isr.h" */
