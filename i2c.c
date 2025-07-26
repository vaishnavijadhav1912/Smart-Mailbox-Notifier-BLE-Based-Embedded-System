/*
 * i2c.c
 *
 *  Created on: Feb 9, 2025
 *      Author: Vaishnavi Jadhav
 */


#include "src\i2c.h"
#include "log.h"
#include "app.h"
#include "letimer.h"
#include "src\log.h"
#include "sl_i2cspm.h"
#include "src\scheduler.h"
#include "em_cmu.h"


static uint8_t light_raw[2];




static I2C_TransferSeq_TypeDef transferSequence;
static uint8_t cmd_data;





/**
 * Initializes the I2C0 peripheral with the specified configuration.
 *
 * This function configures the I2C0 peripheral, including GPIO pin modes and
 * I2C speed settings, to communicate with the light sensor.
 *
 */

void i2c_init(void) {


       GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeWiredAndPullUp, 1);
       GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeWiredAndPullUp, 1);


    I2CSPM_Init_TypeDef i2cInit = {
        .port = I2C0,
        .sclPort = I2C_SCL_PORT,
        .sclPin = I2C_SCL_PIN,
        .sdaPort = I2C_SDA_PORT,
        .sdaPin = I2C_SDA_PIN,
        .portLocationScl = I2C_SCL_LOC,
        .portLocationSda = I2C_SDA_LOC,
        .i2cRefFreq = 0,
        .i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
        .i2cClhr = i2cClockHLRStandard
    };

    I2CSPM_Init(&i2cInit);



}




/*
 *  Turn on power to the light sensor.
 *
 */
void Sensor_powerOn(void)
{

    GPIO_PinOutSet(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN);

}


/*
 *  Turn off power to the light sensor.
 *
 */
void Sensor_powerOff(void)
{

    GPIO_PinOutClear(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN);
}






/******************************************************************************
 * @brief  Send a single-byte I²C command to the BH1750 light sensor.
 *
 * Description:
 *   Prepares and initiates an interrupt-driven I²C write transaction to the
 *   BH1750 (default I²C address: 0x23) using the global @c transferSequence.
 *   The command byte (e.g., 0x20 for continuous high-res mode) is written to
 *   the sensor to configure or trigger measurement.
 *
 * Workflow:
 *   - Sets slave address and write flags.
 *   - Configures a 1-byte command buffer.
 *   - Enables I²C0 interrupts (MSTOP and RXDATAV) and NVIC IRQ.
 *   - Starts the I²C transfer using @c I2C_TransferInit().
 *   - Logs any immediate (non-in-progress) error via @c LOG_ERROR with
 *     detailed classification of the failure type.
 *
 * @note The transfer is non-blocking. Completion is handled in the I²C ISR.
 ******************************************************************************/

void send_I2C_command(void) {

        I2C_TransferReturn_TypeDef transferStatus;


        cmd_data = 0x20;

        transferSequence.addr = (0x23 << 1);
        transferSequence.flags = I2C_FLAG_WRITE;
        transferSequence.buf[0].data = &cmd_data;
        transferSequence.buf[0].len = 1;

        I2C_IntEnable(I2C0, I2C_IEN_MSTOP | I2C_IEN_RXDATAV);
        NVIC_EnableIRQ(I2C0_IRQn);


        transferStatus = I2C_TransferInit(I2C0, &transferSequence);
         if (transferStatus != i2cTransferInProgress) {
                 LOG_ERROR("I2C_TransferInit() failed with error code: %d", transferStatus);

                 switch (transferStatus) {
                     case i2cTransferDone:
                         //LOG_INFO("I2C Transfer completed immediately.");
                         break;
                     case i2cTransferNack:
                         LOG_ERROR("I2C Transfer NACK received.");
                         break;
                     case i2cTransferBusErr:
                         LOG_ERROR("I2C Transfer Bus Error.");
                         break;
                     case i2cTransferArbLost:
                         LOG_ERROR("I2C Transfer Arbitration Lost.");
                         break;
                     case i2cTransferUsageFault:
                         LOG_ERROR("I2C Transfer Usage Fault.");
                         break;
                     case i2cTransferSwFault:
                         LOG_ERROR("I2C Transfer Software Fault.");
                         break;
                     default:
                         LOG_ERROR("Unknown I2C transfer error.");
                         break;
                 }
             }
         }









