#include <LSM303_D.h>
#include <Wire.h>
#include <math.h>


float CLSM303_D::m_arrayMagConversion[] = {0.080, 0.0160, 0.320, 0.479}; // mgauss/LSB
float CLSM303_D::m_arrayAccelConversion[] = {0.061, 0.122, 0.183, 0.244, 0.732}; // mg/LSB

// ----------------------------------------------
// Construction, destruction and initilialisation
// ----------------------------------------------

CLSM303_D::CLSM303_D(int32_t nSensorID): CSensor(nSensorID, 0, 0)
{
	// These values lead to an assumed magnetometer bias of 0.
	// Use the Calibrate example program to determine appropriate values for your particular unit. 
	// The Heading example demonstrates how to adjust these values in your own sketch.
	m_eAccelScale = CTRL2_ACCEL_BADWIDTH_2G;
	m_MagScale = CTRL6_MAG_SCALE_2G;
	
	m_calibMag.setMinMax(129.20, 121.88, 19.01, 0.00, 479.34, 474.58);
	m_calibAccel.setAverages(0.64, 7.34, 0.075, 0.65, 7.32, 7.97, 0.71, 7.38, 0.5, 1000);
}

CLSM303_D::~CLSM303_D()
{
}

void CLSM303_D::begin(uint8_t nSDOPinNum, uint8_t nSDOPinState)
{
	CSensor::begin(nSDOPinNum, nSDOPinState);
	
	if (nSDOPinState == HIGH)
		setDevAddr(LSM303D_ADDR_SDO_HIGH, LSM303D_ADDR_SDO_HIGH);
	else if (nSDOPinState == LOW)
		setDevAddr(LSM303D_ADDR_SDO_LOW, LSM303D_ADDR_SDO_LOW);
	else
	  Serial.println("CLSM303_D::begin(uint8_t nSDOPinNum, uint8_t nSDOPinState): '" + String(nSDOPinState) + "' is not a valid pin state for parameter 'nSDOPinState'...");
  
	setDefaultMode();
}

// ----------------------------------------------
// Device settings
// ----------------------------------------------

void CLSM303_D::setDefaultMode(void)
{
	writeReg(INT_CTRL_M, INT_CTRL_M_DEFAULT);
	writeReg(CTRL0, CTRL0_DEFAULT);
	writeReg(CTRL1, CTRL1_DEFAULT);
	writeReg(CTRL2, CTRL2_DEFAULT);
	writeReg(CTRL3, CTRL3_DEFAULT);
	writeReg(CTRL4, CTRL4_DEFAULT);
	writeReg(CTRL5, CTRL5_DEFAULT);
	writeReg(CTRL6, CTRL6_DEFAULT);
	writeReg(CTRL7, CTRL7_DEFAULT);
	writeReg(FIFO_CTRL, FIFO_CTRL_DEFAULT);
	writeReg(IG_CFG1, IG_CFG_DEFAULT);
	writeReg(IG_CFG2, IG_CFG_DEFAULT);
	writeReg(CLICK_CFG, CLICK_CFG_DEFAULT);
	m_eAccelScale = CTRL2_ACCEL_BADWIDTH_6G;
	m_MagScale = CTRL6_MAG_SCALE_4G;
	//Serial.println(String("*************** ") + getWireError() + " ***************");
}

void CLSM303_D::setIntCtrlM(int_crl_m_axes eAxes, bool bOpenDrain, bool bActiveHigh, uint16_t nThreshold, bool bLatchEnable, bool b4DEnable)
{
	uint8_t nVal = INT_CTRL_M_ENABLE | (uint8_t)eAxes;
	
	if (bOpenDrain)
		nVal |= INT_CTRL_M_OPEN_DRAIN_ENABLE;
	
	if (bActiveHigh)
		nVal |= INT_CTRL_M_ACTIVE_HIGH;

	if (bLatchEnable)
		nVal |= INT_CTRL_M_LATCH_ENABLE;

	if (b4DEnable)
		nVal |= INT_CTRL_M_4D_ENABLE;
	
	writeReg(INT_THS_H_M, INT_THS_L_M, nThreshold);
}

