#include "Alarm.h"




uint8_t nMotionPin = 2;
bool bIntruderDetected = false;


void intruderDetected()
{
	bIntruderDetected = true;
}

void setup()
{
	Serial.begin(115200);
	pinMode(nMotionPin, INPUT);
	attachInterrupt(digitalPinToInterrupt(nMotionPin), intruderDetected, RISING);
}

void loop()
{
  if (!alarm.checkBattery())
  {
    alarm.soundBatteryAlarm(10, true);
  }
  else if (bIntruderDetected)
  {
  	bIntruderDetected = false;
    alarm.soundIntruderAlarm(20, true, 40, true);
  }
}