/******************************************************************************
 * @brief  Kick off an interrupt‑driven I²C read of two bytes from the light
 *         sensor (slave address 0x23).
 *
 * Workflow
 *   • Configures the global @p transferSequence for a read transaction
 *     (slave address <<1 with R/ W bit = 0, 2‑byte buffer @p light_raw).
 *   • Enables the I2C0 IRQ so that completion will be handled in
 *     @c I2C0_IRQHandler.
 *   • Calls @c I2C_TransferInit() to start the transfer and checks the
 *     immediate return code for parameter or bus‑level faults.
 *
 * Error handling
 *   Any negative @c I2C_TransferReturn_TypeDef is decoded and reported with
 *   @c LOG_ERROR, providing clear diagnostics for in‑progress, NACK, bus,
 *   arbitration, usage, or software faults.
 *
 * @note  The function returns after the transfer is launched; successful
 *        completion and data availability are signalled in the ISR.
 ******************************************************************************/

void Read_Sensor(void)
{
    transferSequence.addr = 0x23 << 1; // 0x23 << 1
    transferSequence.flags = I2C_FLAG_READ;

    // Store data directly into light_raw buffer (2 bytes)
    transferSequence.buf[0].data = light_raw;
    transferSequence.buf[0].len = 2;

    NVIC_EnableIRQ(I2C0_IRQn);

    I2C_TransferReturn_TypeDef transferStatus = I2C_TransferInit(I2C0, &transferSequence);

    if (transferStatus < 0) {
        switch (transferStatus) {
            case i2cTransferInProgress:
                LOG_ERROR("I2C Read Error: Transfer already in progress");
                break;
            case i2cTransferNack:
                LOG_ERROR("I2C Read Error: NACK received");
                break;
            case i2cTransferBusErr:
                LOG_ERROR("I2C Read Error: Bus error");
                break;
            case i2cTransferArbLost:
                LOG_ERROR("I2C Read Error: Arbitration lost");
                break;
            case i2cTransferUsageFault:
                LOG_ERROR("I2C Read Error: Usage fault (invalid parameters)");
                break;
            case i2cTransferSwFault:
                LOG_ERROR("I2C Read Error: Software fault");
                break;
            default:
                LOG_ERROR("I2C Read Error: Unknown error code = %d", transferStatus);
                break;
        }
    }
}






/******************************************************************************
 * @brief  Convert the 2‑byte BH1750 reading in @c light_raw[] to lux.
 *
 * Procedure
 *   1. Merge the MSB/LSB bytes into a 16‑bit @p raw_value.
 *   2. Reject the sensor’s error sentinel (0xFFFF) and return 0 lx.
 *   3. Scale the raw count to physical units using the BH1750 high‑resolution
 *      factor (lux = raw / 1.2).
 *   4. Log the computed lux value for diagnostic purposes.
 *
 * @return  Ambient‑light level in lux (floating‑point).  Returns 0.0 f on an
 *          invalid 0xFFFF reading.
 ******************************************************************************/

float Process_Light(void)
{
    uint16_t raw_value;
    float lux;


    raw_value = (light_raw[0] << 8) | light_raw[1];


    if (raw_value == 0xFFFF) {
        LOG_ERROR("❌ Invalid BH1750 reading: 0xFFFF");
        return 0.0f;  // <-- fix here
    }


    lux = raw_value / 1.2f;


    LOG_INFO("BH1750 Ambient Light: %.2f lux", lux);

    return lux;


}