void CLSM303_D::setCtrl0HPF(bool bHPFInt2, bool bHPFInt1, bool bHPFClick, bool bFIFOThreshold, bool bFIFO, bool bBoot)
{
	uint8_t nVal = 0;
	
	if (bHPFInt2)
		nVal |= CTRL0_HPF_INT2;

	if (bHPFInt1)
		nVal |= CTRL0_HPF_INT1;

	if (bHPFClick)
		nVal |= CTRL0_HPF_CLICK_ENABLE;

	if (bFIFOThreshold)
		nVal |= CTRL0_FIFO_THRESH_ENABLE;

	if (bFIFO)
		nVal |= CTRL0_FIFO_ENABLE;

	if (bBoot)
		nVal |= CTRL0_BOOT_ENABLE;

	writeReg(CTRL0, nVal);
}

void CLSM303_D::setCtrl1AccelData(e_ctrl1_power_mode ePower, bool bBDUEnable, bool bXAxis, bool bYAxis, bool bZAxis)
{
	uint8_t nVal = (uint8_t)ePower;
	
	if (bBDUEnable)
		nVal |= CTRL1_BDU_ENABLE;
	
	if (bXAxis)
		nVal |= CTRL1_X_ENABLE;
	
	if (bYAxis)
		nVal |= CTRL1_Y_ENABLE;
	
	if (bZAxis)
		nVal |= CTRL1_Z_ENABLE;

	writeReg(CTRL1, nVal);
}

void CLSM303_D::setMagAxesOffset(uint16_t nXAxis, uint16_t nYAxis, uint16_t nZAxis)
{
	writeReg(OFFSET_X_H_M, OFFSET_X_L_M, nXAxis);
	writeReg(OFFSET_Y_H_M, OFFSET_Y_L_M, nXAxis);
	writeReg(OFFSET_Z_H_M, OFFSET_Z_L_M, nXAxis);
}

void CLSM303_D::setCtrl2AccelScale(ctrl2_aaf_bandwidth eAAFBandWidth, ctrl2_accel_scale eAccelScale, bool bEnableSelfTest, bool bEnableSPI3WireEnable)
{
	uint8_t nVal = (uint8_t)eAAFBandWidth | (uint8_t)eAccelScale;
	
	if (bEnableSelfTest)
		nVal |= CTRL2_ACCEL_SELF_TEST;

	if (bEnableSPI3WireEnable)
		nVal |= CTRL2_ACCEL_SPI_3WIRE_ENABLE;

	writeReg(CTRL2, nVal);
}

void CLSM303_D::setCtrl34Int(bool nForInt1, bool bBoot, bool bClick, bool bIG1, bool bIG2, bool bMag, bool bAccelDR, bool bMagDR, bool bFIFOEmpty)
{
	uint8_t nVal = 0;
	
	if (bBoot)
		nVal |= CTRL_INT_BOOT;
	
	if (bClick)
		nVal |= CTRL_INT_CLICK;
	
	if (bIG1)
		nVal |= CTRL_INT_IG1;
	
	if (bIG2)
		nVal |= CTRL_INT_IG2;
	
	if (bMag)
		nVal |= CTRL_INT_MAGNETIC;
	
	if (bAccelDR)
		nVal |= CTRL_INT_ACCEL_DR;
	
	if (bMagDR)
		nVal |= CTRL_INT_MAG_DR;
	
	if (bFIFOEmpty)
		nVal |= CTRL_INT_FIFO_EMPTY;
	
	if (nForInt1)
		writeReg(CTRL3, nVal);
	else
		writeReg(CTRL4, nVal);
}

