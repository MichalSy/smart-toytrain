#pragma once
#include <Arduino.h>

using namespace std;

class MotorManager
{

private:
    MotorManager() = default;

    int motor1Pin1 = D4;
    int motor1Pin2 = D5;
    int motorPwmSignal = D1;

    int currentSpeed = 0;

public:
    static MotorManager *getInstance()
    {
        static MotorManager instance;
        return &instance;
    }

    void init()
    {
        pinMode(motorPwmSignal, OUTPUT);
        pinMode(motor1Pin1, OUTPUT);
        pinMode(motor1Pin2, OUTPUT);
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
    }

    void update()
    {
        if (currentSpeed > 0)
        {
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
        }
        else if (currentSpeed < 0)
        {
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, HIGH);
        }
        else
        {
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, LOW);
        }
        analogWrite(motorPwmSignal, abs(currentSpeed));
    }

    void setSpeed(int speed)
    {   
        currentSpeed = speed;        
    }
};