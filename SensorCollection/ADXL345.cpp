//CADXL345 accelerometer function library
//Barrett Anderies
//Sep. 15, 2012

#include <Wire.h>
#include "ADXL345.h"

//Construction, destruction and initialisation.

CADXL345::CADXL345(int32_t nSensorID): CSensor(nSensorID)
{
}

CADXL345::~CADXL345()
{
}

void CADXL345::begin(uint8_t nSDOPinNum, uint8_t nSDOPinState)
{
	CSensor::begin(nSDOPinNum, nSDOPinState);
	
	if (nSDOPinState == HIGH)
		setDevAddr(ADDR_SDO_HIGH, ADDR_SDO_HIGH);
	else if (nSDOPinState == LOW)
		setDevAddr(ADDR_SDO_LOW, ADDR_SDO_HI);
	else
	  Serial.println("CADXL345::begin(uint8_t nSDOPinNum, uint8_t nSDOPinState): '" + String(nSDOPinState) + "' is not a valid pin state for parameter 'nSDOPinState'...");

	setPOWER_CTL(POWER_MEASURE_2Hz);
	setDATA_FORMAT(RANGE_FULL_RES_8G);
	setBW_RATE(DATARATE_100_HZ);
	
	writeReg(BW_RATE, BW_RATE_DEFAULT);
	writeReg(POWER_CTL, POWER_CTL_DEFAULT);
	writeReg(DATA_FORMAT, DATA_FORMAT_DEFAULT);
	writeReg(FIFO_CTL, FIFO_CTL_DEFAULT);
	writeReg(TAP_AXES, TAP_AXES_DEFAULT);
	writeReg(INT_ENABLE, INT_DEFAULT);
	writeReg(INT_MAP, INT_DEFAULT);
	writeReg(ACT_INACT_CTL, ACT_INACT_CTL_DEFAULT);	
/*
	Serial.println("############################################");
	Serial.print("m_dOffsetX = ");
	Serial.println(m_dOffsetX);
	Serial.print("m_dOffsetY = ");
	Serial.println(m_dOffsetY);
	Serial.print("m_dOffsetZ = ");
	Serial.println(m_dOffsetZ);
	Serial.println("############################################");
*/
}

// Device settings.

void CADXL345::setPowerCtrl(bool bLink, bool bAutoSleep, bool bMeasure, bool bSleep, e_sleep_data_rate eSleepDataRate)
{
	uint8_t nVal = (uint8_t)eSleepDataRate;
	
	if (bLink)
		nVal |= POWER_CTL_LINK;
	if (bAutoSleep)
		nVal |= POWER_CTL_AUTO_SLEEP;
	if (bMeasure)
		nVal |= POWER_CTL_MEASURE;
	if (bSleep)
		nVal |= POWER_CTL_SLEEP;
	
	writeReg(POWER_CTL, nVal);
}

void CADXL345::setDataFormat(bool bSelfTest, bool bSPI3Wire, bool bIntInvert, bool bFullRes, bool bJustify, e_scale eScale)
{
	uint8_t nVal = (uint8_t)eScale;
	
	if (bSelfTest)
		nVal |= DATA_FORMAT_SELF_TEST;
	if (bSPI3Wire)
		nVal |= DATA_FORMAT_SPI_3WIRE;
	if (bIntInvert)
		nVal |= DATA_FORMAT_INT_INVERT;
	if (bFullRes)
		nVal |= DATA_FORMAT_FULL_RES;
	if (bJustify)
		nVal |= DATA_FORMAT_JUSTIFY;
	
	writeReg(DATA_FORMAT, nVal);
}

void CADXL345::setFIFOCtrl(e_fifo_mode eFIFOMode, bool bTrigger, uint8_t nFIFOSize)
{
	uint8_t nVal = (uint8_t)eFIFOMode;
	
	if (bTrigger)
		nVal |= FIFO_CTL_TRIGGER;
	
	if (eFIFOMode != FIFO_CTL_FIFO_MODE_BYPASS)
		nVal |= nFIFOSize;
	
	writeReg(FIFO_CTL, nVal);
}

// Devices status

