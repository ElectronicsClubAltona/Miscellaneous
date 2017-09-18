#include <Wire.h>
#include <DeviceBase.h>




//############################################################
//# Class CDeviceBase: base class for all devices on module
//############################################################

// Read a two byte integer from the specified registers
int16_t CDeviceBase::readReg(const uint8_t nHighByteRegAddr, const uint8_t nLowByteRegAddr)
{
	int16_t nHighByte = readReg(nHighByteRegAddr),
			nLowByte = readReg(nLowByteRegAddr);

	return (nHighByte << 8) | nLowByte;
}

uint8_t CDeviceBase::readReg(const uint8_t nRegAddr)
{
	Wire.beginTransmission(m_nDevAddr);
	Wire.write(nRegAddr);
	Wire.endTransmission();
	Wire.requestFrom(m_nDevAddr, (uint8_t)1);
	return Wire.read();
}

void CDeviceBase::writeReg(const uint8_t nRegAddr, const uint8_t nValue)
{
	Wire.beginTransmission(m_nDevAddr);
	Wire.write(nRegAddr);       
	Wire.write(nValue);
	Wire.endTransmission();
}

// Write bits to specified register.
void CDeviceBase::writeRegBits(const uint8_t nRegAddr, const uint8_t nBitMask, const uint8_t nNewBits)
{
	uint8_t nCurrentVal = readReg(nRegAddr);

	writeReg(nRegAddr, (nCurrentVal & nBitMask) | nNewBits);
}

// Enable and disable a feature by setting or resetting a bit.
void CDeviceBase::writeRegBit(const bool bEnable, const uint8_t nRegAddr, const uint8_t nBitMask)
{
	uint8_t nCurrentVal = readReg(nRegAddr);

	if (bEnable)
		writeReg(nRegAddr, nCurrentVal | (uint8_t)nBitMask);
	else
		writeReg(nRegAddr, nCurrentVal & (uint8_t)~nBitMask);
}

// Serial print the contents of the register.
void CDeviceBase::print(const uint8_t nRegAddr)
{
	Serial.print("########## ");
	Serial.print(getRegName(nRegAddr));
	Serial.print(" = ");
	Serial.print(readReg(nRegAddr), BIN);
	Serial.println("##########");
}

// Serial print the specified byte.
void CDeviceBase::print(const char *strLabel, const uint8_t nByte, const uint8_t nBase)
{
	Serial.print("########## ");
	Serial.print(strLabel);
	Serial.print(" = ");
	Serial.print(nByte, nBase);
	Serial.println("##########");
}

// Serial print the specified integer.
void CDeviceBase::print(const char *strLabel, const int16_t nInt, const uint8_t nBase)
{
	Serial.print("########## ");
	Serial.print(strLabel);
	Serial.print(" = ");
	Serial.print(nInt, nBase);
	Serial.println("##########");
}

// Serial print the specified integer.
void CDeviceBase::print(const char *strLabel, const uint16_t nInt, const uint8_t nBase)
{
	Serial.print("########## ");
	Serial.print(strLabel);
	Serial.print(" = ");
	Serial.print(nInt, nBase);
	Serial.println("##########");
}

// Serial print the specified integer.
void CDeviceBase::print(const char *strLabel, const int32_t nInt, const uint8_t nBase)
{
	Serial.print("########## ");
	Serial.print(strLabel);
	Serial.print(" = ");
	Serial.print(nInt, nBase);
	Serial.println("##########");
}

// Serial print the specified integer.
void CDeviceBase::print(const char *strLabel, const uint32_t nInt, const uint8_t nBase)
{
	Serial.print("########## ");
	Serial.print(strLabel);
	Serial.print(" = ");
	Serial.print(nInt, nBase);
	Serial.println("##########");
}

// Serial print the specified float.
void CDeviceBase::print(const char *strLabel, const double dFloat, const uint8_t nBase)
{
	Serial.print("########## ");
	Serial.print(strLabel);
	Serial.print(" = ");
	Serial.print(dFloat);
	Serial.println("##########");
}




//############################################################
//# Class CAxesDeviceBase: base class for  devices with axes
//############################################################

CAxesDeviceBase::CAxesDeviceBase(uint8_t nDevAddr): CDeviceBase(nDevAddr)
{	
	m_fScaleFactor = m_dZeroZ = m_dZeroY = m_dZeroX = 0;
}

CAxesDeviceBase::~CAxesDeviceBase()
{	
	m_fScaleFactor = m_dZeroZ = m_dZeroY = m_dZeroX = 0;
}

// Setup tasks e.g. device configuration. The base class just starts up Serial and Wire.
void CAxesDeviceBase::begin()
{
	CDeviceBase::begin();
	doZeroDevice();
}

// Enable and disable axes.
void CAxesDeviceBase::enableAxes(const bool bX, const bool bY, const bool bZ)
{
	enableXAxis(bX);
	enableYAxis(bY);
	enableZAxis(bZ);
}

typedef struct{ double m_dReading; uint16_t m_nFrequency; } SingleReading;

bool findReading(const SingleReading* pReadings, const uint16_t nNumReadings, const double dReading, uint16_t &nReadingIndex)
{
	uint16_t nI = 0;
	bool bFound = false;

	for (nI = 0; nI < nNumReadings; nI++)
	{
		if (dReading == pReadings[nI].m_dReading)
		{
			nReadingIndex = nI;
			bFound = true;
			break;
		}
	}
	return bFound;
}

double CAxesDeviceBase::doTakeReadingsForAxis(const char cAxis)
{
	const uint16_t nTotal = 100;
	uint16_t nI = 0, nNumReadings = 0, nJ = 0, nFrequency = 0;
	double dAxisReading = 0.0;
	SingleReading readings[nTotal];

	memset(readings, 0, sizeof readings);

	// Take nTotal readings and record the reading and its frequency
	for (nI = 0, nJ = 0; nI < nTotal; nI++)
	{
		if (cAxis == 'x')
			dAxisReading  = getX();
		else if (cAxis == 'y')
			dAxisReading  = getY();
		else if (cAxis == 'z')
			dAxisReading  = getZ();

		if (findReading(readings, nNumReadings, dAxisReading, nJ))
			readings[nJ].m_nFrequency++;
		else
		{
			readings[nNumReadings].m_dReading = dAxisReading;
			readings[nNumReadings].m_nFrequency++;
			nNumReadings++;
		}
	}
	// Find the most repeated reading
	for (nI = 0, nJ = 0; nI < nNumReadings; nI++)
	{
		if (readings[nI].m_nFrequency > nFrequency)
			nJ = nI;
	}
	return readings[nJ].m_dReading;
}

// Take a series of readings and use these to set m_dZeroZ, m_dZeroY & m_dZeroX
void CAxesDeviceBase::doZeroDevice()
{
	m_dZeroX = doTakeReadingsForAxis('x');
	m_dZeroY = doTakeReadingsForAxis('y');
	m_dZeroZ = doTakeReadingsForAxis('z');
}


