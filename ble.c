
/*
 * Created on: Feb 7, 2025
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
 * Author name: Vaishnavi Jadhav, Anuja Joshi
=======
 * Author name: Vaishnavi Jadhav
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
 */
#include "ble.h"
#include "gatt_db.h"
#include "sl_bluetooth.h"
#include "sl_bt_api.h"
#include "src/log.h"
#include "ble.h"
#include "src/log.h"
#include "i2c.h"
#include "src/ble.h"
#include "app_log.h"
#include "app_assert.h"
#include "src/lcd.h"
#include "ble_device_type.h"
#include <math.h>
#include "src/scheduler.h"
#include "src/i2c.h"
#include <inttypes.h>
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
#include "buzzer.h"


#define SL_SLEEPTIMER_FREQ_HZ 32768 // typical for Gecko boards


//mailbox
static uint8_t mail_status = 0;
static uint32_t event_timestamp = 0;
static bool event_already_triggered = false;
static uint32_t total_mail_count = 0;



static uint8_t pending_passkey_conn_handle = 0xFF;
=======

static uint8_t pending_passkey_conn_handle = 0xFF;
static bool passkey_confirmed = 0;
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
 uint16_t min_interval = 60;
 uint16_t max_interval = 60;
 uint16_t slave_latency = 4;
 uint16_t supervision_timeout = 82;
 uint16_t min_ce_length = 0;
 uint16_t max_ce_length = 4;


 uint16_t serverMinInterval    = 60;
 uint16_t serverMaxInterval    = 60;
 uint16_t serverSlaveLatency   = 4;
 uint16_t serverTimeout        = 100;
 uint16_t serverMinCeLength    = 0;
 uint16_t serverMaxCeLength    = 0xFFFF;

<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
// static const uint8_t light_char_uuid[2] = {0x1C, 0x2A};
=======
 static const uint8_t light_char_uuid[2] = {0x1C, 0x2A};
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c

static const uint8_t htm_service_uuid[2] = {0x09, 0x18};


<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c



=======
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
/***************************************************************************//**
 * @brief
 *   Returns a pointer to the global BLE data structure.
 *
 * @return
 *   Pointer to the ble_data_struct_t instance.
 ******************************************************************************/
static ble_data_struct_t ble_data;

ble_data_struct_t* getBleDataPtr(void) {
    return &ble_data;
}



<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c


/******************************************************************************
 * @brief  Reset mailbox statistics and notify the connected client.
 *
 * Actions performed:
 *   1. Clear all mailbox‑related state variables:
 *        • total_mail_count      ← 0
 *        • event_already_triggered ← false
 *        • mail_status           ← 0
 *   2. Update the on‑board display to show “No Mails”.
 *   3. If a bonded client is connected, indications are enabled, and no
 *      indication is already pending, transmit the updated state via
 *      update_mailbox_event() so the client’s view is synchronised.
 ******************************************************************************/
void reset_mail_counter()
{
    total_mail_count = 0;
    event_already_triggered = false;   // ✅ must be FALSE now
    mail_status = 0;

    displayPrintf(DISPLAY_ROW_8, "Mails: 0");
    displayPrintf(DISPLAY_ROW_TEMPVALUE, "No Mails");
    displayPrintf(DISPLAY_ROW_10, " ");

    if (ble_data.isConnected && ble_data.indicationsEnabled && !ble_data.indication_in_flight && ble_data.is_bonded)
    {
        update_mailbox_event();
        //LOG_INFO("Server: Sent Reset Indication after reset_mail_counter()");
    }
}



=======
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
/***************************************************************************//**
 * @brief
 *   Initializes the BLE stack and device configuration.
 *
 * @details
 *   - Sets the HTM service and light characteristic UUIDs.
 *   - Stops and deletes any existing advertiser set (if previously created).
 *   - Retrieves the device's Bluetooth identity address.
 *   - For a Server build:
 *       * Creates a new advertising set, configures its timing,
 *         generates advertising data, and starts advertising.
 *       * Updates the LCD display with Server-specific information.
 *   - For a Client build:
 *       * Configures the scanner parameters.
 *       * Updates the LCD display with Client-specific information.
 ******************************************************************************/

