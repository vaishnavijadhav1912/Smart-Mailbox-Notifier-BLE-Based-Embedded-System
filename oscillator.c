/*
 * oscillator.c
 *
 *  Created on: Jan 30, 2025
 *      Author: Vaishnavi Jadhav, Anuja Joshi
 */


#include <src/oscillator.h>
#include "app.h"
#include "em_cmu.h"



#define ULFRCO_FREQUENCY  1000    // ULFRCO runs at 1 kHz
#define LFXO_FREQUENCY    32768   // LFXO runs at 32.768 kHz
#define PRESCALER         4       // Prescaler value for LFXO in EM0, EM1, EM2


uint32_t configure_letimer_clock(void){

     uint32_t  clk_freq;

#if (LOWEST_ENERGY_MODE == 3)

    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);// Use ULFRCO (1 kHz) for EM3
    clk_freq = ULFRCO_FREQUENCY;
#else

    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); // Use LFXO (32.768 kHz) for EM0, EM1, and EM2 with a prescaler of 4


    CMU_ClockDivSet(cmuClock_LETIMER0, cmuClkDiv_4);  // Set prescaler to 4

    clk_freq = LFXO_FREQUENCY / PRESCALER;    // Adjust clock frequency due to prescaler
#endif

    return clk_freq;


}



