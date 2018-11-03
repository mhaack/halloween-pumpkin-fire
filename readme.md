# IoT Halloween Jack-O-Lantern flamethrower project

[![Build Status](https://circleci.com/gh/mhaack/halloween-pumpkin-fire/tree/master.svg?style=shield)](https://circleci.com/gh/mhaack/halloween-pumpkin-fire/tree/master)

A flame throwing Jack-O-Lantern is a real Halloween highlight and definitely something to impress the trick-or-treaters and your neighbors. An internet controlled IoT flame throwing Jack-O-Lantern is even better.

This Jack-O-Lantern is internet connected and can be controlled via MQTT protocol. That way the project can be integrated into a home automation solution like [OpenHab](https://www.openhab.org) or [Home Assistant](https://www.home-assistant.io) to control the flamethrower from there or via Alexa or Google Home.

Your are "not so much" interested in Halloween? This is for you as well, a little fire will make even the most bold of people think twice about approaching your door.

This is actually my second version of the flame throwing Jack-O-Lantern. I already build one last year using a modified room spray. But these do not allow to control the flame directly so I decided I need to build a more pro version which allows to control the duration and intensity of the flame.

This repository contains the Arduino software of the project and some details about the electronics. The full project description including the preparation of the pumpkin, the flamethrower part and the entire assembly can be found on [Hackster.io](https://www.hackster.io/markushaack/iot-halloween-jack-o-lantern-flamethrower-using-esp8266-e340a6).

## Hardware

The following components are needed:

-   ESP8266 (Wemos D1 mini, Nodemcu) or ESP32
-   1-3 PIR motion sensors
-   Strong servo motor
-   SSD1306 display (optional)
-   Prototyping board
-   male & female pin headers
-   USB cable and power supply

The display is not really needed for this project, it is just used for fun to display some little animation and information which of the motion sensors triggered the fire.

As usual I got the most parts from [Aliexpress](https://www.aliexpress.com) but all the parts should be available via other sources like ebay or amazon.com as well. A full list of all the materials and tools needed to be build this projects can be found in the [Hackster.io] project description.(https://www.hackster.io/markushaack/iot-halloween-jack-o-lantern-flamethrower-using-esp8266-e340a6).

## Software

The code is writen in C++, `halloween.cpp` is the main class.

The following software libraries are used. If using PlatformIO all dependencies are resolved automatically.

-   [Homie V2](https://github.com/marvinroger/homie-esp8266) including dependencies
-   [SSD1306 driver for ESP8266 platform](https://github.com/squix78/esp8266-oled-ssd1306)
-   [NTPClient to connect to a time server](https://github.com/arduino-libraries/NTPClient)
-   Optionally PlatformIO environment for building the code

### Homie

I used [Homie](https://github.com/marvinroger/homie-esp8266) to better modularize the software parts into dedicated "nodes" to control the servo, the display and get the PIR motion sensor inputs. Homie provides the MQTT protocol support, see [Homie specification](https://git.io/homieiot) for details.

After uploading the software to the board it has to be configured to connect to WiFi and MQTT. Homie provides multiple ways to do this, I prefer to create and upload a config file. Alternatively the configuration UI can be used.

### MQTT commands and config

The flamethrower supports one import command: switch on the fire 😀. It can be triggered via MQTT with `homie/<device id>/fire/on/set` with the value `true`.

Command line example:

```
mosquitto_pub -h <mqtt broker host> -t homie/<device id>/fire/on/set -m true
```

The following config parameters are available via config file or MQTT message (see Homie documentation how to use):

| Parameter    | Type | Usage                                                                                           |
| ------------ | ---- | ----------------------------------------------------------------------------------------------- |
| fireInterval | long | min. interval in sec between flame activations (if motion was detected) to avoid permanent fire |
| fireDuration | long | duration of one flame shot in ms aka. time until servo moves back to initial position           |
| flipScreen   | bool | flip the display screen vertically                                                              |

## Building the circuit

The electronics part is not that complex. Wire the servo, PIR motion sensor(s) and display to the ESP8266 board and done.

| Wemos D1 mini | SSD1306 display | PIR Sensor | Servo   |
| ------------- | --------------- | ---------- | ------- |
| 5V            |                 | VIN        | VIN     |
| 3.3V          | VIN             |            |
| GND           | GND             | GND        | GND     |
| D0            |                 | Sensor 1   |
| D1            | SCL             |            |
| D2            | SDA             |            |
| D5            |                 | Sensor 2   |
| D6            |                 | Sensor 3   |
| D8            |                 |            | CONTROL |

The software is build the way it can get trigger signals from up to three PIR motion sensors. But the project will also work with only one sensor connected. The diagram bellow shows two sensors, my test setup in the picture only uses one sensor.

<img src="https://github.com/mhaack/halloween-pumpkin-fire/blob/master/doc/halloween-pumpkin-fire.png"  alt="Breadboard" width="640">

<img src="https://github.com/mhaack/halloween-pumpkin-fire/blob/master/doc/electronics-1.jpg"  alt="Electronics 1" width="640">
