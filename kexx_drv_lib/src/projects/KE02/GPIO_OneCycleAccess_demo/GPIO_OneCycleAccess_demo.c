
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
*******************************************************************************
*
* @file FastGPIO_demo.c
*
* @author freescale
*
* @version 0.0.1
*
* @date Jul-15-2011
*
* @brief 
*   The GPIO registers are also aliased to the FastGPIO interface on the 
*   Cortx-M0+ from address 0xF800_0000. Access via the FastGPIO interface
*   occur in parallel with any instruction fetches and will therefore complete
*   in a single cycle.
*   This example provides a demonstration of Fast GPIO access. Toggle GPIO PTB2
*   via Fast GPIO register, the toggle frequency can be as fast as 10MHz, while
*   the core clock (not the bus clock) is 20MHz, indicating the Fast GPIO access
*   in a single cycle.
*   NOTE that the optimization option must be "level high" in C compile configuration.
*******************************************************************************/

#include "common.h"
#include "rtc.h"
#include "gpio.h"
#include "sim.h"
#include "sysinit.h"
#include "start.h"

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

/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void RTC_Task(void);


/******************************************************************************
* Global functions
******************************************************************************/


/********************************************************************/
int main (void)
{  
    /* Perform processor initialization */
    sysinit();
    cpu_identify();

    printf("\nRunning the GPIO_OneCycle_demo project.\n");
    printf("\nBy default Core Clock is 20MHz");
    printf("\nBy one cycle GPIO access, PTB2 output frequency is expected to be 10MHz");

    /* Enable Bus Clock Out on PTH2, to check Bus Clock */
    SIM_EnableClockOutput();

    /* config GPIO PTB2 as a general-purpose output */
    GPIO_Init(GPIOA, GPIO_PTB2_MASK, GPIO_PinOutput);

    /* set PTB2 output 0 */
    GPIO_PinClear(GPIO_PTB2);
    
    for(;;)
    {
        /* toggle PTB2 by FastGPIO, the frequency is CoreClock/2 */
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);         
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK); 
        FGPIO_Toggle(FGPIOA, GPIO_PTB2_MASK);
    }
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: RTC_Task
*
* @brief callback routine of RTC driver which does what you want to do at 
*        every RTC period.
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void RTC_Task(void)
{
    /* toggle LED1 */
    LED0_Toggle();
}
/********************************************************************/
