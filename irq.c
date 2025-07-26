/*
 * irq.c
 *
 *  Created on: Jan 30, 2025
 *      Author: Vaishnavi Jadhav
 */


#include "src/irq.h"
#include "src/gpio.h"
#include "em_core.h"
#include <stdint.h>
#include "em_letimer.h"
#include "scheduler.h"
#include <stdio.h>
#include "src/log.h"
#include "em_letimer.h"
#include "src/scheduler.h"
#include "em_cmu.h"
#include "sl_i2cspm.h"
#include "em_i2c.h"
#include "sl_bt_api.h"



#define LETIMER_MAX_COUNT  3000

 //static volatile uint32_t milliseconds = 0; // Global timestamp counter
static volatile uint32_t rollover_count = 1;
 const uint32_t TIMER_TICKS_PER_ROLLOVER = 3000; // Adjust based on your timer settings




 /*
  *
  * This function increases the power mode requirement to EM1 (Energy Mode 1).
  */
 void powerManagerEnable(void) {

     sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
 }

 /*
  *
  * This function removes the power mode requirement to EM1 (Energy Mode 1).
  */

 void powerManagerDisable(void) {

     sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);

 }


/* LETIMER0 Interrupt Service Routine:
 *
 * This function handles interrupts from LETIMER0. It processes two events:
 * - LETIMER0_UF_EVENT: Increments the `milliseconds` counter and sets the
 *   corresponding event in the scheduler.
 * - LETIMER0_COMP1_EVENT: Disables the COMP1 interrupt and sets the corresponding
 *   event in the scheduler.
 */
 void LETIMER0_IRQHandler(void) {
     uint32_t flags = LETIMER_IntGet(LETIMER0);
     LETIMER_IntClear(LETIMER0, flags);  // Clear all pending flags

     if (flags & LETIMER_IF_UF) {


         scheduler_Set_Event_UF();
              rollover_count++;
          }

     if (flags & LETIMER_IF_COMP1) {

         scheduler_Set_Event_Comp_1();
     }


 }


/*
 *
 * This function calculates the total elapsed time in milliseconds since the timer started.
 */
 uint32_t letimerMilliseconds(void)
 {
     uint32_t current_count = LETIMER_CounterGet(LETIMER0);
     uint32_t elapsed_milliseconds = (rollover_count * TIMER_TICKS_PER_ROLLOVER)
                                     + (TIMER_TICKS_PER_ROLLOVER - current_count);
     return elapsed_milliseconds;
 }



 /*I2C0 Interrupt Service Routine:
  *
  * This function handles interrupts from I2C0 and processes I2C transfer results.
  * - If the transfer completes successfully (`i2cTransferDone`), it sets the
  *   `EVENT_I2C_TRANSFER_DONE` event in the scheduler.
  * - If an error occurs, it logs the error message and provides detailed information
  *   based on the error type.
  */

 void I2C0_IRQHandler(void)
 {
     I2C_TransferReturn_TypeDef transferStatus = I2C_Transfer(I2C0);

     if (transferStatus == i2cTransferDone)
     {


         scheduler_Set_Event_I2C();
     }
     else if (transferStatus < 0) {

         LOG_ERROR("I2C Transfer Error: %d", transferStatus);

         switch (transferStatus) {
             case i2cTransferNack:
                 LOG_ERROR("I2C NACK received: address or data not acknowledged.");
                 break;

             case i2cTransferBusErr:
                 LOG_ERROR("I2C Bus Error detected.");
                 break;

             case i2cTransferArbLost:
                 LOG_ERROR("I2C Arbitration Lost: another master took control.");
                 break;

             case i2cTransferUsageFault:
                 LOG_ERROR("I2C Usage Fault: incorrect API usage detected.");
                 break;

             case i2cTransferSwFault:
                 LOG_ERROR("I2C Software Fault: internal software error occurred.");
                 break;

             default:
                 LOG_ERROR("Unknown I2C Transfer Error: %d", transferStatus);
                 break;
         }
     }
 }



 /******************************************************************************
  * @brief  GPIO interrupt handler for even-numbered pins (e.g., PB0).
  *
  * Checks if PB0 triggered the interrupt, clears the corresponding interrupt
  * flag, and signals the Bluetooth stack with @c EVENT_BUTTON_PB0_PRESSED
  * via @c scheduler_Set_Event_PB0().
  ******************************************************************************/

 void GPIO_EVEN_IRQHandler(void)
  {

      uint32_t intFlags = GPIO_IntGet();


      if (intFlags & (1 << PB0_pin)) {

          GPIO_IntClear(1 << PB0_pin);


          scheduler_Set_Event_PB0();

      }


  }


 /******************************************************************************
  * @brief  GPIO interrupt handler for odd-numbered pins (e.g., PB1).
  *
  * Checks if PB1 triggered the interrupt, clears the corresponding interrupt
  * flag, and signals the Bluetooth stack with @c EVENT_BUTTON_PB1_PRESSED
  * via @c scheduler_Set_Event_PB1().
  ******************************************************************************/
 void GPIO_ODD_IRQHandler(void)
 {

     uint32_t intFlags = GPIO_IntGet();


     if (intFlags & (1 << PB1_pin)) {

         GPIO_IntClear(1 << PB1_pin);


         scheduler_Set_Event_PB1();

     }


 }


