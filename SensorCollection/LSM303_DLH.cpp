#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include <limits.h>
#include "LSM303_DLH.h"


//================================================================================================
// Class CLSM303_DLH_Accelerator
//================================================================================================

float CLSM303_DLH_Accelerator::m_arraySensitivity[] = {0.001, 0.002, 0.004, 0.012};

// ----------------------------------------------
// Construction, destruction and initilialisation
// ----------------------------------------------
CLSM303_DLH_Accelerator::CLSM303_DLH_Accelerator(int32_t nSensorID): CSensor(nSensorID, 0, 0)
{
  m_nSensorID = nSensorID;
}

CLSM303_DLH_Accelerator::~CLSM303_DLH_Accelerator()
{
}

void CLSM303_DLH_Accelerator::begin(uint8_t nSDOPinNum, uint8_t nSDOPinState)
{
	CSensor::begin(nSDOPinNum, nSDOPinState);
	
	if (nSDOPinState == HIGH)
		setDevAddr(ADDR_ACCEL_SDO_HIGH | ADDR_ACCEL_WRITE, ADDR_ACCEL_SDO_HIGH | ADDR_ACCEL_READ);
	else if (nSDOPinState == LOW)
		setDevAddr(ADDR_ACCEL_SDO_LOW | ADDR_ACCEL_WRITE, ADDR_ACCEL_SDO_LOW | ADDR_ACCEL_READ);
	else
		Serial.println("LSM303_DLH.cpp, line 158, function bool CLSM303_DLH_Accelerator::begin(uint8_t nSDOPinState) - invalid nValue for parameter nSDOPinState...");
	  
	writeReg(CTRL_REG1_A, CTRL_REG1_A_DEFAULT);
	writeReg(CTRL_REG2_A, CTRL_REG2_A_DEFAULT);
	writeReg(CTRL_REG3_A, CTRL_REG3_A_DEFAULT);
	writeReg(CTRL_REG4_A, CTRL_REG4_A_DEFAULT);
	m_eScale = (e_scale_type)(CTRL_REG4_A_DEFAULT && 0b00110000);
	writeReg(CTRL_REG5_A, CTRL_REG5_A_DEFAULT);
	writeReg(CTRL_REG6_A, CTRL_REG6_A_DEFAULT);

	writeReg(FIFO_CTRL_REG_A, FIFO_CTRL_REG_A_DEFAULT);
	writeReg(INT1_CFG_A, INT1_CFG_A_DEFAULT);
	writeReg(INT2_CFG_A, INT2_CFG_A_DEFAULT);

	writeReg(CLICK_CFG_A, CLICK_CFG_A_DEFAULT);
	writeReg(CLICK_THS_A, CLICK_THS_A_DEFAULT);
	writeReg(TIME_LIMIT_A, TIME_LIMIT_A_DEFAULT);
	writeReg(TIME_LATENCY_A, TIME_LATENCY_A_DEFAULT);
	writeReg(TIME_WINDOW_A, TIME_WINDOW_A_DEFAULT);
}

// ----------------------------------------------
// Device settings
// ----------------------------------------------

void CLSM303_DLH_Accelerator::setCtrlReg1(e_ctrl_reg1_power_mode ePowerMode, bool bLowPwerEnabled, bool bXAxis, bool bYAxis, bool bZAxis)
{
	uint8_t nVal = (uint8_t)ePowerMode;
	
	if (bLowPwerEnabled)
		nVal != CTRL_REG1_A_LOW_POWER_ENABLE;
	
	if (bXAxis)
		nVal != CTRL_REG1_X_A_ENABLE;
	
	if (bYAxis)
		nVal != CTRL_REG1_Y_A_ENABLE;
	
	if (bZAxis)
		nVal != CTRL_REG1_Z_A_ENABLE;

	writeReg(CTRL_REG1_A, nVal);
}

void CLSM303_DLH_Accelerator::setCtrlReg(e_ctrl_reg2_hpf_mode eHPFilterMode, e_ctrl_reg2_hpf_cutoff eHPFCutOff, bool bHPFOnData, bool bHPFOnClick, bool bHPFAOIInt1, bool bHPFAOIInt2)
{
	uint8_t nVal = (uint8_t)eHPFilterMode | (uint8_t)eHPFCutOff;
	
	if (bHPFOnData)
		nVal |= CTRL_REG2_A_HPF_DATA_ENABLED;
	
	if (bHPFOnClick)
		nVal |= CTRL_REG2_A_HPF_CLICK_ENABLED;
	
	if (bHPFAOIInt1)
		nVal |= CTRL_REG2_A_HPF_AOI_INT1_ENABLED;
	
	if (bHPFAOIInt2)
		nVal |= CTRL_REG2_A_HPF_AOI_INT2_ENABLED;
	
	writeReg(CTRL_REG2_A, nVal);
}

