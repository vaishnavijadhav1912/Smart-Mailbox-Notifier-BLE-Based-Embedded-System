/*
  gpio.c
 
   Created on: Dec 12, 2018
       Author: Dan Walkes
   Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

   March 17
   Dave Sluiter: Use this file to define functions that set up or control GPIOs.
   
   Jan 24, 2023
   Dave Sluiter: Cleaned up gpioInit() to make it less confusing for students regarding
                 drive strength setting. 

 *
 * Student edit: Add your name and email address here:
 * @student   Vaishnavi Jadhav, Vaishnavi.Jadhav@Colorado.edu
 *
 
 */


// *****************************************************************************
// Students:
// We will be creating additional functions that configure and manipulate GPIOs.
// For any new GPIO function you create, place that function in this file.
// *****************************************************************************

#include <stdbool.h>
#include "em_gpio.h"
#include <string.h>
#include "em_cmu.h"  // Add this line to include CMU functions

#include "gpio.h"

#include <stdbool.h>


// Student Edit: Define these, 0's are placeholder values.
//
// See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
// and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
// to determine the correct values for these.
// If these links have gone bad, consult the reference manual and/or the datasheet for the MCU.
// Change to correct port and pins:
#define LED0_port   (gpioPortF)
#define LED0_pin   (4)
#define LED1_port   (gpioPortF)
#define LED1_pin   (5)
// Define GPIO ports and pins for LCD and Sensor
#define SENSOR_ENABLE_PORT   gpioPortD  // Change based on hardware
#define SENSOR_ENABLE_PIN    15     // Change based on hardware
#define EXTCOMIN_PORT        gpioPortD  // Change based on hardware
#define EXTCOMIN_PIN         15         // Change based on hardware




// Set GPIO drive strengths and modes of operation
void gpioInit()
{
    // Student Edit:
  CMU_ClockEnable(cmuClock_GPIO, true);  // Enable the clock
    // Set the port's drive strength. In this MCU implementation, all GPIO cells
    // in a "Port" share the same drive strength setting. 
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong); // Strong, 10mA
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak); // Weak, 1mA
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak); // Weak, 1mA
	
	// Set the 2 GPIOs mode of operation
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	// Configure SENSOR_ENABLE pin as output and turn it ON
	    //GPIO_PinModeSet(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN, gpioModePushPull, false);
	    //gpioSensorEnSetOn(); // Ensure LCD and sensor stay enabled

	    // Configure EXTCOMIN pin as output (for LCD refresh toggle)
	   // GPIO_PinModeSet(EXTCOMIN_PORT, EXTCOMIN_PIN, gpioModePushPull, false);


	GPIO_PinModeSet(PB0_port, PB0_pin, gpioModeInput, 1);
	  GPIO_ExtIntConfig(PB0_port, PB0_pin, PB0_pin, true, true, true);
	  NVIC_EnableIRQ(GPIO_EVEN_IRQn);

	  GPIO_PinModeSet(PB1_port, PB1_pin, gpioModeInput, 1);
	    GPIO_ExtIntConfig(PB1_port, PB1_pin, PB1_pin, true, true, true);
	    NVIC_EnableIRQ(GPIO_ODD_IRQn);

} // gpioInit()


void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port, LED0_pin);
}


void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port, LED0_pin);
}


void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port, LED1_pin);
}


void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port, LED1_pin);
}

void gpioLed0Toggle(void) {
    GPIO_PinOutToggle(LED0_port, LED0_pin);
}

void gpioSensorEnSetOn(void) {
    GPIO_PinOutSet(SENSOR_ENABLE_PORT, SENSOR_ENABLE_PIN); // Enable Sensor & LCD
}

void gpioSetDisplayExtcomin(bool value) {
    if (value) {
        GPIO_PinOutSet(EXTCOMIN_PORT, EXTCOMIN_PIN);
    } else {
        GPIO_PinOutClear(EXTCOMIN_PORT, EXTCOMIN_PIN);
    }
}

