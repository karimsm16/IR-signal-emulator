# IR Signal Emulator

This project is my first step toward understanding infrared communication with the ESP32.

The current goal is simple: receive an IR signal from any remote control, store it, and transmit it again. At the same time, I'm learning how OTA (Over-the-Air) updates work so I can upload new firmware without a cable.

## Current Features

* Receive an IR signal using an IR receiver.
* Store the last received signal in RAM.
* Transmit the stored signal using an IR LED.
* Blink the ESP32 built-in LED while receiving and transmitting.
* Support OTA updates using PlatformIO.

## Hardware Used

* ESP32 Dev Module
* IR receiver (in my case : SM0038 )
* IR LED
* 3 Push buttons
* LED ( built_in LED )
* Breadboard
* Jumper wires

## Bill of Materials (BOM)

Below is a CSV-style BOM you can copy into a file named `BOM.csv` or edit as needed.

```csv
Item,Part,Quantity,Notes,Approx. Cost (USD)
1,ESP32 Dev Module,1,Any ESP32 dev board (e.g., ESP32 DevKitC),5-15
2,IR receiver (SM0038 / TSOP382),1,Receiver module used in this project,0.5-3
3,IR LED,1,High-brightness IR LED,0.1-1
4,Push button,3,Momentary push buttons (Save/Send/Delete),0.1-0.5 each
5,Indicator LED,1,Optional external LED (built-in LED can be used),0.05-0.2
6,Breadboard,1,For prototyping,3-10
7,Jumper wires,~10,Male-to-male jumper wires,1-3
8,Resistor 220Ω,1,Current-limiting resistor for IR LED,0.01-0.2
9,USB cable,1,For initial programming,1-3
10,Power supply (5V),1,USB power bank or adapter,5-15
```

And the same data as a Markdown table for quick reading:

| Item | Part                             | Quantity | Notes                                      | Approx. Cost (USD) |
|------|----------------------------------|----------|--------------------------------------------|--------------------:|
| 1    | ESP32 Dev Module                 | 1        | Any ESP32 dev board (e.g., ESP32 DevKitC)  | 5–15               |
| 2    | IR receiver (SM0038 / TSOP382)   | 1        | Receiver module used in this project       | 0.5–3              |
| 3    | IR LED                           | 1        | High-brightness IR LED                     | 0.1–1              |
| 4    | Push button                      | 3        | Momentary push buttons (Save/Send/Delete)  | 0.1–0.5 each       |
| 5    | Indicator LED                    | 1        | Optional external LED (built-in OK)        | 0.05–0.2           |
| 6    | Breadboard                       | 1        | For prototyping                            | 3–10               |
| 7    | Jumper wires                     | ~10      | Male-to-male jumper wires                  | 1–3                |
| 8    | Resistor 220Ω                    | 1        | Current-limiting resistor for IR LED       | 0.01–0.2           |
| 9    | USB cable                        | 1        | For initial programming                    | 1–3                |
| 10   | Power supply (5V)                | 1        | USB power bank or adapter                  | 5–15               |

## the evolution of the project 

✅ Added FOTA (Firmware Over The Air)
I integrated the ArduinoOTA library so I can upload firmware to my ESP32 over Wi-Fi instead of connecting a USB cable every time.

I mainly did it because I wanted to try something new. It wasn’t that hard to implement, but understanding what the library actually does internally took much longer than writing the code itself.

While researching, I wrote a lot of notes on paper. I’m planning to turn everything I learn during this project into a few PDF files and upload them to GitHub once the project is finished. Hopefully they'll help others starting out.

✅ Enabled IR Reception
The receiver started receiving signals… but there was a problem.

The Serial Monitor kept printing random signals with an UNKNOWN protocol even when I wasn’t touching the remote.

After a bit of investigation, I realized my circuit was sitting next to a window. Since sunlight contains infrared radiation, I moved everything into a nearly dark room. Most of the random signals stopped.

Unfortunately, the receiver still couldn’t detect my remote.

I couldn’t find the datasheet for my exact IR receiver. The only one I found was for another module with the same name but different dimensions, so after several random attempts I finally figured out the correct wiring and orientation.

That was probably the most satisfying moment of the day.

✅ Added an IR Noise Filter
Even in a dark room, a few random signals still appeared.

Instead of ignoring them, I wanted to understand why.

That led me to spend a couple of hours learning how the IRremoteESP8266 library works, what raw timing data actually means, and how the library recognizes different protocols.

In the end, I built a simple filter:

