#ifndef __DEVICE_BASE_H__
#define __DEVICE_BASE_H__

#include <Arduino.h>
#include <Wire.h>




//############################################################
//# Class CDeviceBase: base class for all devices
//############################################################
class CDeviceBase
{
	public:
		//####################################################
		//# CONSTRUCTION & INITIALISATION
		//####################################################
		CDeviceBase(uint8_t nDevAddr)
		{	
			m_nDevAddr = nDevAddr;
		};

		~CDeviceBase()
		{	
			m_nDevAddr = 0;
		};

		// Setup tasks e.g. device configuration.
		virtual void begin()
		{
			Serial.begin(115200);
			Wire.begin();
		};

		//####################################################
		//# USEFUL DEBUGGING FUNCTIONS
		//####################################################
		
		// Serial print the contents of the register.
		void print(const uint8_t nRegAddr);

		// Serial print the specified string.
		void print(const char *strLabel)
		{
			Serial.println(strLabel);
		};

		// Serial print the specified byte.
		void print(const char *strLabel, const uint8_t nByte, const uint8_t nBase = DEC);

		// Serial print the specified integer.
		void print(const char *strLabel, const int16_t nInt, const uint8_t nBase = DEC);

		// Serial print the specified integer.
		void print(const char *strLabel, const uint16_t nInt, const uint8_t nBase = DEC);

		// Serial print the specified integer.
		void print(const char *strLabel, const int32_t nInt, const uint8_t nBase = DEC);

		// Serial print the specified integer.
		void print(const char *strLabel, const uint32_t nInt, const uint8_t nBase = DEC);

		// Serial print the specified float.
		void print(const char *strLabel, const double dFloat, const uint8_t nBase = DEC);

		// Get the register name from the value e.g."WHO_AM_I" - must be implemented in the  derived class.
		virtual const char *getRegName(const uint8_t nRegAddr) = 0;

	protected:

		//####################################################
		//# HELPER FUNCTIONS
		//####################################################

		// Write bits to specified register.
		void writeRegBits(const uint8_t nRegAddr, const uint8_t nBitMask, const uint8_t nNewBits);

		// Enable and disable a feature by setting or resetting a bit in the specified register.
		void writeRegBit(const bool bEnable, const uint8_t nRegAddr, const uint8_t nBitMask);

		// Read a two byte integer from the specified registers
		int16_t readReg(const uint8_t nHighByteRegAddr, const uint8_t nLowByteRegAddr);

		// Read the value from a register
		uint8_t readReg(const uint8_t nRegAddr);

		// Write a value to a register
		void writeReg(const uint8_t nRegAddr, const uint8_t nValue);

		//####################################################
		//# DATA MEMBERS
		//####################################################

		// The I2C device address;
		uint8_t m_nDevAddr;

};




//########################################################################################################
//# Class CAxesDeviceBase: base class for  devices with axes
//########################################################################################################
class CAxesDeviceBase: public CDeviceBase
{
	public:
		//################################################################################################
		//# CONSTRUCTION & INITIALISATION
		//################################################################################################
		CAxesDeviceBase(uint8_t nDevAddr);

		~CAxesDeviceBase();

		// Setup tasks e.g. device configuration. The base class just starts up Serial and Wire.
		virtual void begin();

    //####################################################
    //# COMMON FUNCTIONS FOR DERIVED CLASSES
    //####################################################
    
    // Change the devices scale setting.
    virtual void setScale(const uint8_t nValue) = 0;

    // Put the device into normal mode.
    virtual void setNormalMode() = 0;

    // Put the device into power down mode.
    virtual void setPowerDownMode() = 0;

    // Put the device into sleep mode.
    virtual void setSleepMode() = 0;

    // Change the data sampling rate.
    virtual void setDataRate(const uint8_t nValue) = 0;

    // Change the device band width.
    virtual void setBandWidth(const uint8_t nValue) = 0;

		//################################################################################################
		//# GET AXES DATA
		//################################################################################################

		// Enable and disable axes.
		void enableAxes(const bool bX, const bool bY, const bool bZ);

		// Enable and disable  X axis.
		virtual void enableXAxis(const bool bEnable) = 0;

		// Enable and disable  Y axis.
		virtual void enableYAxis(const bool bEnable) = 0;

		// Enable and disable  Z axis.
		virtual void enableZAxis(const bool bEnable) = 0;

		// Get a16 bit integer from high and low byte registers.
		double getAxis(const uint8_t nHighByteRegAddr, const uint8_t nLowByteRegAddr)
		{
			return (readReg(nHighByteRegAddr, nLowByteRegAddr) * m_fScaleFactor) - m_dZeroX;
		};

		// Get X axis data - implement in derived class with a call to double getX(const uint8_t nHighByteRegAddr, const uint8_t nLowByteRegAddr)
		// using the appropriate register addresses.
		virtual double getX() = 0;

		// Get Y axis data - implement in derived class with a call to double getY(const uint8_t nHighByteRegAddr, const uint8_t nLowByteRegAddr)
		// using the appropriate register addresses.
		virtual double getY() = 0;

		// Get Z axis data - implement in derived class with a call to double getZ(const uint8_t nHighByteRegAddr, const uint8_t nLowByteRegAddr)
		// using the appropriate register addresses.
		virtual double getZ() = 0;

		//####################################################
		//# USEFUL DEBUGGING FUNCTIONS
		//####################################################
		
		void printX()
		{
			print("X", getX());
		};

		void printY()
		{
			print("Y", getY());
		};

		void printZ()
		{
			print("Z", getZ());
		};

protected:

		//################################################################################################
		//# HELPER FUNCTIONS
		//################################################################################################
		
		// Take a series of readings and use these to set m_dZeroZ, m_dZeroY & m_dZeroX - call this 
		// function in the void beging() function of your derived class
		void doZeroDevice();
		double doTakeReadingsForAxis(const char cAxis);

		//################################################################################################
		//# DATA MEMBERS
		//################################################################################################

		// The scale factor for this device - 250dps, 500dps or 2000dps.
		double m_fScaleFactor;

		// These are bias levels that are subtracted or added to the axes output to get them as close to zero as possible when the sensor is not moving.
		// Just comment out them out  from the lines "return (readReg(nHighByteRegAddr, nLowByteRegAddr) * m_fScaleFactor) /*- m_dZeroX*/;" in functions 
		// "float getX(const uint8_t nHighByteRegAddr, const uint8_t nLowByteRegAddr)" etc, observe the unmodified output from your sensor, set their 
		// values in the constructor  of your derived class and re-enstate the "- m_dZeroX" etc.
		// If your sensor is outputing a mainly negative value for the X axis then set m_dZeroX to the median negative value in the constructor.
		double m_dZeroZ, m_dZeroY, m_dZeroX;
};

#endif
