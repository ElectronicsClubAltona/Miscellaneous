#include <stdio.h>
#include "BMP180.h"

CBMP180::CBMP180(int32_t nSensorID): CSensor(nSensorID, 0, 0)
{
    m_nAC1 = m_nAC2 = m_nAC3 = m_nAC4 = m_nAC5 = m_nAC6 = m_nVB1 = m_nVB2 = m_nMB = m_nMC = m_nMD = m_nLastTemperatureData = 0;
}

void CBMP180::begin()
{
      CSensor::begin();
	  setDevAddr(DEVICE_ADDR, DEVICE_ADDR);
	  DoReset();

      // Load the devices calibration data
      m_nAC1 = readReg(CALIB_AC1_HIGH, CALIB_AC1_LOW);
      m_nAC2 = readReg(CALIB_AC2_HIGH, CALIB_AC2_LOW);
      m_nAC3 = readReg(CALIB_AC3_HIGH, CALIB_AC3_LOW);
      m_nAC4 = readReg(CALIB_AC4_HIGH, CALIB_AC4_LOW);
      m_nAC5 = readReg(CALIB_AC5_HIGH, CALIB_AC5_LOW);
      m_nAC6 = readReg(CALIB_AC6_HIGH, CALIB_AC6_LOW);
      m_nVB1 = readReg(CALIB_VB1_HIGH, CALIB_VB1_LOW);
      m_nVB2 = readReg(CALIB_VB2_HIGH, CALIB_VB2_LOW);
      m_nMB = readReg(CALIB_MB_HIGH, CALIB_MB_LOW);
      m_nMC = readReg(CALIB_MC_HIGH, CALIB_MC_LOW);
      m_nMD = readReg(CALIB_MD_HIGH, CALIB_MD_LOW);
/*	  
	  print("m_nAC1", m_nAC1);
	  print("m_nAC2", m_nAC2);
	  print("m_nAC3", m_nAC3);
	  print("m_nAC4", m_nAC4);
	  print("m_nAC5", m_nAC5);
	  print("m_nAC6", m_nAC6);
	  print("m_nVB1", m_nVB1);
	  print("m_nVB2", m_nVB2);
	  print("m_nMB", m_nMB);
	  print("m_nMC", m_nMC);
	  print("m_nMD", m_nMD);
*/
}

uint8_t CBMP180::GetDelay(uint8_t nCommand)
{
	uint8_t nOSS = nCommand & 0xC0, nMillis = 0;

    switch (nOSS)
    {
        case OSS1_TYPE: nMillis = OSS1_WAIT; break;
        case OSS2_TYPE: nMillis = OSS2_WAIT; break;
        case OSS4_TYPE: nMillis = OSS4_WAIT; break;
        case OSS8_TYPE: nMillis = OSS8_WAIT; break;
    }
    return nMillis;
}

float CBMP180::GetTemperature1()
{
    uint32_t nRawPressure = 0;
    
    writeReg(REG_CTRL_MEASURE, CMD_TEMPERATURE1);
    delay(GetDelay(CMD_TEMPERATURE1));
    nRawPressure = readReg(REG_MEASURE_HIGH, REG_MEASURE_LOW);

    return CalculateRealTemperature(nRawPressure);
}

float CBMP180::GetTemperature2()
{
    uint32_t nRawPressure = 0;
    
    writeReg(REG_CTRL_MEASURE, CMD_TEMPERATURE2);
    delay(GetDelay(CMD_TEMPERATURE2));
    nRawPressure = readReg(REG_MEASURE_HIGH, REG_MEASURE_LOW);

    return CalculateRealTemperature(nRawPressure);
}

float CBMP180::GetTemperature4()
{
    uint32_t nRawPressure = 0;
    
    writeReg(REG_CTRL_MEASURE, CMD_TEMPERATURE4);
    delay(GetDelay(CMD_TEMPERATURE4));
    nRawPressure = readReg(REG_MEASURE_HIGH, REG_MEASURE_LOW);

    return CalculateRealTemperature(nRawPressure);
}

float CBMP180::GetTemperature8()
{
    uint32_t nRawPressure = 0;
    
    writeReg(REG_CTRL_MEASURE, CMD_TEMPERATURE8);
    delay(GetDelay(CMD_TEMPERATURE8));
    nRawPressure = readReg(REG_MEASURE_HIGH, REG_MEASURE_LOW);

    return CalculateRealTemperature(nRawPressure);
}

float CBMP180::GetPressure1()
{
    int32_t nRawPressure = 0;
	uint32_t nByteHigh = 0, nByteLow = 0, nByteXLow = 0;

    writeReg(REG_CTRL_MEASURE, CMD_PRESSURE1);
    delay(GetDelay(CMD_PRESSURE1));
	nByteHigh = readReg(REG_MEASURE_HIGH);
	nByteLow = readReg(REG_MEASURE_LOW);
	nByteXLow = readReg(REG_MEASURE_XLOW);
    nRawPressure = ((nByteHigh << 16) | (nByteLow << 8) | (nByteXLow)) >> (8 - (OSS1_TYPE >> 6));

    return (float)CalculateRealPressure(nRawPressure, OSS1_TYPE >> 6) / 1000.0;
}