void bleInit(void) {
    sl_status_t sc;
    ble_data_struct_t *ptr_ble = getBleDataPtr();

<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
    sc= sl_bt_sm_configure(0x0F, sm_io_capability_displayyesno);
=======

>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
    ble_data.htm_service_uuid[0] = 0x09;
    ble_data.htm_service_uuid[1] = 0x18;
    ble_data.light_char_uuid[0] = 0x1C;
    ble_data.light_char_uuid[1] = 0x2A;


    if (ptr_ble->advertisingSetHandle != 0xFF) {
        sc = sl_bt_advertiser_stop(ptr_ble->advertisingSetHandle);
        if (sc != SL_STATUS_OK && sc != SL_STATUS_INVALID_STATE) {
            LOG_ERROR("Advertising stop unsuccessful: 0x%04lx", sc);
        }
        sc = sl_bt_advertiser_delete_set(ptr_ble->advertisingSetHandle);
        if (sc != SL_STATUS_OK && sc != SL_STATUS_INVALID_STATE) {
            LOG_ERROR("Advertising delete unsuccessful.: 0x%04lx", sc);
        }
    }


    sc = sl_bt_system_get_identity_address(&ptr_ble->myAddress, &ptr_ble->addr_type);


    if (sc != SL_STATUS_OK) {
        LOG_ERROR("BT address not received: 0x%04lx", sc);
    }

#if DEVICE_IS_BLE_SERVER
    // ------------------- SERVER SETUP -------------------
    sc = sl_bt_advertiser_create_set(&ptr_ble->advertisingSetHandle);
    if (sc != SL_STATUS_OK) {
        LOG_ERROR(" Advertiser set not created: 0x%04lx", sc);
    }
    sc = sl_bt_advertiser_set_timing(ptr_ble->advertisingSetHandle, 400, 400, 0, 0);
    if (sc != SL_STATUS_OK) {
        LOG_ERROR("Advertising timing not set.: 0x%04lx", sc);
    }
    sc = sl_bt_legacy_advertiser_generate_data(ptr_ble->advertisingSetHandle, sl_bt_advertiser_general_discoverable);
    if (sc != SL_STATUS_OK) {
        LOG_ERROR("Advertising data not generated.: 0x%04lx", sc);
    }
    sc = sl_bt_legacy_advertiser_start(ptr_ble->advertisingSetHandle, sl_bt_legacy_advertiser_connectable);
    if (sc != SL_STATUS_OK) {
        LOG_ERROR("Advertising not started.: 0x%04lx", sc);
    }

    displayPrintf(DISPLAY_ROW_NAME, "Mail box notifier server");
    displayPrintf(DISPLAY_ROW_CONNECTION, "Advertising");
    displayPrintf(DISPLAY_ROW_ASSIGNMENT, "Light sensor");




#else

<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
    sc = sl_bt_sm_set_bondable_mode(1);
                  if (sc!= SL_STATUS_OK) {
                      LOG_ERROR("Bondable mode not set.: 0x%04lx", sc);
                  }

=======
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
       displayPrintf(DISPLAY_ROW_NAME, "Mail box notifier client");
       displayPrintf(DISPLAY_ROW_CONNECTION, "Discovering");
       displayPrintf(DISPLAY_ROW_ASSIGNMENT, "Light sensor");

    sc = sl_bt_scanner_set_parameters(sl_bt_gap_phy_1m, 80, 40);
    if (sc != SL_STATUS_OK) {
        LOG_ERROR("Scanner parameters not set: 0x%04lx", sc);
    }

<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
    ble_data.is_bonded = false;
    ble_data.indication_in_flight = false;
=======
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c


#endif

}



/***************************************************************************//**
 * @brief
 *   Converts an IEEE-11073 32-bit floating-point value to a signed integer.
 *
 * @details
 *   The input buffer is assumed to be in the format:
 *     - [0]     : Flags byte
 *     - [1-3]   : Mantissa (in 2's complement, little-endian)
 *     - [4]     : Exponent (in 2's complement)
 *
 *   The function applies 10^exponent scaling to the mantissa.
 *
 * @param[in] buffer_ptr
 *   Pointer to a 5-byte array containing the IEEE-11073 float.
 *
 * @return
 *   The converted signed integer.
 ******************************************************************************/
int32_t FLOAT_TO_INT32(const uint8_t *buffer_ptr) {


<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
//    uint8_t     signByte = 0;
=======
    uint8_t     signByte = 0;
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
    int32_t     mantissa = 0;
    int8_t exponent = (int8_t)buffer_ptr[4];


    if (buffer_ptr[3] & 0x80) {
        mantissa = 0xFF000000;
    }

    mantissa |= ((int32_t)buffer_ptr[1] << 0)  |
                ((int32_t)buffer_ptr[2] << 8)  |
                ((int32_t)buffer_ptr[3] << 16);

    return (int32_t)(mantissa * pow(10, exponent));
}



<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
/******************************************************************************
 * handle_ble_event()
 *
 * Central dispatcher for all Bluetooth stack events routed to the application.
 * The implementation supports both roles—server and client—selected at build
 * time via DEVICE_IS_BLE_SERVER.
 *
 * Major duties
 *  ─ System boot  : initialise display, set advertising / scanning, configure
 *                   security and bonding options.
 *  ─ Connection   : open/close processing, parameter negotiation, and
 *                   automatic restart of advertising (server) or scanning
 *                   (client) after link loss.
 *  ─ GATT server  : manage CCCD changes, indication confirmations/time‑outs,
 *                   and user‑write handling (reset‑mail‑counter).
 *  ─ GATT client  : discover services/characteristics, act on incoming
 *                   indications, and issue writes to reset the counter.
 *  ─ Security     : passkey display and confirmation, bond success/failure.
 *  ─ External I/O : translate GPIO button presses into passkey confirms or
 *                   client‑side commands.
 *  ─ UI refresh   : drive the periodic display refresh from a soft‑timer.
 *
 * @param evt  Pointer to the incoming Bluetooth event (sl_bt_msg_t).
 *
 * The function must execute quickly; long or blocking operations should be
 * deferred to other tasks or event queues.
 ******************************************************************************/
=======
/***************************************************************************//**
 * @brief
 *   Handles all Bluetooth events from the BLE stack.
 *
 * @details
 *   Processes different BLE events for both Server and Client configurations.
 *   - **System Boot:** Initializes BLE settings and display.
 *   - **Soft Timer Event:** Triggers display updates.
 *   - **Scanner Advertisement Report (Client Only):** Connects to the known server.
 *   - **Connection Opened:** Updates connection status and parameters.
 *   - **Connection Closed:** Resets state and restarts advertising or scanning.
 *   - **External Signals:** Handles button press events for passkey confirmation.
 *   - **GATT Server Events (Server Only):** Handles characteristic indications and bonding.
 *   - **GATT Client Discovery and Indications (Client Only):** Discovers service/characteristics and handles received indications.
 *   - **Bonding Events:** Manages passkey entry, bonding confirmation, and error handling.
 *
 * @param[in] evt
 *   Pointer to the incoming Bluetooth event message.
 ******************************************************************************/

