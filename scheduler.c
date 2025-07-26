
//Created on: Feb 7, 2025
//Author name: Vaishnavi Jadhav, Anuja Joshi


#include "src/scheduler.h"
#include "em_core.h"
#include "src/log.h"
#include "sl_power_manager.h"
#include "src/letimer.h"
#include "src/i2c.h"
#include "src/scheduler.h"
#include "src/i2c.h"
#include "src/letimer.h"
#include "sl_power_manager.h"
#include "em_core.h"
#include "src/log.h"
#include "src/irq.h"
#include "sl_bt_api.h"
#include "app.h"
#include "src/ble.h"
#include "gatt_db.h"
#include "sl_bluetooth.h"
#include "app_assert.h"
#include "sl_bt_api.h"
#include "lcd.h"
#include "gatt_db.h"
#include "src/ble.h"
#include "ble.h"
#include "buzzer.h"
#include "ultrasonic.h"



typedef struct {
    uint8_t connectionHandle;
    uint8_t serviceHandle;
    uint8_t characteristicHandle;
} ble_data_t;
ble_data_t ble_data;
sl_status_t sc;
#define DISPLAY_ROW_TEMPVALUE 3



bool want_trigger;
bool check_distance = false;
#define wait_delay 11000



static volatile uint32_t event_flags = 0;
static state_t current_State = IDLE_STATE;


/*
 *
 * This function initializes the event flag variable to EVENT_NONE.
 */
void scheduler_init(void) {
    event_flags = EVENT_NONE;
    //current_state = STATE_IDLE;
}
/*
 *
 *This function sets a specified event flag in a thread-safe manner.
 */

void scheduler_set_event(uint32_t event) {
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    event_flags |= event;
    CORE_EXIT_CRITICAL();
}

/*
 * This function clears a specified event flag in a thread-safe manner.
 */

void scheduler_clear_event(uint32_t event) {
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    event_flags &= ~event;
    CORE_EXIT_CRITICAL();
}


/*
 *
 * This function returns the event flags without modifying them.
 */
uint32_t scheduler_get_event(void) {
    return event_flags;
}


/**
 * Schedules the Underflow event signal.
 */
void scheduler_Set_Event_UF() {

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  sl_bt_external_signal(LETIMER0_UF_EVENT);
  CORE_EXIT_CRITICAL();

}


/**
 * Schedules the Compare 1 event signal.
 */

void scheduler_Set_Event_Comp_1() {

   CORE_DECLARE_IRQ_STATE;
   CORE_ENTER_CRITICAL();
  sl_bt_external_signal(LETIMER0_COMP1_EVENT );
  CORE_EXIT_CRITICAL();

}


/**
 * Schedules the I2C transfer complete event signal.
 */

void scheduler_Set_Event_I2C() {

  CORE_DECLARE_IRQ_STATE;
   CORE_ENTER_CRITICAL();
  sl_bt_external_signal(EVENT_I2C_TRANSFER_DONE  );
  CORE_EXIT_CRITICAL();

}



/******************************************************************************
 * @brief  Raise an external signal event for PB0 press.
 *
 * Notifies the Bluetooth stack that button PB0 was pressed by issuing the
 * @c EVENT_BUTTON_PB0_PRESSED signal via @c sl_bt_external_signal().
 * The operation is wrapped in a critical section to maintain atomicity when
 * called from interrupt or asynchronous contexts.
 ******************************************************************************/
void scheduler_Set_Event_PB0(void)
{
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    sl_bt_external_signal(EVENT_BUTTON_PB0_PRESSED );
    CORE_EXIT_CRITICAL();
}


/******************************************************************************
 * @brief  Raise an external signal event for PB1 press.
 *
 * This function signals the Bluetooth stack that button PB1 was pressed by
 * posting the @c EVENT_BUTTON_PB1_PRESSED signal using
 * @c sl_bt_external_signal(). The call is wrapped in a critical section to
 * ensure atomicity if invoked from an interrupt context.
 ******************************************************************************/