float CBMP180::GetPressure2()
{
    int32_t nRawPressure = 0;
	uint32_t nByteHigh = 0, nByteLow = 0, nByteXLow = 0;

    writeReg(REG_CTRL_MEASURE, CMD_PRESSURE2);
    delay(GetDelay(CMD_PRESSURE2));
	nByteHigh = readReg(REG_MEASURE_HIGH);
	nByteLow = readReg(REG_MEASURE_LOW);
	nByteXLow = readReg(REG_MEASURE_XLOW);
    nRawPressure = ((nByteHigh << 16) | (nByteLow << 8) | (nByteXLow)) >> (8 - (OSS2_TYPE >> 6));

    return (float)CalculateRealPressure(nRawPressure, OSS2_TYPE >> 6) / 1000.0;
}

float CBMP180::GetPressure4()
{
    int32_t nRawPressure = 0;
	uint32_t nByteHigh = 0, nByteLow = 0, nByteXLow = 0;

    writeReg(REG_CTRL_MEASURE, CMD_PRESSURE4);
    delay(GetDelay(CMD_PRESSURE4));
	nByteHigh = readReg(REG_MEASURE_HIGH);
	nByteLow = readReg(REG_MEASURE_LOW);
	nByteXLow = readReg(REG_MEASURE_XLOW);
    nRawPressure = ((nByteHigh << 16) | (nByteLow << 8) | (nByteXLow)) >> (8 - (OSS4_TYPE >> 6));

    return (float)CalculateRealPressure(nRawPressure, OSS4_TYPE >> 6) / 1000.0;
}

float CBMP180::GetPressure8()
{
    int32_t nRawPressure = 0;
	uint32_t nByteHigh = 0, nByteLow = 0, nByteXLow = 0;

    writeReg(REG_CTRL_MEASURE, CMD_PRESSURE8);
    delay(GetDelay(CMD_PRESSURE8));
	nByteHigh = readReg(REG_MEASURE_HIGH);
	nByteLow = readReg(REG_MEASURE_LOW);
	nByteXLow = readReg(REG_MEASURE_XLOW);
    nRawPressure = ((nByteHigh << 16) | (nByteLow << 8) | (nByteXLow)) >> (8 - (OSS8_TYPE >> 6));

    return (float)CalculateRealPressure(nRawPressure, OSS8_TYPE >> 6) / 1000.0;
}

float CBMP180::CalculateRealTemperature(uint32_t nRawTemperature)
{
    int nX2 = 0, nB5 = 0;
	long nX1 = 0;
	float fTemperature = 0.0;
	
	nX1 = (((long)nRawTemperature - (long)m_nAC6) * (long)m_nAC5) >> 15;
    nX2 = ((long)m_nMC << 11) / (nX1 + m_nMD);
    nB5 = m_nLastTemperatureData = nX1 + nX2;
    fTemperature = (float)((nB5 + 8) >> 4);  /* temperature in 0.1 deg C*/
	fTemperature /= 10.0;
/*
	print("nRawTemperature", nRawTemperature);
	print("nX1", nX1);
	print("nX2", nX2);
	print("nB5", nB5);
	print("nTemperature", nTemperature);
	print("fTemperature", fTemperature);
*/
    return fTemperature;
}

int32_t CBMP180::CalculateRealPressure(uint32_t nRawPressure, uint8_t nOverSampling)
{
	// Algorithm taken from BMP180 datasheet.
	int32_t nB6 = m_nLastTemperatureData - 4000,
			nX1 = (m_nVB2 * (nB6 * nB6) >> 12) >> 11,
			nX2 = (m_nAC2 * nB6) >> 11,
			nX3 = nX1 + nX2,
			nB3 = (((((long)m_nAC1) * 4 + nX3) << nOverSampling) + 2) >> 2,
			nB4 = 0,
			nPressure = 0;
	uint32_t nB7 = 0;
			
    nX1 = (m_nAC3 * nB6) >> 13;
    nX2 = (m_nVB1 * (nB6 * nB6 >> 12)) >> 16;
    nX3 = ((nX1 + nX2) + 2) >> 2;
    nB4 = (m_nAC4 * (uint32_t)((nX3 + 32768))) >> 15;
    nB7 = (uint32_t)(((nRawPressure - nB3)) * (50000 >> nOverSampling));

    if (nB7 < 0x80000000)
		nPressure = ((nB7 * 2) / nB4);
    else
		nPressure = ((nB7 / nB4) << 1);
		
    nX1 = (nPressure >> 8) * (nPressure >> 8);
    nX1 = (nX1 * 3038) >> 16;
    nX2 = (-7357 * nPressure) >> 16;
    nPressure = nPressure + ((nX1 + nX2 + 3791) >> 4);

    return nPressure;
}

float CBMP180::CalculateAltitude(float fPressure)
{
	// Calculate altitude from sea level.
    float fAltitude = 44330.0 * (1.0 - pow(fPressure / 101.325, 0.1903));
    return fAltitude;
}

void CBMP180::DoDisplay()
{
	Serial.println("Temperature: " + String(GetTemperature2()) + String(char(176)) + "C");
	Serial.println("Pressure: " + String(GetPressure2()) + "kPa");
	Serial.println("Altitude: " + String(GetAltitude2()) + "m");
}




CBMP180 bmp180;

