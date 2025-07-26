//
////Author name: Vaishnavi Jadhav
//
//

#include <src/oscillator.h>
#include "em_cmu.h"
#include "em_letimer.h"
#include "em_core.h"
#include "gpio.h"
#include "app.h"
#include "letimer.h"
#include "irq.h"
#include "scheduler.h"
#include "stdio.h"
#include "src/log.h"

#define LED_PERIOD_MS    3000    // LED blink period (3 sec)
#define MS_TO_SEC        1000    // 1 second = 1000 milliseconds
#define MIN_DELAY_US 1           // Min valid delay in microseconds
#define MAX_DELAY_US 1000000     // Max valid delay in microseconds
#define MICROSECONDS_IN_ONE_SEC 1000000
#define LETIMER_MAX_COUNT 65536


uint32_t clk_freq; //global variable


 /**
  *  Initializes LETIMER0 for periodic process using the Underflow (UF) interrupt.
  *
  * - Enables the LETIMER0 clock.
  * - Configures LETIMER0 with the selected clock source.
  * - Sets up periodic mode using COMP0 as the top value.
  * - Enables the underflow interrupt (UF) and configures NVIC.
  * - Starts LETIMER0.
  */

void letimer_init(void) {
    CMU_ClockEnable(cmuClock_LETIMER0, true);


    clk_freq = configure_letimer_clock();



    LETIMER_Init_TypeDef letimerInit = LETIMER_INIT_DEFAULT;
    letimerInit.enable = false;
    letimerInit.comp0Top = true;

    LETIMER_Init(LETIMER0, &letimerInit);


    uint32_t period_cnt = (clk_freq * LED_PERIOD_MS) / MS_TO_SEC;
    LETIMER_CompareSet(LETIMER0, 0, period_cnt);


    LETIMER_IntEnable(LETIMER0, LETIMER_IF_UF);
    NVIC_EnableIRQ(LETIMER0_IRQn);

    LETIMER_Enable(LETIMER0, true);
}



/**
 *  Generates delay for a specified time in microseconds using LETIMER0.
 *
 *
 */

void timerWaitUs(uint32_t us_wait) {
    if (us_wait < MIN_DELAY_US  || us_wait >  MAX_DELAY_US) {

        LOG_ERROR("\r\n Invalid delay \r\n");
           // return;
    }

    uint32_t ticksToWait = (us_wait * clk_freq) / MICROSECONDS_IN_ONE_SEC;

    uint32_t startTicks = LETIMER_CounterGet(LETIMER0);

    while ((startTicks - LETIMER_CounterGet(LETIMER0)) < ticksToWait);
}



/*
 * Sets up a delay using LETIMER0 COMP1 interrupt.
 * Computes the required ticks, handles wrap-around, and enables the COMP1 interrupt.
 */
void timerWaitUs_irq(uint32_t us_wait) {
    if (us_wait < MIN_DELAY_US || us_wait > MAX_DELAY_US) {
        LOG_ERROR("\r\n Invalid delay \r\n");
    }

    uint32_t ticksToWait = (us_wait * clk_freq) / MICROSECONDS_IN_ONE_SEC;
    uint32_t currentTicks = LETIMER_CounterGet(LETIMER0);


    uint32_t comp1_value = (currentTicks >= ticksToWait) ? (currentTicks - ticksToWait) : (currentTicks - ticksToWait + LETIMER_MAX_COUNT);


    LETIMER_CompareSet(LETIMER0, 1, comp1_value);


    LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP1);
}