void CLSM303_D::setCtrl5Mag(bool bTempEnable, e_ctrl5_mag_resolution eMagRes, e_ctrl5_mag_data_rate eMagDataRate, bool bInt1Latch, bool bInt2Latch)
{
	uint8_t nVal = (uint8_t)eMagRes | (uint8_t)eMagDataRate;
	
	if (bTempEnable)
		nVal |= CTRL5_TEMP_ENABLE;
	
	if (bInt1Latch)
		nVal |= CTRL5_INT1_LATCH;
	
	if (bInt2Latch)
		nVal |= CTRL5_INT2_LATCH;
	
	writeReg(CTRL5, nVal);
}

void CLSM303_D::setCtrl7MAgMode(e_hpf_mode eHPFMode, bool bFilterEnable, bool bTempOnly, bool bMagLowPower, e_mag_data_mode eMagDataMode, uint16_t nXAxisRef, uint16_t nYAxisRef, uint16_t nZAxisRef)
{
	uint8_t nVal = (uint8_t)eHPFMode | (uint8_t)eMagDataMode;
	
	if (bFilterEnable)
		nVal |= CTRL7_FILTER_ENABLE;
	
	if (bTempOnly)
		nVal |= CTRL7_TEMPERATURE_ONLY;
	
	if (bMagLowPower)
		nVal |= CTRL7_MAG_LOW_POWER_ENABLE;
	
	writeReg(CTRL7, nVal);
}

void CLSM303_D::setFIFOCtrl(e_fifo_ctrl eFIFOCtrl, uint8_t nFIFOThreshold)
{
	uint8_t nVal = (uint8_t)eFIFOCtrl;
	
	writeReg(FIFO_CTRL, nVal);
	if (nFIFOThreshold <= 0b00011111)
		writeReg(FIFO_CTRL, nFIFOThreshold);
	else
		Serial.println("CLSM303_D::setFIFOCtrl(e_fifo_ctrl eFIFOCtrl, uint8_t nFIFOThreshold) - parameter 'nFIFOThreshold' is larger than 0x1F...");
}

void CLSM303_D::setIgCfg(bool bForIG1, e_ig_cfg_int_mode eMode, bool nXHigh, bool nXLow, bool nYHigh, bool nYLow, bool nZHigh, bool nZLow)
{
	reg_addr nRegAddr = BLANK;
	uint8_t nVal = (uint8_t)eMode;
	
	if (bForIG1)
		nRegAddr = IG_CFG1;
	else
		nRegAddr = IG_CFG2;
	
	if (nXLow)
		nVal |= IG_CFG1_X_LOW;
	if (nXHigh)
		nVal |= IG_CFG1_X_HIGH;	
	if (nYLow)
		nVal |= IG_CFG1_Y_LOW;
	if (nYHigh)
		nVal |= IG_CFG1_Y_HIGH;
	if (nZLow)
		nVal |= IG_CFG1_Z_LOW;
	if (nZHigh)
		nVal |= IG_CFG1_Z_HIGH;
	
	writeReg(nRegAddr, nVal);
}

void CLSM303_D::setClickCfg(bool bDCXAxis, bool bSCXAxis, bool bDCYAxis, bool bSCYAxis, bool bDCZAxis, bool bSCZAxis)
{
	uint8_t nVal = 0;
	
	if (bDCXAxis)
		nVal |= CLICK_CFG_DC_X;
	if (bSCXAxis)
		nVal |= CLICK_CFG_SC_X;
	if (bDCYAxis)
		nVal |= CLICK_CFG_DC_Y;
	if (bSCYAxis)
		nVal |= CLICK_CFG_SC_Y;
	if (bDCZAxis)
		nVal |= CLICK_CFG_DC_Z;
	if (bSCZAxis)
		nVal |= CLICK_CFG_SC_Z;
	
	writeReg(CLICK_CFG, nVal);
}

// ----------------------------------------------
// Read status registers
// ----------------------------------------------