>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
void handle_ble_event(sl_bt_msg_t *evt) {
    sl_status_t sc;
    ble_data_struct_t *ptr_ble = getBleDataPtr();

 switch (SL_BT_MSG_ID(evt->header)) {
/*****************************************************************************************************************************************
 *                                           SYSTEM BOOT ID
 *
 **********************************************************************************************************************/
 case sl_bt_evt_system_boot_id:
            displayInit();
            bleInit();
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
            // Enable bonding
            sl_bt_sm_set_bondable_mode(1);

            // Configure authentication
            sl_bt_sm_configure(0x0F, sm_io_capability_displayyesno);

#if DEVICE_IS_BLE_SERVER


=======
#if DEVICE_IS_BLE_SERVER

>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
            displayPrintf(DISPLAY_ROW_NAME, "Mail box server");
            displayPrintf(DISPLAY_ROW_CONNECTION, "Advertising");
            displayPrintf(DISPLAY_ROW_ASSIGNMENT, "Team 19");
            {
                bd_addr address;
                uint8_t address_type;
                sl_bt_system_get_identity_address(&address, &address_type);
                displayPrintf(DISPLAY_ROW_BTADDR, "%02X:%02X:%02X:%02X:%02X:%02X",
                              address.addr[0],
                              address.addr[1],
                              address.addr[2],
                              address.addr[3],
                              address.addr[4],
                              address.addr[5]);
            }
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
            //sl_bt_sm_delete_bondings();
#else
            //sl_bt_sm_delete_bondings();
=======
            sl_bt_sm_delete_bondings();
#else
            sl_bt_sm_delete_bondings();
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
            displayPrintf(DISPLAY_ROW_NAME, "Mail box client");
            displayPrintf(DISPLAY_ROW_ASSIGNMENT, "Team 19");
            displayPrintf(DISPLAY_ROW_CONNECTION, "Discovering");
            displayPrintf(DISPLAY_ROW_BTADDR2, "");
            displayPrintf(DISPLAY_ROW_TEMPVALUE, "");
            {
                bd_addr client_address;
                uint8_t client_address_type;
                sl_bt_system_get_identity_address(&client_address, &client_address_type);
                displayPrintf(DISPLAY_ROW_BTADDR, "%02X:%02X:%02X:%02X:%02X:%02X",
                              client_address.addr[0],
                              client_address.addr[1],
                              client_address.addr[2],
                              client_address.addr[3],
                              client_address.addr[4],
                              client_address.addr[5]);
            }

            {


                sc = sl_bt_connection_set_default_parameters(min_interval,
                                                             max_interval,
                                                              slave_latency,
                                                              supervision_timeout,
                                                               min_ce_length,
                                                               max_ce_length);
                if (sc != SL_STATUS_OK) {
                    LOG_ERROR("Client parameters not set.: 0x%04lx", sc);
                }
            }

            sc = sl_bt_scanner_start(sl_bt_gap_phy_1m, sl_bt_scanner_discover_generic);
            if (sc != SL_STATUS_OK) {
                LOG_ERROR("Scanner failed to start.: 0x%04lx", sc);
            }



#endif
            break;

/***************************************************************************************************************
 *                         SOFT TIMER ID
 *
*************************************************************************************************************** */
        case sl_bt_evt_system_soft_timer_id:
            if (evt->data.evt_system_soft_timer.handle == 0x01) {
                displayUpdate();
            }
            break;

#if (!DEVICE_IS_BLE_SERVER)
/******************************************************************************************************************
 *                                  REPORT ID  EVENT (only for client)
 *
 ******************************************************************************************************************/

        case sl_bt_evt_scanner_legacy_advertisement_report_id:
        {
            bd_addr server_address_received = evt->data.evt_scanner_legacy_advertisement_report.address;
            bd_addr server_address_expected = SERVER_BT_ADDRESS;
            //bd_addr server_address_corrected;


<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
=======
//            for (int i = 0; i < 6; i++) {
//                server_address_corrected.addr[i] = server_address_received .addr[5 - i];
//            }
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c




            if (memcmp(server_address_received.addr, server_address_expected.addr, sizeof(bd_addr)) == 0)
            {



                sc = sl_bt_scanner_stop();
                if (sc != SL_STATUS_OK) {
                    LOG_ERROR("Scanner not stopped.: 0x%04X", sc);
                }


                sc = sl_bt_connection_open(
                    server_address_received ,
                    evt->data.evt_scanner_legacy_advertisement_report.address_type,
                    sl_bt_gap_phy_1m,
                    &ptr_ble->connectionHandle);

                if (sc != SL_STATUS_OK) {
                    LOG_ERROR("Connection open id failed: 0x%04lx", sc);
                }
            }
            break;
        }
#endif
/*****************************************************************************************************************
 *                                          CONNECTION OPEN ID
 *
***************************************************************************************************************** */

case sl_bt_evt_connection_opened_id:
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
  if (evt->data.evt_connection_opened.bonding != 0xFF)
      {
          displayPrintf(DISPLAY_ROW_CONNECTION, "Bond Reconnect");
          ptr_ble->is_bonded = true;
      }
      else
      {
          displayPrintf(DISPLAY_ROW_CONNECTION, "New Connection");
          ptr_ble->is_bonded = false;
      }

      ptr_ble->isConnected = true;
      ptr_ble->connectionHandle = evt->data.evt_connection_opened.connection;

  #if DEVICE_IS_BLE_SERVER
      sc = sl_bt_advertiser_stop(ptr_ble->advertisingSetHandle);
      if (sc != SL_STATUS_OK)
      {
          LOG_ERROR("advertising stop failed.: 0x%04lx", sc);
      }

      sc = sl_bt_connection_set_parameters(ptr_ble->connectionHandle,
                                            serverMinInterval,
                                            serverMaxInterval,
                                            serverSlaveLatency,
                                            serverTimeout,
                                            serverMinCeLength,
                                            serverMaxCeLength);
      if (sc != SL_STATUS_OK)
      {
          LOG_ERROR("Server parameters not set. 0x%04lx", sc);
      }
=======
#if DEVICE_IS_BLE_SERVER

            ptr_ble->isConnected = true;

            ptr_ble->connectionHandle = evt->data.evt_connection_opened.connection;

            displayPrintf(DISPLAY_ROW_CONNECTION, "");

            displayPrintf(DISPLAY_ROW_ASSIGNMENT, "Team 19");

            sc = sl_bt_advertiser_stop(ptr_ble->advertisingSetHandle);
            if (sc != SL_STATUS_OK) {
                LOG_ERROR("advertising stop failed.: 0x%04lx", sc);
            }


            sc = sl_bt_connection_set_parameters(ptr_ble->connectionHandle,
                                                              serverMinInterval,
                                                              serverMaxInterval,
                                                              serverSlaveLatency,
                                                              serverTimeout,
                                                              serverMinCeLength,
                                                              serverMaxCeLength);
            if (sc != SL_STATUS_OK) {
                LOG_ERROR("Server parameters not set. 0x%04lx", sc);
            }
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
#else

            ptr_ble->isConnected = true;
                ptr_ble->connectionHandle = evt->data.evt_connection_opened.connection;

                // Retrieve the server address directly from the connection event.
                bd_addr server_addr = evt->data.evt_connection_opened.address;
                bd_addr expected_server = SERVER_BT_ADDRESS;

                // Compare directly without reversing the address.
                if (memcmp(server_addr.addr, expected_server.addr, sizeof(bd_addr)) == 0) {
                    displayPrintf(DISPLAY_ROW_BTADDR2, "%02X:%02X:%02X:%02X:%02X:%02X",
                                  server_addr.addr[0],
                                  server_addr.addr[1],
                                  server_addr.addr[2],
                                  server_addr.addr[3],
                                  server_addr.addr[4],
                                  server_addr.addr[5]);
                } else {
                    //LOG_WARN("Client connected to an incorrect server!");
                }

                displayPrintf(DISPLAY_ROW_CONNECTION, "Connected");


#endif
            break;


/*****************************************************************************************************************
 *                                         CONNECTION CLOSE ID
 *
 ******************************************************************************************************************/

case sl_bt_evt_connection_closed_id:
#if DEVICE_IS_BLE_SERVER
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
  ptr_ble->isConnected             = false;
          ptr_ble->indication_in_flight    = false;
          ptr_ble->is_bonded               = false;
          ptr_ble->indicationsEnabled      = false;

=======
            ptr_ble->isConnected = false;
            ptr_ble->indication_in_flight = false;
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
            displayPrintf(DISPLAY_ROW_CONNECTION, "Advertising");

            // Restart advertising after disconnection.
            sc = sl_bt_legacy_advertiser_generate_data(ptr_ble->advertisingSetHandle, sl_bt_advertiser_general_discoverable);
            if (sc != SL_STATUS_OK) {
                LOG_ERROR("Failed to regenerate advertising data: 0x%04lx", sc);
            }
            sc = sl_bt_legacy_advertiser_start(ptr_ble->advertisingSetHandle, sl_bt_legacy_advertiser_connectable);
            if (sc != SL_STATUS_OK) {
                LOG_ERROR("Failed to restart advertising: 0x%04lx", sc);
            }
#else
            ptr_ble->isConnected = false;

            displayPrintf(DISPLAY_ROW_BTADDR2, "");

            displayPrintf(DISPLAY_ROW_TEMPVALUE, "");

            displayPrintf(DISPLAY_ROW_CONNECTION, "Discovering");

            sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m, sl_bt_scanner_discover_generic);
            if (sc != SL_STATUS_OK) {
                LOG_ERROR("Scanner did not restart. 0x%04lx", sc);
            }
#endif

            break;





#if DEVICE_IS_BLE_SERVER
 /**********************************************************************************************************************
  *                                       GATT SERVER CHARACTERISTICS STATUS ID
  *
  *********************************************************************************************************************/

        case sl_bt_evt_gatt_server_characteristic_status_id:
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
            if (evt->data.evt_gatt_server_characteristic_status.characteristic ==  gattdb_mail_status ) {
=======
            if (evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement) {
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
                if (evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_client_config) {
                    if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == sl_bt_gatt_indication) {
                        ptr_ble->indicationsEnabled = true;
                    }

                else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == sl_bt_gatt_disable) {
                        ptr_ble->indicationsEnabled = false;
                        displayPrintf(DISPLAY_ROW_TEMPVALUE, "");
                    }
                }
                else if (evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_confirmation) {
                    ptr_ble->indication_in_flight = false;
                }
            }
            break;

        case sl_bt_evt_gatt_server_indication_timeout_id:
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
           {
             LOG_ERROR("Indication timeout — in_flight = false");

               ptr_ble->indication_in_flight = false;
           }
           break;
