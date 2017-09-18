#include <Arduino.h>
#include "Common.h"
#include "Debug.h"




#define DEBUG 1
#define serialHC05 Serial2




CDebug::CDebug()
{
}

CDebug::~CDebug()
{
}

void CDebug::init()
{
}

void CDebug::highlight(const char cCh)
{
  #ifdef DEBUG
    for (uint8_t nI = 0; nI < 10; nI++)
      Serial.print(cCh);
  #elif defined DIAGNOSTIC
    for (uint8_t nI = 0; nI < 10; nI++)
      serialHC05.print(cCh);
  #endif
}

void CDebug::dump(const char cChar, const uint32_t nVal)
{
  #ifdef DEBUG
    highlight(cChar);
    Serial.print(nVal);
    highlight(cChar);
    Serial.println("");
  #elif defined DIAGNOSTIC
    highlight(cChar);
    serialHC05.print(nVal);
    highlight(cChar);
    serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char cChar, const int32_t nVal)
{
  #ifdef DEBUG
    highlight(cChar);
    Serial.print(nVal);
    highlight(cChar);
    Serial.println("");
  #elif defined DIAGNOSTIC
    highlight(cChar);
    serialHC05.print(nVal);
    highlight(cChar);
    serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char cChar, const uint16_t nVal)
{
  #ifdef DEBUG
    highlight(cChar);
    Serial.print(nVal);
    highlight(cChar);
    Serial.println("");
  #elif defined DIAGNOSTIC
    highlight(cChar);
    serialHC05.print(nVal);
    highlight(cChar);
    serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char cChar, const int16_t nVal)
{
  #ifdef DEBUG
    highlight(cChar);
    Serial.print(nVal);
    highlight(cChar);
    Serial.println("");
  #elif defined DIAGNOSTIC
    highlight(cChar);
    serialHC05.print(nVal);
    highlight(cChar);
    serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char cChar, const float fVal)
{
  dump(cChar, (double)fVal); 
}

void CDebug::dump(const char cChar, const double dVal)
{
  #ifdef DEBUG
    highlight(cChar);
    Serial.print(dVal);
    highlight(cChar);
    Serial.println("");
  #elif defined DIAGNOSTIC
    highlight(cChar);
    serialHC05.print(dVal);
    highlight(cChar);
    serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char cChar, const char* str)
{
  #ifdef DEBUG
    highlight(cChar);
    Serial.print(str);
    highlight(cChar);
    Serial.println("");
  #elif defined DIAGNOSTIC
    highlight(cChar);
    serialHC05.print(str);
    highlight(cChar);
    serialHC05.print("\r\n");
  #endif
}

#ifndef __SAM3X8E__
  void CDebug::dump(const char cChar, const __FlashStringHelper* str)
  {
    #ifdef DEBUG
      highlight(cChar);
      Serial.print(str);
      highlight(cChar);
      Serial.println("");
    #elif defined DIAGNOSTIC
      highlight(cChar);
      serialHC05.print(str);
      highlight(cChar);
      serialHC05.print("\r\n");
    #endif
  }
#endif

void CDebug::dump(const char cChar, const char cCharData)
{
  #ifdef DEBUG
    highlight(cChar);
    Serial.print(cCharData);
    highlight(cChar);
    Serial.println("");
  #elif defined DIAGNOSTIC
    highlight(cChar);
    serialHC05.print(cCharData);
    highlight(cChar);
    serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char cChar, IPAddress IPAddr)
{
  #ifdef DEBUG
    highlight(cChar);
    Serial.print(IPAddr);
    highlight(cChar);
    Serial.println("");
  #elif defined DIAGNOSTIC
    highlight(cChar);
    serialHC05.print(IPAddr);
    highlight(cChar);
    serialHC05.print("\r\n");
  #endif
}

