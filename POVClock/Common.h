#ifndef __COMMON_H
#define __COMMON_H

#include "CString.h"




#ifndef __SAM3X8E__
  #define USE_PROGMEM true
#endif

typedef enum {SUN, MON, TUE, WED, THU, FRI, SAT} day_of_week;
typedef void (*GlobalFunctionPtrType)();

#define STRINGIFY(A) #A




/*
__AVR_ATmega168__   ATmega 168    Arduino Decimilia and older
__AVR_ATmega328P__  ATmega 328P   Arduino Duemilanove and Uno
__AVR_ATmega1280__  ATmega 1280   Arduino Mega
__AVR_ATmega2560__  ATmega 2560   Arduino Mega 2560
__AVR_ATmega32U4__  ATmega 32U4   Arduino Leonardo
__SAM3X8E__         AT91SAM3X8E   Arduino Due
*/




//*************************************************************************************************
// Replacement for SoftReset.h function for Due board
//*************************************************************************************************
      
void resetArd();
void resetESP();

#ifdef __SAM3X8E__
  char *dtostre(const double dVal, const uint8_t nWidth, const uint8_t nPrec, char *strOut);
  char *dtostrf(double dVal, const uint8_t nWidth, const uint8_t nPrec, char *strOut);
  #define F(STR) STR
  #define __FlashStringHelper char
#endif




//*************************************************************************************************
// Conversion functions
//*************************************************************************************************

const char* fromChar(const char cCh);
const char* fromUint(const uint32_t nNum, const uint8_t nBase);
const char* fromInt(const int32_t nNum, const uint8_t nBase);
const char* fromReal(const double dNum, const uint8_t nDecimalPlaces);

uint32_t toUint(const char *strNum, const uint8_t nBase);
int32_t toInt(const char *strNum, const uint8_t nBase);
double toReal(const char *strNum, const uint8_t nBase);

#endif


