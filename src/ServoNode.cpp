/*
 * ServoNode.cpp
 * Servo node to control a servo motor.
 *
 * Version: 1.0
 * Author: Markus Haack (http://github.com/mhaack)
 */

#include "ServoNode.h"
#include <Homie.hpp>

ServoNode::ServoNode(const char *id, const int pin, const unsigned int initPos) : HomieNode(id, "Servo") {
    _pin = pin;
    _initPos = initPos;
}

void ServoNode::setup() {
    advertise("position").settable();
    if (_pin > DEFAULTPIN) {
        servo.attach(_pin);
        _position = _initPos;
        servo.write(_initPos);
    }
}

bool ServoNode::handleInput(const String &property, const HomieRange &range, const String &value) {
    Homie.getLogger() << "Recieved MQTT message: " << value << endl;
    setPos(value.toInt());
    return true;
}

unsigned int ServoNode::getPos() { return _position; }

void ServoNode::setPos(const unsigned int position) {
    if (_pin > DEFAULTPIN) {
        _position = position;
        move();
        setProperty("position").send(String(_position));
        Homie.getLogger() << "Servo set to " << _position << endl;
    } else {
        Homie.getLogger() << "Error: No Servo pin defined" << endl;
    }
}

void ServoNode::move() { servo.write(_position); }