#ifndef __SAM3X8E__
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint32_t nVal, const bool bNewLine)
  {  
    #ifdef DEBUG
      Serial.print(strVarName);
      Serial.print(F(" = "));
      Serial.print(nVal);
      if (bNewLine)
        Serial.println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int32_t nVal, const bool bNewLine)
  {
    #ifdef DEBUG
      Serial.print(strVarName);
      Serial.print(F(" = "));
      Serial.print(nVal);
      if (bNewLine)
        Serial.println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
   #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint16_t nVal, const bool bNewLine)
  {  
    #ifdef DEBUG
      Serial.print(strVarName);
      Serial.print(F(" = "));
      Serial.print(nVal);
      if (bNewLine)
        Serial.println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
   #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int16_t nVal, const bool bNewLine)
  {
    #ifdef DEBUG
      Serial.print(strVarName);
      Serial.print(F(" = "));
      Serial.print(nVal);
      if (bNewLine)
        Serial.println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint8_t nVal, const bool bNewLine)
  {  
    #ifdef DEBUG
      Serial.print(strVarName);
      Serial.print(F(" = "));
      Serial.print(nVal);
      if (bNewLine)
        Serial.println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int8_t nVal, const bool bNewLine)
  {
    #ifdef DEBUG
      Serial.print(strVarName);
      Serial.print(F(" = "));
      Serial.print(nVal);
      if (bNewLine)
        Serial.println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }

#endif

void CDebug::dump(const char* strVarName, const uint32_t nVal, const bool bNewLine)
{  
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const int32_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const uint16_t nVal, const bool bNewLine)
{  
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const int16_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const uint8_t nVal, const bool bNewLine)
{  
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const int8_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const __FlashStringHelper* strVarName, const float fVal, const bool bNewLine)
{
  dump(strVarName, (double)fVal, bNewLine);
}

void CDebug::dump(const __FlashStringHelper* strVarName, const double dVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(dVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(dVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const char* str, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(str);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(str);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const __FlashStringHelper* str, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(str);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(str);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const __FlashStringHelper* strVarName, const char* str, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(str);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(str);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

#ifndef __SAM3X8E__
  void CDebug::dump(const __FlashStringHelper* strVarName, const __FlashStringHelper* str, const bool bNewLine)
  {
    #ifdef DEBUG
      Serial.print(strVarName);
      Serial.print(F(" = "));
      Serial.print(str);
      if (bNewLine)
        Serial.println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(str);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
#endif

void CDebug::dump(const __FlashStringHelper* strVarName, const char cChar, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(cChar);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(cChar);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const __FlashStringHelper* strVarName, IPAddress IPAddr, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(strVarName);
    Serial.print(F(" = "));
    Serial.print(IPAddr);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(IPAddr);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const uint16_t nVal, const bool bNewLine)
{  
  #ifdef DEBUG
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const int16_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const float fVal, const bool bNewLine)
{
  log((double)fVal, bNewLine);
}

void CDebug::log(const double dVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(dVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(dVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const char* str, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(str);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(str);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

#ifndef __SAM3X8E__
  void CDebug::log(const __FlashStringHelper* str, const bool bNewLine)
  {
    #ifdef DEBUG
      Serial.print(str);
      if (bNewLine)
        Serial.println();
    #elif defined DIAGNOSTIC
      serialHC05.print(str);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
#endif

void CDebug::log(const char cChar, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(cChar);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(cChar);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const uint8_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const int8_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const uint32_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const int32_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(nVal);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(IPAddress IPAddr, const bool bNewLine)
{
  #ifdef DEBUG
    Serial.print(IPAddr);
    if (bNewLine)
      Serial.println();
  #elif defined DIAGNOSTIC
    serialHC05.print(IPAddr);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

#ifdef __SAM3X8E__
  void CDebug::logRuntimeError(const char* strFileName, const int16_t nLineNum)
  {
    #ifdef DEBUG
      Serial.print(F("RUNTIME ERROR: "));
      Serial.print(strFileName);
      Serial.print(F(" line number "));
      Serial.println(nLineNum);  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("RUNTIME ERROR: "));
      serialHC05.print(strFileName);
      serialHC05.print(F(" line number "));
      serialHC05.print(nLineNum);
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileFoundError(const char* strFileName)
  {
    #ifdef DEBUG
      Serial.print(F("File '"));
      Serial.print(strFileName);
      Serial.println(F("' was not found!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' was not found!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileFoundError(const __FlashStringHelper* strFileName)
  {
    #ifdef DEBUG
      Serial.print(F("File '"));
      Serial.print(strFileName);
      Serial.println(F("' was not found!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' was not found!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileOpenError(const char* strFileName)
  {
    #ifdef DEBUG
      Serial.print(F("File '"));
      Serial.print(strFileName);
      Serial.println(F("' could not be opened!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' could not be opened!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileOpenError(const __FlashStringHelper* strFileName)
  {
    #ifdef DEBUG
      Serial.print(F("File '"));
      Serial.print(strFileName);
      Serial.println(F("' could not be opened!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' could not be opened!"));  
      serialHC05.print("\r\n");
    #endif
  }
#else
  void CDebug::logRuntimeError(const __FlashStringHelper* strFileName, const int16_t nLineNum)
  {
    #ifdef DEBUG
      Serial.print(F("RUNTIME ERROR: "));
      Serial.print(strFileName);
      Serial.print(F(" line number "));
      Serial.println(nLineNum);  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("RUNTIME ERROR: "));
      serialHC05.print(strFileName);
      serialHC05.print(F(" line number "));
      serialHC05.print(nLineNum);
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileFoundError(const char* strFileName)
  {
    #ifdef DEBUG
      Serial.print(F("File '"));
      Serial.print(strFileName);
      Serial.println(F("' was not found!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' was not found!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileFoundError(const __FlashStringHelper* strFileName)
  {
    #ifdef DEBUG
      Serial.print(F("File '"));
      Serial.print(strFileName);
      Serial.println(F("' was not found!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' was not found!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileOpenError(const char* strFileName)
  {
    #ifdef DEBUG
      Serial.print(F("File '"));
      Serial.print(strFileName);
      Serial.println(F("' could not be opened!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' could not be opened!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileOpenError(const __FlashStringHelper* strFileName)
  {
    #ifdef DEBUG
      Serial.print(F("File '"));
      Serial.print(strFileName);
      Serial.println(F("' could not be opened!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' could not be opened!"));  
      serialHC05.print("\r\n");
    #endif
  }
#endif

CDebug debug;