=======
          ptr_ble->indication_in_flight = false;
            break;
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c

            /****************************************************************************************************************
                 *                            BONDING ID
            **************************************************************************************************************/


<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
             case sl_bt_evt_sm_confirm_bonding_id:
             {
                 sc = sl_bt_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection, 1);
                 if (sc != SL_STATUS_OK) {
                     LOG_ERROR("Bonding failed during confirmation process. Status code: 0x%04lx", sc);

                 }
             }
             break;

             case sl_bt_evt_sm_confirm_passkey_id:
             {
                 pending_passkey_conn_handle = evt->data.evt_sm_confirm_passkey.connection;

                 displayPrintf(DISPLAY_ROW_PASSKEY, "%06lu",
                               evt->data.evt_sm_confirm_passkey.passkey);
                 displayPrintf(DISPLAY_ROW_ACTION, "Confirm with PB0");
             }
             break;

             case sl_bt_evt_sm_bonded_id:
             {
                 ptr_ble->is_bonded = true;
                 displayPrintf(DISPLAY_ROW_CONNECTION, "Bonded");
                 // 🔥 Clear passkey and action rows
                    displayPrintf(DISPLAY_ROW_PASSKEY, "");
                    displayPrintf(DISPLAY_ROW_ACTION, "");

             }
             break;

             case sl_bt_evt_sm_bonding_failed_id:
             {
                 ptr_ble->is_bonded = false;
                 displayPrintf(DISPLAY_ROW_CONNECTION, "Bond Fail");
                 LOG_ERROR("Server: Bonding Failed");
             }
             break;


