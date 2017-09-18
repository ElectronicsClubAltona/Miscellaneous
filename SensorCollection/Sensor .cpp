#include "Sensor.h"
#include <Wire.h>


//================================================================================================ 
// Class CMedian - to store minimum and maxium values, and their average, for device calibration
// purposes.
//================================================================================================ 


//================================================================================================ 
// Class CMagCalibration - to store calibration data for 3 magnetic axes - x, y and z
//================================================================================================ 

CMagCalibration::CMagCalibration(double dMaxX, double dMinX, double dMaxY, double dMinY, double dMaxZ, double dMinZ)
{
	m_medianX.setMinMax(dMaxX, dMinX);
	m_medianY.setMinMax(dMaxY, dMinY);
	m_medianZ.setMinMax(dMaxZ, dMinZ);
}

CMagCalibration& CMagCalibration::operator =(CMagCalibration& other)
{
	m_medianX = other.m_medianX;
	m_medianY = other.m_medianY;
	m_medianZ = other.m_medianZ;
	
	return *this;
}

void CMagCalibration::setMinMax(double dMaxX, double dMinX, double dMaxY, double dMinY, double dMaxZ, double dMinZ)
{
	m_medianX.setMinMax(dMaxX, dMinX);
	m_medianY.setMinMax(dMaxY, dMinY);
	m_medianZ.setMinMax(dMaxZ, dMinZ);
}


//================================================================================================ 
// Class CAccelCalibration - to store average reading for an axis at 1, -1 and 0 G
//================================================================================================ 

CAccelAxisCalibration::CAccelAxisCalibration()
{
	m_dScaleFactor = m_dOffset = 0.0;
	m_nCount = 0;
}

CAccelAxisCalibration& CAccelAxisCalibration::operator =(CAccelAxisCalibration& other)
{
	m_dScaleFactor = other.m_dScaleFactor;
	m_dOffset = other.m_dOffset;
	m_nCount = other.m_nCount;
}
		
void CAccelAxisCalibration::setAverages(double dAveragePlusG, double dAverageMinusG, double dAverageZeroG, uint16_t nCount)
{
	m_dScaleFactor = 2.0 / (dAveragePlusG - dAverageMinusG);
	m_dOffset = (dAveragePlusG + dAverageMinusG) / 2.0;
	m_nCount = nCount;
}

double CAccelAxisCalibration::calibrate(double dReading)
{
Serial.println("@@@@@@@@@@@@@@@@@@@");
Serial.println("m_dScaleFactor = " + String(m_dScaleFactor));
Serial.println("m_dOffset = " + String(m_dOffset));
Serial.println("dReading = " + String(dReading));
Serial.println("@@@@@@@@@@@@@@@@@@@");

	return m_dScaleFactor * (dReading - m_dOffset);
}


//================================================================================================ 
// Class CAccelCalibration - to store calibration data for 3 acceleration axes - x, y and z
//================================================================================================ 

CAccelCalibration::CAccelCalibration()
{
}

CAccelCalibration& CAccelCalibration::operator =(CAccelCalibration& other)
{
	m_calibX = other.m_calibX;
	m_calibY = other.m_calibY;
	m_calibZ = other.m_calibZ;
}

void CAccelCalibration::setAverages(double dXAverMinusG, double dXAverPlusG, double dXAverZeroG,
									double dYAverMinusG, double dYAverPlusG, double dYAverZeroG,
									double dZAverMinusG, double dZAverPlusG, double dZAverZeroG, uint16_t nCount)
{
		m_calibX.setAverages(dXAverPlusG, dXAverMinusG, dXAverZeroG, nCount);
		m_calibY.setAverages(dYAverPlusG, dYAverMinusG, dYAverZeroG, nCount);
		m_calibZ.setAverages(dZAverPlusG, dZAverMinusG, dZAverZeroG, nCount);
}

						
//================================================================================================ 
// Class CVector - to store device readings
//================================================================================================ 

CVector::CVector()
{
	m_dX = 0;
	m_dX = 0;
	m_dZ = 0;
}

CVector::CVector(double fX, double fY, double fZ)
{
	m_dX = fX;
	m_dX = fY;
	m_dZ = fZ;
}

