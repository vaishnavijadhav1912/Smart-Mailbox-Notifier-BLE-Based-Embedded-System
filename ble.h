/*
 * ble.h
 *
 *  Created on: Feb 20, 2025
 *      Author: vaish
 */




#ifndef SRC_BLE_H_
#define SRC_BLE_H_



#include <stdbool.h>
#include <stdint.h>
#include "sl_bluetooth.h"
#include "gatt_db.h"


#define UINT8_TO_BITSTREAM(p, n)    { *(p)++ = (uint8_t)(n); }

#define UINT32_TO_BITSTREAM(p, n)   {                       \
  *(p)++ = (uint8_t)(n);                                    \
  *(p)++ = (uint8_t)((n) >> 8);                             \
  *(p)++ = (uint8_t)((n) >> 16);                            \
  *(p)++ = (uint8_t)((n) >> 24);                            \
}

#define INT32_TO_FLOAT(m, e) ( (int32_t)((((uint32_t)(m)) & 0x00FFFFFFU) | (((uint32_t)(e)) << 24)) )

<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.h
//typedef struct {
//
//  uint32_t htm_service_handle;
//  uint32_t temp_char_handle;
//  bd_addr myAddress;
//  uint8_t advertisingSetHandle;
//  uint8_t connectionHandle;
//  bool isConnected;
//  uint8_t addr_type;
//  bool indicationsEnabled;
//  bool indication_in_flight;
//  int latestTemperature;
//  uint8_t htm_service_uuid[2];
//  uint8_t light_char_uuid[2];
//  bool passkey_pending;
//  bool is_bonded;
//
//} ble_data_struct_t;


//typedef struct {
//    uint32_t htm_service_handle;
//    uint32_t temp_char_handle;
//    uint32_t mailbox_event_handle;   // <-- NEW! For mail status + timestamp
//    bd_addr myAddress;
//    uint8_t advertisingSetHandle;
//    uint8_t connectionHandle;
//    bool isConnected;
//    uint8_t addr_type;
//    bool indicationsEnabled;
//    bool indication_in_flight;
//    int latestTemperature;
//    uint8_t htm_service_uuid[2];
//    uint8_t light_char_uuid[2];
//    bool passkey_pending;
//    bool is_bonded;
//    uint16_t reset_mail_counter_handle;
//} ble_data_struct_t;

typedef struct {
    uint32_t htm_service_handle;
    uint32_t mail_char_handle;
    uint32_t mailbox_event_handle;
    bd_addr myAddress;
    uint8_t advertisingSetHandle;
    uint8_t connectionHandle;
    bool isConnected;
    uint8_t addr_type;
    bool indicationsEnabled;
    bool indication_in_flight;
    int latestTemperature;
    uint8_t htm_service_uuid[2];
    uint8_t light_char_uuid[2];
    bool passkey_pending;
    bool is_bonded;
    uint16_t reset_mail_counter_handle;   // <<< ADD THIS!!
=======
typedef struct {

  uint32_t htm_service_handle;
  uint32_t temp_char_handle;
  bd_addr myAddress;
  uint8_t advertisingSetHandle;
  uint8_t connectionHandle;
  bool isConnected;
  uint8_t addr_type;
  bool indicationsEnabled;
  bool indication_in_flight;
  int latestTemperature;
  uint8_t htm_service_uuid[2];
  uint8_t light_char_uuid[2];
  bool passkey_pending;
  bool is_bonded;

>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.h
} ble_data_struct_t;


ble_data_struct_t* getBleDataPtr(void);

// Function to convert an IEEE-11073 floating point format to a 32-bit integer
int32_t FLOAT_TO_INT32(const uint8_t *buffer_ptr);

// Function to initialize BLE settings and start advertising/scanning
void ble_init(void);

// Function to handle BLE events
void handle_ble_event(sl_bt_msg_t *evt);


// Function to send temperature data via BLE indications (alternative version)
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble.h
void bleInit(void);
void update_mailbox_event();
void Send_Mail_Status(void);
=======
void Send_Lux(void);
void bleInit(void);
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble.h




#endif /* SRC_BLE_H_ */
