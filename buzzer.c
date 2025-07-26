/*
 * buzzer.c
 *
 *  Created on: 28-Apr-2025
 *      Author: Anuja Joshi
 */


#include "em_gpio.h"
#include "sl_udelay.h"

/* pick any free GPIO */
#define BUZZ_PORT   gpioPortD
#define BUZZ_PIN    10


/******************************************************************************
 * @brief  Initialize the GPIO pin connected to the buzzer.
 *
 * Configures the buzzer pin as a push-pull output and sets its initial
 * state to low (off).
 ******************************************************************************/
void buzzer_gpio_init(void)
{
  GPIO_PinModeSet(BUZZ_PORT, BUZZ_PIN, gpioModePushPull, 0);
}


/******************************************************************************
 * @brief  Generate a blocking beep on the buzzer for a specified duration.
 *
 * Turns on the buzzer GPIO, waits for the specified number of milliseconds
 * (converted to microseconds using a busy-wait delay), and then turns it off.
 *
 * @param[in] ms  Duration of the beep in milliseconds.
 *
 * @note This is a blocking delay using @c sl_udelay_wait(), and the CPU remains
 *       active for the entire duration.
 ******************************************************************************/
void buzzer_beep_blocking(uint16_t ms)
{
  GPIO_PinOutSet(BUZZ_PORT, BUZZ_PIN);
  sl_udelay_wait((uint32_t)ms * 1000U);
  GPIO_PinOutClear(BUZZ_PORT, BUZZ_PIN);
}
