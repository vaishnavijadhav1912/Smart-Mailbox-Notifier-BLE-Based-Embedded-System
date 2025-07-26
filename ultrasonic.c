/*
 * ultrasonic.c
 *
 *  Created on: 28-Apr-2025
 *      Author: Anuja Joshi
 */


/***************************************************************************//**
 *  The pulse width is measured by polling ECHO and busy-waiting
 *  with sl_udelay_wait(1).  One iteration = 1 µs.
 *  Maximum measurable distance  ~4 m  (echo ≤ 20 ms).
 ******************************************************************************/

#include "em_gpio.h"
#include "sl_udelay.h"
#include "ultrasonic.h"
#include "em_cmu.h"
#include "log.h"
#include "scheduler.h"

#define TRIG_PORT   gpioPortD //push-pull
#define TRIG_PIN    11
#define ECHO_PORT   gpioPortD
#define ECHO_PIN    12        //input

bool waiting_echo   = false;

/******************************************************************************
 * @brief  Generate a 10 microsecond trigger pulse on the ultrasonic sensor.
 *
 * Sets the TRIG pin high for exactly 10 µs, then pulls it low. This is used
 * to initiate a distance measurement on the HC-SR04 ultrasonic sensor.

 ******************************************************************************/
static inline void trig_pulse_10us(void)
{
  GPIO_PinOutSet(TRIG_PORT, TRIG_PIN);
  sl_udelay_wait(10);                    /* 10 µs high */
  GPIO_PinOutClear(TRIG_PORT, TRIG_PIN);
}


/******************************************************************************
 * @brief  Initialize GPIO pins for the HC-SR04 ultrasonic sensor.
 *
 * Enables the GPIO clock and configures:
 *   - ECHO pin as input to receive pulse duration
 ******************************************************************************/
void hcsr04_init(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);

  GPIO_PinModeSet(TRIG_PORT, TRIG_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(ECHO_PORT, ECHO_PIN, gpioModeInput,     0);
}



/******************************************************************************
 * @brief  Measure distance using the HC-SR04 ultrasonic sensor.
 *
 * Sends a 10 µs trigger pulse and performs a blocking wait to:
 *   1. Detect the rising edge on the ECHO pin (start of echo)
 *   2. Measure the pulse duration (echo high time)
 *   3. Convert the time to distance using speed of sound approximation
 *
 * @return Distance in millimetres (uint32_t), or 0 if:
 *         - No echo rising edge within 60 ms
 *         - Echo pulse duration exceeds 30 ms (out of range)
 *
 * @note This is a blocking function with a worst-case runtime of ~60 ms.
 ******************************************************************************/
uint32_t hcsr04_read_distance_mm(void)
{
  trig_pulse_10us();

  /* -------- wait for rising edge (echo goes HIGH) -------- */
  uint32_t timeout_us = 60000;
  while (!GPIO_PinInGet(ECHO_PORT, ECHO_PIN)) {
    if (--timeout_us == 0) return 0;
    sl_udelay_wait(1);
  }

  /* -------- measure HIGH time (echo pulse width) --------- */
  uint32_t pulse_us = 0;
  while (GPIO_PinInGet(ECHO_PORT, ECHO_PIN)) {
    if (++pulse_us > 30000) return 0;      /* >30 ms ⇒ out of range */
    sl_udelay_wait(1);
  }

  return (pulse_us * 172UL) / 1000UL;
}


/******************************************************************************
 * @brief  Perform a distance measurement.
 *
 *   - Performs a blocking read from the HC-SR04 sensor
 *   - Logs the measured distance if valid
 *
 * If no trigger is set, the function briefly sleeps to reduce CPU usage and
 * returns 0 to indicate no measurement was taken.
 *
 * @return Measured distance in millimetres, or 0 if not triggered or timed out.
 ******************************************************************************/
uint32_t ultra_task_process(void)
{
  if(scheduler_take_ultra_trigger())
  {
    //want_trigger = false;                 // consume flag

    uint16_t d_cm = hcsr04_read_distance_mm();  // blocking read
    if (d_cm == 0)
    {
      //LOG_INFO("Out of range / timeout\r\n");
    }
    else
    {
        LOG_INFO("Distance: %lu cm\r\n",(unsigned long)d_cm);

    }
    return d_cm;
  }

  sl_sleeptimer_delay_millisecond(1);

  return 0;                               // no fresh data
}
