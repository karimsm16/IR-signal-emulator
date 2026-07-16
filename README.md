# IR Signal Emulator

This project is my first step toward understanding infrared communication with the ESP32.

The current goal is simple: receive an IR signal from any remote control, store it, and transmit it again. At the same time, I'm learning how OTA (Over-the-Air) updates work so I can upload new firmware without reconnecting a USB cable every time.

## Current Features

* Receive an IR signal using an IR receiver.
* Store the last received signal in RAM.
* Transmit the stored signal using an IR LED.
* Blink the ESP32 built-in LED while receiving and transmitting.
* Support OTA updates using PlatformIO.

## Hardware Used

* ESP32 Dev Module
* IR receiver
* IR LED
* Push button
* LEDs
* Breadboard
* Jumper wires

## Project Status

The project is still under development.

## What I Learned

So far, this project has helped me understand:
* How OTA updates work on the ESP32.
* How to use PlatformIO for embedded development.
* How to organize embedded code into smaller, more readable parts.

More features will be added as the project progresses.
