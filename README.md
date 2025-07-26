

# Smart Mailbox Notifier – BLE-Based Embedded System

## Overview
This project implements a low-power, Bluetooth Low Energy (BLE)-based embedded system to notify users when new mail arrives in a physical mailbox. It uses a combination of ambient light and distance sensing to detect the presence of mail and communicates the status to a bonded BLE client. The system also includes local alerts via an LCD and buzzer.

## Features
- 📦 Mail detection using BH1750 light sensor and ultrasonic distance sensor
- 📡 BLE notifications to bonded client with timestamps
- 🔋 Ultra-low power operation using LETIMER and EM2 sleep modes
- 📟 LCD for local display of mail status and time
- 🔔 Buzzer alerts when new mail is detected
- 🔒 BLE bonding and security features
- 🔁 Client-initiated reset command via BLE

## Hardware Used
- EFR32 Blue Gecko (BG13 series)
- BH1750 Light Sensor (I²C)
- Ultrasonic Distance Sensor (GPIO-triggered)
- LCD Display
- Buzzer (GPIO-driven)

## Firmware Highlights
- Developed using Simplicity Studio and C
- Custom GATT profile for mail status and timestamp indication
- Event-driven architecture for sensing and sleeping
- LETIMER-based wake-up to reduce power consumption
- State machine for mail detection and BLE communication
- EM2-compatible peripheral setup for energy-efficient operation

## Power Efficiency
- Uses EM2 sleep mode for most operation time
- LETIMER wakes the system only on necessary intervals
- Achieves >80% reduction in power usage compared to active polling

## Usage
1. Power on the device; it initializes all sensors and enters low-power mode.
2. When mail is inserted, the light level drops and distance changes, triggering detection.
3. The system updates the LCD and buzzer and sends BLE indication with a timestamp.
4. The user can reset the mail counter from the bonded client device.



