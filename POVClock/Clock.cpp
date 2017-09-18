#include <Arduino.h>
#include "Clock.h"
#include "debug.h"

const char* CClock::m_arrayDOW[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

CClock::CClock()
{
  m_nMode = DISPLAY_TIME;
  m_colorDivision = rgb_color(0, 0, 255);
  m_colorRim = rgb_color(255, 0, 0);
  m_colorHour = rgb_color(0, 255, 0);
  m_colorMinute = rgb_color(0, 255, 0);
  m_colorSecond = rgb_color(0, 255, 0);
  m_colorText = rgb_color(255, 255, 255);
}

CClock::~CClock()
{
}

void CClock::setDateTime(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMon, const uint8_t nYear, const uint8_t nHr, const uint8_t nMin, const uint8_t nSec)
{
    m_RTC.switchTo24h();
    m_RTC.setHours(nHr);
    m_RTC.setMinutes(nMin);
    m_RTC.setSeconds(nSec);
    m_RTC.setYear(nYear);
    m_RTC.setMonth(nMon);
    m_RTC.setDate(nDOM);
    m_RTC.setDayOfWeek(nDOW);
    m_RTC.setClock();
    
}

void CClock::point(const uint8_t nLEDNum, const rgb_color colour) 
{
    if ((nLEDNum >= 0) && (nLEDNum < NUM_LEDS))
        m_arrayLEDColors[nLEDNum] = colour;
}

void CClock::line(const uint8_t nStartLEDNum, const uint8_t nLength, const rgb_color colour)
{
    uint8_t nI = 0, nEndLEDNum = nStartLEDNum + nLength;
    
    if ((nStartLEDNum >= 0) && (nStartLEDNum < NUM_LEDS) && (nEndLEDNum >= 0) && (nEndLEDNum < NUM_LEDS) && (nEndLEDNum > nStartLEDNum))
    {
        for (nI = nStartLEDNum; nI <= nEndLEDNum; nI++)
            m_arrayLEDColors[nI] = colour;
    }
}

void CClock::doDisplayRim(const uint16_t nAngleSec, const uint16_t nAngleRot)
{
    m_arrayLEDColors[MAX_LED] = m_colorRim;
}

void CClock::doDisplaySecondHand(const uint16_t nAngleSec, const uint16_t nAngleRot)
{
  /*
    if (nAngleRot == nAngleSec)
        line(2, NUM_LEDS - 3);
  */
}

void CClock::doDisplayMinuteHand(const uint16_t nAngleMin, const uint16_t nAngleRot)
{
  /*
    if (nAngleRot == nAngleMin)
        line(2, NUM_LEDS - 5);
    else if ((nAngleRot == (nAngleMin - 1)) || (nAngleRot == (nAngleMin + 1)))
        line(2, NUM_LEDS - 6);
    else if ((nAngleRot == (nAngleMin - 2)) || (nAngleRot == (nAngleMin + 2)))
        line(2, NUM_LEDS - 7);
  */
}

void CClock::doDisplayHourHand(const uint16_t nAngleHour, const uint16_t nAngleRot)
{
  /*
    if (nAngleRot == nAngleHour)
        line(2, NUM_LEDS - 7);
    else if ((nAngleRot == (nAngleHour - 1)) || (nAngleRot == (nAngleHour + 1)))
        line(2, NUM_LEDS - 8);
    else if ((nAngleRot == (nAngleHour - 2)) || (nAngleRot == (nAngleHour + 2)))
        line(2, NUM_LEDS - 9);
    else if ((nAngleRot == (nAngleHour - 3)) || (nAngleRot == (nAngleHour + 3)))
        line(2, NUM_LEDS - 10);
    else if ((nAngleRot == (nAngleHour - 4)) || (nAngleRot == (nAngleHour + 4)))
        line(2, NUM_LEDS - 11);
    else if ((nAngleRot == (nAngleHour - 5)) || (nAngleRot == (nAngleHour + 5)))
        line(2, NUM_LEDS - 12);
   */
}

void CClock::doDisplayDivision(const uint16_t nAngleRot)
{
    if ((nAngleRot % 30) == 0.0)
    {
        line(MAX_LED - 2, MAX_LED - 1, m_colorDivision);
    }
    else if ((nAngleRot % 60) == 0.0)
    {
        point(MAX_LED - 1, m_colorDivision);
    }
}

char* CClock::formatNum(const unsigned int nNum)
{
    static char strNum[3] = "\0\0";
    
    utoa(nNum, strNum, 10);
    if (strlen(strNum) == 1)
    {
        strNum[1] = strNum[0];
        strNum[0] = '0';
    }
    return strNum;
}

char* CClock::getFormattedTime(const bool bPadded, const bool bFromDateTime)
{
    static char strTime[12] = "\0\0\0\0\0\0\0\0\0\0\0";
    uint8_t nMaxLen = 0;
    
    m_RTC.switchTo24h();  
    m_RTC.readClock();

    if (bPadded)
        nMaxLen = 2;

    strcpy(strTime, formatNum(m_RTC.getHours()));
    strcat(strTime, ":");
    strcat(strTime, formatNum(m_RTC.getMinutes()));
    strcat(strTime, ":");
    strcat(strTime, formatNum(m_RTC.getSeconds()));

    return strTime;
}

char* CClock::getFormattedDateTime(const bool bPadded)
{
    static char strDateTime[26] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

    strcpy(strDateTime, getFormattedDate(bPadded, true));
    strcat(strDateTime, " ");
    strcat(strDateTime, getFormattedTime(bPadded, true));

    return strDateTime;
}

char* CClock::getFormattedDate(const bool bPadded, const bool bFromDateTime)
{
    static char strDate[17] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    
    m_RTC.switchTo24h();  
    m_RTC.readClock();

    strcpy(strDate, m_arrayDOW[m_RTC.getDayOfWeek()]);
    strcat(strDate, " ");
    strcat(strDate, formatNum(m_RTC.getDate()));
    strcat(strDate, "-"); 
    strcat(strDate, formatNum(m_RTC.getMonth()));
    strcat(strDate, "-");
    strcat(strDate, formatNum(m_RTC.getYear()));

    return strDate;
}

extern uint32_t nLastMicros, lunRotPeriod;

void CClock::doDisplay()
{
    m_RTC.switchTo24h();  
    m_RTC.readClock();

    static uint8_t nDisplayTimeCount = 1;
    uint32_t lunMicrosElapsed = micros() - nLastMicros;
    uint16_t nRotAngle = ((float)lunMicrosElapsed / lunRotPeriod) * 360,
			       nAngleSec = ((float)m_RTC.getSeconds() / 60) * 360,
             nAngleMin = ((float)(m_RTC.getMinutes() / 60) * 360) + (((float)m_RTC.getSeconds() / 60) * 30),
             nAngleHour = ((float)(m_RTC.getHours() / 12) * 360) + (((float)m_RTC.getMinutes() / 60) * 30) + (((float)m_RTC.getSeconds() / 60) * 6);
    
    
    if (isDisplayTimeMode())
    {
  	    //debug.log(getFormattedTime(true));
  		  //debug.log("nLastMicros = ", false);
  		  //debug.log(nLastMicros);
     
        //doDisplayRim(nDisplayTimeCount, m_arrayClockFields[SECOND].m_nFieldVal);
        //doDisplayHourHand(nRotAngle, nAngleHour + (nAngleMin * 30) + (nAngleSec * 6));
        //doDisplayMinuteHand(nRotAngle, nAngleMin + (nAngleSec * 6));
        //doDisplaySecondHand(nRotAngle, nAngleSec);
        //doDisplayDivision(nRotAngle);
    }
/*
    else if (isDisplayDateTimeMode())
    {
        m_text = getFormattedDateTime();
        m_text.doDisplay(this, nRotAngle);
	      debug.log(m_text);
    }
    else if (isDisplayDateMode())
    {
        m_text = getFormattedDate();
        m_text.doDisplay(this, nRotAngle);
	      debug.log(m_text);
    }
*/
    m_LEDStrip.write(m_arrayLEDColors, NUM_LEDS);
}













