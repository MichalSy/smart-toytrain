#pragma once

#include <Arduino.h>

XboxSeriesXControllerESP32_asukiaaa::Core xboxController;

using namespace std;

class ControllerManager
{

private:
    ControllerManager() = default;

public:
    static ControllerManager *getInstance()
    {
        static ControllerManager instance;
        return &instance;
    }

    void init()
    {
        xboxController.begin();
    }

    void update()
    {
        xboxController.onLoop();
        if (xboxController.isConnected())
        {
            if (xboxController.isWaitingForFirstNotification())
            {
                Serial.println("waiting for first notification");
            }
            else
            {
                // Serial.println("Address: " + xboxController.buildDeviceAddressStr());

                // if (xboxController.xboxNotif.trigRT != 0)
                // {
                //     targetSpeed = xboxController.xboxNotif.trigRT / 1023.0 * 255.0;
                // }
                // else if (xboxController.xboxNotif.trigLT != 0)
                // {
                //     targetSpeed = -(xboxController.xboxNotif.trigLT / 1023.0 * 255.0);
                // }
                // else
                // {
                //     targetSpeed = 0;
                // }

                // Serial.println((String) "targetSpeed: " + targetSpeed + ", current: " + currentSpeed);

                // if (currentSpeed > 0)
                // {
                //     digitalWrite(motor1Pin1, LOW);
                //     digitalWrite(motor1Pin2, HIGH);
                //     analogWrite(motorPwmSignal, currentSpeed);
                // }
                // else if (currentSpeed < 0)
                // {
                //     digitalWrite(motor1Pin1, HIGH);
                //     digitalWrite(motor1Pin2, LOW);
                //     analogWrite(motorPwmSignal, -currentSpeed);
                // }
                // else
                // {
                //     analogWrite(motorPwmSignal, 0);
                // }
            }
        }
        else
        {
            Serial.println("not connected");
            if (xboxController.getCountFailedConnection() > 2)
            {
                ESP.restart();
            }
        }
    }
};