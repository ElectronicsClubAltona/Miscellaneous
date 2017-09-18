#include <Wire.h>
#include <L3GD20.h>




//############################################################
//# Class CL3GD20: class for the L3GD20 model (gyro)
//############################################################

CL3GD20::CL3GD20(uint8_t nDevAddr): CAxesDeviceBase(nDevAddr)
{
	m_fScaleFactor = 0.0;
}

CL3GD20::~CL3GD20()
{
	m_fScaleFactor = 0;
}

// Get the register name from the value e.g."WHO_AM_I".
const char *CL3GD20::getRegName(const uint8_t nRegAddr)
{
	static String strRegName;

	switch (nRegAddr)
	{
		case WHO_AM_I: strRegName = "WHO_AM_I"; break;
		case CTRL_REG1: strRegName = "CTRL_REG1"; break;
		case CTRL_REG2: strRegName = "CTRL_REG2"; break;
		case CTRL_REG3: strRegName = "CTRL_REG3"; break;
		case CTRL_REG4: strRegName = "CTRL_REG4"; break;
		case CTRL_REG5: strRegName = "CTRL_REG5"; break;
		case REFFERENCE: strRegName = "REFFERENCE"; break;
		case OUT_TEMP: strRegName = "OUT_TEMP"; break;
		case STATUS_REG: strRegName = "STATUS_REG"; break;
		case OUT_X_L: strRegName = "OUT_X_L"; break;
		case OUT_X_H: strRegName = "OUT_X_H"; break;
		case OUT_Y_L: strRegName = "OUT_Y_L"; break;
		case OUT_Y_H: strRegName = "OUT_Y_H"; break;
		case OUT_Z_L: strRegName = "OUT_Z_L"; break;
		case OUT_Z_H: strRegName = "OUT_Z_H"; break;
		case FIFO_CTRL_REG: strRegName = "FIFO_CTRL_REG"; break;
		case FIFO_SRC_REG: strRegName = "FIFO_SRC_REG"; break;
		case INT1_CFG: strRegName = "INT1_CFG"; break;
		case INT1_SRC: strRegName = "INT1_SRC"; break;
		case INT1_TSH_XH: strRegName = "INT1_TSH_XH"; break;
		case INT1_TSH_XL: strRegName = "INT1_TSH_XL"; break;
		case INT1_TSH_YH: strRegName = "INT1_TSH_YH"; break;
		case INT1_TSH_YL: strRegName = "INT1_TSH_YL"; break;
		case INT1_TSH_ZH: strRegName = "INT1_TSH_ZH"; break;
		case INT1_TSH_ZL: strRegName = "INT1_TSH_ZL"; break;
		case INT1_DURATION: strRegName = "INT1_DURATION"; break;
		default: strRegName = "Unknown register value"; break;
	}
	return strRegName.c_str();
}

// Configure the device and carry out any other necessary setup tasks.
void CL3GD20::begin()
{
	Wire.begin();
	m_fScaleFactor = SCALE_FACTOR_250DPS;
	writeReg(CTRL_REG1, CTRL_REG1_DEFAULT);
	writeReg(CTRL_REG2, CTRL_REG2_DEFAULT);
	writeReg(CTRL_REG3, CTRL_REG3_DEFAULT);
	writeReg(CTRL_REG4, CTRL_REG4_DEFAULT);
	writeReg(CTRL_REG5, CTRL_REG5_DEFAULT);
	CAxesDeviceBase::begin();
}

// Change the data sampling rate.
void CL3GD20::setDataRate(uint8_t nValue)
{
	// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | DR | DR | BW | BW | PW | ZE | YE | XE |
	// +----+----+----+----+----+----+----+----+
	if ((nValue == CTRL_REG1_RATE95) || (nValue == CTRL_REG1_RATE190) || (nValue == CTRL_REG1_RATE380) || (nValue == CTRL_REG1_RATE760))
		writeRegBits(CTRL_REG1, (uint8_t)0b00111111, nValue);
	else
		print("Runtime error in void setDataRate(const uint8_t nValue): invalid paramater value");
}

