#include <Base64.h>
#include <APA102.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Clock.h"
#include "Time.h"
#include "debug.h"


const uint8_t nRotPhotoTransPin = 19;
const uint8_t nBTRxPin = 10;
const uint8_t nBTTxPin = 11;


CClock POVClock;
volatile uint32_t nTimeSlice = 0, nLastMicros = 0, nMicros = 0, nRotPeriod = 0;
WiFiUDP udp;
NTPClient ntp(udp, "0.au.pool.ntp.org", 10 * 60 * 60);
CTime Time;

hw_timer_t *pTimer = NULL;
volatile SemaphoreHandle_t semaphoreTimer;
portMUX_TYPE muxTimer = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer()
{
    // Increment the counter and set the time of ISR
    portENTER_CRITICAL_ISR(&muxTimer);
    portEXIT_CRITICAL_ISR(&muxTimer);
    
    // Give a semaphore that we can check in the loop
    xSemaphoreGiveFromISR(semaphoreTimer, NULL);

    Time.update();
    debug.log("Time: ", false);
    debug.log(ntp.getFormattedTime().c_str());
}

void setupTimer()
{
    // Create semaphore to inform us when the timer has fired
    semaphoreTimer = xSemaphoreCreateBinary();
  
    // Use 1st timer of 4 (counted from zero).
    // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
    // info).
    pTimer = timerBegin(0, 80, true);
  
    // Attach onTimer function to our timer.
    timerAttachInterrupt(pTimer, &onTimer, true);
  
    // Set alarm to call onTimer function every second (value in microseconds).
    // Repeat the alarm (third parameter)
    timerAlarmWrite(pTimer, 1000000, true);
  
    // Start an alarm
    timerAlarmEnable(pTimer);
}

void setup() 
{
    /******************************************************************
    Board	    int.0    int.1    int.2    int.3    int.4    int.5
    Uno, Ethernet       2        3	 	 	 	 
    Mega2560            2        3       21       20       19       18
    Leonardo            3        2        0        1        7
    *******************************************************************/
    attachInterrupt(0, doStartDisplay, RISING);  // Digital pin 2
    pinMode(nRotPhotoTransPin, INPUT);

    // 300, 600, 1200, 2400, 4800, *9600*, 14400, *19200*, 28800, *38400*, *57600*, or *115200*
    Serial.begin(115200);
/*
    WiFi.begin("Telstra6A61B7", "0AC9703D29");

    debug.log(F("Connecting to wifi network "), false);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    debug.log(F("succeeded!"));
    ntp.begin();
    ntp.update();
    Time.dow(ntp.getDay());
    Time.dom(ntp.getDate());
    Time.month(ntp.getMonth());
    Time.year(ntp.getYear() % 1000);
    Time.hour(ntp.getHours());
    Time.minute(ntp.getMinutes());
    Time.second(ntp.getSeconds());

    debug.log("Time: ", false);
    debug.log(ntp.getFormattedTime().c_str());
*/
    setupTimer();
}

void  doStartDisplay()
{
    long unsigned nMask = 0;    
    
    nMicros = micros();
    nRotPeriod = nMicros - nLastMicros;
    nLastMicros = nMicros;
    nMask = nRotPeriod >> 31;
    nRotPeriod = (nMask ^ nRotPeriod) - nMask;
} 

void loop() 
{
    POVClock.doDisplay();

    debug.log(F("nRotPhotoTransPin = "), false);
    debug.log(digitalRead(nRotPhotoTransPin));
    //delay(500);
}

