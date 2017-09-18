/*
  CString.cpp - Lightweight printable string class
  Copyright (c) 2009 Mikal Hart.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/




#include <Arduino.h>
#include <MyBase64.h>
#include "CString.h"
#include "Common.h"
#include "debug.h"




//****************************************************************
//* Construction and destuction
//****************************************************************

// So you can declare a global CString variable, but make sure you call init(...) before using it.
CString::CString()
{ 
  m_strBuff = m_strEndPtr = m_strStartPtr = NULL;
  m_nBuffSize = 0;
}

void CString::init(char *strBuff, uint16_t nSize)
{ 
  m_strBuff = strBuff;
  m_nBuffSize = nSize;
  m_strEndPtr = m_strBuff + strlen(m_strBuff) - 1;
  m_strStartPtr = m_strBuff;
  memset(m_strBuff, 0, nSize);
}

CString::CString(CString& strOther):  m_strBuff(strOther.m_strBuff), m_nBuffSize(strOther.m_nBuffSize)
{
  m_strEndPtr = m_strBuff + strlen(m_strBuff) - 1;
  m_strStartPtr = m_strBuff;
}

//****************************************************************
//* Concatenation operator functions
//****************************************************************
void CString::operator +=(const char cCh)
{
  uint16_t nLen = strlen(m_strBuff);
  if ((nLen + 2) <= m_nBuffSize)
  {
	  m_strBuff[nLen] = cCh;
	  m_strBuff[nLen + 1] = 0;
  }
}
  
void CString::operator +=(CString& str)
{
  if ((strlen(m_strBuff) + str.length() + 1) <= m_nBuffSize)
    strcat(m_strBuff, str.c_str());
}
  
void CString::operator +=(const char* str)
{
  if ((strlen(m_strBuff) + strlen(str) + 1) <= m_nBuffSize)
    strcat(m_strBuff, str);
}
  
#ifndef __SAM3X8E__
  void CString::operator +=(const __FlashStringHelper* str)
  {
    if ((strlen(m_strBuff) + strlen_P((PGM_P)str) + 1) <= m_nBuffSize)
      strcat_P(m_strBuff, (PGM_P)str);
  }
#endif
  
//****************************************************************
//* Assignment operator functions
//****************************************************************
CString& CString::operator =(const char cCh)
{
  if (m_nBuffSize >= 2)
  {
    m_strBuff[0] = cCh;
    m_strBuff[1] = 0;
  }
  return *this;
}
  
CString& CString::operator =(const char* str)
{
  if ((strlen(str) + 1) <= m_nBuffSize)
  {
    strncpy(m_strBuff, str, m_nBuffSize);
  }
  return *this;
}
  
CString& CString::operator =(CString& str)
{
  if ((strlen(str) + 1) <= m_nBuffSize)
    strncpy(m_strBuff, str.c_str(), m_nBuffSize);
  return *this;
}
  
CString& CString::operator =(IPAddress& ip)
{
  if (m_nBuffSize >= 16)
  {
    char *strTemp = new char[16], strDot[2] = ".";
    memset(strTemp, 0, 16);
    
    strcat(strTemp, fromUint(ip[0]));
    strcat(strTemp, strDot);
    strcat(strTemp, fromUint(ip[1]));
    strcat(strTemp, strDot);
    strcat(strTemp, fromUint(ip[2]));
    strcat(strTemp, strDot);
    strcat(strTemp, fromUint(ip[3]));
  
    if (strlen(strTemp) < capacity())
      *this = strTemp;
    delete strTemp;
  }
  return *this;
}

#ifndef __SAM3X8E__
  CString& CString::operator =(const __FlashStringHelper* str)
  {
    if ((strlen_P((PGM_P)str) + 1) <= m_nBuffSize)
      strncpy_P(m_strBuff, (PGM_P)str, m_nBuffSize);
    return *this;
  }
#endif
  
//****************************************************************
//* Comparaison operator functions
//****************************************************************
bool CString::operator ==(const char *str) 
{ 
  return (m_nBuffSize > 0) && (strcmp(m_strBuff, str) == 0); 
}

#ifndef __SAM3X8E__
  bool CString::operator ==(const __FlashStringHelper *str) 
  { 
    return (m_nBuffSize > 0) && (strcmp_P(m_strBuff, (PGM_P)str) == 0); 
  }
#endif

bool CString::operator >=(const char *str)
{
  return (m_nBuffSize > 0) && (strcmp(m_strBuff, str) >= 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator >=(const __FlashStringHelper *str)
  {
    return (m_nBuffSize > 0) && (strcmp_P(m_strBuff, (PGM_P)str) >= 0); 
  }
#endif

bool CString::operator >(const char *str)
{
  return (m_nBuffSize > 0) && (strcmp(m_strBuff, str) > 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator >(const __FlashStringHelper *str)
  {
    return (m_nBuffSize > 0) && (strcmp_P(m_strBuff, (PGM_P)str) > 0); 
  }
#endif

bool CString::operator <=(const char *str)
{
  return (m_nBuffSize > 0) && (strcmp(m_strBuff, str) <= 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator <=(const __FlashStringHelper *str)
  {
    return (m_nBuffSize > 0) && (strcmp_P(m_strBuff, (PGM_P)str) <= 0); 
  }
#endif

bool CString::operator <(const char *str)
{
  return (m_nBuffSize > 0) && (strcmp(m_strBuff, str) < 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator <(const __FlashStringHelper *str)
  {
    return (m_nBuffSize > 0) && (strcmp_P(m_strBuff, (PGM_P)str) < 0); 
  }
#endif

bool CString::operator !=(const char *str)
{
  return (m_nBuffSize > 0) && (strcmp(m_strBuff, str) != 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator !=(const __FlashStringHelper *str)
  {
    return (m_nBuffSize > 0) && (strcmp_P(m_strBuff, (PGM_P)str) != 0); 
  }
#endif

//****************************************************************
//* Miscellaneous functions
//****************************************************************

const char* CString::substring(uint16_t nStartPos, uint16_t nEndPos)
{
  static char *str = NULL;
  uint16_t nI = 0, nSize = 0;

  if ((nEndPos >= nStartPos) && (nStartPos >= 0) && (nStartPos < strlen(m_strBuff)) && (nEndPos >= 0) && (nEndPos <= strlen(m_strBuff)))
  {
    if (str)
    {
      delete str;
      str = NULL;
    }
    nSize = nEndPos - nStartPos;
    str = new char[nSize + 2];
    memset(str, 0, nSize + 2);
  
    if (nEndPos == strlen(m_strBuff))
      nEndPos = strlen(m_strBuff) - 1;
      
    m_strStartPtr = m_strBuff + nStartPos;
    m_strEndPtr = m_strBuff + nEndPos;

    while (m_strStartPtr <= m_strEndPtr)
    {
      str[nI++] = *m_strStartPtr;
      m_strStartPtr++;
    }
  }
  else
    str[0] = 0;
    
  return str;
}

//****************************************************************
//* Search functions functions
//****************************************************************

int16_t CString::indexOf(const char cCh, const int16_t nPos)
{
  int16_t nI = 0;
	  
  m_strStartPtr = m_strBuff;
  if (nPos < length())
  {
    m_strStartPtr = m_strBuff + nPos;
    nI = nPos;
  }
  while ((m_strBuff[nI] != cCh) && (nI < m_nBuffSize) && (nI < strlen(m_strBuff)))
	  nI++;
  if ((nI >= strlen(m_strBuff)) && (nI < m_nBuffSize))
	  nI = -1;
  
  return nI;
}
 
int16_t CString::indexOf(const char* str, const int16_t nPos)
{
  int16_t nI = -1;
	  
  m_strStartPtr = m_strBuff;
  if (nPos < length())
    m_strStartPtr = m_strBuff + nPos;

  m_strStartPtr = strstr(m_strStartPtr, str);
  if (m_strStartPtr)
	  nI = m_strStartPtr - m_strBuff;
  
  return nI;
}

#ifndef __SAM3X8E__
  int16_t CString::indexOf(const __FlashStringHelper* str, const int16_t nPos)
  {
    int16_t nI = -1;
  	  
    m_strStartPtr = m_strBuff;
    if (nPos < length())
      m_strStartPtr = m_strBuff + nPos;
        
    m_strStartPtr = strstr_P(m_strStartPtr, (PGM_P)str);
    if (m_strStartPtr)
  	  nI = m_strStartPtr - m_strBuff;
    
    return nI;
  }
#endif

int16_t CString::reverseIndexOf(const char cCh, const int16_t nPos)
{
  int16_t nI = 0;

  if (nPos == -1)
    nI = strlen(m_strBuff) - 1;
  else
    nI = nPos;
    
  while ((m_strBuff[nI] != cCh) && (nI >= 0))
	  nI--;
  
  return nI;
}

int16_t CString::reverseIndexOf(const char* str, const int16_t nPos)
{
  int16_t nI = -1;
  
  if ((nPos >= 0) && (nPos < length()))
    m_strStartPtr = m_strBuff + nPos;
  else
    m_strStartPtr = m_strBuff + strlen(m_strBuff) - 1;
	  
  while (m_strStartPtr >= m_strBuff)
  {
	  if (strstr(m_strStartPtr, str) != NULL)
	  {
		  nI = m_strStartPtr - m_strBuff;
		  break;
	  }
	  m_strStartPtr--;
  }
  return nI;
}

#ifndef __SAM3X8E__
  int16_t CString::reverseIndexOf(const __FlashStringHelper* str, const int16_t nPos)
  {
    int16_t nI = -1;
    
    if ((nPos >= 0) && (nPos < length()))
      m_strStartPtr = m_strBuff + nPos;
    else
      m_strStartPtr = m_strBuff + strlen(m_strBuff) - 1;
  	  
    while (m_strStartPtr >= m_strBuff)
    {
  	  if (strstr_P(m_strStartPtr, (PGM_P)str) != NULL)
  	  {
  		  nI = m_strStartPtr - m_strBuff;
  		  break;
  	  }
  	  m_strStartPtr--;
    }
    return nI;
  }
#endif
  
//****************************************************************
//* Formatting and conversion functions
//****************************************************************
void CString::format(const char *str, ...) 
{ 
  va_list argptr;  
  va_start(argptr, str); 
  int nRet = vsnprintf(m_strBuff, m_nBuffSize - 1, str, argptr);
  if (nRet == 0)
    debug.logRuntimeError(F("CString.cpp"), __LINE__);
}

const char* CString::fromBool(const bool bVal)
{
  static char strTemp[6] = {0,0,0,0,0,0};

  if (m_nBuffSize >= 6)
  {
    memset(strTemp, 0, 6);
    if (bVal)
      strcpy_P(strTemp, (PGM_P)F("true"));
    else
      strcpy_P(strTemp, (PGM_P)F("false"));
  } 
  return strTemp;
}

void CString::convertHex2Bin(char *strHex, const uint8_t nCapacity)
{
  char strTemp[nCapacity];

  memset(strTemp, 0, nCapacity);
  for (uint16_t nI = 0; nI < strlen(strHex); nI++)
  {
  	if ((strlen(strTemp) + 4) <= nCapacity)
  	{
  	  switch (strHex[nI])
  	  {
  	    case '0': strcat_P(strTemp, (PGM_P)F("0000")); break;
  	    case '1': strcat_P(strTemp, (PGM_P)F("0001")); break;
  	    case '2': strcat_P(strTemp, (PGM_P)F("0010")); break;
  	    case '3': strcat_P(strTemp, (PGM_P)F("0011")); break;
  	    case '4': strcat_P(strTemp, (PGM_P)F("0100")); break;
  	    case '5': strcat_P(strTemp, (PGM_P)F("0101")); break;
  	    case '6': strcat_P(strTemp, (PGM_P)F("0110")); break;
  	    case '7': strcat_P(strTemp, (PGM_P)F("0111")); break;
  	    case '8': strcat_P(strTemp, (PGM_P)F("1000")); break;
  	    case '9': strcat_P(strTemp, (PGM_P)F("1001")); break;
  	    case 'A':  
  	    case 'a': strcat_P(strTemp, (PGM_P)F("1010")); break;
  	    case 'B':  
  	    case 'b': strcat_P(strTemp, (PGM_P)F("1011")); break;
  	    case 'C':  
  	    case 'c': strcat_P(strTemp, (PGM_P)F("1100")); break;
  	    case 'D':  
  	    case 'd': strcat_P(strTemp, (PGM_P)F("1101")); break;
  	    case 'E':  
  	    case 'e': strcat_P(strTemp, (PGM_P)F("1110")); break;
        case 'F':  
  		  case 'f': strcat_P(strTemp, (PGM_P)F("1111")); break;
  	  };
  	}
	}
  strncpy(strHex, strTemp, nCapacity);
}

const char* CString::fromUint(const uint32_t nNum, const uint8_t nBase)
{
  if (((uint16_t)log10(nNum) + 1) <= m_nBuffSize)
  {
    memset(m_strBuff, 0, m_nBuffSize);
    utoa(nNum, m_strBuff, nBase);
  }
  return m_strBuff;
}

const char* CString::fromInt(const int32_t nNum, const uint8_t nBase)
{
  if (((uint16_t)log10(nNum) + 2) <= m_nBuffSize)
  {
    memset(m_strBuff, 0, m_nBuffSize);
    itoa(nNum, m_strBuff, nBase);
  }
  return m_strBuff;
}

const char* CString::fromReal(const double dNum, const uint8_t nDecimalPlaces)
{
  if (((uint16_t)log10(floor(dNum)) + nDecimalPlaces + 2) <= m_nBuffSize)
  {
    memset(m_strBuff, 0, m_nBuffSize);
  
    #ifdef __SAM3X8E__
      dtostrf(dNum, (nDecimalPlaces + 2), nDecimalPlaces, m_strBuff);
    #else
      dtostrf(dNum, (nDecimalPlaces + 2), nDecimalPlaces, m_strBuff);
    #endif
  }
  return m_strBuff;
}

uint32_t CString::toUint(const uint8_t nBase)
{
	return ::toUint(m_strBuff, nBase);
}

int32_t CString::toInt(const uint8_t nBase)
{
	return ::toInt(m_strBuff, nBase);
}

double CString::toReal(const uint8_t nBase)
{
	return ::toReal(m_strBuff, nBase);
}

//****************************************************************
//* Modification functions
//****************************************************************

void CString::encodeBase64()
{
  char *strTempBuff = new char[strlen(m_strBuff) * 2];
  memset(strTempBuff, 0, strlen(m_strBuff) * 2);
  base64_decode(strTempBuff, m_strBuff, m_nBuffSize - 1);
  *this = strTempBuff;
  delete strTempBuff;
}

void CString::decodeBase64()
{
  char *strTempBuff = new char[strlen(m_strBuff)];
  memset(strTempBuff, 0, strlen(m_strBuff));
  base64_decode(strTempBuff, m_strBuff, m_nBuffSize - 1);
  *this = strTempBuff;
  delete strTempBuff;
}

void CString::insert(const char cCh, const uint16_t nBeforePos)
{
  char str[2] = {cCh, 0};
  insert(str, nBeforePos);
}

#ifndef __SAM3X8E__
  void CString::insert(const __FlashStringHelper* str, const uint16_t nBeforePos)
  {
    uint16_t nLen = strlen_P((PGM_P)str) + 1;
    char* strTemp = new char[nLen];
    memset(strTemp, 0, nLen);
    insert(strTemp, nBeforePos);
    delete strTemp;
  }
#endif

void CString::insert(const char* str, const uint16_t nBeforePos)
{
  char* strTemp = NULL;
  uint16_t nCopyLength = 0, nLength = 0;
  
  if (((strlen(m_strBuff) + strlen(str) + 1) <= m_nBuffSize) && (nBeforePos >= 0) && (nBeforePos < strlen(m_strBuff)))
  {
    m_strStartPtr = m_strBuff + nBeforePos;
    nCopyLength = strlen(m_strBuff) - (m_strStartPtr - m_strBuff) + 1;
    nLength = strlen(m_strBuff);
    strTemp = new char[nCopyLength];
    memset(strTemp, 0, nCopyLength);
    memmove(strTemp, m_strStartPtr, nCopyLength);
    *m_strStartPtr = 0;
    strcat(m_strBuff, str);
    strcat(m_strBuff, strTemp);
    m_strBuff[nLength + strlen(str)] = 0;
    delete strTemp;
  }
}

void CString::replace(const char cFind, const char cReplace, const uint16_t nStartFrom)
{
  for (uint16_t nI = nStartFrom, nLen = strlen(m_strBuff); nI < nLen; nI++)
  {
    if (m_strBuff[nI] == cFind)
    {
      m_strBuff[nI] = cReplace;
      break;
    }
  }
}

void CString::removeCh(const char cFind)
{
  char* strTempPtr = NULL;
  uint16_t nLengthMove = 0, nEnd = 0;
  
  for (m_strStartPtr = m_strBuff, m_strEndPtr = m_strBuff + strlen(m_strBuff) - 1; m_strStartPtr <= m_strEndPtr; m_strStartPtr++)
  {
    if (*m_strStartPtr == cFind)
    {
      strTempPtr = m_strStartPtr + 1;
      nLengthMove = strlen(m_strBuff) - (strTempPtr - m_strBuff);
      nEnd = strlen(m_strBuff) - 1;
      memmove(m_strStartPtr, strTempPtr, nLengthMove);
      m_strBuff[nEnd] = 0;
    }
  }
}

#ifndef __SAM3X8E__
  void CString::remove(const __FlashStringHelper* strFind)
  {
    char* strTemp = new char[strlen_P((char*)strFind) + 1];
    memset(strTemp, 0, strlen_P((char*)strFind) + 1);
    strcpy_P(strTemp, (char*)strFind);
    remove(strTemp);
    delete strTemp;
  }
#endif

void CString::remove(const char* strFind)
{
  uint16_t nLengthMove = 0, nLength = 0;
  
  while (m_strStartPtr = strstr(m_strBuff, strFind))
  {
    m_strEndPtr = m_strStartPtr + strlen(strFind);
    nLengthMove = strlen(m_strBuff) - (m_strEndPtr - m_strBuff);
    nLength = strlen(m_strBuff);
    memmove(m_strStartPtr, m_strEndPtr, nLengthMove);
    m_strBuff[nLength - strlen(strFind)] = 0;
  }
}

#ifndef __SAM3X8E__
  void CString::replace(const __FlashStringHelper* strFind, const __FlashStringHelper* strReplace, const uint16_t nStartFrom)
  {
    char *strTemp1 = new char[strlen_P((char*)strFind)],
         *strTemp2 = new char[strlen_P((char*)strReplace)];
    
    memset(strTemp1, 0, strlen_P((char*)strFind));
    memset(strTemp2, 0, strlen_P((char*)strReplace));
    strcpy_P(strTemp1, (char*)strFind);
    strcpy_P(strTemp2, (char*)strReplace);
    replace(strTemp1, strTemp2, nStartFrom);
    delete strTemp1;
    delete strTemp2;
  }

  void CString::replace(const char* strFind, const __FlashStringHelper* strReplace, const uint16_t nStartFrom)
  {
    char* strTemp = new char[strlen_P((char*)strReplace) + 1];
    
    memset(strTemp, 0, strlen_P((char*)strReplace) + 1);
    strcpy_P(strTemp, (char*)strReplace);
    replace(strFind, strTemp, nStartFrom);
    delete strTemp;
  }

  void CString::replace(const __FlashStringHelper* strFind, const char* strReplace, const uint16_t nStartFrom)
  {
    char* strTemp = new char[strlen_P((char*)strFind) + 1];
    
    memset(strTemp, 0, strlen_P((char*)strFind) + 1);
    strcpy_P(strTemp, (char*)strFind);
    replace(strTemp, strReplace, nStartFrom);
    delete strTemp;
  }
#endif

void CString::replace(const char* strFind, const char* strReplace, const uint16_t nStartFrom)
{
  uint16_t nLengthMove = 0, nNewLength = 0;
  char *strTempBuff = NULL;

  if ((strlen(m_strBuff) - strlen(strFind) + strlen_P((PGM_P)strReplace) + 1) <= m_nBuffSize)
  {
    if (nStartFrom < strlen(m_strBuff))
    {
      m_strStartPtr = m_strBuff + nStartFrom;
      m_strEndPtr = strstr(m_strStartPtr, strFind);
  
      if (m_strEndPtr)
      {
        nNewLength = strlen(m_strBuff) + strlen(strReplace) - strlen(strFind);
  
        if (nNewLength < m_nBuffSize)
        {
          strTempBuff = new char[nNewLength + 1];
          memset(strTempBuff, 0, nNewLength + 1);
          strncpy(strTempBuff, m_strBuff, m_strEndPtr - m_strStartPtr);
          strcat(strTempBuff, strReplace);
          strcat(strTempBuff, m_strEndPtr + strlen(strFind));
          memset(m_strBuff, 0, m_nBuffSize);
          strcpy(m_strBuff, strTempBuff);
          delete strTempBuff;
        }
      }
    }
  }
}

void CString::remove(const uint16_t nPos, uint16_t nCount)
{
  if ((nPos >= 0) && (nPos < strlen(m_strBuff)) && ((nPos + nCount) <= strlen(m_strBuff)))
  {
    m_strStartPtr = m_strBuff + nPos;
    m_strEndPtr = m_strBuff + nPos + nCount;
    uint8_t nLength = strlen(m_strBuff), nLengthMove = strlen(m_strBuff) - (m_strEndPtr - m_strBuff) + 1;
    memmove(m_strStartPtr, m_strEndPtr, nLengthMove);
    m_strBuff[nLength - nCount] = 0;
  }
}

void CString::toLower()
{
  for (uint16_t nI = 0; nI < strlen(m_strBuff); nI++)
  {
    if ((m_strBuff[nI] >= 'A') && (m_strBuff[nI] <= 'Z'))
      m_strBuff[nI] += 32;
  }
}

void CString::toUpper()
{
  for (uint16_t nI = 0; nI < strlen(m_strBuff); nI++)
  {
    if ((m_strBuff[nI] >= 'a') && (m_strBuff[nI] <= 'z'))
      m_strBuff[nI] -= 32;
  }
}

void CString::concat(const char *strStart, const char* strEnd, char* strDest, const uint16_t nSize)
{
	uint16_t nI = strlen(strDest);

	while ((strStart <= strEnd) && (nI < nSize))
	{
		strDest[nI] = strStart[0];
		strDest[nI + 1] = 0;
		strStart++;
		nI++;
	}
}

void CString::copy(const char *strStart, const char* strEnd, char* strDest, const uint16_t nSize)
{
  uint16_t nI = 0;

  memset(strDest, 0, nSize);
  while ((strStart <= strEnd) && (nI < nSize))
  {
    strDest[nI] = *strStart;
    strStart++;
    nI++;
  }
}

void CString::trimLeft()
{
  m_strStartPtr = m_strBuff;
  m_strEndPtr = m_strBuff + strlen(m_strBuff) - 1;
  while (isWhitespace(*m_strStartPtr) && (m_strStartPtr <= m_strEndPtr))
	  m_strStartPtr++;

  uint16_t nMoveLength = m_strEndPtr - m_strStartPtr + 1;
  memmove(m_strBuff, m_strStartPtr, nMoveLength);
  m_strBuff[nMoveLength] = 0;
}

void CString::trimRight()
{
  m_strStartPtr = m_strBuff;
  m_strEndPtr = m_strBuff + strlen(m_strBuff) - 1;
  while (isWhitespace(*m_strEndPtr) && (m_strEndPtr >= m_strStartPtr))
	  m_strEndPtr--;
  m_strEndPtr++;
  *m_strEndPtr = 0;
}

void CString::trim()
{
  trimLeft();
  trimRight();
}

void CString::padLeft(const char cCh, const uint8_t nLength)
{
  while ((strlen(m_strBuff) < nLength) && (strlen(m_strBuff) < m_nBuffSize))
  {
    insert(cCh, 0); 
  }
}

void CString::padRight(const char cCh, const uint8_t nLength)
{
  while ((strlen(m_strBuff) < nLength) && (strlen(m_strBuff) < m_nBuffSize))
  {
    m_strBuff[strlen(m_strBuff)] = cCh;
    m_strBuff[strlen(m_strBuff) + 1] = 0;
  }
  
}






//#define DTOSTR_ALWAYS_SIGN 0x01
//#define DTOSTR_PLUS_SIGN   0x02
//#define DTOSTR_UPPERCASE   0x04







