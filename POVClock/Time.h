#pragma once


#include <Arduino.h>




class CTime
{
    public:
        // Construction & destruction
        CTime();
        virtual ~CTime();

        uint8_t dow();
        uint8_t dom();
        uint8_t month();
        uint16_t year();
        uint8_t hour();
        uint8_t minute();
        uint8_t second();
        void set(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHour, const uint8_t nMinute, const uint8_t nSecond);
        void dow(const uint8_t nDOW);
        void dom(const uint8_t nDOM);
        void month(const uint8_t nMonth);
        void year(const uint16_t nYear);
        void hour(const uint8_t nHour);
        void minute(const uint8_t nMinute);
        void second(const uint8_t nSecond);

        void update();
 
    protected:
        // Data
        volatile uint8_t m_nDOW, m_nDOM, m_nMonth, m_nHour, m_nMinute, m_nSecond;
        volatile uint16_t m_nYear;

        // Helpers
        bool isLeapYear();
        uint8_t calcDOW();
        uint8_t getMaxDOM();

};