// Change the device band width.
void CL3GD20::setBandWidth(uint8_t nValue)
{
	// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | DR | DR | BW | BW | PW | ZE | YE | XE |
	// +----+----+----+----+----+----+----+----+
	if ((nValue == CTRL_REG1_BANDWIDTH1) || (nValue == CTRL_REG1_BANDWIDTH2) || (nValue == CTRL_REG1_BANDWIDTH3) || (nValue == CTRL_REG1_BANDWIDTH4))
		writeRegBits(CTRL_REG1, (uint8_t)0b11001111, nValue);
	else
		print("Runtime error in void CL3GD20::setBandWidth(const uint8_t nValue): invalid paramater value");
}

void CL3GD20::setScale(uint8_t nValue /* An appropriate define from header file. */)
{
	// CTRL_REG4: block data, endian, scale and SPI settings.
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// | BDU | BLE | FS  | FS  |  -  |  0  |  0  | SIM |
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// #define CTRL_REG4_SCALE_250DPS			(uint8_t)0x00
	// #define CTRL_REG4_SCALE_500DPS			(uint8_t)0x10
	// #define CTRL_REG4_SCALE_2000DPS			(uint8_t)0x20
	const uint8_t nMask = (uint8_t)0b11000001;

	if (nValue == CTRL_REG4_SCALE_250DPS)
	{
		writeRegBits(CTRL_REG4, nMask, CTRL_REG4_SCALE_250DPS);
		m_fScaleFactor = SCALE_FACTOR_250DPS;
	}
	else if (nValue == CTRL_REG4_SCALE_500DPS)
	{
		writeRegBits(CTRL_REG4, nMask, CTRL_REG4_SCALE_500DPS);
		m_fScaleFactor = SCALE_FACTOR_500DPS;
	}
	else if (nValue == CTRL_REG4_SCALE_2000DPS)
	{
		writeRegBits(CTRL_REG4, nMask, CTRL_REG4_SCALE_2000DPS);
		m_fScaleFactor = SCALE_FACTOR_2000DPS;
	}
	else
		print("Runtime error in void CL3GD20::setScale(const uint8_t nValue): invalid paramater value");
	
}

// Set or remove an interrupt to trigger on high or low rotational speed on the X axis
// bHighOrLowInt: 0 = interrupt triggered on low rotational speed, 1 = interrupt triggered on high rotational speed
// bEnable: 0 = disable interrupt specified by bHighOrLowInt, 1 = enable interrupt specified by bHighOrLowInt
// nThreshold: threshold value for interrupt triggering.
// nDuration: 0 = interrupts are triggered as soon as rotational speed exceeds threshold, >0 = interrupts are
//            generated only if rotational speed exceeds threshold for this period of time
void CL3GD20::setIntX(const bool bHighOrLowInt, const bool bEnable, const int16_t nThreshold, const uint8_t nDuration, const bool bLatch)
{
	// INT1_CFG - interrupt configuration register.
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// |   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// | AND/OR |  LIR   |  ZHIE  |  ZLIE  |  YHIE  |  YLIE  |  XHIE  |  XLIE  |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// AND/OR: combination of interrupt events - 0 = OR combination of interrupt events (default), 1 = AND combination of interrupt events.
	// LIR: latch interrupt request - 0 = interrupt request not latched (default), 1 =  interrupt request latched and cleared by reading INT1_SRC reg.
	// ZHIE: enable interrupt generation on Z high event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value higher than preset threshold.
	// ZLIE: enable interrupt generation on Z low event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value lower than preset threshold.
	// YHIE: enable interrupt generation on Y high event - 0 = disable interrupt request(default), 1 = enable interrupt request on measured value higher than preset threshold.
	// YLIE: enable interrupt generation on Y low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	// XHIE enable interrupt generation on X high event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value higher than preset threshold.
	// XLIE: enable interrupt generation on X low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	uint8_t nValue = readReg(INT1_CFG);

	if (bLatch)
		writeReg(INT1_CFG, nValue | 0b01000000);
	else
		writeReg(INT1_CFG, nValue & 0b10111111);

	nValue = readReg(INT1_CFG);
	if (bHighOrLowInt)
	{
		if (bEnable)
			writeReg(INT1_CFG, nValue | 0b00000010);
		else
			writeReg(INT1_CFG, nValue & 0b11111101);
	}
	else
	{
		if (bEnable)
			writeReg(INT1_CFG, nValue | 0b00000001);
		else
			writeReg(INT1_CFG, nValue & 0b11111110);
	}

	// Set the threshold
	writeReg(INT1_TSH_XL, nThreshold & 0b0000000011111111);
	writeReg(INT1_TSH_XH, nThreshold >> 8);

	// INT1_DURATION: interrupt duration register.
	// +------+------+------+------+------+------+------+------+
	// |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
	// +------+------+------+------+------+------+------+------+
	// | WAIT |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
	// +------+------+------+------+------+------+------+------+
	// WAIT: interrupt mode - 0 = interrupt falls immediately if the signal crosses the selected threshold, 
	//						  1 =  if the signal crosses the selected threshold the interrupt falls only after the duration has counted the number of samples 
	//							   at the selected data rate, written into the duration counter register.
	nValue = readReg(INT1_DURATION);

	if (nDuration > 0)
		writeReg(INT1_DURATION, nDuration | 0b10000000);
	else
		writeReg(INT1_DURATION, nDuration & 0b01111111);
}

