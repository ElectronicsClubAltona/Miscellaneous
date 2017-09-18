#pragma once

#include "Arduino.h"




class CAlarm
{
	public:
		// Construction, destruction & initialisation
		CAlarm();
		virtual ~CAlarm();

		// Interface
    void soundIntruderAlarm(const uint16_t nSeconds, const bool bFlashLED, const uint16_t nFlashInt, const bool bVapor);
    void soundBatteryAlarm(const uint16_t nSeconds, const bool bFlashLED);
    bool checkBattery();

	protected:
    // Helpers
    uint8_t getAlarmVolume();
    
    // Data
    uint8_t m_nLEDAnode, m_nLEDCathode, m_nAlarmVolume, m_nVapourPin, m_nBatteryPin, m_nAlarmVolumePin;
};

extern CAlarm alarm;

