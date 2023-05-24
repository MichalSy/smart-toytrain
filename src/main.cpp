#include <Arduino.h>
#include "manager/TrainManager.cpp"

TrainManager *trainManager = TrainManager::getInstance();

void setup()
{
  Serial.begin(115200);

  trainManager->init();
}

void loop()
{
  trainManager->update();
  delay(10);
}