#include <Wire.h>
#include <SensorCollection.h>

CL3GD20 gyro;

int8_t nLEDPin = -1;

void ISR1()
{
  nLEDPin = 8;
}

void ISR2()
{
  nLEDPin = 9;
}

void setup()
{
  Serial.begin(115200);
  gyro.begin();
  gyro.enableInt1(true, true);

  // void setIntX(const bool bHighOrLowInt, const bool bEnable, const int16_t nThreshold, const uint8_t nDuration = 0, const bool bLatch = false);
  gyro.setIntY(true, true, 1);
gyro.print(CTRL_REG3);
gyro.print(INT1_CFG);
gyro.print(INT1_TSH_YH);
gyro.print(INT1_TSH_YL);
gyro.print(INT1_DURATION);
  //gyro.setIntY(false, false, 10);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  attachInterrupt(0, ISR1, RISING);
  attachInterrupt(1, ISR2, RISING);
}

void loop()
{
  if (nLEDPin > -1)
  {
    digitalWrite(nLEDPin, HIGH);
    delay(200);
    digitalWrite(nLEDPin, LOW);
    nLEDPin = -1;
  }
  gyro.printX();
  gyro.printY();
  gyro.printZ();
  Serial.println("");
  delay(1000);
}
