/*
 * ble_device_type.h
 *
 *  Created on: Feb 16, 2019
 *      Author: dan walkes
 *
 *      Editor: Mar 14, 2021, Dave Sluiter
 *      Change: Added some vertical white space and bd_addr array indices
 *
 * Editor: Feb 26, 2022, Dave Sluiter
 * Change: Added comment about use of .h files.
 *
 *
 * Student edit: Vaishnavi, Anuja
 * @student    Awesome Student, Awesome.Student@Colorado.edu
 *
 */

// Students: Remember, a header file (a .h file) generally defines an interface
//           for functions defined within an implementation file (a .c file).
//           The .h file defines what a caller (a user) of a .c file requires.
//           At a minimum, the .h file should define the publicly callable
//           functions, i.e. define the function prototypes. #define and type
//           definitions can be added if the caller requires theses.

#ifndef SRC_BLE_DEVICE_TYPE_H_
#define SRC_BLE_DEVICE_TYPE_H_
#include <stdbool.h>

/*
 * Students:
 * Set to 1 to configure this build as a BLE server.
 * Set to 0 to configure as a BLE client
 */
#define DEVICE_IS_BLE_SERVER 0











// Students:
// For your Bluetooth Client implementations, starting with A7,
// set this #define to the bd_addr of the Gecko that will be your Server.
// These values are from one of my Geckos, to serve as an example for you:
//                   bd_addr  [0]   [1]   [2]   [3]   [4]   [5] <- array indices
//#define SERVER_BT_ADDRESS {{ 0x84, 0x2e, 0x14, 0xec, 0x4d, 0x1b }}  // Updated server address
//#define SERVER_BT_ADDRESS { 0x84, 0x2e, 0x14, 0xec, 0x4d, 0x1b }
//#define SERVER_BT_ADDRESS  {{ 0x84, 0x2E, 0x14, 0xEC, 0x4D, 0x1B }}

//#define SERVER_BT_ADDRESS {{0x1b, 0x4d, 0xec, 0x14, 0x2e, 0x84}}
<<<<<<< HEAD:Mailbox_Notifier_IoT_project/src/ble_device_type.h
//#define SERVER_BT_ADDRESS {{ 0x92, 0x18, 0xFB, 0x50, 0x4B, 0x80 }}
=======
#define SERVER_BT_ADDRESS {{ 0x92, 0x18, 0xFB, 0x50, 0x4B, 0x80 }}
>>>>>>> d0573f62274ac3bfa07b40ea9f98778a7240d58e:BH1750 - Working/src/ble_device_type.h
// This also can work:
//#define SERVER_BT_ADDRESS (bd_addr) { .addr = { 0x85, 0x61, 0x17, 0x57, 0x0b, 0x00 } }

#define SERVER_BT_ADDRESS {{0x1B, 0x4D, 0xEC, 0x14, 0x2E, 0x84 }}

#if DEVICE_IS_BLE_SERVER

#define BUILD_INCLUDES_BLE_SERVER 1
#define BUILD_INCLUDES_BLE_CLIENT 0
#define BLE_DEVICE_TYPE_STRING "Server"
static inline bool IsServerDevice() { return true; }
static inline bool IsClientDevice() { return false; }

#else

#define BUILD_INCLUDES_BLE_SERVER 0
#define BUILD_INCLUDES_BLE_CLIENT 1
#define BLE_DEVICE_TYPE_STRING "Client"
static inline bool IsClientDevice() { return true;}
static inline bool IsServerDevice() { return false; }

#endif

#endif /* SRC_BLE_DEVICE_TYPE_H_ */
