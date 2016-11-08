/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file     Led.h
*
* @author   Freescale
*
* @version  0.0.1
*
* @date     Jun 25, 2013
*
* @brief    provide driver for LED APIs.
*
******************************************************************************/



#ifndef _LED_H_
#define _LED_H_

/******************************************************************************
* Includes
******************************************************************************/

/******************************************************************************
* Constants
******************************************************************************/

/******************************************************************************
* Macros
******************************************************************************/



typedef struct{

unsigned long OnTime;
unsigned long OffTime;
}LED_CONTROL;


#define FTM_Cn_MODE  (FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK)
#define FTM_MODULE    1000
#define FTM_INIT_VAL    25
#define SET_LED_GREEN(x)   FTM2->CONTROLS[2].CnV = x
#define SET_LED_RED(x)     FTM2->CONTROLS[3].CnV = x
#define SET_LED_BLUE(x)    


/******************************************************************************
* Types
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/
extern void LED_DriveByFtm( void );
extern void LED_CallBack ( void );
extern void LED_BlueControl(unsigned long OnTime,unsigned long OffTime);
extern void LED_GreenControl(unsigned long OnTime,unsigned long OffTime);
extern void LED_RedControl(unsigned long OnTime,unsigned long OffTime);

#endif /* _MA64_FREEDOM_H_ */
