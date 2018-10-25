/*
 * halloween.cpp
 *
 * Halloween Jack O Lantern
 *
 * Version: 1.0.0
 * Author: Markus Haack (http://github.com/mhaack)
 */

#include <Homie.h>
#include <NTPClient.h>
#include <OLEDDisplayUi.h>
#include <SSD1306.h>
#include <WiFiUdp.h>

#include "DisplayNode.h"
#include "PIRSensorNode.h"
#include "ServoNode.h"
#include "images.h"

// servo positions
const unsigned int OFF_POS = 45;
const unsigned int ON_POS = 160;

// init time client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

// init Homie nodes
PIRSensorNode pirSensorNode1("sensor-1", D0);
PIRSensorNode pirSensorNode2("sensor-2", D5);
PIRSensorNode pirSensorNode3("sensor-3", D6);
ServoNode servoNode("servo", D8, OFF_POS);
HomieNode fireNode("fire", "switch");

// init display & display node
SSD1306Wire display(0x3c, D2, D1);
OLEDDisplayUi ui(&display);
DisplayNode displayNode("display", display, ui, timeClient);

// only trigger fire once within a given time interval
const unsigned int DEFAULT_FIRE_INTERVAL = 15;  // seconds
const unsigned int DEFAULT_FIRE_DURATION = 500; // milliseconds seconds
boolean fireOn = false;
long lastFire;
String lastFireTime;
HomieSetting<long> fireIntervalSetting("fireInterval", "The fire interval in seconds");
HomieSetting<long> fireDurationSetting("fireDuration", "The fire duration in milliseconds");

// declaring method prototypes
void drawPumpkinFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawSensorFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void fire(bool fire);
bool fireHandler(const HomieRange &range, const String &value);
void loopHandler();

void setup() {
    Serial.begin(115200);
    Serial << endl << endl;

    Homie_setFirmware("halloween-pumpkin", "1.0.0");
    Homie.disableResetTrigger();
    // Homie.disableLedFeedback();
    Homie.setLoggingPrinter(&Serial);
    Homie.setLoopFunction(loopHandler);
    Homie.onEvent([](HomieEvent event) { displayNode.Event(event); });

    fireIntervalSetting.setDefaultValue(DEFAULT_FIRE_INTERVAL).setValidator([](long candidate) {
        return candidate > 0;
    });
    fireDurationSetting.setDefaultValue(DEFAULT_FIRE_DURATION).setValidator([](long candidate) {
        return candidate > 0;
    });

    // init display
    displayNode.addFrame(drawPumpkinFrame);
    displayNode.addFrame(drawSensorFrame);
    displayNode.enableStatusFrame(true);

    // Set callback handler for PIR events
    pirSensorNode1.onChange([](bool motion) { fire(motion); });
    pirSensorNode2.onChange([](bool motion) { fire(motion); });
    pirSensorNode3.onChange([](bool motion) { fire(motion); });

    // init the main fire node
    fireNode.advertise("on").settable(fireHandler);

    timeClient.begin();
    Homie.setup();
}

void loop() { Homie.loop(); }

void loopHandler() {
    timeClient.update();
    if (fireOn && (millis() - lastFire >= fireDurationSetting.get() || lastFire == 0)) {
        fire(false);
    }
}

bool fireHandler(const HomieRange &range, const String &value) {
    if (value != "true") {
        return false;
    }

    lastFire = 0;
    fire(true);
    Homie.getLogger() << "Trigger fire via MQTT" << endl;
    return true;
}

void drawPumpkinFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(x + 64, y + 0, "Jack O Lantern");
    display->setFont(ArialMT_Plain_24);
    display->drawString(x + 64, y + 22, "=");
    display->drawXbm(x + 16, y + 18, pumpkin_width, pumpkin_height, pumpkin_bits);
    if (fireOn) {
        display->drawXbm(x + 88, y + 18, fire_width, fire_height, fire_bits);
    } else {
        display->drawXbm(x + 80, y + 18, nofire_width, nofire_height, nofire_bits);
    }
}

void drawSensorFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y) {
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->drawString(x + 64, y + 0, "Jack O Lantern");
    display->setTextAlignment(TEXT_ALIGN_LEFT);

    display->drawString(x, y + 13,
                        "S1: " + String(pirSensorNode1.isMotionDetected() ? "true" : "false") +
                            " S2: " + String(pirSensorNode2.isMotionDetected() ? "true" : "false") +
                            " S3: " + String(pirSensorNode3.isMotionDetected() ? "true" : "false"));
    display->drawString(x, y + 24, "Fire: " + String(fireOn ? "on" : "off"));
    display->drawString(x, y + 35, "Last Fire at: " + lastFireTime);
}

void fire(bool fire) {
    if (fire) {
        if (millis() - lastFire >= fireIntervalSetting.get() * 1000UL || lastFire == 0) {
            servoNode.setPos(ON_POS);
            fireNode.setProperty("on").send(String(fire));
            fireOn = true;
            lastFire = millis();
            lastFireTime = timeClient.getFormattedTime();
            Homie.getLogger() << "Trigger Fire at " << lastFireTime << endl;
        } else {
            Homie.getLogger() << "Ignore fire event, last one is less then " << fireIntervalSetting.get() << "s"
                              << endl;
        }
    } else {
        servoNode.setPos(OFF_POS);
        fireNode.setProperty("on").send(String(fire));
        fireOn = false;
    }
}
