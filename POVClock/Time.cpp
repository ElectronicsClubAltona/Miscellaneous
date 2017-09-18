#include "Time.h"




CTime::CTime()
{
    m_nDOW = m_nDOM = m_nMonth = m_nHour = m_nMinute = m_nSecond = 0;
    m_nYear = 0;
}

CTime::~CTime()
{
}

uint8_t CTime::dow()
{
    return m_nDOW;
}

uint8_t CTime::dom()
{
    return m_nDOM;
}

uint8_t CTime::month()
{
    return m_nMonth;
}

uint16_t CTime::year()
{
    return m_nYear;
}

uint8_t CTime::hour()
{
    return m_nHour;
}

uint8_t CTime::minute()
{
    return m_nMinute;
}

uint8_t CTime::second()
{
    return m_nSecond;
}

void CTime::set(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHour, const uint8_t nMinute, const uint8_t nSecond)
{
    m_nDOW = nDOW;
    m_nDOM = nDOM;
    m_nMonth = nMonth;
    m_nHour = nHour;
    m_nMinute = nMinute;
    m_nSecond = nSecond;
    m_nYear = nYear;
}

void CTime::dow(const uint8_t nDOW)
{
    m_nDOW = nDOW;
}

void CTime::dom(const uint8_t nDOM)
{
    m_nDOM = nDOM;
}

void CTime::month(const uint8_t nMonth)
{
    m_nMonth = nMonth;
}

void CTime::year(const uint16_t nYear)
{
    m_nYear = nYear;
}

void CTime::hour(const uint8_t nHour)
{
    m_nHour = nHour;
}

void CTime::minute(const uint8_t nMinute)
{
    m_nMinute = nMinute;
}

void CTime::second(const uint8_t nSecond)
{
    m_nSecond = nSecond;
}

bool CTime::isLeapYear()
{
  return ((m_nYear % 4) == 0) || (((m_nYear % 100) == 0) && ((m_nYear % 400) == 0));
}

uint8_t CTime::calcDOW()
{
  static int arrayTable[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
   
  uint16_t nYear = m_nYear - m_nMonth < 3;

  return ((nYear + nYear / 4 - nYear / 100 + nYear / 400 + arrayTable[m_nMonth - 1] + m_nDOM) % 7);
}

uint8_t CTime::getMaxDOM()
{
  uint8_t nMaxDays = 0;
  bool bLeapYear = isLeapYear();

  switch (m_nMonth)
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12: nMaxDays = 31;
             break;
    case 4:
    case 6:
    case 9:
    case 11: nMaxDays = 30;
             break;
  case 2: 
            if (bLeapYear)
              nMaxDays = 29;
            else
              nMaxDays = 28;
  }
  return nMaxDays;
}

void CTime::update()
{
    m_nSecond++;
    if (m_nSecond > 59)
    {
        m_nSecond = 0;
        m_nMinute++;
        if (m_nMinute > 59)
        {
            m_nMinute = 0;
            m_nHour++;
            if (m_nHour > 23)
            {
                m_nHour = 0;
                m_nDOM++;
                if (m_nDOM > getMaxDOM())
                {
                    m_nDOM = 1;
                    m_nMonth++;
                    if (m_nMonth > 12)
                    {
                        m_nMonth = 1;
                        m_nYear++;
                    }
                }
            }
        }
    }
}