// Set or remove an interrupt to trigger on high or low rotational speed on the Y axis
// bHighOrLowInt: 0 = interrupt triggered on low rotational speed, 1 = interrupt triggered on high rotational speed
// bEnable: 0 = disable interrupt specified by bHighOrLowInt, 1 = enable interrupt specified by bHighOrLowInt
// nThreshold: threshold value for interrupt triggering.
// nDuration: 0 = interrupts are triggered as soon as rotational speed exceeds threshold, >0 = interrupts are
//            generated only if rotational speed exceeds threshold for this period of time
void CL3GD20::setIntY(const bool bHighOrLowInt, const bool bEnable, const int16_t nThreshold, const uint8_t nDuration, const bool bLatch)
{
	// INT1_CFG - interrupt configuration register.
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// |   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// | AND/OR |  LIR   |  ZHIE  |  ZLIE  |  YHIE  |  YLIE  |  XHIE  |  XLIE  |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// AND/OR: combination of interrupt events - 0 = OR combination of interrupt events (default), 1 = AND combination of interrupt events.
	// LIR: latch interrupt request - 0 = interrupt request not latched (default), 1 =  interrupt request latched and cleared by reading INT1_SRC reg.
	// ZHIE: enable interrupt generation on Z high event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value higher than preset threshold.
	// ZLIE: enable interrupt generation on Z low event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value lower than preset threshold.
	// YHIE: enable interrupt generation on Y high event - 0 = disable interrupt request(default), 1 = enable interrupt request on measured value higher than preset threshold.
	// YLIE: enable interrupt generation on Y low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	// XHIE enable interrupt generation on X high event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value higher than preset threshold.
	// XLIE: enable interrupt generation on X low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	uint8_t nValue = readReg(INT1_CFG);

	if (bLatch)
		writeReg(INT1_CFG, nValue | 0b01000000);
	else
		writeReg(INT1_CFG, nValue & 0b10111111);

	nValue = readReg(INT1_CFG);
	if (bHighOrLowInt)
	{
		if (bEnable)
			writeReg(INT1_CFG, nValue | 0b00001000);
		else
			writeReg(INT1_CFG, nValue & 0b11110111);
	}
	else
	{
		if (bEnable)
			writeReg(INT1_CFG, nValue | 0b00000100);
		else
			writeReg(INT1_CFG, nValue & 0b11111011);
	}

	// Set the threshold
	writeReg(INT1_TSH_YL, nThreshold & 0b0000000011111111);
	writeReg(INT1_TSH_YH, nThreshold >> 8);

	// INT1_DURATION: interrupt duration register.
	// +------+------+------+------+------+------+------+------+
	// |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
	// +------+------+------+------+------+------+------+------+
	// | WAIT |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
	// +------+------+------+------+------+------+------+------+
	// WAIT: interrupt mode - 0 = interrupt falls immediately if the signal crosses the selected threshold, 
	//						  1 =  if the signal crosses the selected threshold the interrupt falls only after the duration has counted the number of samples 
	//							   at the selected data rate, written into the duration counter register.
	if (nDuration > 0)
		writeReg(INT1_DURATION, nDuration | 0b10000000);
	else
		writeReg(INT1_DURATION, nDuration & 0b01111111);
}

