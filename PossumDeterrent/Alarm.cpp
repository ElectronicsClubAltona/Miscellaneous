#include <toneAC.h>
#include "Alarm.h"




CAlarm::CAlarm()
{
  m_nBatteryPin = A0;
  m_nAlarmVolumePin = A1;
  m_nVapourPin = 13;
  m_nLEDAnode = 12;
  m_nLEDCathode = 11;
  pinMode(m_nLEDAnode, OUTPUT);
  digitalWrite(m_nLEDAnode, LOW);
  pinMode(m_nLEDCathode, OUTPUT);  
  digitalWrite(m_nLEDCathode, LOW);
  pinMode(m_nVapourPin, OUTPUT);
  digitalWrite(m_nVapourPin, LOW);
  pinMode(m_nBatteryPin, INPUT);
  pinMode(m_nAlarmVolumePin, INPUT);
  m_nAlarmVolume = 10;
}

CAlarm::~CAlarm()
{
}

uint8_t CAlarm::getAlarmVolume()
{
  uint16_t nVal = analogRead(m_nAlarmVolumePin);
  return (nVal * 10) / 1023;
}

bool CAlarm::checkBattery()
{
  float fVoltage = analogRead(m_nBatteryPin) * (5.0 / 1000);
  return fVoltage >= 4.9;
}

void CAlarm::soundBatteryAlarm(const uint16_t nSeconds, const bool bFlashLED)
{
  uint32_t nLastMillis = millis(), nLastOnMillis = millis(), nTotalMillis = (uint32_t)nSeconds * 1000;
  uint16_t nOnMillis = 200, nOffMillis = 3100;
  bool bOn = false;
  
  while (true)
  {
    if (!bOn && ((millis() - nLastOnMillis) >= nOffMillis))
    {
      toneAC(1000, getAlarmVolume());
      bOn = true;
      nLastOnMillis = millis();
      if (bFlashLED)
        digitalWrite(m_nLEDAnode, bOn);
    }
    if (bOn && ((millis() - nLastOnMillis) >= nOnMillis))
    {
      noToneAC();
      bOn = false;
      nLastOnMillis = millis();
      if (bFlashLED)
        digitalWrite(m_nLEDAnode, bOn);
    }
    if ((millis() - nLastMillis) >= nTotalMillis)
    {
      break;
    }
  }  
  noToneAC();
  digitalWrite(m_nVapourPin, LOW);
  digitalWrite(m_nLEDAnode, LOW);
}

void CAlarm::soundIntruderAlarm(const uint16_t nSeconds, const bool bFlashLED, const uint16_t nFlashInt, const bool bVapor)
{
  uint32_t nLastMillis = millis(), nLastMillisLED = millis(), nFreq = 125, nTotalMillis = (uint32_t)nSeconds * 1000;
  bool bLEDOn = true;

  digitalWrite(m_nVapourPin, bVapor);
  
  while (true)
  {  
    toneAC(nFreq, getAlarmVolume());
    delayMicroseconds(500);
    
    nFreq += 10;
    if (nFreq >= 18000)
      nFreq = 125;
      
    if (bFlashLED)
    {
      if ((millis() - nLastMillisLED) >= nFlashInt)
      {
Serial.print(F("2222222, "));
Serial.println(bLEDOn);
        digitalWrite(m_nLEDAnode, bLEDOn);
        bLEDOn = !bLEDOn;
        nLastMillisLED = millis();
      }
    }
    if ((millis() - nLastMillis) >= nTotalMillis)
    {
      break;
    }
  }
  noToneAC();
  digitalWrite(m_nVapourPin, LOW);
  digitalWrite(m_nLEDAnode, LOW);
}

CAlarm alarm;


