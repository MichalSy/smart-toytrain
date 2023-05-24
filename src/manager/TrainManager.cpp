#pragma once
#include <Arduino.h>

#include "ControllerManager.cpp"
#include "MotorManager.cpp"

using namespace std;

enum TrainState
{
    STOPPED,
    MOVING_FORWARD,
    MOVING_BACKWARD
};

class TrainManager
{

private:
    TrainManager() = default;

    TrainState trainState = STOPPED;
    int targetSpeed = 0;
    float currentSpeed = 0;

    bool isForwardBlocked = false;
    bool isBackwardBlocked = false;

    ControllerManager *controllerManager = ControllerManager::getInstance();
    MotorManager *motorManager = MotorManager::getInstance();

public:
    static TrainManager *getInstance()
    {
        static TrainManager instance;
        return &instance;
    }

    void init()
    {
        controllerManager->init();
        motorManager->init();
    }

    void update()
    {
        controllerManager->update();

        float triggerSpeedDownValuePercent = controllerManager->currentState.trigLT / 1023.0;
        float triggerSpeedUpValuePercent = controllerManager->currentState.trigRT / 1023.0;
        // Serial.println((String)(triggerSpeedDownValuePercent * 255.0) + " " + (triggerSpeedUpValuePercent * 255.0));

        if (trainState == STOPPED)
        {
            if (triggerSpeedDownValuePercent > 0 && !isBackwardBlocked)
            {
                trainState = MOVING_BACKWARD;
                isForwardBlocked = true;
            }
            else if (triggerSpeedUpValuePercent > 0  && !isForwardBlocked)
            {
                trainState = MOVING_FORWARD;
                isBackwardBlocked = true;
            }
        }

        if (trainState == MOVING_FORWARD)
        {
            if (triggerSpeedDownValuePercent == 0)
            {
                targetSpeed = targetSpeed + (triggerSpeedUpValuePercent * 2.5);
                if (targetSpeed > 255)
                    targetSpeed = 255;
            }
            else
            {
                targetSpeed = targetSpeed * (1.0 - (triggerSpeedDownValuePercent / 10.0));
                if (targetSpeed < 0)
                    targetSpeed = 0;
            }
        }
        else if (trainState == MOVING_BACKWARD)
        {
            if (triggerSpeedUpValuePercent == 0)
            {
                targetSpeed = targetSpeed - (triggerSpeedDownValuePercent * 2.5);
                if (targetSpeed < -255)
                    targetSpeed = -255;
            }
            else
            {
                targetSpeed = targetSpeed * (1.0 - (triggerSpeedUpValuePercent / 10.0));
                if (targetSpeed > 0)
                    targetSpeed = 0;
            }
        }

        if (targetSpeed == 0 && currentSpeed < 30 && currentSpeed > -30)
        {
            currentSpeed = 0;
            trainState = STOPPED;

            if (triggerSpeedDownValuePercent == 0)
                isBackwardBlocked = false;

            if (triggerSpeedUpValuePercent == 0)
                isForwardBlocked = false;
        }
        else if (targetSpeed > currentSpeed)
        {
            currentSpeed += 10;
            if (currentSpeed > targetSpeed)
                currentSpeed = targetSpeed;
        }
        else if (targetSpeed < currentSpeed)
        {
            currentSpeed -= 5;
            if (currentSpeed < targetSpeed)
                currentSpeed = targetSpeed;
        }
        motorManager->setSpeed((int)currentSpeed);
        motorManager->update();

        String trainStateString = trainState == STOPPED ? "STOPPED" : trainState == MOVING_FORWARD ? "MOVING_FORWARD"
                                                                                                   : "MOVING_BACKWARD";
        Serial.println((String) "TrainState: " + trainStateString + " Target: " + targetSpeed + ", Current: " + currentSpeed);
    }
};