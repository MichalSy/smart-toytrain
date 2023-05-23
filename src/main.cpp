#pragma warning(disable : 4507 34 4385 164)

#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
#include "manager/TrainManager.cpp"

// Required to replace with your xbox address
// XboxSeriesXControllerESP32_asukiaaa::Core
// xboxController("44:16:22:5e:b2:d4");

// any xbox controller
XboxSeriesXControllerESP32_asukiaaa::Core xboxController;

// Motor A
int motor1Pin1 = D5;
int motor1Pin2 = D4;
int motorPwmSignal = D1;

float targetSpeed = 0;
float currentSpeed = 0;

TrainManager *trainManager = TrainManager::getInstance();

void setup()
{
  Serial.begin(115200);

  while (!Serial)
  {
    ;
  } // wait for serial port to connect

  pinMode(motorPwmSignal, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  xboxController.begin();

  trainManager->init();
}

void loop()
{

  if (targetSpeed == 0 && (currentSpeed < 30 || currentSpeed > -30))
  {
    currentSpeed = 0;
  }

  if (targetSpeed > currentSpeed)
  {
    currentSpeed = currentSpeed + 8;
    if (currentSpeed > 255)
    {
      currentSpeed = 255;
    }
  }
  else if (targetSpeed < currentSpeed)
  {
    currentSpeed = currentSpeed - 40;
    if (currentSpeed < -255)
    {
      currentSpeed = -255;
    }
  }

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

      if (xboxController.xboxNotif.trigRT != 0)
      {
        targetSpeed = xboxController.xboxNotif.trigRT / 1023.0 * 255.0;
      }
      else if (xboxController.xboxNotif.trigLT != 0)
      {
        targetSpeed = -(xboxController.xboxNotif.trigLT / 1023.0 * 255.0);
      }
      else
      {
        targetSpeed = 0;
      }

      Serial.println((String) "targetSpeed: " + targetSpeed + ", current: " + currentSpeed);

      if (currentSpeed > 0)
      {
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        analogWrite(motorPwmSignal, currentSpeed);
      }
      else if (currentSpeed < 0)
      {
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
        analogWrite(motorPwmSignal, -currentSpeed);
      }
      else
      {
        analogWrite(motorPwmSignal, 0);
      }
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

  trainManager->update();
  delay(10);
}