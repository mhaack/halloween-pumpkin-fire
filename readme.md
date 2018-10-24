IoT Halloween Jack-O-Lantern flamethrower project
=================================================

[![Build Status](https://circleci.com/gh/mhaack/halloween-pumpkin-fire/tree/master.svg?style=shield)](https://circleci.com/gh/mhaack/halloween-pumpkin-fire/tree/master)

A flame throwing Jack-O-Lantern is a real Halloween highlight and definitely something to impress the trick-or-treaters and your neighbors. A internet controlled IoT flame throwing Jack-O-Lantern is even better. Your are anti-Halloween? This is for you as well, a little fire will make even the most bold of people think twice about approaching your door and to risk life for the chance of a tiny box of sweets. 

This is actually my second version of the flame throwing jack-o'-lantern 😀 I already build one last year using a modified room spray. But these do not allow to control the flame directly so I decided I need to build a more pro version which allows to control the duration of the flame. 

This repository contains the Arduino software of the project and some details about the electronics. The fill project description including the preparation of the pumpkin, the flamethrower part and the full assemble can be found on Hackster.io.

## Hardware
* ESP8266 (Wemos D1 mini, Nodemcu) or ESP32
* SSD1306 display
* 1-3 PIR motion sensors
* Servo motor
* Prototyping board
* male & female pin headers
* USB cable and power supply

The display is not realy needed for this project, it is just used for fun, to display some little animation and information which of the motion sensors triggered the fire.

As usual I got the most parts from [Aliexpress](https://www.aliexpress.com) but all the parts should be available via ebay or amazon.com as well. A full list of all the materials and tools to be build this projects can be found on Hackster.io.

## Building the circuit

The electronics part is not that complex. Wire the servo, PIR motion sensor and display to the ESP8266 board and done.

Wemos D1 mini | SSD1306 display | PIR Sensor  | Servo
------------- | --------------- | ----------- | -----
5V            |                 | VIN         | VIN
3.3V          | VIN             |             | 
GND           | GND             | GND         | GND
D0            |                 | Sensor 1    | 
D1            | SCL             |             |
D2            | SDA             |             |
D5            |                 | Sensor 2    |
D6            |                 | Sensor 3    |
D8            |                 |             | CONTROL

The software is build the way it can get triggers from up to three PIR motion sensors. But the project will also work with only one sensor connected. The diagram bellow only shows two sensors, my test setup in the picture only uses one sensor.

<img src="https://github.com/mhaack/halloween-pumpkin-fire/blob/master/doc/halloween-pumpkin-fire.png"  alt="Breadboard" width="640">

<img src="https://github.com/mhaack/halloween-pumpkin-fire/blob/master/doc/electronics-1.jpg"  alt="Electronics 1" width="640">


## Software

The following software libraries are used. If using PlatformIO all dependencies are resolved automatically.

- [Homie V2](https://github.com/marvinroger/homie-esp8266) including dependencies
- [SSD1306 driver for ESP8266 platform](https://github.com/squix78/esp8266-oled-ssd1306)
- [NTPClient to connect to a time server](https://github.com/arduino-libraries/NTPClient)
- Optionally PlatformIO environment for building the code

I used [Homie](https://github.com/marvinroger/homie-esp8266) to better modularize the software parts into dedicated "nodes" to control the servo, the display and get the PIR motion sensor inputs. Since Homie has MQTT support build in the project can also be integrate into a home automation solution like [OpenHab](https://www.openhab.org) or [Home Assistant](https://www.home-assistant.io) to control the flamethrower from there or via Alexa or Google Home.

The following two config parameters are available via config file or MQTT message (see Homie documentation how to use):

Parameter           | Type        | Usage
------------------- | ----------- | -------
fireInterval        | long        | min. interval in sec between flame activations (if motion was detected) to avoid permanent fire
fireDuration        | long        | duration of one flame shot in ms aka. time until servo moves back to initial position
flipScreen          | bool        | flip the display screen vertically