void CLSM303_DLH_Accelerator::setCtrlReg3(bool bClick, bool bAOT1, bool bAOT2, bool bDRDY1, bool bDRDY2, bool bWTM, bool bOverrun)
{
	uint8_t nVal = 0;
	
	if (bClick)
		nVal |= CTRL_REG3_A_INT1_CLICK;
	if (bAOT1)
		nVal |= CTRL_REG3_A_INT1_AOT1;
	if (bAOT2)
		nVal |= CTRL_REG3_A_INT1_AOT2;
	if (bDRDY1)
		nVal |= CTRL_REG3_A_INT1_DRDY1;
	if (bDRDY2)
		nVal |= CTRL_REG3_A_INT1_DRDY2;
	if (bWTM)
		nVal |= CTRL_REG3_A_INT1_WTM;
	if (bOverrun)
		nVal |= CTRL_REG3_A_INT1_OVERRUN;
	
	writeReg(CTRL_REG3_A, nVal);
}

void CLSM303_DLH_Accelerator::setCtrlReg4(bool bBDU, bool bBLE, e_scale_type eScale, bool bHighRes, bool bSPI3Wire)
{
	uint8_t nVal = 0;
	
	if (bBDU)
		nVal |= CTRL_REG4_A_BDU_LATCHED;
	if (bBLE)
		nVal |= CTRL_REG4_A_BLE_ENABLED;

	nVal |= (uint8_t)eScale;
	m_eScale = eScale;

	if (bHighRes)
		nVal |= bHighRes;
	if (bSPI3Wire)
		nVal |= CTRL_REG4_A_SPI_3WIRE_ENBLED;

	writeReg(CTRL_REG4_A, nVal);
}

void CLSM303_DLH_Accelerator::setCtrlReg5(bool bBoot, bool bFIFO, bool bLirInt1, bool b4DInt1, bool bLitInt2, bool b4DInt2)
{
	uint8_t nVal = 0;
	
	if (bBoot)
		nVal |= CTRL_REG5_A_BOOT;
	else if (bFIFO)
		nVal |= CTRL_REG5_A_FIFO;
	else if (bLirInt1)
		nVal |= CTRL_REG5_A_LIR_INT1;
	else if (b4DInt1)
		nVal |= CTRL_REG5_A_4D_INT1;
	else if (bLitInt2)
		nVal |= CTRL_REG5_A_LIR_INT2;
	else if (b4DInt2)
		nVal |= CTRL_REG5_A_4D_INT2;
	
	writeReg(CTRL_REG5_A, nVal);
}

void CLSM303_DLH_Accelerator::setCtrlReg6(bool bIntPad2, bool bInt1Pad2, bool bInt2Pad2, bool bRebootPad2, bool bAFSPad2, bool bIntActiveLow)
{
	uint8_t nVal = 0;
	
	if (bIntPad2)
		nVal |= CTRL_REG6_A_CLICK_INT_PAD2_ENABLE;
	else if (bInt1Pad2)
		nVal |= CTRL_REG6_A_CLICK_INT1_PAD2_ENABLE;
	else if (bInt2Pad2)
		nVal |= CTRL_REG6_A_CLICK_INT2_PAD2_ENABLE;
	else if (bRebootPad2)
		nVal |= CTRL_REG6_A_CLICK_REBOOT_PAD2_ENABLE;
	else if (bAFSPad2)
		nVal |= CTRL_REG6_A_CLICK_AFS_PAD2_ENABLE;
	else if (bIntActiveLow)
		nVal |= CTRL_REG6_A_INT_ACTIVE_LOW_ENABLE;
	
	writeReg(CTRL_REG6_A, nVal);
}

void CLSM303_DLH_Accelerator::setFIFOCtrlReg(e_fifo_ctrl_reg_mode eFIFOMode, e_fifo_ctrl_reg_trigger eTriggerMode)
{
	uint8_t nVal = 0;
	
	nVal |= (uint8_t)eFIFOMode | (uint8_t)eTriggerMode;
	writeReg(FIFO_CTRL_REG_A, nVal);
}

