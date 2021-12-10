# ESP32 projects

[![GitHub release](https://img.shields.io/github/release/kreier/ESP32.svg)](https://GitHub.com/kreier/ESP32/releases/)
[![MIT license](https://img.shields.io/github/license/kreier/ESP32)](https://kreier.mit-license.org/)

After working with Arduino since 2015 I was more and more challenged by the limitations and needs of exernal modules to expand the capabilites. Since 2018 I started to first work with the [ESP8266](../ESP8266) thanks to the included WiFi and later to this ESP32 for faster speed, more storage and integrated Bluetooth.

Thanks to advancements you can get now an TTGO T-Display with WiFi, LiPo battery charge controller, USB-C connector, two input buttons and 1.18" color IPS display 135x240px for less than an original Arduino Uno (with just USB-B and nothing else). So let's list some repositories that build on the ESP32:

## [T-Display](../t-display)

<img src="ESP32-starfield.gif" align="right">

__2020/11/24__

A cost effective solution for students to enter the world of programming their own devices. With integrated display and battery connector their projects can be carried anywhere and the results shown to others. The integrated WiFi connects to the world.


## [Solarmeter](../solar/solarmeter) 

__2020/06/23__

Measures the created electric energy on our renewable energy station at AISVN including photovoltaic cells, wind generator, battery voltages and temperature


## [T200](../T200) 

__2019/04/08__

A robot car controlled by Bluetooth BLE.

## Further smaller projects

The ESP32 are well suited for our data collection project at AISVN with many analog pins, deep sleep modes to work long on battery power and WiFi capabilities to transfer data to the internet. This is accompanied by sufficient fast processing power and large memory. Some projects that use the ESP32 are:


- [Webserver in MicroPython](https://github.com/kreier/python2018/tree/master/micropython/webserver) with button control 2019/12/16

![Webserver LED control](https://raw.githubusercontent.com/kreier/python2018/master/micropython/webserver/20191216.gif)

This repository collects smaller [sniplets](sniplets) and projects that don't require its own repository. Some of these are:

- [Blink2020](sniplets/Blink2020) only short flashing, HIGH/LOW easier to alter
- [Blink2020sweep](sniplets/Blink2020sweep) find the pin the LED is attached to (different with every board ...)
- [ESP32 power consumption](ESP32_power_consumtion) test script to place the ESP32 in different power modes to measure the energy consumption
- [I2C scanner](sniplets/i2c_scan) scans the i2c bus for devices
- [i2cdetect](sniplets/i2cdetect) replicates the output of the Raspberry Pi output. Here with rtc and flash on ZS-042:

```
i2cdetect
Scanning address range 0x00-0x7F
 
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  
50: -- -- -- -- -- -- -- 57 -- -- -- -- -- -- -- --  
60: -- -- -- -- -- -- -- -- 68 -- -- -- -- -- -- --  
70: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  

```

- [startup-post](startup-post) See a message at [https://sites.google.com/view/startup-post](https://sites.google.com/view/startup-post) after a succesfull boot and internet connection of your IoT project.

## Power Consumption

The measurements were taken at 28/11/2020 and are documented in the Excel file in [ESP power consumption](https://github.com/kreier/ESP32/tree/master/sniplets/ESP32_power_consumption).

All values are in mA:

|               |   TTGO  |    TTGO   |   TTGO  |     WEMOS    |    DOIT   |
|---------------|:-------:|:---------:|:-------:|:------------:|:---------:|
| USB           | T18 3.0 | T-Display | T-Koala | LoLin32 lite | DEVKIT V1 |
| on            | 47      | 68        | 35,8    | 47           | 68        |
| wifi          | 70      | 108       | 100     | 83           | 100       |
| wifi transmit | 85      |           |         | 116          | 151       |
| radio off     |         |           |         | 47           | 56        |
| 80 MHz        | 35      |           |         | 60           | 38        |
| light sleep   | 19      |           | 0,79    | 8            | 17        |
| deep sleep    |         | 0,35      |         | 4            | 16        |

And now looking for prospect temp station candidates:

| LiPo          | T-Display | T-Koala | LoLin32 lite |
|---------------|:---------:|:-------:|:------------:|
| on            | 68        | 50      | 43           |
| wifi          |           | 104     | 73           |
| wifi transmit | 108       | 131     | 103          |
| radio off     |           | 37      | 43           |
| 80 MHz        |           | 21      | 24           |
| light sleep   |           | 2,5     | 1,6          |
| deep sleep    | 0,35      | 0,79    | 0,063        |


## History

This repository started in March 2019. I was new to the ESP8266 and ESP32 and took the first steps with both, documentation on GitHub and using BLE. The [T200](https://github.com/kreier/T200) was a successful robot car project from this time, controlled by Bluetooth BLE.

I wrote back then:

> 2019/03/24

We had several Arduino Uno projects at our school. But soon you want to add some bluetooth or WiFi functionality to your project. You need another module, library, cable and so on. Why not use the ESP32 that has all that out of the box? And much more RAM and storage for data and all kinds of projects? That's why it's here. Let's start simple.

### [Visit the Wiki](https://github.com/kreier/ESP32/wiki)!

![ESP32](IMG-5684.JPG)

## Blink
Unlike the Arduino the build-in LED is not connected to pin 13, but pin 2. Everything else is the same. Once it blinks you know that the module works, you can upload stuff and you have a simple output signal for further experiments. If you use the board "**DOIT ESP32 DEVKIT V1**" you can use the "**File > Examples > 01.Basics > Blink** out of the box.

## BLE with iPhone remote
Bluetooth Low Energy BLE is a little more complicated than Bluetooth 2.0. The ESP can do both, and the serial connection with an ESP32 is pretty straight forward. Comparable with a HC-05.

BLE is a little more complicated, because there are services, characteristics, descriptors, properties, values and several of them with their own UUID. Using nRF Connect, BLE Scanner and LightBlue it was easy to connect to the ESP with the build-in examples. But I wanted to use the remote app GoBLE. More in the [Wiki](https://github.com/kreier/ESP32/wiki).