//
             case sl_bt_evt_gatt_server_user_write_request_id:
             {
                 uint16_t char_id = evt->data.evt_gatt_server_user_write_request.characteristic;

                 if (char_id == gattdb_reset_mail_counter)  // ⭐ Reset Mail counter char
                 {
                     uint8_t received_value = evt->data.evt_gatt_server_user_write_request.value.data[0];

                     if (received_value == 1) {
                         reset_mail_counter();       // Reset everything
                         update_mailbox_event();      // Send new Mails:0 via indication
                         //LOG_INFO("Server: Reset Mail Counter on client request");
                     }

                     sl_bt_gatt_server_send_user_write_response(
                         evt->data.evt_gatt_server_user_write_request.connection,
                         evt->data.evt_gatt_server_user_write_request.characteristic,
                         SL_STATUS_OK
                     );
                 }
             }
             break;



#endif//server end
             case sl_bt_evt_system_external_signal_id:
             {
                 uint32_t signals = evt->data.evt_system_external_signal.extsignals;

                 switch (signals)
                 {


                   case EVENT_BUTTON_PB0_PRESSED:
                   {
                   #if DEVICE_IS_BLE_SERVER
                       // SERVER SIDE - Confirm Passkey
                       if (pending_passkey_conn_handle != 0xFF)
                       {
                           sl_status_t sc = sl_bt_sm_passkey_confirm(pending_passkey_conn_handle, 1);
                           if (sc == SL_STATUS_OK)
                           {
                               displayPrintf(DISPLAY_ROW_ACTION, "Passkey Confirmed (Server)");
                           }
                           else
                           {
                               LOG_ERROR("Server: Passkey confirm failed: 0x%04lx", sc);
                           }
                           pending_passkey_conn_handle = 0xFF; // Clear after confirmation
                       }
                   }


#else
    // CLIENT SIDE - Request Server to Reset Mail Counter

        if (ble_data.isConnected)
        {
            uint8_t reset_request = 1;

            sl_status_t sc = sl_bt_gatt_write_characteristic_value( // ⭐ assign return value!
                ble_data.connectionHandle,
                ble_data.reset_mail_counter_handle,
                sizeof(reset_request),
                &reset_request
            );

            if (sc == SL_STATUS_OK)
            {
                //LOG_INFO("Client sent Reset Mail Counter request to Server.");
            }
            else
            {
                LOG_ERROR("Client failed to send Reset Mail Counter request: 0x%04lx", sc);
            }
        }



    break;
                 }


       //----------------------------------------
                 // CLIENT SIDE: PB1 Pressed
                 //----------------------------------------
                 case EVENT_BUTTON_PB1_PRESSED:
                 {
                     if (pending_passkey_conn_handle != 0xFF)
                     {
                         // Confirm the passkey (Client Side)
                         sl_status_t status = sl_bt_sm_passkey_confirm(pending_passkey_conn_handle, 1);
                         if (status == SL_STATUS_OK)
                         {
                             displayPrintf(DISPLAY_ROW_ACTION, "Passkey Confirmed (Client)");
                         }
                         else
                         {
                             LOG_ERROR("Client: Passkey confirm failed: 0x%04lx", status);
                         }
                         pending_passkey_conn_handle = 0xFF; // Clear after confirmation
                     }
                     else
                     {
                         // No passkey pending yet ➔ start security request
                         sl_status_t sc = sl_bt_sm_increase_security(ble_data.connectionHandle);
                         if (sc == SL_STATUS_OK)
                         {
                             displayPrintf(DISPLAY_ROW_ACTION, "Security Requested (Client)");
                         }
                         else
                         {
                             LOG_ERROR("Client: Security request failed: 0x%04lx", sc);
                         }
                     }


                     break;
                 }
             #endif

                 //----------------------------------------
                 // DEFAULT
                 //----------------------------------------
                 default:
                     break;
                 } // end of switch(signals)
             }


             break; // end of case sl_bt_evt_system_external_signal_id