// Set or remove an interrupt to trigger on high or low rotational speed on the Z axis
// bHighOrLowInt: 0 = interrupt triggered on low rotational speed, 1 = interrupt triggered on high rotational speed
// bEnable: 0 = disable interrupt specified by bHighOrLowInt, 1 = enable interrupt specified by bHighOrLowInt
// nThreshold: threshold value for interrupt triggering.
// nDuration: 0 = interrupts are triggered as soon as rotational speed exceeds threshold, >0 = interrupts are
//            generated only if rotational speed exceeds threshold for this period of time
void CL3GD20::setIntZ(const bool bHighOrLowInt, const bool bEnable, const int16_t nThreshold, const uint8_t nDuration, const bool bLatch)
{
	// INT1_CFG - interrupt configuration register.
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// |   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// | AND/OR |  LIR   |  ZHIE  |  ZLIE  |  YHIE  |  YLIE  |  XHIE  |  XLIE  |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// AND/OR: combination of interrupt events - 0 = OR combination of interrupt events (default), 1 = AND combination of interrupt events.
	// LIR: latch interrupt request - 0 = interrupt request not latched (default), 1 =  interrupt request latched and cleared by reading INT1_SRC reg.
	// ZHIE: enable interrupt generation on Z high event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value higher than preset threshold.
	// ZLIE: enable interrupt generation on Z low event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value lower than preset threshold.
	// YHIE: enable interrupt generation on Y high event - 0 = disable interrupt request(default), 1 = enable interrupt request on measured value higher than preset threshold.
	// YLIE: enable interrupt generation on Y low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	// XHIE enable interrupt generation on X high event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value higher than preset threshold.
	// XLIE: enable interrupt generation on X low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	uint8_t nValue = readReg(INT1_CFG);

	if (bLatch)
		writeReg(INT1_CFG, nValue | 0b01000000);
	else
		writeReg(INT1_CFG, nValue & 0b10111111);

	nValue = readReg(INT1_CFG);
	if (bHighOrLowInt)
	{
		if (bEnable)
			writeReg(INT1_CFG, nValue | 0b00100000);
		else
			writeReg(INT1_CFG, nValue & 0b11011111);
	}
	else
	{
		if (bEnable)
			writeReg(INT1_CFG, nValue | 0b00010000);
		else
			writeReg(INT1_CFG, nValue & 0b11101111);
	}

	// Set the threshold
	writeReg(INT1_TSH_YL, nThreshold & 0b0000000011111111);
	writeReg(INT1_TSH_YH, nThreshold >> 8);

	// INT1_DURATION: interrupt duration register.
	// +------+------+------+------+------+------+------+------+
	// |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
	// +------+------+------+------+------+------+------+------+
	// | WAIT |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
	// +------+------+------+------+------+------+------+------+
	// WAIT: interrupt mode - 0 = interrupt falls immediately if the signal crosses the selected threshold, 
	//						  1 =  if the signal crosses the selected threshold the interrupt falls only after the duration has counted the number of samples 
	//							   at the selected data rate, written into the duration counter register.
	if (nDuration > 0)
		writeReg(INT1_DURATION, nDuration | 0b10000000);
	else
		writeReg(INT1_DURATION, nDuration & 0b01111111);
}

// Enable interrupts in INT1 pin and set the interrupt polarity
void CL3GD20::enableInt1(const bool bEnable, const bool bLowToHigh)
{
	// CTRL_REG3: interrupt settings.
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// | IE1 | IB1 | IP1 | OD  | DR2 | WM2 | FO2 | FE2 |
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// IE1: interrupt enable on INT1 pin - 0 = disable (default), 1 = enable.
	// IB1: boot status available on INT1 - 0 = disable (default). 1 = enable.
	// IP1: interrupt active configuration on INT1 - 0 = interrupt signalled high to low transition (default), 1 = interrupt signalled by low to high transition.
	// OD: push-pull / open drain - 0 = push- pull (default), 1 = open drain.
	// DR2: date-ready on DRDY/INT2 - 0 = disable (default), 1 = enable.
	// WM2: FIFO watermark interrupt on DRDY/INT2 - 0 = disable (default), 1 = enable.
	// FO2: FIFO overrun interrupt on DRDY/INT2 - 0 = disable (default),  1 = enable.
	// FE2: FIFO empty interrupt on DRDY/INT2 - 0 = disable (default), 1 = enable.
	
	uint8_t nCurrentVal = readReg(CTRL_REG3);
	writeReg(CTRL_REG3, nCurrentVal | 0b10000000);
	setIntPol(bLowToHigh);
}

