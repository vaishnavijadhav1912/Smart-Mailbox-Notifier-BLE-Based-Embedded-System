/*
 * scheduler.h
 *
 *  Created on: Feb 5, 2025
 *      Author: Vaishnavi Jadhav, Anuja Joshi
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include <stdint.h>
#include <stdbool.h>
#include "sl_bt_api.h"





// Event flag definitions used with sl_bt_external_signal() and scheduler logic

#define EVENT_NONE                  0x00
#define LETIMER0_UF_EVENT          (1 << 0)
#define LETIMER0_COMP1_EVENT       (1 << 1)
#define EVENT_I2C_TRANSFER_DONE    (1 << 2)
#define EVENT_BUTTON_PB0_PRESSED     (1 << 4)
#define EVENT_BUTTON_PB1_PRESSED  (1 << 6)





typedef enum {
    IDLE_STATE,
    POWER_ON,
    WRITE_I2C_COMMAND,
    WAIT_FOR_MEASUREMENT,
    PROCESS_DATA,
    DISTANCE_MEASUREMENT
}state_t;


typedef enum {
    DISC_STATE_IDLE = 0,
    DISC_STATE_FINDING_SERVICE,
    DISC_STATE_FINDING_CHARACTERISTIC,
    DISC_STATE_ENABLING_INDICATION,
    DISC_STATE_MONITORING_CONNECTION
} discovery_state_t;


void scheduler_init(void);
void scheduler_set_event(uint32_t event);
void scheduler_clear_event(uint32_t event);
uint32_t scheduler_get_event(void);
void scheduler_Set_Event_UF(void);
void scheduler_Set_Event_Comp_1(void);
void scheduler_Set_Event_I2C(void);
void scheduler_Set_Event_PB1(void);
void scheduler_Set_Event_PB0(void);
bool scheduler_take_ultra_trigger(void);
void sensor_state_machine(sl_bt_msg_t *evt);
void discovery_state_machine(sl_bt_msg_t *evt);





#endif /* SRC_SCHEDULER_H_ */