bool CLSM303_D::getIGStatus(bool bForIG1, bool nXLow, bool nXHigh, bool nYLow, bool nYHigh, bool nZLow, bool nZHigh)
{
	uint8_t nRegAddr = 0, nBitFlags = 0;
	
	if (bForIG1)
		nRegAddr = IG_SRC1;
	else
		nRegAddr = IG_SRC2;
	
	if (nXLow)
		nBitFlags |= IG_SRC_INT_X_LOW;
	if (nXHigh)
		nBitFlags |= IG_SRC_INT_X_HIGH;
	
	if (nYLow)
		nBitFlags |= IG_SRC_INT_Y_LOW;
	if (nYHigh)
		nBitFlags |= IG_SRC_INT_Y_HIGH;
	
	if (nZLow)
		nBitFlags |= IG_SRC_INT_Z_LOW;
	if (nZHigh)
		nBitFlags |= IG_SRC_INT_Z_HIGH;
	
	return getStatus(nRegAddr, nBitFlags);
}

// ----------------------------------------------
// Device readings
// ----------------------------------------------

CVector& CLSM303_D::getAxesA()
{
	static CVector vect;
	float fConversion = m_arrayAccelConversion[m_eAccelScale > 3];

	vect.m_dX = (readReg(OUT_X_H_A, OUT_X_L_A) * fConversion) / 1000;
	vect.m_dY = (readReg(OUT_Y_H_A, OUT_Y_L_A) * fConversion) / 1000;
	vect.m_dZ = (readReg(OUT_Z_H_A, OUT_Z_L_A) * fConversion) / 1000;
Serial.println(fConversion);
	//vect.m_dX = m_calibAccel.calibrateX(vect.m_dX);
	//vect.m_dY = m_calibAccel.calibrateY(vect.m_dY);
	//vect.m_dY = m_calibAccel.calibrateZ(vect.m_dZ);
	
	return vect;
}

CVector& CLSM303_D::getAxesM()
{
	static CVector vect;
	float fConversion = m_arrayAccelConversion[m_eAccelScale > 3];
	
	vect.m_dX = (readReg(OUT_X_H_M, OUT_X_L_M) * fConversion) - m_calibMag.getMedianX();
	vect.m_dY = (readReg(OUT_Y_H_M, OUT_Y_L_M) * fConversion) - m_calibMag.getMedianX();
	vect.m_dZ = (readReg(OUT_Z_H_M, OUT_Z_L_M) * fConversion) - m_calibMag.getMedianX();

	return vect;
}

void CLSM303_D::getAxes(CVector &vectMag, CVector &vectAccel)
{
	vectAccel = getAxesA();
	vectMag = getAxesM();
}

// Returns the angular difference in the horizontal plane between the "vectFrom" vector and north, in degrees.

// Description of fHeading algorithm:
// 		Shift and scale the magnetic reading based on calibration data to find the North vector. Use the 
//      acceleration readings to determine the Up vector (gravity is measured as an upward acceleration). The 
//      cross product of North and Up vectors is East. The vectors East and North form a basis for the horizontal 
//      plane. The From vector is projected into the horizontal plane and the angle between the projected vector
// 		and horizontal north is returned.
// 
// 		The default vector here is chosen to point along the surface of the PCB, in the direction of the top of 
//      the text on the silkscreen. This is the +X axis on the Pololu CLSM303_DD carrier and the -Y axis on the 
//		Pololu CLSM303_DDLHC, CLSM303_DDLM, and CLSM303_DDLH carriers.
float CLSM303_D::getHeading(CVector vectFrom)
{
	CVector vectMag, vectAccel, vectEast, vectNorth;
    
	getAxes(vectMag, vectAccel);

    // Subtract offset (average of min and max) vectFrom magnetometer readings
    vectMag.m_dX -= m_calibMag.getMedianX();
    vectMag.m_dY -= m_calibMag.getMedianY();
    vectMag.m_dZ -= m_calibMag.getMedianZ();

	return CVector::ComputHeading(vectAccel, vectMag, vectFrom);
}