void CLSM303_DLH_Accelerator::setIntCfg(bool bForInt1, e_int_cfg eIntMode, bool bXHigh, bool bYHigh, bool bZHigh, uint8_t nThreshold, uint8_t nDuration)
{
	uint8_t nRegVal = 0;
	e_accel_registers eRegCfgAddr = ACCEL_BLANK, eRegThreshAddr = ACCEL_BLANK, eRegDurAddr = ACCEL_BLANK;
	
	if (bForInt1)
	{
		eRegCfgAddr = INT1_CFG_A;
		eRegThreshAddr = INT1_THS_A;
		eRegDurAddr = INT1_DURATION_A;
	}
	else
	{
		eRegCfgAddr = INT2_CFG_A;
		eRegThreshAddr = INT2_THS_A;
		eRegDurAddr = INT2_DURATION_A;
	}
	nRegVal = (uint8_t)eIntMode;
	
	if (bXHigh)
		nRegVal |= INT_CFG_A_X_HIGH;
	else
		nRegVal |= INT_CFG_A_X_LOW;
		
	if (bYHigh)
		nRegVal |= INT_CFG_A_Y_HIGH;
	else
		nRegVal |= INT_CFG_A_Y_LOW;

	if (bZHigh)
		nRegVal |= INT_CFG_A_Z_HIGH;
	else
		nRegVal |= INT_CFG_A_Z_LOW;
	
	writeReg(eRegCfgAddr, nRegVal);
	
	if (nThreshold > 0xEF)
		Serial.println("LSM303_DLH.cpp, CLSM303_DLH_Accelerator::setInt1Cfg(...) - value for 'nThreshold' must be between 0x00 and 0xEF...");
	else if (nThreshold > 0xEF)
		Serial.println("LSM303_DLH.cpp, CLSM303_DLH_Accelerator::setInt1Cfg(...) - value for 'nDuration' must be between 0x00 and 0xEF...");
	else
	{
		writeReg(eRegThreshAddr, nThreshold);
		writeReg(eRegDurAddr, nDuration);
	}
}

void CLSM303_DLH_Accelerator::setClicikClickInt(bool bXDoubleClick, bool bYDoubleClick, bool bZDoubleClick, uint8_t nThreshold, uint8_t nTimeLimit, uint8_t nLatency, uint8_t nWindow)
{
	uint8_t nVal = 0;
	
	if (bXDoubleClick)
		nVal |= CLICK_CFG_A_X_DC;
	else
		nVal |= CLICK_CFG_A_X_SC;

	if (bYDoubleClick)
		nVal |= CLICK_CFG_A_Y_DC;
	else
		nVal |= CLICK_CFG_A_Y_SC;

	if (bZDoubleClick)
		nVal |= CLICK_CFG_A_Z_DC;
	else
		nVal |= CLICK_CFG_A_Z_SC;

	writeReg(CLICK_CFG_A, nVal);

	if (nThreshold <= 0xEF)
		writeReg(CLICK_THS_A, nThreshold);
	else
		Serial.println("LSM303_DLH.cpp, CLSM303_DLH_Accelerator::setClicikClickInt(...) - value for 'nThreshold' must be between 0x00 and 0xEF...");

	if (nTimeLimit <= 0xEF)
		writeReg(TIME_LIMIT_A, nTimeLimit);
	else
		Serial.println("LSM303_DLH.cpp, CLSM303_DLH_Accelerator::setClicikClickInt(...) - value for 'nTimeLimit' must be between 0x00 and 0xEF...");

	writeReg(TIME_LATENCY_A, nLatency);
	writeReg(TIME_WINDOW_A, nWindow);
}

// ----------------------------------------------
// Read status registers
// ----------------------------------------------

bool CLSM303_DLH_Accelerator::getIntSrc(bool bForInt1, uint8_t nFlags)
{
	bool bResult = false;
	
	if (bForInt1)
		bResult = getStatus(INT1_SOURCE_A, nFlags);
	else
		bResult = getStatus(INT2_SOURCE_A, nFlags);
	
	return bResult;
}
 
// ----------------------------------------------
// Device readings
// ----------------------------------------------