=======
            case sl_bt_evt_sm_confirm_bonding_id:
                            sc = sl_bt_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection, 1);
                            if (sc != SL_STATUS_OK) {
                                LOG_ERROR("Bonding confirm failed: 0x%04X", (unsigned int)sc);
                            }
                            break;


            case sl_bt_evt_sm_confirm_passkey_id:
                   {
                       uint32_t passkey = evt->data.evt_sm_confirm_passkey.passkey;
                       displayPrintf(DISPLAY_ROW_PASSKEY, "Passkey %06lu", passkey);
                       displayPrintf(DISPLAY_ROW_ACTION, "Confirm with PB0");
                       ble_data.passkey_pending = true;
                       ble_data.connectionHandle = evt->data.evt_sm_confirm_passkey.connection;
                   }
                       break;

             case sl_bt_evt_sm_bonded_id:
                       displayPrintf(DISPLAY_ROW_CONNECTION, "Bonded");
                       ble_data.passkey_pending = false;
                       break;

            case sl_bt_evt_sm_bonding_failed_id:
                       LOG_ERROR("Bonding failed: 0x%04X", (unsigned int)evt->data.evt_sm_bonding_failed.reason);
                       break;




#endif
            case sl_bt_evt_system_external_signal_id:
            {
                uint32_t signals = evt->data.evt_system_external_signal.extsignals;

                switch (signals)
                {
            #if DEVICE_IS_BLE_SERVER
                    case EVENT_BUTTON_PB0_PRESSED:
                    {
                        if (pending_passkey_conn_handle != 0xFF)
                        {
                            sl_status_t sc = sl_bt_sm_bonding_confirm(pending_passkey_conn_handle, 1);
                            if (sc == SL_STATUS_OK)
                            {
                                displayPrintf(DISPLAY_ROW_ACTION, "Bonding Confirmed (Server)");
                            }
                            else
                            {
                                LOG_ERROR("Server: Bonding confirm failed: 0x%04lx", sc);
                            }
                            pending_passkey_conn_handle = 0xFF;
                        }
                        break;
                    }
            #else
                    case EVENT_BUTTON_PB1_PRESSED:
                    {
                        if (pending_passkey_conn_handle != 0xFF)
                        {
                            sl_status_t status = sl_bt_sm_passkey_confirm(pending_passkey_conn_handle, 1);
                            if (status == SL_STATUS_OK)
                            {
                                displayPrintf(DISPLAY_ROW_ACTION, "Passkey Confirmed (Client)");
                            }
                            else
                            {
                                LOG_ERROR("Client: Passkey confirm failed: 0x%04lx", status);
                            }
                            pending_passkey_conn_handle = 0xFF;
                        }
                        break;
                    }
            #endif
                    default:
                        break;
                }
            }
            break;
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c



#if !DEVICE_IS_BLE_SERVER

/*****************************************************************************************************************
 *                                    GATT SERVICE ID
 *
 *******************************************************************************************************************/
        case sl_bt_evt_gatt_service_id:
        {

            if ((evt->data.evt_gatt_service.uuid.len == 2) && (memcmp(evt->data.evt_gatt_service.uuid.data, htm_service_uuid, 2) == 0)) {

                ptr_ble->htm_service_handle = evt->data.evt_gatt_service.service;


            }
        }
        break;

/*************************************************************************************************************************
 *                                      GATT CHARACTERISTIC ID
 *
 ***********************************************************************************************************************/
        case sl_bt_evt_gatt_characteristic_id:
            {
                const uint8_t light_char_uuid[2] = { 0x1C, 0x2A };
                if ((evt->data.evt_gatt_characteristic.uuid.len == sizeof(light_char_uuid)) && (memcmp(evt->data.evt_gatt_characteristic.uuid.data, light_char_uuid, sizeof(light_char_uuid)) == 0)) {
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
                    ptr_ble->mail_char_handle = evt->data.evt_gatt_characteristic.characteristic;
                }
            }



            const uint8_t reset_mail_counter_uuid[2] = {0x1D, 0x2A};

            if (evt->data.evt_gatt_characteristic.uuid.len == 2 &&
                memcmp(evt->data.evt_gatt_characteristic.uuid.data, reset_mail_counter_uuid, 2) == 0)
            {
                ptr_ble->reset_mail_counter_handle = evt->data.evt_gatt_characteristic.characteristic;
            }
            break;

=======
                    ptr_ble->temp_char_handle = evt->data.evt_gatt_characteristic.characteristic;
                }
            }
            break;
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
/*********************************************************************************************************************
 *                                      GATT PROCEDURE COMPLETED ID
 *
 **********************************************************************************************************************/
        case sl_bt_evt_gatt_procedure_completed_id:
        {
          displayPrintf(DISPLAY_ROW_CONNECTION, " ");
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c

          if (ble_data.isConnected)
              {
                  //LOG_INFO("Reset mail counter locally after write completed.");
                  reset_mail_counter();  // NOW SAFE to reset client side view
              }
=======
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
        }
        break;

