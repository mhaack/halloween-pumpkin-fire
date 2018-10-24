/*
 * PIRSensorNode.cpp
 * Homie Node to read simple HC-SR501 PIR sensor data.
 *
 * Version: 1.0
 * Author: Markus Haack (http://github.com/mhaack)
 */

#include "PIRSensorNode.h"
#include <Homie.hpp>

PIRSensorNode::PIRSensorNode(const char *id, const int sensorPin, TSensorCallback sensorCallback)
    : HomieNode(id, "PIR Sensor") {
    _sensorPin = sensorPin;
    _sensorCallback = sensorCallback;
}

void PIRSensorNode::onChange(TSensorCallback sensorCallback) { _sensorCallback = sensorCallback; }

void PIRSensorNode::setup() {
    pinMode(_sensorPin, INPUT);
    advertise("motion");
}

void PIRSensorNode::loop() {
    _val = digitalRead(_sensorPin);
    if (_val == HIGH) {
        if (motionDetected == false) {
            motionDetected = true;
            setProperty("motion").send(String(motionDetected ? "true" : "false"));
            if (_sensorCallback) {
                _sensorCallback(motionDetected);
            }
            Homie.getLogger() << "Motion detected - sensor: " << getId() << endl;
        }
    } else {
        if (motionDetected == true) {
            motionDetected = false;
            setProperty("motion").send(String(motionDetected ? "true" : "false"));
            if (_sensorCallback) {
                _sensorCallback(motionDetected);
            }
            Homie.getLogger() << "Motion ended" << endl;
        }
    }
}