// Set interrupt polarity: high to low or low to high
void CL3GD20::setIntPol(const bool bLowToHigh)
{
	// CTRL_REG3: interrupt settings.
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// | IE1 | IB1 | IP1 | OD  | DR2 | WM2 | FO2 | FE2 |
	// +-----+-----+-----+-----+-----+-----+-----+-----+
	// IE1: interrupt enable on INT1 pin - 0 = disable (default), 1 = enable.
	// IB1: boot status available on INT1 - 0 = disable (default). 1 = enable.
	// IP1: interrupt active configuration on INT1 - 0 = interrupt signalled high to low transition (default), 1 = interrupt signalled by low to high transition.
	// OD: push-pull / open drain - 0 = push- pull (default), 1 = open drain.
	// DR2: date-ready on DRDY/INT2 - 0 = disable (default), 1 = enable.
	// WM2: FIFO watermark interrupt on DRDY/INT2 - 0 = disable (default), 1 = enable.
	// FO2: FIFO overrun interrupt on DRDY/INT2 - 0 = disable (default),  1 = enable.
	// FE2: FIFO empty interrupt on DRDY/INT2 - 0 = disable (default), 1 = enable.
	uint8_t nCurrentVal = readReg(CTRL_REG3);

	if (bLowToHigh)
		writeReg(CTRL_REG3, nCurrentVal | 0b00100000);
	else
		writeReg(CTRL_REG3, nCurrentVal & 0b11011111);
}

// Interrupts, set with setIntX(...) etc trigger interrupts one at a time.
void CL3GD20::setORInts()
{
	// INT1_CFG - interrupt configuration register.
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// |   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// | AND/OR |  LIR   |  ZHIE  |  ZLIE  |  YHIE  |  YLIE  |  XHIE  |  XLIE  |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// AND/OR: combination of interrupt events - 0 = OR combination of interrupt events (default), 1 = AND combination of interrupt events.
	// LIR: latch interrupt request - 0 = interrupt request not latched (default), 1 =  interrupt request latched. Cleared by reading INT1_SRC reg.
	// ZHIE: enable interrupt generation on Z high event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value higher than preset threshold.
	// ZLIE: enable interrupt generation on Z low event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value lower than preset threshold.
	// YHIE: enable interrupt generation on Y high event - 0 = disable interrupt request(default), 1 = enable interrupt request on measured value higher than preset threshold.
	// YLIE: enable interrupt generation on Y low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	// XHIE enable interrupt generation on X high event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value higher than preset threshold.
	// XLIE: enable interrupt generation on X low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	uint8_t nCurrentVal = readReg(INT1_CFG);

	writeReg(INT1_CFG, nCurrentVal & 0b01111111);
}

// Interrupts, set with setIntX(...) etc can trigger interrupts simultaneously.
void CL3GD20::setANDInts()
{
	// INT1_CFG - interrupt configuration register.
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// |   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// | AND/OR |  LIR   |  ZHIE  |  ZLIE  |  YHIE  |  YLIE  |  XHIE  |  XLIE  |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// AND/OR: combination of interrupt events - 0 = OR combination of interrupt events (default), 1 = AND combination of interrupt events.
	// LIR: latch interrupt request - 0 = interrupt request not latched (default), 1 =  interrupt request latched. Cleared by reading INT1_SRC reg.
	// ZHIE: enable interrupt generation on Z high event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value higher than preset threshold.
	// ZLIE: enable interrupt generation on Z low event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value lower than preset threshold.
	// YHIE: enable interrupt generation on Y high event - 0 = disable interrupt request(default), 1 = enable interrupt request on measured value higher than preset threshold.
	// YLIE: enable interrupt generation on Y low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	// XHIE enable interrupt generation on X high event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value higher than preset threshold.
	// XLIE: enable interrupt generation on X low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	uint8_t nCurrentVal = readReg(INT1_CFG);

	writeReg(INT1_CFG, nCurrentVal | 0b10000000);
}

