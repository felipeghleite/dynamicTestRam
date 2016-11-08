/*
 * isr.h
 *
 *  Created on: 05/08/2016
 *      Author: Felipe
 */

#ifndef SOURCES_ISR_H_
#define SOURCES_ISR_H_

#undef VECTOR_028
#define VECTOR_028 UART0_Isr

#undef VECTOR_029
#define VECTOR_029 UART1_Isr

#undef VECTOR_030
#define VECTOR_030 UART2_Isr

#undef VECTOR_036
#define VECTOR_036 RTC_IRQHandler

extern void UART0_Isr(void);
extern void UART1_Isr(void);
extern void UART2_Isr(void);
extern void RTC_IRQHandler(void);



#endif /* SOURCES_ISR_H_ */