bool CADXL345::getFIFOStatus(uint8_t& nFIFOValues)
{
	uint8_t nVal = readReg(FIFO_STATUS);
	
	nFIFOValues = nVal & 0b00111111;
	
	return nVal & FIFO_STATUS_TRIG;
}

bool CADXL345::getActivityTapStatus(uint8_t nBitFlags)
{
	uint8_t nVal = readReg(ACT_TAP_STATUS);
	
	if (nBitFlags > 0)
		nVal &= nBitFlags;
	
	return nVal > 0;
}

// Interrupt settings
void CADXL345::setTapInterrupt(void (* pSingleTapISR)(), uint8_t nArduinoSingleTapIntNo, void (* pDoubleTapISR)(), uint8_t nArduinoDoubleTapIntNo, 
								double dTapThreshold, double dTapDuration, double dTapLatency, double dTapWindow, bool bX, bool bY, bool bZ)
{
	attachInterrupt(nArduinoSingleTapIntNo, pSingleTapISR, RISING);
	attachInterrupt(nArduinoDoubleTapIntNo, pDoubleTapISR, RISING);
	setTapAxes(false, bX, bY, bZ);
	setTap(nTapThreshold, nTapDuration, nTapLatency, nTapWindow);
	setIntEnable(false, true, true, false, false, false, false, false);
	setIntMap(false, false, true, false, false, false, false, false);
}
							
void CADXL345::setSingleTapInterrupt(void (* pISR)(), uint8_t nArduinoIntNo, bool bINT2, double dTapThreshold, double dTapDuration, bool bX, bool bY, bool bZ)
{
	attachInterrupt(nArduinoIntNo, pISR, RISING);
	setTap(nTapThreshold, nTapDuration, 0.0, 0.0);
	setTapAxes(true, bX, bY, bZ);
	setIntEnable(false, true, false, false, false, false, false, false);
	setIntMap(false, bINT2, false, false, false, false, false, false);
}

void CADXL345::setFreeFallInterrupt(void (* pISR)(), uint8_t nArduinoIntNo, bool bInt2, double dThreshold, double dTime)
{
	attachInterrupt(nArduinoIntNo, pISR, RISING);
	setFreeFall(nThreshold, nTime);
	setIntEnable(false, false, false, true, false, false, false, false);
	setIntMap(false, false, false, bInt2, false, false, false, false);
}

void CADXL345::setIntEnable(bool bDataReady, bool bSingleTap, bool bDoubleTap, bool bFreeFall, 
							bool bActivity, bool bInactivity, bool bWaterMark, bool bOverrun)
{
	uint8_t nVal = 0;
	
	if (bDataReady)
		nVal |= INT_DATA_READY;

	if (bSingleTap)
		nVal |= INT_SINGLE_TAP;

	if (bDoubleTap)
		nVal |= INT_DOUBLE_TAP;

	if (bFreeFall)
		nVal |= INT_FREE_FALL;

	if (bActivity)
		nVal |= INT_ACTIVITY;

	if (bInactivity)
		nVal |= INT_INACTIVITY;

	if (bWaterMark)
		nVal |= INT_WATER_MARK;

	if (bOverrun)
		nVal |= INT_OVERRUN;

	writeReg(INT_ENABLE, nVal);
}

void CADXL345::setIntMap(bool bDataReadyInt1, bool bSingleTapInt1, bool bDoubleTapInt1, bool bFreeFallInt1, 
							bool bActivityInt1, bool bInactivityInt1, bool bWaterMarkInt1, bool bOverrunInt1)
{
	uint8_t nVal = 0;
	
	if (!bDataReadyInt1)
		nVal |= INT_DATA_READY;

	if (!bSingleTapInt1)
		nVal |= INT_SINGLE_TAP;

	if (!bDoubleTapInt1)
		nVal |= INT_DOUBLE_TAP;

	if (!bFreeFallInt1)
		nVal |= INT_FREE_FALL;

	if (!bActivityInt1)
		nVal |= INT_ACTIVITY;

	if (!bInactivityInt1)
		nVal |= INT_INACTIVITY;

	if (!bWaterMarkInt1)
		nVal |= INT_WATER_MARK;

	if (!bOverrunInt1)
		nVal |= INT_OVERRUN;

	writeReg(INT_ENABLE, nVal);
}

