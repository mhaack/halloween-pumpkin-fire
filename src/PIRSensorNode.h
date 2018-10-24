/*
 * PIRSensorNode.h
 * Homie Node to read simple HC-SR501 PIR sensor data.
 *
 * Version: 1.0
 * Author: Markus Haack (http://github.com/mhaack)
 */

#ifndef SRC_PIRNODE_H
#define SRC_PIRNODE_H

#include <HomieNode.hpp>

#define DEFAULTPIN -1

class PIRSensorNode : public HomieNode {

  public:
    typedef std::function<void(bool)> TSensorCallback;

    PIRSensorNode(const char *id, const int sensorPin = DEFAULTPIN, TSensorCallback sensorCallback = NULL);
    void onChange(TSensorCallback sensorCallback);
    float isMotionDetected() const { return motionDetected; }

  protected:
    virtual void setup() override;
    virtual void loop() override;

  private:
    TSensorCallback _sensorCallback;
    bool motionDetected = false;
    int _sensorPin;
    int _val = 0;
};

#endif
