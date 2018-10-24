/*
 * ServoNode.h
 * Servo node to control a servo motor.
 *
 * Version: 1.0
 * Author: Markus Haack (http://github.com/mhaack)
 */

#ifndef SRC_ServoNode_H
#define SRC_ServoNode_H

#include <HomieNode.hpp>
#include <Servo.h>

#define DEFAULTPIN -1

class ServoNode : public HomieNode {

  private:
    int _pin;
    int _initPos;
    int _position;
    Servo servo;
    void move();

  public:
    ServoNode(const char *id, const int pin = DEFAULTPIN, const unsigned int initPos = 90);
    void setPos(const unsigned int position);
    unsigned int getPos();

  protected:
    virtual void setup() override;
    virtual bool handleInput(const String &property, const HomieRange &range, const String &value) override;
};

#endif
