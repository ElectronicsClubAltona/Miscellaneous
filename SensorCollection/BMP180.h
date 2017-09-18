#ifndef __BMP180_H__
#define __BMP180_H__

#include <Arduino.h>
#include "Sensor.h"


#define DEVICE_ADDR					(uint8_t)0x77


typedef enum
{
	// Calibration data registers
	CALIB_AC1_HIGH 						= 0xAA,
	CALIB_AC1_LOW 						= 0xAB,
	CALIB_AC2_HIGH 						= 0xAC,
	CALIB_AC2_LOW 						= 0xAD,
	CALIB_AC3_HIGH 						= 0xAE,
	CALIB_AC3_LOW 						= 0xAF,
	CALIB_AC4_HIGH 						= 0xB0,
	CALIB_AC4_LOW 						= 0xB1,
	CALIB_AC5_HIGH 						= 0xB2,
	CALIB_AC5_LOW 						= 0xB3,
	CALIB_AC6_HIGH 						= 0xB4,
	CALIB_AC6_LOW 						= 0xB5,
	CALIB_VB1_HIGH 						= 0xB6,
	CALIB_VB1_LOW 						= 0xB7,
	CALIB_VB2_HIGH 						= 0xB8,
	CALIB_VB2_LOW 						= 0xB9,
	CALIB_MB_HIGH 						= 0xBA,
	CALIB_MB_LOW 						= 0xBB,
	CALIB_MC_HIGH 						= 0xBC,
	CALIB_MC_LOW 						= 0xBD,
	CALIB_MD_HIGH 						= 0xBE,
	CALIB_MD_LOW 						= 0xBF,
	
	// Measurement out registers
	REG_CTRL_MEASURE 					= 0xF4,
	REG_MEASURE_HIGH 					= 0xF6,
	REG_MEASURE_LOW  					= 0xF7,
	REG_MEASURE_XLOW 					= 0xF8,

	// Reset  registers
	REG_RESET							= 0xE0,
	CMD_RESET							= 0xB6
} e_red_addr;

//===================================================================================================================================================
// Measurement control register
//===================================================================================================================================================

// REG_CTRL_MEASURE: 
// +----+----+----+----+----+----+----+----+
// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
// +----+----+----+----+----+----+----+----+
// | OSS     |SCO | MEASURMENT TYPE        |
// +----+----+----+----+----+----+----+----+
// 
// OSS: oversampling type (00 - once, 01 - twice, 10 - 4 times, 11 - 8 times)
// SCO: start of conversion (stays at 1 during conversion and 0 once conversion is complete)

#define CMD_SCO             0x20

#define OSS1_TYPE           0x00 /*1 sample*/
#define OSS2_TYPE           0x40 /*2 samples*/
#define OSS4_TYPE           0x80 /*4 samples*/
#define OSS8_TYPE           0xC0 /*8 samples*/

#define CMD_TEMPERATURE     0x0E
#define CMD_PRESSURE        0x14

#define CMD_TEMPERATURE1    CMD_TEMPERATURE | CMD_SCO | OSS1_TYPE
#define CMD_TEMPERATURE2    CMD_TEMPERATURE | CMD_SCO | OSS2_TYPE
#define CMD_TEMPERATURE4    CMD_TEMPERATURE | CMD_SCO | OSS4_TYPE
#define CMD_TEMPERATURE8    CMD_TEMPERATURE | CMD_SCO | OSS8_TYPE

#define CMD_PRESSURE1       CMD_PRESSURE | CMD_SCO | OSS1_TYPE
#define CMD_PRESSURE2       CMD_PRESSURE | CMD_SCO | OSS2_TYPE
#define CMD_PRESSURE4       CMD_PRESSURE | CMD_SCO | OSS4_TYPE
#define CMD_PRESSURE8       CMD_PRESSURE | CMD_SCO | OSS8_TYPE

#define OSS1_WAIT           5 /*milliseconds*/
#define OSS2_WAIT           8 /*milliseconds*/
#define OSS4_WAIT           14 /*milliseconds*/
#define OSS8_WAIT           26 /*milliseconds*/

//===================================================================================================================================================


//===================================================================================================================================================
// Measurement out registers
//===================================================================================================================================================

// REG_MEASURE_HIGH, REG_MEASURE_LOW and REG_MEASURE_XLOW (optional): 
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | 19 | 18 | 17 | 16 | 15 | 14 | 13 | 12 | 11 | 10 | 9  | 8  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | REG_MEASURE_HIGH                      | REG_MEASURE_LOW                       | REG_MEASURE_XLOW  |
// +---------------------------------------+---------------------------------------+-------------------+

// REG_MEASURE_HIGH and REG_MEASURE_LOW
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | 15 | 14 | 13 | 12 | 11 | 10 | 9  | 8  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | REG_MEASURE_HIGH                      | REG_MEASURE_LOW                       |
// +---------------------------------------+---------------------------------------+

//===================================================================================================================================================


//#############################################################################
//# Class CBMP180: class for the BMP180 (barametric pressure sensor)
//#############################################################################
class CBMP180: public CSensor
{
    public:
  
        // Construction and initialisation
        CBMP180(int32_t nSensorID = -1);
		virtual ~CBMP180()
		{
		};
        virtual void begin();

        // Mesaurement functions
        float GetTemperature1();
        float GetTemperature2();
        float GetTemperature4();
        float GetTemperature8();

		float GetPressure1();
		float GetPressure2();
		float GetPressure4();
		float GetPressure8();

		float GetAltitude1()
		{
			return CalculateAltitude(GetPressure1());
		};
		float GetAltitude2()
		{
			return CalculateAltitude(GetPressure2());
		};
		float GetAltitude4()
		{
			return CalculateAltitude(GetPressure4());
		};
		float GetAltitude8()
		{
			return CalculateAltitude(GetPressure8());
		};
		void DoDisplay();

    private:
   
        // Private data members
        int16_t m_nAC1, m_nAC2, m_nAC3, m_nAC4, m_nAC5, m_nAC6,
                  m_nVB1, m_nVB2, m_nMB, m_nMC, m_nMD, m_nLastTemperatureData;

		// Helper functions
        uint8_t GetDelay(uint8_t nCommand);
        float CalculateRealTemperature(uint32_t nRawTemperature);
        float CalculateRealPressure(uint32_t nRawPressure);
		float CalculateAltitude(float fPressure);
		int32_t CalculateRealPressure(uint32_t nRawPressure, uint8_t nOverSampling);
		void DoReset()
		{
			writeReg(REG_RESET, CMD_RESET);
			delay(100);
		};
		uint16_t readReg(e_red_addr eHighRegAddr, e_red_addr eLowRegAddr)
		{
			return CSensor::readReg16((uint8_t)eHighRegAddr, (uint8_t)eLowRegAddr);
		};
		uint8_t readReg(e_red_addr eRegAddr)
		{
			return CSensor::readReg8((uint8_t)eRegAddr);
		};
		void writeReg(e_red_addr eRegAddr, uint8_t nValue)
		{
			CSensor::writeReg((uint8_t)eRegAddr, nValue);
		};
		void writeReg(e_red_addr eHighRegAddr, e_red_addr eLowRegAddr, uint16_t nValue)
		{
			CSensor::writeReg((uint8_t)eHighRegAddr, (uint8_t)eLowRegAddr, nValue);
		};

};




extern CBMP180 bmp180;




#endif
