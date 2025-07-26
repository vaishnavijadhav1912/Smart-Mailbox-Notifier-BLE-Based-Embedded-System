

/*
 * i2c.h
 *
 *
 *      Author: Vaishnavi Jadhav
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "sl_i2cspm.h"
#include "em_gpio.h"


#define BH1750_I2C_ADDRESS    (0x23) //I2C address of Si7021 sensor




// GPIO for Power Management
#define SENSOR_ENABLE_PORT    gpioPortD
#define SENSOR_ENABLE_PIN     15 // Pin number for sensor

#define I2C_SCL_PORT  gpioPortC
#define I2C_SCL_PIN   10
#define I2C_SCL_LOC   14  // e.g. corresponds to PC10

#define I2C_SDA_PORT  gpioPortC
#define I2C_SDA_PIN   11
#define I2C_SDA_LOC   16  // e.g. corresponds to PC11




#define VL53L1X_I2C_ADDRESS  0x29

#define BH1750_I2C_ADDRESS    (0x23)


extern uint8_t i2c_rx_buffer[2];  // ✅ Correct - this only declares it

void i2c_init(void);

void Sensor_powerOn(void);

void Sensor_powerOff(void);

void send_I2C_command(void);

void Read_Sensor(void);

float Process_Light(void);


#endif /* I2C_H_ */

