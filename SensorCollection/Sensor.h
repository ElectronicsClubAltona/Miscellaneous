#ifndef _SENSOR_H
#define _SENSOR_H


#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif


//=============================================================================================== 
// Class CMedian - to store minimum and maxium values, and their average, for device calibration
// purposes.
//================================================================================================ 
class CMedian
{
	public:
		CMedian()
		{
			m_dMedian = 0.0;
		};
		CMedian(CMedian& other)
		{
			(*this) = other;
		};
		CMedian& operator =(CMedian& other)
		{
			m_dMedian = other.m_dMedian;
		};
		void setMinMax(double dMax, double dMin)
		{
			m_dMedian = (dMin + dMax) / 2;
		};
		double getMedian()
		{
			return m_dMedian;
		};
		
	protected:
		double m_dMedian;
};


//================================================================================================ 
// Class CMagCalibration - to store calibration data for 3 magnetic axes - x, y and z
//================================================================================================ 
class CMagCalibration
{
	public:
		CMagCalibration()
		{
		};
		CMagCalibration(double dMaxX, double dMinX, double dMaxY, double dMinY, double dMaxZ, double dMinZ);
		CMagCalibration(CMagCalibration& other)
		{
			(*this) = other;
		};
		CMagCalibration& operator =(CMagCalibration& other);
		void setMinMax(double dMaxX, double dMinX, double dMaxY, double dMinY, double dMaxZ, double dMinZ);
		void setMinMaxX(double dMaxX, double dMinX)
		{
			m_medianX.setMinMax(dMaxX, dMinX);
		};
		void setMinMaxY(double dMaxY, double dMinY)
		{		
			m_medianY.setMinMax(dMaxY, dMinY);
		};
		void setMinMaxZ(double dMaxZ, double dMinZ)
		{
			m_medianZ.setMinMax(dMaxZ, dMinZ);
		};
		float getMedianX()
		{
			return m_medianX.getMedian();
		};

		float getMedianY()
		{
			return m_medianY.getMedian();
		};

		float getMedianZ()
		{
			return m_medianZ.getMedian();
		};

	protected:
		CMedian m_medianX, m_medianY, m_medianZ;
};


//================================================================================================ 
// Class CAccelCalibration - to store average reading for an axis at 1, -1 and 0 G
// For each axis:
// 		1) Get an average reading with the axis pointing down (-G).
//		2) Get an average reading with the axis pointing down (+G).
//		3) Get an average reading with the axis horizontal (0G).
// ScaleFactor = Average+G - Average-G / device scale setting(2G, 4G, 6G, 8G or 16G)
// Offset = -Average0G
// CalibrationFactor = (1 / ScaleFactor) + Offset
// Such that 9.8 - Reading = CalibrationFactor
//================================================================================================ 

class CAccelAxisCalibration
{
	public:
		CAccelAxisCalibration();
		CAccelAxisCalibration(CAccelAxisCalibration& other)
		{
			(*this) = other;
		};
		CAccelAxisCalibration& operator =(CAccelAxisCalibration& other);
		
		void setAverages(double dAveragePlusG, double dAverageMinusG, double dAverageZeroG, uint16_t nCount);
		
		double calibrate(double dReading);

	protected:
		double m_dScaleFactor, m_dOffset;
		uint16_t m_nCount;
};

//================================================================================================ 
// Class CAccelCalibration - to store calibration data for 3 acceleration axes - x, y and z
//================================================================================================ 

class CAccelCalibration
{
	public:
		
		CAccelCalibration();
		CAccelCalibration(CAccelCalibration& other)
		{
			(*this) = other;
		};
		CAccelCalibration& operator =(CAccelCalibration& other);
		
		void setAverages(double dXAverMinusG, double dXAverPlusG, double dXAverZeroG,
							double dYAverMinusG, double dYAverPlusG, double dYAverZeroG,
							double dZAverMinusG, double dZAverPlusG, double dZAverZeroG, uint16_t nCount);
	
		double calibrateX(double dReading)
		{
			return m_calibX.calibrate(dReading);
		};
		
		double calibrateY(double dReading)
		{
			return m_calibY.calibrate(dReading);
		};
		
		double calibrateZ(double dReading)
		{
			return m_calibZ.calibrate(dReading);
		};
		
	protected:
		CAccelAxisCalibration m_calibX, m_calibY, m_calibZ;
};

//================================================================================================ 
// Class CVector - to store device readings
//================================================================================================ 
class CVector
{
	public:
		double m_dX, m_dY, m_dZ;
		
		// Constructors
		CVector();
		CVector(double fX, double fY, double fZ);
		CVector(long nX, long nY, long nZ);
		CVector(int nX, int nY, int nZ);
		
		// Operators
		CVector operator =(CVector vectOther);
		void operator -=(CVector vectOther);
		void operator +=(CVector vectOther);
		CVector operator -(CVector vectOther);
		CVector operator +(CVector vectOther);
		CVector crossProduct(CVector &vect);
		float dotProduct(const CVector &vect);
		void normalise();

		static CVector& CrossProduct(const CVector &vectA, const CVector &vectB);
		static float DotProduct(const CVector &vectA, const CVector &vectB);
		static void Normalise(CVector &vectReadings);
		static float ComputHeading(CVector& vectAccel, CVector& vectMag, CVector vectFrom = CVector(1.0, 0.0, 0.0));
};
		
		
//================================================================================================ 
// Class CSensor - base class for all sensor classes
//================================================================================================ 
class CSensor 
{
	public:
		// Constructors & destructors
		CSensor(int32_t nSensorID, uint8_t nDevAddrWrite, uint8_t nDevAddrRead);
		virtual ~CSensor();
		virtual void begin(uint8_t nSDOPinNum = 0, uint8_t nSDOPinState = 0);

		void setDevAddr(uint8_t nDevAddrWrite, uint8_t nDevAddrRead);
  

	protected:
		// Implementation
		int32_t 	m_nSensorID;
		uint8_t		m_nDevAddrWrite, m_nDevAddrRead, m_nWireError, m_nSDOPinNum, m_nSDOPinState;
		CVector		m_vectMinReadings, m_vectMaxReadings;
		uint32_t	m_nI2CTimeOut;
		
		// Helper functions
		void writeReg(uint8_t nRegAddr, uint8_t nValue);
		void writeReg(uint8_t nRegAddrMSB, uint8_t nRegAddrLSB, uint16_t nValue);
		uint8_t readReg8(uint8_t nRegAddr);
		uint16_t readReg16(uint8_t nRegAddrMSB, uint8_t nRegAddrLSB);
		bool dataReady(uint8_t nNumBytes);
		const char* getWireError();
		void printWireError()
		{
			Serial.println(String("***************** ") + getWireError() + " *****************");
		};
		float calculateTemperature(uint8_t nHighRegAddr, uint8_t nLowRegAddr, uint8_t nNumLSBPerDegree);
		bool getStatus(uint8_t nRegAddr, uint8_t nBitFlags);
};

#endif