// Clear all X, Y & Z interrupts
void CL3GD20::removeXYZInts()
{
	// INT1_CFG - interrupt configuration register.
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// |   7    |   6    |   5    |   4    |   3    |   2    |   1    |   0    |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// | AND/OR |  LIR   |  ZHIE  |  ZLIE  |  YHIE  |  YLIE  |  XHIE  |  XLIE  |
	// +--------+--------+--------+--------+--------+--------+--------+--------+
	// AND/OR: combination of interrupt events - 0 = OR combination of interrupt events (default), 1 = AND combination of interrupt events.
	// LIR: latch interrupt request - 0 = interrupt request not latched (default), 1 =  interrupt request latched. Cleared by reading INT1_SRC reg.
	// ZHIE: enable interrupt generation on Z high event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value higher than preset threshold.
	// ZLIE: enable interrupt generation on Z low event - 0 = disable interrupt request (default), 1 = enable interrupt request on measured value lower than preset threshold.
	// YHIE: enable interrupt generation on Y high event - 0 = disable interrupt request(default), 1 = enable interrupt request on measured value higher than preset threshold.
	// YLIE: enable interrupt generation on Y low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	// XHIE enable interrupt generation on X high event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value higher than preset threshold.
	// XLIE: enable interrupt generation on X low event - 0 = disable interrupt request (default), 1 =  enable interrupt request on measured value lower than preset threshold.
	uint8_t nCurrentVal = readReg(INT1_CFG);

	writeReg(INT1_CFG, nCurrentVal & 0b00000000);
}

// Did a high value on the X axis trigger the interrupt
bool CL3GD20::isXHighInt()
{
	// INT1_SRC: interrupt source register.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | 0  | IA | ZH | ZL | YH | YL | XH | XL |
	// +----+----+----+----+----+----+----+----+
	// IA: interrupt active - 0 = no interrupt has been generated (default), 1 = one or more interrupts have been generated.
	// ZH: Z high - 0 = no interrupt (default), 1 = Z high event has occurred.
	// ZL: Z low - 0 = no interrupt (default), 1 = Z low event has occurred.
	// YH: Y high - 0 = no interrupt (default), 1 = Y high event has occurred.
	// YL: Y low - 0 = no interrupt (default), 1 = Y low event has occurred.
	// XH: X high - 0 = no interrupt (default), 1 = X High event has occurred.
	// XL: X low - 0 = no interrupt (default), 1 = X Low event has occurred.
	uint8_t nVal = readReg(INT1_SRC);

	return (nVal & 0b00000010) > 0;
}

// Did a low value on the X axis trigger the interrupt
bool CL3GD20::isXLowInt()
{
	// INT1_SRC: interrupt source register.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | 0  | IA | ZH | ZL | YH | YL | XH | XL |
	// +----+----+----+----+----+----+----+----+
	// IA: interrupt active - 0 = no interrupt has been generated (default), 1 = one or more interrupts have been generated.
	// ZH: Z high - 0 = no interrupt (default), 1 = Z high event has occurred.
	// ZL: Z low - 0 = no interrupt (default), 1 = Z low event has occurred.
	// YH: Y high - 0 = no interrupt (default), 1 = Y high event has occurred.
	// YL: Y low - 0 = no interrupt (default), 1 = Y low event has occurred.
	// XH: X high - 0 = no interrupt (default), 1 = X High event has occurred.
	// XL: X low - 0 = no interrupt (default), 1 = X Low event has occurred.
	uint8_t nVal = readReg(INT1_SRC);

	return (nVal & 0b00000001) > 0;
}

// Did a high value on the Y axis trigger the interrupt
bool CL3GD20::isYHighInt()
{
	// INT1_SRC: interrupt source register.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | 0  | IA | ZH | ZL | YH | YL | XH | XL |
	// +----+----+----+----+----+----+----+----+
	// IA: interrupt active - 0 = no interrupt has been generated (default), 1 = one or more interrupts have been generated.
	// ZH: Z high - 0 = no interrupt (default), 1 = Z high event has occurred.
	// ZL: Z low - 0 = no interrupt (default), 1 = Z low event has occurred.
	// YH: Y high - 0 = no interrupt (default), 1 = Y high event has occurred.
	// YL: Y low - 0 = no interrupt (default), 1 = Y low event has occurred.
	// XH: X high - 0 = no interrupt (default), 1 = X High event has occurred.
	// XL: X low - 0 = no interrupt (default), 1 = X Low event has occurred.
	uint8_t nVal = readReg(INT1_SRC);

	return (nVal & 0b00001000) > 0;
}

