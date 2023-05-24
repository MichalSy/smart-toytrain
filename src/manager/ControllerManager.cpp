#pragma once

#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

using namespace std;

class ControllerManager
{

private:
    ControllerManager() = default;

    XboxSeriesXControllerESP32_asukiaaa::Core xboxController;

public:
    XboxControllerNotificationParser currentState;

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
            currentState = xboxController.xboxNotif;
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