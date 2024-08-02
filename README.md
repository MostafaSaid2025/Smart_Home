# Smart_Home

This project showcases a Smart Home system using three PIC18f46K20 microcontrollers (MCUs).

Features:
Master MCU:
Displays date, time, and temperature.
Sends temperature data to the second MCU via I2C protocol.
Second MCU:
Controls a fan based on the temperature data received from the Master MCU.
Third MCU:
Controls lighting with two modes:
Manual Mode: Utilizes a push button with interrupt handling to prevent button bouncing.
Automatic Mode: Uses UART communication, allowing for Bluetooth module integration (simulated using a virtual terminal in Proteus).
Future Development:
Implementing a similar system using FreeRTOS on an STM32 ARM-based MCU.
