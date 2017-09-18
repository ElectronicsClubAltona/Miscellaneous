/*
 Name:    PetLocator.ino
 Created: 20-May-17 9:49:58 PM
 Author:  greg
*/

#include <avr/sleep.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <TimerFreeTone.h>
#include "CString.h"
#include "debug.h"




volatile bool bWakeup = false;
uint8_t nButtonPin = 2, nSpeakerPin = 9, nLEDPin = 13, nRx = 2, nTx= 3;
RH_ASK driver(2000/*BAUD*/, nRx, nTx);

void WakeupISR()
{
  bWakeup = true;
}

bool isTransmitter()
{
  typedef enum{eNone, eIsTransmitter, eIsReceiver} eType;
  static eType nType = 0;
  const uint8_t nIDPin = A0;

  if (nType == eNone)
  {
    pinMode(nIDPin, INPUT);
    if (digitalRead(nIDPin) == HIGH)
      nType = eIsTransmitter;
    else
      nType = eIsReceiver;
  }
  return nType == eIsTransmitter;
}

void SleepNow()
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and
     * wake up sources are available in which sleep mode.
     *
     * In the avr/sleep.h file, the call names of these sleep modes are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we
     * choose the according
     * sleep mode: SLEEP_MODE_PWR_DOWN
     *
     */  
    // Sleep mode is set here
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    
    // Enables the sleep bit in the mcucr register so sleep is possible. just a safety pin
    sleep_enable();
 
    /* Now it is time to enable an interrupt. We do it here so an
     * accidentally pushed interrupt button doesn't interrupt
     * our running program. if you want to be able to run
     * interrupt code besides the sleep function, place it in
     * setup() for example.
     *
     * In the function call attachInterrupt(A, B, C)
     * A   can be either 0 or 1 for interrupts on pin 2 or 3.  
     *
     * B   Name of a function you want to execute at interrupt for A.
     *
     * C   Trigger mode of the interrupt pin. can be:
     *             LOW        a low level triggers
     *             CHANGE     a change in level triggers
     *             RISING     a rising edge of a level triggers
     *             FALLING    a falling edge of a level triggers
     *
     * In all but the IDLE sleep modes only LOW can be used.
     */
     
    // Use interrupt 0 (pin 2) and run function WakeupISR when pin 2 gets LOW
    if (isTransmitter())
      attachInterrupt(digitalPinToInterrupt(nButtonPin), WakeupISR, HIGH);
    else
      attachInterrupt(digitalPinToInterrupt(nRx), WakeupISR, HIGH);

    Serial.println("SLEEPING!");
    delay(100);
    sleep_mode();

    // Here the device is actually put to sleep!! 
    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

    // First thing after waking from sleep: disable sleep...
    sleep_disable();
    Serial.println("WAKING");
    
    // Disables interrupt 0 on pin 2 so the WakeupISR code will not be executed during normal running time.
    if (isTransmitter())
      detachInterrupt(digitalPinToInterrupt(nButtonPin));
    else
      detachInterrupt(digitalPinToInterrupt(nRx));
}
 
 // The setup function runs once when you press reset or power the board
void setup() 
{
  uint16_t nBitsPerSecond = 4000;
  Serial.begin(115200);
  if (!driver.init())
    Serial.println("RH_ASK init failed!");

  if (isTransmitter())
  {
    debug.log(F("TRANSMITTER"));
    pinMode(nButtonPin, INPUT);
  }
  else
  {
    debug.log(F("RECEIVER"));
    pinMode(nRx, INPUT);
  }
  pinMode(nTx, OUTPUT);
  pinMode(nLEDPin, OUTPUT);

  doTrigger();
}

void doTrigger()
{
  uint32_t nMillis = millis(), nMillisEnd = nMillis + (5/*seconds*/ * 1000), nLastMillis = 0;
  uint16_t nInterval = 50;
  bool bOn = true;

  while (nMillis <= nMillisEnd)
  {
    if ((nMillis - nLastMillis) >= nInterval)
    {
      if (!bOn)
      {
        digitalWrite(nLEDPin, LOW);
        bOn = true;
      }
      else
      {
        digitalWrite(nLEDPin, HIGH);
        TimerFreeTone(nSpeakerPin, 5000, 100);
        bOn = false;
      }
      nLastMillis = nMillis;
    }
    nMillis = millis();
  }
  digitalWrite(nLEDPin, LOW);
}

// The loop function runs over and over again until power down or reset
void loop() 
{
  CBuff<24> buff;
  CString strMsg(buff);
  
  if (bWakeup)
  {
    bWakeup = false;
    
    if (isTransmitter())
    {
      digitalWrite(nLEDPin, HIGH);
      strMsg = F("TTTTTTTTTTTTTTTTTTTT");
      driver.send(strMsg, strMsg.length());
      debug.log(F("Message sent: "), false);
      debug.log(strMsg);
      delay(1000);
      digitalWrite(nLEDPin, LOW);
    }
    else
    {
      if (driver.recv(strMsg, strMsg.capacity()))
      {
        debug.log(F("Message received: "), false);
        debug.log(strMsg);
        if (strMsg.indexOf(F("T")) > -1)
          doTrigger();
      }
    }
  }
  SleepNow();
}