Ignore every UNKNOWN protocol.
Ignore signals whose raw timing is below a chosen threshold.
If you’re interested in this topic, let me know. I wrote pages of notes while researching it, and I’d like to convert them into PDFs and upload them with the project.

✅ Enabled IR Transmission
Next, I added:

An IR LED
A Save button
A Send button
A Delete button
This part actually went much faster than I expected.

* Debugging
This was definitely the hardest stage.

The first bug was that if Wi-Fi failed to connect, the rest of my code never executed.

The second one took me much longer to understand.

The received signal wasn’t being saved correctly.

After several hours of debugging, I discovered that the problem wasn’t the receiver—it was my program logic. Since the loop() function runs thousands of times every second, the variable holding the last valid signal kept getting overwritten.

The solution was simply to keep a copy of the last valid signal instead of relying on the receiver’s working variable.

* Added FOTA (Firmware Over The Air)
I integrated the ArduinoOTA library so I can upload firmware to my ESP32 over Wi-Fi instead of connecting a USB cable every time.

I mainly did it because I wanted to try something new. It wasn’t that hard to implement, but understanding what the library actually does internally took much longer than writing the code itself.

While researching, I wrote a lot of notes on paper. I’m planning to turn everything I learn during this project into a few PDF files and upload them to GitHub once the project is finished. Hopefully they'll help others starting out.

* Enabled IR Reception
The receiver started receiving signals… but there was a problem.

The Serial Monitor kept printing random signals with an UNKNOWN protocol even when I wasn’t touching the remote.

After a bit of investigation, I realized my circuit was sitting next to a window. Since sunlight contains infrared radiation, I moved everything into a nearly dark room. Most of the random signals stopped.

Unfortunately, the receiver still couldn’t detect my remote.

I couldn’t find the datasheet for my exact IR receiver. The only one I found was for another module with the same name but different dimensions, so after several random attempts I finally figured out the correct wiring and orientation.

That was probably the most satisfying moment of the day.

* Added an IR Noise Filter
Even in a dark room, a few random signals still appeared.

Instead of ignoring them, I wanted to understand why.

That led me to spend a couple of hours learning how the IRremoteESP8266 library works, raw timing data , and how the library recognizes different protocols.

In the end, I built a simple filter:

Ignore every UNKNOWN protocol.
Ignore signals whose raw timing is below a chosen threshold, and to select the appropriate threshold I took a look on supported protocols in this library , and I found that the IR noise has typical timing much shorter than real remote signals, so filtering them by length worked well.

##  Common Protocols & Code Lengths

| **Protocol**   | **Typical Code Length** | **Notes** |
|----------------|--------------------------|-----------|
| NEC            | 32 bits                  | Very common in TVs, set-top boxes, and consumer electronics. |
| Sony           | 12, 15, or 20 bits       | Different Sony devices use different lengths. |
| RC5            | 14 bits                  | Philips standard, widely used in audio/video equipment. |
| RC6            | 20 bits                  | Microsoft MCE remotes often use this. |
| Panasonic      | 48 bits                  | Longer codes, used in TVs and A/V systems. |
| Sharp          | 15 bits                  | Used in Sharp TVs and devices. |
| Samsung        | 32 bits                  | Common in Samsung TVs and appliances. |
| Mitsubishi     | 16 bits                  | Used in some A/C units and TVs. |
| JVC            | 16 bits                  | Common in JVC audio/video equipment. |
| LG             | 28 bits                  | Used in LG TVs and A/C units. |
| Denon          | 15 bits                  | Audio equipment remotes. |
| Whirlpool/Carrier/Daikin/Fujitsu/Hitachi (A/C) | Variable (48–128 bits) | A/C protocols transmit full state info. |



# Enabled IR Transmission
Next, I added:

An IR LED
A Save button
A Send button
A Delete button
This part actually went much faster than I expected.

# Debugging

The first bug was that if Wi-Fi failed to connect, the rest of my code never executed.

The second one took me much longer to understand.

The received signal wasn’t being saved correctly.

After several hours of debugging, I discovered that the problem wasn’t the receiver—it was my program logic. Since the loop() function runs thousands of times every second, the variable holding the last valid signal kept getting overwritten.

The solution was simply to keep a copy of the last valid signal instead of relying on the receiver’s working variable.


# DEMO video :

[watch the demo video :](https://drive.google.com/file/d/1Dl5kCzZhK6UkSzBTNzrBFPRyDWZuxoFS/view?usp=sharing)
