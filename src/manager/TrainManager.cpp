#pragma once

#include <Arduino.h>

#include "ControllerManager.cpp"

using namespace std;

class TrainManager
{

private:
    TrainManager() = default;

    ControllerManager *controllerManager = ControllerManager::getInstance();

public:
    static TrainManager *getInstance()
    {
        static TrainManager instance;
        return &instance;
    }

    void init()
    {
    }

    void update()
    {
    }
};