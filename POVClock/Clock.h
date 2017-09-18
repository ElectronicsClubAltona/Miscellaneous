#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include <RealTimeClockDS1307.h>
#include <APA102.h>
#include "DisplayElem.h"




const uint8_t NUM_LEDS = 32;
const uint8_t MAX_LED = NUM_LEDS - 1;
const uint8_t DISPLAY_TIME = 0;
const uint8_t DISPLAY_DATE = 1;
const uint8_t DISPLAY_DATETIME = 2;




class CClock
{
    public:
    
        // Construction, destruction and initialisation
        CClock();   
        ~CClock();
        
        void setDateTime(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMon, const uint8_t nYear, const uint8_t nHr, const uint8_t nMin, const uint8_t nSec);
        void doDisplay();         
        
        // Display modes
        inline bool isDisplayDateTimeMode()
        {
            return m_nMode == DISPLAY_DATETIME;
        };
        
        inline bool isDisplayDateMode()
        {
            return m_nMode == DISPLAY_DATE;
        };
        
        inline bool isDisplayTimeMode()
        {
            return m_nMode == DISPLAY_TIME;
        };

        rgb_color getTextColor()
        {
           return m_colorText;
        };
       
        // Helper functions
        void point(const uint8_t nLEDNum, const rgb_color colour);
        void line(const uint8_t nStartLEDNum, const uint8_t nLength, const rgb_color colour);

    protected:
      
        // Helper functions
        char* formatNum(const unsigned int nNum);
        char* getFormattedTime(const bool bPadded = false, const bool bFromDateTime = false);
        char* getFormattedDate(const bool bPadded = false, const bool bFromDateTime = false);
        char* getFormattedDateTime(const bool bPadded = false);
               
        // Display clock parts
        void doDisplayRim(const uint16_t nAngleMin, const uint16_t nAngleRot);
        void doDisplaySecondHand(const uint16_t nAngleSec, const uint16_t nAngleRot);
        void doDisplayMinuteHand(const uint16_t nAngleMin, const uint16_t nAngleRot);
        void doDisplayHourHand(const uint16_t nAngleHour, const uint16_t nAngleRot);
        void doDisplayDivision(const uint16_t nAngleRot);
        
        // Data     
        uint8_t m_nMode; 
        CText m_text;
        RealTimeClockDS1307 m_RTC;
        static const char* m_arrayDOW[];
        
        // Create an ledStrip object and specify the pin it will use.
        APA102<21, 22> m_LEDStrip;

        // Create a buffer for holding the colors (3 bytes per color).
        rgb_color m_arrayLEDColors[NUM_LEDS];

        // Colours
        rgb_color m_colorDivision, m_colorRim, m_colorHour, m_colorMinute, m_colorSecond, m_colorText;
};

#endif