void scheduler_Set_Event_PB1(void)
{
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    sl_bt_external_signal(EVENT_BUTTON_PB1_PRESSED);
    CORE_EXIT_CRITICAL();
}



/******************************************************************************
 * @brief  Atomically check and consume the ultrasonic trigger flag.
 *
 * This function safely reads the global @c want_trigger flag inside a critical
 * section to prevent race conditions with interrupt or main thread contexts.
 * If the flag is set, it clears it (consumes it) and returns true. Otherwise,
 * it returns false.
 *
 * @return true  if a trigger was pending and is now cleared
 * @return false if no trigger was set
 ******************************************************************************/
bool scheduler_take_ultra_trigger(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  bool pending = want_trigger;
  want_trigger = false;          // consume
  CORE_EXIT_CRITICAL();
  return pending;
}


/******************************************************************************
 * @brief  Handles external signals for light and distance sensing operations.
 *
 * This state machine responds to @c sl_bt_evt_system_external_signal_id events
 * and coordinates:
 *   - Power cycling and I2C communication with the BH1750 light sensor
 *   - Conversion of light sensor data to lux
 *   - Conditional triggering of ultrasonic distance measurement
 *   - BLE mailbox status notification if distance criteria are met
 *
 * The function advances through internal states based on received signal flags
 * (e.g., LETIMER events, I2C transfer complete).
 *
 * @param[in] evt  Pointer to the Bluetooth event structure containing external signals.*/
void sensor_state_machine(sl_bt_msg_t *evt) {



    if (SL_BT_MSG_ID(evt->header) != sl_bt_evt_system_external_signal_id) {
        return;
    }

    uint32_t signal_ex = evt->data.evt_system_external_signal.extsignals;


    while (signal_ex != 0) {
        uint32_t signal = signal_ex & (-signal_ex);

        switch (current_State) {
            case IDLE_STATE:
                if (signal == LETIMER0_UF_EVENT ) {
                    Sensor_powerOn();
                    timerWaitUs_irq(120000);
                    current_State = POWER_ON;
                }
                break;

            case POWER_ON:
                if (signal == LETIMER0_COMP1_EVENT ) {
                    powerManagerEnable();
                    send_I2C_command();
                    current_State = WRITE_I2C_COMMAND;
                }
                break;

            case WRITE_I2C_COMMAND:
                if (signal == EVENT_I2C_TRANSFER_DONE) {
                    powerManagerDisable();
                    timerWaitUs_irq(200000);
                    current_State = WAIT_FOR_MEASUREMENT;
                }
                break;

            case WAIT_FOR_MEASUREMENT:
                if (signal == LETIMER0_COMP1_EVENT) {
                    powerManagerEnable();
                    Read_Sensor();
                    current_State = PROCESS_DATA;
                }
                break;

            case PROCESS_DATA:
                if (signal == EVENT_I2C_TRANSFER_DONE) {

                    powerManagerDisable();
                    NVIC_DisableIRQ(I2C0_IRQn);


                    float lux = Process_Light();
                    if (lux > 50.0f)
                    {
                        check_distance = true;
                    }
                    if (check_distance)
                    {
                        check_distance = false;
                        want_trigger = true;
                        current_State = DISTANCE_MEASUREMENT;
                    }
                    else{
                        current_State = IDLE_STATE;
                    }



                }
                break;

            case DISTANCE_MEASUREMENT:
                  {

                      powerManagerEnable();
                      uint32_t dist = ultra_task_process();
                      if(dist < 8U)
                      {
                          Send_Mail_Status();
                      }
                      powerManagerDisable();
                      current_State = IDLE_STATE;
                 }

                  break;

        }


        signal_ex &= (signal_ex - 1);
    }
}