/***********************************************************************************************************************
 *                                       GATT CHARACTERISTIC VALUE ID
 *
 **********************************************************************************************************************/

        case sl_bt_evt_gatt_characteristic_value_id:
        {
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c





          if (evt->data.evt_gatt_characteristic_value.characteristic == ptr_ble->mail_char_handle) {
              uint8_t *mail_status_data = evt->data.evt_gatt_characteristic_value.value.data;

              uint8_t status = mail_status_data[0];
              uint32_t timestamp = ((uint32_t)mail_status_data[4] << 24) |
                                    ((uint32_t)mail_status_data[3] << 16) |
                                    ((uint32_t)mail_status_data[2] << 8) |
                                    ((uint32_t)mail_status_data[1]);

              uint32_t received_mail_count = ((uint32_t)mail_status_data[8] << 24) |
                                              ((uint32_t)mail_status_data[7] << 16) |
                                              ((uint32_t)mail_status_data[6] << 8) |
                                              ((uint32_t)mail_status_data[5]);

              if (received_mail_count == 0) {
                  // ➔ Reset detected
                 // displayPrintf(DISPLAY_ROW_9, "No Mail");
                  displayPrintf(DISPLAY_ROW_8, "Mails: 0");
              }
              else if (status == 1) {
                      LOG_INFO("/nbuzzer triggered");
                      buzzer_beep_blocking(1000);



                      uint32_t seconds = timestamp / 1000;
                      uint32_t minutes = seconds / 60;
                      uint32_t hours = minutes / 60;

                      seconds = seconds % 60;
                      minutes = minutes % 60;

                      if (hours == 0)
                          {

                              displayPrintf(DISPLAY_ROW_TEMPVALUE, "New mail at %02lu:%02lu", minutes, seconds);
                          }
                          else
                          {

                              displayPrintf(DISPLAY_ROW_TEMPVALUE, "New mail at %02lu:%02lu:%02lu", hours, minutes, seconds);
                          }


                  displayPrintf(DISPLAY_ROW_8, "Total Mails: %lu", received_mail_count);
              }
              else {
                  displayPrintf(DISPLAY_ROW_TEMPVALUE, "No Mail");
              }

              sl_status_t sc = sl_bt_gatt_send_characteristic_confirmation(ptr_ble->connectionHandle);
              if (sc != SL_STATUS_OK) {
                  LOG_ERROR("Client failed to confirm indication. 0x%04X", sc);
              }
          }

=======
            if (evt->data.evt_gatt_characteristic_value.characteristic == ptr_ble->temp_char_handle) {
                uint8_t *temp_data = evt->data.evt_gatt_characteristic_value.value.data;

                int32_t temp = FLOAT_TO_INT32(temp_data);

                displayPrintf(DISPLAY_ROW_TEMPVALUE, "Lux: %d ", temp);

                sl_status_t sc = sl_bt_gatt_send_characteristic_confirmation(ptr_ble->connectionHandle);
                if (sc != SL_STATUS_OK) {
                    LOG_ERROR("Client  failed to confirm indication. 0x%04X", sc);
                }
            }
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
        }
        break;


        case sl_bt_evt_sm_confirm_bonding_id:
            {
                sl_status_t sc = sl_bt_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection, 1);
                if (sc != SL_STATUS_OK) {
                    LOG_ERROR("Client: Bonding confirm failed: 0x%04lx", sc);
                }
            }
            break;

        case sl_bt_evt_sm_confirm_passkey_id:
            {
                pending_passkey_conn_handle = evt->data.evt_sm_confirm_passkey.connection;


                displayPrintf(DISPLAY_ROW_PASSKEY, "%06lu",
                              evt->data.evt_sm_confirm_passkey.passkey);
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
                displayPrintf(DISPLAY_ROW_ACTION, "Press PB1 to Confirm");
=======
                displayPrintf(DISPLAY_ROW_ACTION, "Press PB0 to Confirm");
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
            }
            break;


        case sl_bt_evt_sm_bonded_id:
            {
                ptr_ble->is_bonded = true;
                displayPrintf(DISPLAY_ROW_CONNECTION, "Bonded");
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
                   displayPrintf(DISPLAY_ROW_PASSKEY, "");
                   displayPrintf(DISPLAY_ROW_ACTION, "");
=======
                displayPrintf(DISPLAY_ROW_PASSKEY, " ");
                displayPrintf(DISPLAY_ROW_ACTION, " ");
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c

            }
            break;
        case sl_bt_evt_sm_bonding_failed_id:
           {
               ptr_ble->is_bonded = false;
               displayPrintf(DISPLAY_ROW_CONNECTION, "Bond Fail");
               LOG_ERROR("Client: Bonding Failed");
           }
           break;

#endif

    }
}

<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c


/*******************************************************************************
 * @brief  Package and transmit a “mailbox event” GATT indication.
 *
 * Constructs a 9 byte payload containing:
 *   • 1‑byte @c mail_status flag
 *   • 4‑byte little‑endian @c event_timestamp (ms since boot)
 *   • 4‑byte little‑endian @c total_mail_count
 *
 * The payload is sent via @c sl_bt_gatt_server_send_indication on the
 * @c gattdb_mail_status characteristic.  On success the function sets
 * @c ble_data.indication_in_flight to true; otherwise it logs an error.
 *
 * @note  The function is silent if the indication fails—retry logic should be
 *        handled by the caller or the BLE event handler.
 ******************************************************************************/
