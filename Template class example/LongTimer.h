#ifndef __LONG_TIMER__
#define __LONG_TIMER__




#include "Common.h"



typedef void (*GlobalFunctionPtrType)(void*);
class CProgramStation;
typedef void (*GlobalStationFunctionPtrType)(CProgramStation* pStation);




template <class function_ptr, class arg_type>
class CLongTimer
{
  public:
    // Construction, destruction & initilisation
    CLongTimer();
    CLongTimer(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const arg_type Arg, const char* strOwnerName, const uint8_t nOwnerID);
    CLongTimer(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const char* strOwnerName, const uint8_t nOwnerID);
    CLongTimer(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const arg_type Arg);
    CLongTimer(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc);
    ~CLongTimer();
    void set(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const arg_type Arg, const char* strOwnerName, const uint8_t nOwnerID);
    void set(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const arg_type Arg);
    void set(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const char* strOwnerName, const uint8_t nOwnerID);
    void set(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc);
    void setInterval(const uint32_t nIntervalSec)
    {
      m_nIntervalMillis = nIntervalSec * 1000;
    }
    void setRepetative(const bool bRepetative)
    {
      m_bRepetative = bRepetative;
    }

    int32_t poll();
    void restart(const bool bRepetative);
    bool isRunning()
    {
      return m_bRepetative || (m_nTrigger > 0);
    }
    int32_t getRemMillis()
    {
      return m_nRemMills;
    }
    char* getRemTime();

    const char* getOwnerName()
    {
      return m_strOwnerName;
    }

    int8_t getOwnerID()
    {
      return m_nOwnerID;
    }

    void setOwner(int8_t nOwnerID, const char* strOwnerName);

    void clearOwner();

  protected:
    // Data
    uint32_t m_nIntervalMillis, m_nTrigger;
    int32_t m_nRemMills;
    bool m_bRepetative;
    static const uint8_t m_nSize = 17;
    char m_strOwnerName[m_nSize];
    int8_t m_nOwnerID;
    function_ptr m_pCallBackFunc;
    arg_type m_Arg;
};




template<class function_ptr, class arg_type>CLongTimer<function_ptr, arg_type>::CLongTimer()
{
  m_nTrigger = m_nIntervalMillis = 0;
  m_bRepetative = false;
  m_pCallBackFunc = NULL;
  m_Arg = NULL;
  m_nRemMills = 0;
  m_nOwnerID = 0;
  memset(m_strOwnerName, 0, m_nSize);
}

template<class function_ptr, class arg_type>CLongTimer<function_ptr, arg_type>::CLongTimer(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const arg_type Arg, const char* strOwnerName, const uint8_t nOwnerID)
{
  set(nIntervalSec, bRepetative, pCallBackFunc, Arg, strOwnerName, nOwnerID);
}

template<class function_ptr, class arg_type>CLongTimer<function_ptr, arg_type>::CLongTimer(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const char* strOwnerName, const uint8_t nOwnerID)
{
  set(nIntervalSec, bRepetative, pCallBackFunc, strOwnerName, nOwnerID);
}

template<class function_ptr, class arg_type>CLongTimer<function_ptr, arg_type>::CLongTimer(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const arg_type Arg)
{
  set(nIntervalSec, bRepetative, pCallBackFunc, Arg);
}

template<class function_ptr, class arg_type>CLongTimer<function_ptr, arg_type>::CLongTimer(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc)
{
  set(nIntervalSec, bRepetative, pCallBackFunc);
}

template<class function_ptr, class arg_type>CLongTimer<function_ptr, arg_type>::~CLongTimer()
{
  m_nTrigger = 0;
}

template<class function_ptr, class arg_type>void CLongTimer<function_ptr, arg_type>::set(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const arg_type Arg)
{
  m_nIntervalMillis = nIntervalSec * 1000;
  m_nTrigger = millis() + m_nIntervalMillis;
  m_bRepetative = bRepetative;
  m_pCallBackFunc = pCallBackFunc;
  m_Arg = Arg;
}

template<class function_ptr, class arg_type>void CLongTimer<function_ptr, arg_type>::set(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const arg_type Arg, const char* strOwnerName, const uint8_t nOwnerID)
{
  memset(m_strOwnerName, 0, m_nSize);
  if (strOwnerName)
    strncpy(m_strOwnerName, strOwnerName, m_nSize);
  m_nOwnerID = nOwnerID;
  set(nIntervalSec, bRepetative, pCallBackFunc, Arg);
}

template<class function_ptr, class arg_type>void CLongTimer<function_ptr, arg_type>::set(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc, const char* strOwnerName, const uint8_t nOwnerID)
{
  set(nIntervalSec, bRepetative, pCallBackFunc, NULL, strOwnerName, nOwnerID);
}

template<class function_ptr, class arg_type>void CLongTimer<function_ptr, arg_type>::set(const uint32_t nIntervalSec, const bool bRepetative, const function_ptr pCallBackFunc)
{
  set(nIntervalSec, bRepetative, pCallBackFunc, NULL);
}

template<class function_ptr, class arg_type>char* CLongTimer<function_ptr, arg_type>::getRemTime()
{
  static char strRemTime[5] = {0, 0, 0, 0, 0};

  if (m_nRemMills > 0)
  {
    char strMin[9];
    memset(strMin, 0, 9);
    strcpy(strMin, fromUint(m_nRemMills / 60000, 10));
    strcat_P(strMin, (char*)F(" min"));
    strcpy(strRemTime, strMin);
  }
  else
  {
    strRemTime[0] = '\0';
  } 
  return strRemTime;
}

template<class function_ptr, class arg_type>void CLongTimer<function_ptr, arg_type>::restart(const bool bRepetative)
{
  m_bRepetative = bRepetative;
  m_nTrigger = millis() + m_nIntervalMillis;
}

template<class function_ptr, class arg_type>int32_t CLongTimer<function_ptr, arg_type>::poll()
{
  if (m_nTrigger > 0)
  {
    m_nRemMills = m_nTrigger - millis();
  
    if (m_nRemMills <= 0)
    {
      if (m_pCallBackFunc)
      {
        m_pCallBackFunc(m_Arg);
      }
      if (m_bRepetative)
        m_nTrigger = millis() + m_nIntervalMillis;
      else
        m_nTrigger = 0;
      
      m_nRemMills = 0;
    }
  }
  return m_nRemMills;
}

template<class function_ptr, class arg_type>void CLongTimer<function_ptr, arg_type>::setOwner(int8_t nOwnerID, const char* strOwnerName)
{
  m_nOwnerID = nOwnerID;
  strncpy(m_strOwnerName, strOwnerName, m_nSize);
}

template<class function_ptr, class arg_type>void CLongTimer<function_ptr, arg_type>::clearOwner()
{
  m_nOwnerID = -1;
  m_strOwnerName[0] = 0;
}





#endif