CVector::CVector(long nX, long nY, long nZ)
{
	m_dX = nX;
	m_dX = nY;
	m_dZ = nZ;
}

CVector::CVector(int nX, int nY, int nZ)
{
	m_dX = nX;
	m_dX = nY;
	m_dZ = nZ;
}

void CVector::operator -=(CVector vectOther)
{
	m_dX -= vectOther.m_dX;
	m_dX -= vectOther.m_dY;
	m_dZ -= vectOther.m_dZ;
}

void CVector::operator +=(CVector vectOther)
{
	m_dX += vectOther.m_dX;
	m_dX += vectOther.m_dY;
	m_dZ += vectOther.m_dZ;
}

CVector CVector::operator =(CVector vectOther)
{
	m_dX = vectOther.m_dX;
	m_dY = vectOther.m_dY;
	m_dZ = vectOther.m_dZ;
	return *this;
}

CVector CVector::operator -(CVector vectOther)
{
	CVector vect = *this;
	
	vect.m_dX -= vectOther.m_dX;
	vect.m_dY -= vectOther.m_dY;
	vect.m_dZ -= vectOther.m_dZ;
	
	return vect;
}

CVector CVector::operator +(CVector vectOther)
{
	CVector vect = *this;
	
	vect.m_dX += vectOther.m_dX;
	vect.m_dY += vectOther.m_dY;
	vect.m_dZ += vectOther.m_dZ;
	
	return vect;
}

CVector& CVector::CrossProduct(const CVector &vectA, const CVector &vectB)
{
	static CVector vectOut;
	
	vectOut.m_dX = (vectA.m_dY * vectB.m_dZ) - (vectA.m_dZ * vectB.m_dY);
	vectOut.m_dY = (vectA.m_dZ * vectB.m_dX) - (vectA.m_dX * vectB.m_dZ);
	vectOut.m_dZ = (vectA.m_dX * vectB.m_dY) - (vectA.m_dY * vectB.m_dX);
	
	return vectOut;
}

float CVector::DotProduct(const CVector &vectA, const CVector &vectB)
{
	return (vectA.m_dX * vectB.m_dX) + (vectA.m_dY * vectB.m_dY) + (vectA.m_dZ * vectB.m_dZ);
}

void CVector::Normalise(CVector &vectReadings)
{
	double dMag = sqrt(DotProduct(vectReadings, vectReadings));
	vectReadings.m_dX /= dMag;
	vectReadings.m_dY /= dMag;
	vectReadings.m_dZ /= dMag;
}

float CVector::ComputHeading(CVector& vectAccel, CVector& vectMag, CVector vectFrom)
{
	CVector vectEast, vectNorth;
	
    // compute vectEast and vectNorth
    vectEast = CVector::CrossProduct(vectMag, vectAccel);
    CVector::Normalise(vectEast);
    vectNorth = CVector::CrossProduct(vectAccel, vectEast);
    CVector::Normalise(vectNorth);

    // compute dHeading
    double dHeading = atan2(CVector::DotProduct(vectEast, vectFrom), CVector::DotProduct(vectNorth, vectFrom)) * 180 / M_PI;
    
	if (dHeading < 0) 
		dHeading += 360;
	
    return dHeading;

}

CVector CVector::crossProduct(CVector &vect)
{
	(*this) = CVector::CrossProduct(*this, vect);
}

float CVector::dotProduct(const CVector &vect)
{
	return CVector::DotProduct(*this, vect);
}

void CVector::normalise()
{
	CVector::Normalise(*this);
}


//================================================================================================ 
// Class CSensor - base class for all sensor classes
//================================================================================================ 

CSensor::CSensor(int32_t nSensorID, uint8_t nDevAddrWrite, uint8_t nDevAddrRead) 
{
	m_nSensorID = nSensorID;
	m_nDevAddrWrite = nDevAddrWrite;
	m_nDevAddrRead = nDevAddrRead;
	m_nWireError = 0;
	m_vectMinReadings = CVector(-32767.0, -32767.0, -32767.0);
	m_vectMaxReadings = CVector(+32767.0, +32767.0, +32767.0);
	m_nSDOPinNum = m_nSDOPinState = 0;
	m_nI2CTimeOut = 2000;
}

CSensor::~CSensor() 
{
}