// Did a low value on the Y axis trigger the interrupt
bool CL3GD20::isYLowInt()
{
	// INT1_SRC: interrupt source register.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | 0  | IA | ZH | ZL | YH | YL | XH | XL |
	// +----+----+----+----+----+----+----+----+
	// IA: interrupt active - 0 = no interrupt has been generated (default), 1 = one or more interrupts have been generated.
	// ZH: Z high - 0 = no interrupt (default), 1 = Z high event has occurred.
	// ZL: Z low - 0 = no interrupt (default), 1 = Z low event has occurred.
	// YH: Y high - 0 = no interrupt (default), 1 = Y high event has occurred.
	// YL: Y low - 0 = no interrupt (default), 1 = Y low event has occurred.
	// XH: X high - 0 = no interrupt (default), 1 = X High event has occurred.
	// XL: X low - 0 = no interrupt (default), 1 = X Low event has occurred.
	uint8_t nVal = readReg(INT1_SRC);

	return (nVal & 0b00000100) > 0;
}

// Did a high value on the Z axis trigger the interrupt
bool CL3GD20::isZHighInt()
{
	// INT1_SRC: interrupt source register.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | 0  | IA | ZH | ZL | YH | YL | XH | XL |
	// +----+----+----+----+----+----+----+----+
	// IA: interrupt active - 0 = no interrupt has been generated (default), 1 = one or more interrupts have been generated.
	// ZH: Z high - 0 = no interrupt (default), 1 = Z high event has occurred.
	// ZL: Z low - 0 = no interrupt (default), 1 = Z low event has occurred.
	// YH: Y high - 0 = no interrupt (default), 1 = Y high event has occurred.
	// YL: Y low - 0 = no interrupt (default), 1 = Y low event has occurred.
	// XH: X high - 0 = no interrupt (default), 1 = X High event has occurred.
	// XL: X low - 0 = no interrupt (default), 1 = X Low event has occurred.
	uint8_t nVal = readReg(INT1_SRC);

	return (nVal & 0b00100000) > 0;
}

// Did a low value on the Z axis trigger the interrupt
bool CL3GD20::isZLowInt()
{
	// INT1_SRC: interrupt source register.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | 0  | IA | ZH | ZL | YH | YL | XH | XL |
	// +----+----+----+----+----+----+----+----+
	// IA: interrupt active - 0 = no interrupt has been generated (default), 1 = one or more interrupts have been generated.
	// ZH: Z high - 0 = no interrupt (default), 1 = Z high event has occurred.
	// ZL: Z low - 0 = no interrupt (default), 1 = Z low event has occurred.
	// YH: Y high - 0 = no interrupt (default), 1 = Y high event has occurred.
	// YL: Y low - 0 = no interrupt (default), 1 = Y low event has occurred.
	// XH: X high - 0 = no interrupt (default), 1 = X High event has occurred.
	// XL: X low - 0 = no interrupt (default), 1 = X Low event has occurred.
	uint8_t nVal = readReg(INT1_SRC);

	return (nVal & 0b00010000) > 0;
}

// Was any interrupt triggered.
bool CL3GD20::isInt()
{
	// INT1_SRC: interrupt source register.
	// +----+----+----+----+----+----+----+----+
	// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
	// +----+----+----+----+----+----+----+----+
	// | 0  | IA | ZH | ZL | YH | YL | XH | XL |
	// +----+----+----+----+----+----+----+----+
	// IA: interrupt active - 0 = no interrupt has been generated (default), 1 = one or more interrupts have been generated.
	// ZH: Z high - 0 = no interrupt (default), 1 = Z high event has occurred.
	// ZL: Z low - 0 = no interrupt (default), 1 = Z low event has occurred.
	// YH: Y high - 0 = no interrupt (default), 1 = Y high event has occurred.
	// YL: Y low - 0 = no interrupt (default), 1 = Y low event has occurred.
	// XH: X high - 0 = no interrupt (default), 1 = X High event has occurred.
	// XL: X low - 0 = no interrupt (default), 1 = X Low event has occurred.
	uint8_t nVal = readReg(INT1_SRC);

	return (nVal & 0b01000000) > 0;
}