// Tap settings
void CADXL345::setTapAxes(bool bDCDisable, bool bXEnable, bool bYEnable, bool bZEnable)
{
	uint8_t nVal = 0;
	
	if (bDCDisable)
		nVal |= TAP_AXES_SUPPRESS_DOUBLE_TAP;
	if (bXEnable)
		nVal |= TAP_AXES_X_ENABLE;
	if (bYEnable)
		nVal |= TAP_AXES_Y_ENABLE;
	if (bZEnable)
		nVal |= TAP_AXES_Z_ENABLE;
}

void CADXL345::setTap(double dThreshold_gs, double dDuration_ms, double nLatency_ms, double nWindow_ms)
{
	writeReg(THRESH_TAP, round((dThreshold_gs * 1000) / 62.5/*mg/LSB*/));
	writeReg(DUR, (uint8_t)round((dDuration_ms * 1000000) / 625/*μs/LSB*/));
	writeReg(LATENT, (uint8_t)round(nLatency_ms / 1.25/*ms/LSB*/));
	writeReg(WINDOW, (uint8_t)round(nWindow_ms / 1.25/*ms/LSB*/));
}

// Free fall settings
void CADXL345::setFreeFall(double dThreshold /*mg (300mg to 600mg recommended)*/, double dTime /*ms (100ms to 300ms recommended)*/)
{
	uint8_t nThreshold = (uint8_t)round(dThreshold),
			nTime = (uint8_t)round(dTime);
	
	if ((nThreshold >= 300) && (nThreshold <= 600))
		writeReg(THRESH_FF, nThreshold);
	else
		Serial.println("CADXL345::setFreeFallThreshold(double dThreshold) - dThreshold is outside the recommended range...");

	if ((nTime >= 100) && (nTime <= 300))
		writeReg(TIME_FF, nThreshold);
	else
		Serial.println("CADXL345::setFreeFall(double dThreshold, double dTime) - dTime is outside the recommended range...");
}

// Activity/inactivity settings
void CADXL345::setActInact(bool bActAC, bool bActX, bool bActY, bool bActZ, bool bInactAC, bool bInactX, bool bInactY, bool bInactZ)
{
	uint8_t nVal = 0;
	
	if (bActAC)
		nVal |= ACT_INACT_CTL_ACT_AC;
	
	if (bActX)
		nVal |= ACT_INACT_CTL_ACT_X;
	
	if (bActY)
		nVal |= ACT_INACT_CTL_ACT_Y;
	
	if (bActZ)
		nVal |= ACT_INACT_CTL_ACT_Z;
	
	if (bInactAC)
		nVal |= ACT_INACT_CTL_INACT_AC;
	
	if (bInactX)
		nVal |= ACT_INACT_CTL_INACT_X;
	
	if (bInactY)
		nVal |= ACT_INACT_CTL_INACT_Y;
	
	if (bInactZ)
		nVal |= ACT_INACT_CTL_INACT_Z;
	
	writeReg(ACT_INACT_CTL, nVal);
}


// Helper functions
float CADXL345::getScaleFactor()
{
	uint8_t nDataFormat = readDATA_FORMAT() & 0b00000011;
	float fScaleFact = 0.0;

	if (nDataFormat == 0b00000000)
		fScaleFact = 3.9;
	else if (nDataFormat == 0b00000001)
		fScaleFact = 7.8;
	else if (nDataFormat == 0b00000010)
		fScaleFact = 12.5;
	else if (nDataFormat == 0b00000011)
		fScaleFact = 31.2;

	return fScaleFact;
}

// Read
int16_t CADXL345::getPitch(double dY, double dZ)
{
	double dPitch = atan2(dY, sqrt((dY * dY) + (dZ * dZ)));

	dPitch *= 180.0 / M_PI;

	return (int16_t)round(dPitch);
}

int16_t CADXL345::getRoll(double dX, double dZ)
{
	double dRoll = atan2(-dX, dZ);

	dRoll *= 180.0 / M_PI;
	
	return (int16_t)round(dRoll);
}