void CSensor::begin(uint8_t nSDOPinNum, uint8_t nSDOPinState)
{
	if (nSDOPinNum > 0)
	{
		m_nSDOPinNum = nSDOPinNum;
		if (nSDOPinState > 0)
			m_nSDOPinState = HIGH;
		else
			m_nSDOPinState = LOW;
		pinMode(nSDOPinNum, OUTPUT);
		digitalWrite(nSDOPinNum, m_nSDOPinState);
	}
	Serial.begin(115200);
	Wire.begin();
}

void CSensor::setDevAddr(uint8_t nDevAddrWrite, uint8_t nDevAddrRead)
{
	m_nDevAddrWrite = nDevAddrWrite;
	m_nDevAddrRead = nDevAddrRead;
}


void CSensor::writeReg(uint8_t nRegAddr, uint8_t nValue)
{
	Wire.beginTransmission(m_nDevAddrWrite);
	#if ARDUINO >= 100
		Wire.write(nRegAddr);
		Wire.write(nValue);
	#else
		Wire.send(nRegAddr);
		Wire.send(nValue);
	#endif
	m_nWireError = Wire.endTransmission();
}

void CSensor::writeReg(uint8_t nRegAddrMSB, uint8_t nRegAddrLSB, uint16_t nValue)
{
	writeReg(nRegAddrMSB, nValue >> 8);
	writeReg(nRegAddrLSB, nValue & 0x00FF);
}

bool CSensor::dataReady(uint8_t nNumBytes)
{
	uint32_t nMillisStart = millis(), nDeltaMillis = 0;
	
	m_nWireError = 0;
	
	while ((Wire.available() < nNumBytes) && (m_nWireError == 0))
	{
		nDeltaMillis = millis() - nMillisStart;
		if (nDeltaMillis > m_nI2CTimeOut)
			m_nWireError = 5;
    }
	return m_nWireError == 0;
}

uint8_t CSensor::readReg8(uint8_t nRegAddr)
{
	uint8_t nValue = 0;

	Wire.beginTransmission(m_nDevAddrRead);
	#if ARDUINO >= 100
		Wire.write(nRegAddr);
	#else
		Wire.send(nRegAddr);
	#endif
	m_nWireError = Wire.endTransmission();
	
	if (m_nWireError == 0)
	{
		Wire.requestFrom(m_nDevAddrRead, (uint8_t)1);
		if (dataReady(1))
		{
			#if ARDUINO >= 100
				nValue = Wire.read();
			#else
				nValue = Wire.receive();
			#endif  
			m_nWireError = Wire.endTransmission();
		}
		else
			m_nWireError = 5;
	}
	return nValue;
}

uint16_t CSensor::readReg16(uint8_t nRegAddrMSB, uint8_t nRegAddrLSB)
{
	uint8_t nValueMSB = 0, nValueLSB = 0;
	
	nValueMSB = readReg8(nRegAddrMSB);
	nValueLSB = readReg8(nRegAddrLSB);
	
	return (nValueMSB << 8) | nValueLSB;
}

const char* CSensor::getWireError()
{
	static String strError;
	
	strError = String(m_nWireError) + ": ";
	switch (m_nWireError)
	{
		case 0: strError += "successful..."; break;
		case 1: strError += "data too long to fit in transmit buffer..."; break;
		case 2: strError += "received NACK on transmit of address..."; break;
		case 3: strError += "received NACK on transmit of data..."; break;
		case 4: strError += "other error..."; break;
		case 5: strError += "I2C read timeout..."; break;
	}
	return strError.c_str();
}

float CSensor::calculateTemperature(uint8_t nHighRegAddr, uint8_t nLowRegAddr, uint8_t nNumLSBPerDegree)
{
	double dTemp = 0.0;
	
	// 8 LSB per 1 degree = 1/8 (from data sheet)
	dTemp = ((float)1 / nNumLSBPerDegree) * CSensor::readReg16(nHighRegAddr, nLowRegAddr);
	
	return dTemp;
}

bool CSensor::getStatus(uint8_t nRegAddr, uint8_t nBitFlags)
{
	uint8_t nVal = readReg8(nRegAddr);
	
	if (nBitFlags > 0)
		nVal &= nBitFlags;
	
	return nVal > 0;
}