void update_mailbox_event()
{
    sl_status_t sc;

    uint8_t mail_event_payload[9]; // 1 + 4 + 4 = 9 bytes

    mail_event_payload[0] = mail_status;

    // Timestamp (4 bytes)
    mail_event_payload[1] = (uint8_t)(event_timestamp & 0xFF);
    mail_event_payload[2] = (uint8_t)((event_timestamp >> 8) & 0xFF);
    mail_event_payload[3] = (uint8_t)((event_timestamp >> 16) & 0xFF);
    mail_event_payload[4] = (uint8_t)((event_timestamp >> 24) & 0xFF);

    // Mail Count (4 bytes)
    mail_event_payload[5] = (uint8_t)(total_mail_count & 0xFF);
    mail_event_payload[6] = (uint8_t)((total_mail_count >> 8) & 0xFF);
    mail_event_payload[7] = (uint8_t)((total_mail_count >> 16) & 0xFF);
    mail_event_payload[8] = (uint8_t)((total_mail_count >> 24) & 0xFF);

    sc = sl_bt_gatt_server_send_indication(
        ble_data.connectionHandle,
        gattdb_mail_status,
        sizeof(mail_event_payload),
        mail_event_payload
=======
/***************************************************************************//**
 * @brief
 *   Sends the Lux (light intensity) value via BLE indication.
 *
 * @details
 *   - Checks if the device is connected, bonded, and indications are enabled.
 *   - Reads the current Lux value from the light sensor.
 *   - Formats the Lux value into IEEE-11073 32-bit floating-point format.
 *   - Writes the Lux value to the GATT database attribute.
 *   - Sends the Lux value as a BLE indication to the connected client.
 *   - Updates the indication-in-flight status and display output.
 *
 * @note
 *   - This function will not proceed if the device is not connected, bonded,
 *     or if another indication is already in flight.
 *
 * @warning
 *   - Errors during attribute write or indication send are logged but do not
 *     automatically retry.
 ******************************************************************************/

void Send_Lux() {


  if (!(ble_data.isConnected && ble_data.indicationsEnabled && !ble_data.indication_in_flight)) {
         LOG_ERROR("BLE is not connected or indications are not enabled");
         return;
     }




    uint8_t htm_temperature_buffer[5];
    uint8_t *ptr = htm_temperature_buffer;
    uint32_t htm_temperature_flt;
    uint8_t flags = 0x00;


    float temp = Process_Light();


    UINT8_TO_BITSTREAM(ptr, flags);
    htm_temperature_flt = INT32_TO_FLOAT((temp * 1000), -3);
    UINT32_TO_BITSTREAM(ptr, htm_temperature_flt);


    sl_status_t sc = sl_bt_gatt_server_write_attribute_value(
        gattdb_temperature_measurement,
        0,
        sizeof(htm_temperature_buffer),
        htm_temperature_buffer
    );

    if (sc != SL_STATUS_OK) {
        LOG_ERROR("Failed to write GATT attribute: 0x%04x", (unsigned int)sc);
        return;
    }


    sc = sl_bt_gatt_server_send_indication(
        ble_data.connectionHandle,
        gattdb_temperature_measurement,
        sizeof(htm_temperature_buffer),
        htm_temperature_buffer
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
    );

    if (sc == SL_STATUS_OK) {
        ble_data.indication_in_flight = true;
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.c
    } else {
        LOG_ERROR("Mail event indication failed: 0x%04lx", sc);
    }
}

/*******************************************************************************
 * @brief  Queue a “mail‑status” indication and refresh the user display.
 *
 * The routine verifies that a bonded BLE connection is active and that no
 * indication is currently in progress.  If the prerequisites are met and the
 * event has not yet been processed, it:
 *   1. Sets @c mail_status to 1 and increments @c total_mail_count.
 *   2. Captures a millisecond‑resolution timestamp via the sleeptimer.
 *   3. Updates the corresponding GATT characteristic
 *      (see @c update_mailbox_event()).
 *   4. Presents the timestamp and running total on the display.
 *
 * The function is non‑blocking: it returns immediately if the link is not
 * ready or an indication is already in flight.
 ******************************************************************************/
void Send_Mail_Status(void)
{
    if (!(ble_data.isConnected && ble_data.indicationsEnabled && !ble_data.indication_in_flight && ble_data.is_bonded)) {
        return;
    }

    if (ble_data.indication_in_flight)
    {
        // Indication is still flying, wait!
        return;
    }


        if (!event_already_triggered)
        {
            mail_status = 1;
            event_already_triggered = true;
            total_mail_count++;

            uint32_t msTicks;
            msTicks = sl_sleeptimer_get_tick_count();
            event_timestamp = (msTicks * 1000) / SL_SLEEPTIMER_FREQ_HZ;

            //LOG_INFO("Captured timestamp = %lu ms", event_timestamp);



            update_mailbox_event();

            uint32_t seconds = event_timestamp / 1000;
            uint32_t minutes = seconds / 60;
            uint32_t hours = minutes / 60;

            seconds = seconds % 60;
            minutes = minutes % 60;

            // 🖥️ Update Display: with timestamp!
            if (hours == 0)
            {
                // If <1 hour, just show mm:ss
                displayPrintf(DISPLAY_ROW_TEMPVALUE, "New mail at %02lu:%02lu", minutes, seconds);
            }
            else
            {

                displayPrintf(DISPLAY_ROW_TEMPVALUE, "New mail at  %02lu:%02lu:%02lu", hours, minutes, seconds);
            }

            displayPrintf(DISPLAY_ROW_8, "Total Mails: %lu", total_mail_count);
            displayPrintf(DISPLAY_ROW_10, "User Notified");
            displayPrintf(DISPLAY_ROW_CONNECTION, " ");
        }
    //}
    else
    {
        event_already_triggered = false;
    }
}





=======
        displayPrintf(DISPLAY_ROW_TEMPVALUE, "Lux: %d ", (int)temp);
    } else {
        LOG_ERROR("Failed to send temperature indication: 0x%04x", (unsigned int)sc);
        displayPrintf(DISPLAY_ROW_TEMPVALUE, "");
    }
}

>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.c