void CLSM303_DLH_Accelerator::readRaw(CVector vectData)
{
 	vectData.m_dX = (float)CSensor::readReg16(OUT_X_H_A, OUT_X_L_A), 
	vectData.m_dY = (float)CSensor::readReg16(OUT_Y_H_A, OUT_Y_L_A), 
	vectData.m_dZ = (float)CSensor::readReg16(OUT_Z_H_A, OUT_Z_L_A);
}

CVector& CLSM303_DLH_Accelerator::getAxes()
{
	static CVector vect;
	uint8_t nIndex = m_eScale >> 4;
	
	readRaw(vect);
	vect.m_dX *= m_arraySensitivity[nIndex];
	vect.m_dY *= m_arraySensitivity[nIndex];
	vect.m_dZ *= m_arraySensitivity[nIndex];
	
	return vect;
}




//================================================================================================
// Class CLSM303_DLH_Magnetometer
//================================================================================================

// Gain values from the data sheet (LSB / Gauss)
float CLSM303_DLH_Magnetometer::m_arrayGainXY[] = {1.0 / 1100.0, 1.0 / 855.0, 1.0 / 670.0, 1.0 / 450.0, 1.0 / 400.0, 1.0 / 330.0, 1.0 / 230.0};
float CLSM303_DLH_Magnetometer::m_arrayGainZ[] = {1.0 / 980.0, 1.0 / 760.0, 1.0 / 600.0, 1.0 / 400.0, 1.0 / 355.0, 1.0 / 295.0, 1.0 / 205.0};

// ----------------------------------------------
// Construction, destruction and initialisation
// ----------------------------------------------
CLSM303_DLH_Magnetometer::CLSM303_DLH_Magnetometer(int32_t nSensorID) : CSensor(nSensorID, ADDR_MAG_WRITE, ADDR_MAG_READ)
{
}

CLSM303_DLH_Magnetometer::~CLSM303_DLH_Magnetometer()
{
}

void CLSM303_DLH_Magnetometer::begin(uint8_t nSDOPinNum, uint8_t nSDOPinState)
{
	CSensor::begin(nSDOPinNum, nSDOPinState);
	
	// Enable the magnetometer
	writeReg(CRA_REG_M, CRA_REG_M_RATE_DEFAULT);
	writeReg(CRB_REG_M, CRB_REG_M_GAIN_DEFAULT);
	writeReg(MR_REG_M, MR_REG_M_DEFAULT);
	m_eGain = (e_cra_reg_m_gain)(CRB_REG_M_GAIN_DEFAULT & 0b11100000);
}

// ----------------------------------------------
// Device settings
// ----------------------------------------------

void CLSM303_DLH_Magnetometer::setMagGain(e_cra_reg_m_gain eMagGain)
{
	m_eGain = eMagGain;
	writeReg(CRB_REG_M, (uint8_t)eMagGain);
}
		
// ----------------------------------------------
// Device readings
// ----------------------------------------------

float CLSM303_DLH_Magnetometer::getHeading(CVector vectAccel, CVector vectFrom)
{
	CVector vectMag = getAxes(), vectEast, vectNorth;
	
    // subtract offset (average of min and max) vectFrom magnetometer readings
    vectMag.m_dX -= ((float)m_vectMinReadings.m_dX + m_vectMaxReadings.m_dX) / 2;
    vectMag.m_dY -= ((float)m_vectMinReadings.m_dY + m_vectMaxReadings.m_dY) / 2;
    vectMag.m_dZ -= ((float)m_vectMinReadings.m_dZ + m_vectMaxReadings.m_dZ) / 2;

	return CVector::ComputHeading(vectAccel, vectMag, vectFrom);
}

void CLSM303_DLH_Magnetometer::readRaw(CVector& vectData)
{
	vectData.m_dX = CSensor::readReg16(OUT_X_H_A, OUT_X_L_A);
	vectData.m_dY = CSensor::readReg16(OUT_Y_H_A, OUT_Y_L_A); 
	vectData.m_dZ = CSensor::readReg16(OUT_Z_H_A, OUT_Z_L_A);
}

CVector& CLSM303_DLH_Magnetometer::getAxes()
{
	static CVector vectData;
	uint8_t nIndex = m_eGain >> 5;

 	readRaw(vectData);
	vectData.m_dX *=  m_arrayGainXY[nIndex];
	vectData.m_dY *=  m_arrayGainXY[nIndex];
	vectData.m_dZ *= m_arrayGainZ[nIndex];
	
	return vectData;
}