/******************************************************************************
 * @brief  GATT discovery state machine for the BLE client.
 *
 * This state machine handles service discovery, characteristic discovery,
 * and enabling of GATT indications after a connection is established.
 *
 * State transitions:
 *   - DISC_STATE_IDLE:
 *       Triggered on connection open. Begins primary service discovery using
 *       the known UUID (e.g., mailbox or HTM service).
 *
 *   - DISC_STATE_FINDING_SERVICE:
 *       Waits for procedure completion. If the service was found, initiates
 *       characteristic discovery within the service.
 *
 *   - DISC_STATE_FINDING_CHARACTERISTIC:
 *       Waits for procedure completion. If the expected characteristic is
 *       found, enables GATT indications.
 *
 *   - DISC_STATE_ENABLING_INDICATION:
 *       Waits for confirmation that indications are enabled, then transitions
 *       to monitoring mode.
 *
 *   - DISC_STATE_MONITORING_CONNECTION:
 *       Remains idle until the connection is closed, at which point the state
 *       resets to DISC_STATE_IDLE.
 *
 * @param[in] bleEvent  Pointer to a Bluetooth event structure.
 *
 * @note This is client-side only and should be invoked from the main BLE
 *       event handler in response to stack events.
 ******************************************************************************/

void discovery_state_machine(sl_bt_msg_t *bleEvent)
{

    ble_data_struct_t *bleDataPtr = getBleDataPtr();


    static discovery_state_t discovery_state = DISC_STATE_IDLE;

    sl_status_t status;

    switch (discovery_state) {


        case DISC_STATE_IDLE:
        {

            if (SL_BT_MSG_ID(bleEvent->header) == sl_bt_evt_connection_opened_id)
            {



                status = sl_bt_gatt_discover_primary_services_by_uuid(
                            bleDataPtr->connectionHandle,
                            sizeof(bleDataPtr->htm_service_uuid),
                            bleDataPtr->htm_service_uuid);
                if (status == SL_STATUS_OK) {
                    discovery_state = DISC_STATE_FINDING_SERVICE;
                } else {
                    LOG_ERROR("Service not found.: 0x%04lx", status);

                }
            }
        }
        break;


        case DISC_STATE_FINDING_SERVICE:
        {

            if (SL_BT_MSG_ID(bleEvent->header) == sl_bt_evt_gatt_procedure_completed_id)
            {

                if (bleDataPtr->htm_service_handle != 0)
                {

                    status = sl_bt_gatt_discover_characteristics(
                                bleDataPtr->connectionHandle,
                                bleDataPtr->htm_service_handle);
                    if (status == SL_STATUS_OK) {
                        discovery_state = DISC_STATE_FINDING_CHARACTERISTIC ;
                    } else {
                        LOG_ERROR("Characteristic not found.: 0x%04lx", status);

                        discovery_state = DISC_STATE_IDLE;
                    }
                }

            }
        }
        break;


        case DISC_STATE_FINDING_CHARACTERISTIC:
        {
            if (SL_BT_MSG_ID(bleEvent->header) == sl_bt_evt_gatt_procedure_completed_id)
            {

                if (bleDataPtr->mail_char_handle != 0)
                {



                    status = sl_bt_gatt_set_characteristic_notification(
                                bleDataPtr->connectionHandle,
                                bleDataPtr->mail_char_handle,
                                sl_bt_gatt_indication);

                    if (status == SL_STATUS_OK) {
                        discovery_state = DISC_STATE_ENABLING_INDICATION;
                    } else {
                        LOG_ERROR(" Indications not enabled: 0x%04lx", status);
                        discovery_state = DISC_STATE_IDLE ;
                    }
                }

            }
        }
        break;


        case DISC_STATE_ENABLING_INDICATION:
        {
            if (SL_BT_MSG_ID(bleEvent->header) == sl_bt_evt_gatt_procedure_completed_id)
            {


                discovery_state = DISC_STATE_MONITORING_CONNECTION;
            }
        }
        break;


        case DISC_STATE_MONITORING_CONNECTION:
        {

            if (SL_BT_MSG_ID(bleEvent->header) == sl_bt_evt_connection_closed_id)
            {

                discovery_state = DISC_STATE_IDLE;
            }
        }
        break;

        default:
        {
            LOG_ERROR("Discovery state is not valid. %d\n", discovery_state);
            discovery_state = DISC_STATE_IDLE;
        }
        break;
    }
}



