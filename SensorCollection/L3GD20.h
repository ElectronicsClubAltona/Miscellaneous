#ifndef __L3GD20_H__
#define __L3GD20_H__

#include <Arduino.h>
#include "DeviceBase.h"




//###################################################################################################################################################
//# REGISTER ADDRESSES (https://www.youtube.com/watch?v=EOZX5MOUvhI&list=PLUMWjy5jgHK30fkGrufluENJqZmLZkmqI&index=2)
//###################################################################################################################################################

// WHO_AM_I: 8 bit device address.

//===================================================================================================================================================

// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
// +----+----+----+----+----+----+----+----+
// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
// +----+----+----+----+----+----+----+----+
// | DR | DR | BW | BW | PW | ZE | YE | XE |
// +----+----+----+----+----+----+----+----+
// DR (2 bits): data rate setting.
// BW (2 bits): bandwidth setting.
// +----+----+----------------+--------------+
// | DR | BW | DATA RATE (Hz) | CUT OFF (Hz) |
// +----+----+----------------+--------------+
// | 00 | 00 |       95       |     12.5     |   
// | 00 | 01 |       95       |     25       |
// | 00 | 10 |       95       |     25       |
// | 00 | 11 |       95       |     25       |
// | 01 | 00 |      190       |     12.5     |
// | 01 | 01 |      190       |     25       |
// | 01 | 10 |      190       |     50       |
// | 01 | 11 |      190       |     70       |
// | 10 | 00 |      380       |     20       |
// | 10 | 01 |      380       |     25       |
// | 10 | 10 |      380       |     50       |
// | 10 | 11 |      380       |     100      |
// | 11 | 00 |      760       |     30       |
// | 11 | 01 |      760       |     35       |
// | 11 | 10 |      760       |     50       |
// | 11 | 11 |      760       |     100      |
// +----+----+----------------+--------------+
//
// PW (1 bit): power mode setting.
// ZE (1 bit): Z axis enable.
// YE (1 bit): Y xis enable.
// XE (1 bit): X axis enable.
// +------------+----+----+----+----+
// | POWER MODE | PW | ZE | YE | XE |
// +------------+----+----+----+----+
// | DOWN       | 0  | -  | -  | -  | 
// +------------+----+----+----+----+
// | SLEEP      | 1  | 0  | 0  | 0  | 
// +------------+----+----+----+----+
// | NORMAL     | 1  | -  | -  | -  | 
// +------------+----+----+----+----+


//===================================================================================================================================================

// CTRL_REG2: high pass filter settings.
// +------+------+------+------+------+------+------+------+
// |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
// +------+------+------+------+------+------+------+------+
// |  0   |  0   | HPF  | HPF  | HPCF | HPCF | HPCF | HPCF |
// +------+------+------+------+------+------+------+------+ 
// HPF: high pass filter mode.
// HPCF: high pass filter cut off.
// +-----+----------------------------------------+
// | HPF | FILTER MODE                            |
// +-----+----------------------------------------+
// | 00  | NORMAL (reset reading HP_RESET_FILTER) |
// | 01  | REFERENCE SIGNAL FOR FILTERING         |
// | 10  | NORMAL                                 |
// | 11  | AUTO-RESET ON INTERRUPT                |
// +-----+----------------------------------------+
//
// +--------+--------------+--------------+--------------+--------------+
// | HPCF   | CUT OFF (Hz) | CUT OFF (Hz) | CUT OFF (Hz) | CUT OFF (Hz) |
// | HPCF   | (ODR 95Hz)   | (ODR 190Hz)  | (ODR 380Hz)  | (ODR 760Hz)  |
// +--------+------------- +--------------+--------------+--------------+
// | 0000   |    7.2       |    13.5      |    27        |    51.4      |
// | 0001   |    3.5       |     7.2      |    13.5      |    27        |
// | 0010   |    1.8       |     3.5      |     7.2      |    13.5      | 
// | 0011   |    0.9       |     1.8      |     3.5      |     7.2      |
// | 0100   |    0.45      |     0.9      |     1.8      |     3.5      |
// | 0101   |    0.18      |     0.45     |     0.9      |     1.8      |
// | 0110   |    0.09      |     0.18     |     0.45     |     0.9      |
// | 0111   |    0.045     |     0.09     |     0.18     |     0.45     |
// | 1000   |    0.018     |     0.045    |     0.09     |     0.18     |
// | 1001   |    0.009     |     0.018    |     0.045    |     0.09     |
// +--------+--------------+--------------+--------------+--------------+

//===================================================================================================================================================

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

//===================================================================================================================================================

// CTRL_REG4: block data, endian, scale and SPI settings.
// +-----+-----+-----+-----+-----+-----+-----+-----+
// |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
// +-----+-----+-----+-----+-----+-----+-----+-----+
// | BDU | BLE | FS  | FS  |  -  |  0  |  0  | SIM |
// +-----+-----+-----+-----+-----+-----+-----+-----+
// BDU: block data update - 0 = continuos update (default), 1 = output registers not updated until MSb and LSb reading.
// BLE: big/little endian data selection - 0 = data LSb @ lower address (default), 1 = data MSb @ lower address.
// FS: scale selection - 00 = 250 dps (default), 01 = 500 dps, 10 = 2000 dps, 11 = 2000 dps. A lower setting means you can detect smaller minimum 
//     increments of angular change but at slower angular speeds. A large setting means you can detect faster rates of angular change but at larger 
//     minimum increments.
// SIM: SPI serial interface mode selection - 0 = 4-wire interface (default), 1 = 3-wire interface.

//===================================================================================================================================================

// CTRL_REG5: boot, FIFO and hysteresis filtering settings.
// +-------+-------+-------+-------+-------+-------+-------+-------+
// |  7    |  6    |  5    |  4    |  3    |  2    |  1    |  0    |
// +-------+-------+-------+-------+-------+-------+-------+-------+
// | RMC   | FIFOE | HPFE  |   -   | INT1S | INT1S | OUTS  | OUTS  |
// +-------+-------+-------+-------+-------+-------+-------+-------+
//
// RMC: Reboot memory content - 0 = normal mode (default), 1 = reboot memory content.
// FIFO: enable or disable FIFO - 0 = disable (default), 1 = enable.
// HPFE: high pass filter enable - 0 = disable (default), 1 = enable.
// INT1S: apply hysteresis filering to interrupts - 0 = disable (default)
// OUTS: apply hysteresis filering to outputs - 0 = disable (default)
// +------+-------+-----------------------------------------------------------+
// | HPFE | INT1S | EFFECT ON INTERRUPT GENERATOR                             |
// +------+-------+-----------------------------------------------------------+
// |  0   |  00   | High pass filter not applied, low pass filter not applied |
// |  0   |  01   | High pass filter not applied, low pass filter not applied |
// |  0   |  10   | High pass filter not applied, low pass filter not applied |
// |  0   |  11   | High pass filter not applied, low pass filter not applied |
// |  1   |  00   | High pass filter applied, low pass filter not applied     |
// |  1   |  01   | High pass filter applied, low pass filter not applied     |
// |  1   |  10   | High pass filter applied, low pass filter applied         |
// |  1   |  11   | High pass filter applied, low pass filter applied         |
// +------+-------+-----------------------------------------------------------+
// +------+------+-----------------------------------------------------------+
// | HPFE | OUTS | EFFECT ON DATA REGISTERS & FIFO BUFFER                    |
// +------+------+-----------------------------------------------------------+
// |  0   |  00  | High pass filter not applied, low pass filter not applied |
// |  0   |  01  | High pass filter not applied, low pass filter not applied |
// |  0   |  10  | High pass filter not applied, low pass filter not applied |
// |  0   |  11  | High pass filter not applied, low pass filter not applied |
// |  1   |  00  | High pass filter applied, low pass filter not applied     |
// |  1   |  01  | High pass filter applied, low pass filter not applied     |
// |  1   |  10  | High pass filter applied, low pass filter applied         |
// |  1   |  11  | High pass filter applied, low pass filter applied         |
// +------+------+-----------------------------------------------------------+

//===================================================================================================================================================

// REFFERENCE: reference value for interrupt generation

//===================================================================================================================================================

// OUT_TEMP: temperature data (1LSB/deg, 8-bit resolution, two's complement). If you want to compensate for drift of samplings from gyro due to
//           temperature vraiations.

//===================================================================================================================================================

// STATUS_REG: status register for axes.
// +-------+-------+-------+-------+-------+-------+-------+-------+
// |  7    |  6    |  5    |  4    |  3    |  2    |  1    |  0    |
// +-------+-------+-------+-------+-------+-------+-------+-------+
// | ZXYOR |  ZOR  |  YOR  |  XOR  | ZYXDA |  ZDA  |  ZYA  |  XYA  |
// +-------+-------+-------+-------+-------+-------+-------+-------+
// ZXYOR: X, Y, Z axis data overrun flag - 0 = no overrun has occurred (default), 1 = new data has overwritten the previous data before it was read.
// ZOR: Z axis data overrun flag - 0 = no overrun has occurred (default), 1 = new data has overwritten the previous data before it was read.
// YOR: Y axis data overrun flag - 0 = no overrun has occurred (default), 1 = new data has overwritten the previous data before it was read.
// XOR: X axis data overrun flag - 0 = no overrun has occurred (default), 1 = new data has overwritten the previous data before it was read.
// ZYXDA: X, Y, Z axis new data available - 0 = a new set of data is not yet available (default), 1 = a new set of data is available.
// ZDA: Z axis new data available - 0 = a new set of data is not yet available (default), 1 = a new set of data is available.
// ZYA: Y axis new data available - 0 = a new set of data is not yet available (default), 1 = a new set of data is available.
// XYA: X axis new data available - 0 = a new set of data is not yet available (default), 1 = a new set of data is available.

//===================================================================================================================================================

// OUT_X_H & OUT_X_L: X-axis angular rate data - two’s complement.

//===================================================================================================================================================

// OUT_Y_H & OUT_Y_L: Y-axis angular rate data - two’s complement.

//===================================================================================================================================================

// OUT_Z_H & OUT_Z_L: Z-axis angular rate data - two’s complement.

//===================================================================================================================================================

// FIFO_CTRL_REG: FIFO control.
// +-----+-----+-----+-----+-----+-----+-----+-----+
// | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
// +-----+-----+-----+-----+-----+-----+-----+-----+
// | FM  | FM  | FM  | WTM | WTM | WTM | WTM | WTM |
// +-----+-----+-----+-----+-----+-----+-----+-----+
// FM: FIFO mode selection, default = 00.
// WTM: FIFO threshold & watermark level setting, i.e. the number of slots in the FIFO buffers that will be filled before a water mark interrupt is
//      generated if enabled in CTRL_REG3.
// +-----+-------------------------------------------------------------------------------------------------------------------------------+
// | FM  | MODE                                                                                                                          |
// +-----+-------------------------------------------------------------------------------------------------------------------------------+
// | 000 | BYPASS - FIFO buffer not used and samplings are put in the axes registers.                                                    |
// | 001 | FIFO - sampling data for each axes is put in the next slot of the appropriateand FIFO buffer. When it is full sampling stops. |
// | 010 | FIFIO STREAM - same as above except that when the FIFO buffer is full the oldest data is overwritten.                         |
// | 011 | STREAM TO FIFO - start off in FIFO STREAM mode. A WATERMARK interrupt can be enabled in CTRL_REG3 which will trigger if the   |
// |     |                  FIFO buffer fills up to the level specified in the WTM bits above. After this the FIFO buffer switches to    |
// |     |                  FIFO mode.                                                                                                   |
// | 100 | BYPASS TO STREAM - start off in BYPASS and switch to STREAM mode as a result of a configurable interupt on INT1 (INT1_CFG)    |
// +-----+-------------------------------------------------------------------------------------------------------------------------------+

//===================================================================================================================================================

// FIFO_SRC_REG: FIFO source register.
// +-------+--------+-------+-------+-------+-------+-------+-------+
// |   7   |   6    |   5   |   4   |   3   |   2   |   1   |   0   | 
// +-------+--------+-------+-------+-------+-------+-------+-------+
// |  WTM  | OVRN   | EMPTY |  FSS  |  FSS  |  FSS  |  FSS  |  FSS  |
// +-------+--------+-------+-------+-------+-------+-------+-------+
// WTM: watermark status - 0 = FIFO filling is lower than WTM level, 1 = FIFO filling is equal or higher than WTM level.
// OVRN: overrun bit status - 0 = FIFO is not completely filled, 1 = FIFO is completely filled.
// EMPTY: FIFO empty bit - 0 = FIFO not empty, 1 = FIFO empty.
// FSS: FIFO stored data level

//===================================================================================================================================================

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

//===================================================================================================================================================

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

//===================================================================================================================================================

// INT1_TSH_XH & INT1_TSH_XL: interrupt threshold for X axis (high byte)
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | 15 | 14 | 13 | 12 | 11 | 10 | 9  | 8  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | 0  | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// IT: interrupt threshold bit.

//===================================================================================================================================================

// INT1_TSH_YH & INT1_TSH_YL: interrupt threshold for Y axis (high byte)
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | 15 | 14 | 13 | 12 | 11 | 10 | 9  | 8  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | 0  | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// IT: interrupt threshold bit.

//===================================================================================================================================================


// INT1_TSH_ZH & INT1_TSH_ZL: interrupt threshold for Z axis (high byte)
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | 15 | 14 | 13 | 12 | 11 | 10 | 9  | 8  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | 0  | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT | IT |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// IT: interrupt threshold bit.

//===================================================================================================================================================

// INT1_DURATION: interrupt duration register.
// +------+------+------+------+------+------+------+------+
// |  7   |  6   |  5   |  4   |  3   |  2   |  1   |  0   |
// +------+------+------+------+------+------+------+------+
// | WAIT |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
// +------+------+------+------+------+------+------+------+
// WAIT: interrupt mode - 0 = interrupt falls immediately if the signal crosses the selected threshold, 
//						  1 =  if the signal crosses the selected threshold the interrupt triggers only after the duration has counted the number of samples 
//							   at the selected data rate, written into the duration counter register.

#define DEVICE_ADDR1				(uint8_t)0x6A
#define DEVICE_ADDR2				(uint8_t)0x6B

#define WHO_AM_I				(uint8_t)0x0F
#define CTRL_REG1					(uint8_t)0x20
#define CTRL_REG2					(uint8_t)0x21
#define CTRL_REG3					(uint8_t)0x22
#define CTRL_REG4					(uint8_t)0x23
#define CTRL_REG5					(uint8_t)0x24
#define REFFERENCE					(uint8_t)0x25
#define OUT_TEMP					(uint8_t)0x26
#define STATUS_REG					(uint8_t)0x27
#define OUT_X_L						(uint8_t)0x28
#define OUT_X_H						(uint8_t)0x29
#define OUT_Y_L						(uint8_t)0x2A
#define OUT_Y_H						(uint8_t)0x2B
#define OUT_Z_L						(uint8_t)0x2C
#define OUT_Z_H						(uint8_t)0x2D
#define FIFO_CTRL_REG				(uint8_t)0x2E
#define FIFO_SRC_REG				(uint8_t)0x2F
#define INT1_CFG					(uint8_t)0x30
#define INT1_SRC					(uint8_t)0x31
#define INT1_TSH_XH					(uint8_t)0x32
#define INT1_TSH_XL					(uint8_t)0x33
#define INT1_TSH_YH					(uint8_t)0x34
#define INT1_TSH_YL					(uint8_t)0x35
#define INT1_TSH_ZH					(uint8_t)0x36
#define INT1_TSH_ZL					(uint8_t)0x37
#define	INT1_DURATION				(uint8_t)0x38




//###################################################################################################################################################
//# CTRL_REG1 DEFINES
//###################################################################################################################################################

// Data rate settings
#define CTRL_REG1_RATE95 				(uint8_t)0x00
#define CTRL_REG1_RATE190				(uint8_t)0x40
#define CTRL_REG1_RATE380				(uint8_t)0x80
#define CTRL_REG1_RATE760				(uint8_t)0xC0

// Band width settings
#define CTRL_REG1_BANDWIDTH1			(uint8_t)0x00
#define CTRL_REG1_BANDWIDTH2			(uint8_t)0x10
#define CTRL_REG1_BANDWIDTH3			(uint8_t)0x20
#define CTRL_REG1_BANDWIDTH4			(uint8_t)0x30

// Power settings
#define CTRL_REG1_POWER_DOWN			(uint8_t)0x00
#define CTRL_REG1_POWER_SLEEP			(uint8_t)0x08
#define CTRL_REG1_POWER_X				(uint8_t)0x09
#define CTRL_REG1_POWER_Y				(uint8_t)0x0A
#define CTRL_REG1_POWER_XY				(uint8_t)0x0B
#define CTRL_REG1_POWER_Z				(uint8_t)0x0C
#define CTRL_REG1_POWER_XZ				(uint8_t)0x0D
#define CTRL_REG1_POWER_YZ				(uint8_t)0x0E
#define CTRL_REG1_POWER_XYZ				(uint8_t)0x0F

// Or one data rate, one band width setting with one power setting.
#define CTRL_REG1_DEFAULT				CTRL_REG1_POWER_XYZ | CTRL_REG1_RATE95 | CTRL_REG1_BANDWIDTH1

//###################################################################################################################################################
//# CTRL_REG2 DEFINES
//###################################################################################################################################################

// High pass filter mode
#define CTRL_REG2_HPFM_NORMAL1			(uint8_t)0x00
#define CTRL_REG2_HPFM_NORMAL_REFERENCE	(uint8_t)0x10
#define CTRL_REG2_HPFM_NORMAL2			(uint8_t)0x20
#define CTRL_REG2_HPFM_AUTO_RESET		(uint8_t)0x30	

// High pass filter cut off frequency dependant on data rate and band width setting in CTRL_REG1
#define CTRL_REG2_HPCO_SETTING1			(uint8_t)0x00	
#define CTRL_REG2_HPCO_SETTING2			(uint8_t)0x01	
#define CTRL_REG2_HPCO_SETTING3			(uint8_t)0x02	
#define CTRL_REG2_HPCO_SETTING4			(uint8_t)0x03	
#define CTRL_REG2_HPCO_SETTING5			(uint8_t)0x04	
#define CTRL_REG2_HPCO_SETTING6			(uint8_t)0x05	
#define CTRL_REG2_HPCO_SETTING7			(uint8_t)0x06	
#define CTRL_REG2_HPCO_SETTING8			(uint8_t)0x07	
#define CTRL_REG2_HPCO_SETTING9			(uint8_t)0x08	
#define CTRL_REG2_HPCO_SETTING10		(uint8_t)0x09	

// OR togther one setting from each group.
#define CTRL_REG2_DEFAULT				CTRL_REG2_HPFM_NORMAL1 | CTRL_REG2_HPCO_SETTING1

//###################################################################################################################################################
//# CTRL_REG3 DEFINES
//###################################################################################################################################################

// Interrupt settings - OR these together
#define CTRL_REG3_INT1_ENABLE			(uint8_t)0x80
#define CTRL_REG3_INT1_BOOT_ENABLE		(uint8_t)0x40
#define CTRL_REG3_INT1_CONFIG_ENABLE	(uint8_t)0x20
#define CTRL_REG3_INT1_OPENDRAIN_ENABLE	(uint8_t)0x10
#define CTRL_REG3_INT2_DATAREADY_ENABLE	(uint8_t)0x08
#define CTRL_REG3_INT2_WATERMARK_ENABLE	(uint8_t)0x04
#define CTRL_REG3_INT3_OVERRUN_ENABLE	(uint8_t)0x02
#define CTRL_REG3_INT3_EMPTY_ENABLE		(uint8_t)0x01

// OR togther one setting from each group.
#define CTRL_REG3_DEFAULT				(uint8_t)0x00

//###################################################################################################################################################
//# CTRL_REG4 DEFINES
//###################################################################################################################################################

// BDU: block data update setting
#define CTRL_REG4_BDU_ENABLE			(uint8_t)0x80
#define CTRL_REG4_BDU_DISABLE			(uint8_t)0x00

// BLE: big/little endian data selection
#define CTRL_REG4_BLE_ENABLE			(uint8_t)0x40
#define CTRL_REG4_BLE_DISABLE			(uint8_t)0x00

// FS: scale setting
#define CTRL_REG4_SCALE_250DPS			(uint8_t)0x00
#define SCALE_FACTOR_250DPS				(double)8.75 / 1000
#define CTRL_REG4_SCALE_500DPS			(uint8_t)0x10
#define SCALE_FACTOR_500DPS				(double)17.5 / 1000
#define CTRL_REG4_SCALE_2000DPS			(uint8_t)0x20
#define SCALE_FACTOR_2000DPS			(double)70 / 1000

// OR togther one setting from each group.
#define CTRL_REG4_DEFAULT				CTRL_REG4_BDU_DISABLE | CTRL_REG4_BLE_DISABLE | CTRL_REG4_SCALE_250DPS

//###################################################################################################################################################
//# CTRL_REG5 DEFINES
//###################################################################################################################################################

// Reboot memory content setting.
#define CTRL_REG5_REBOOT_DIABLE			(uint8_t)0x00
#define CTRL_REG5_REBOOT_ENABLE			(uint8_t)0x80

// FIFO setting.
#define CTRL_REG5_FIFO_DISABLE			(uint8_t)0x00
#define CTRL_REG5_FIFO_ENABLE			(uint8_t)0x40

// High pass filter setting.
#define CTRL_REG5_HPFILTER_DISABLE		(uint8_t)0x00
#define CTRL_REG5_HPFILTER_ENABLE		(uint8_t)0x20

// OR one setting from each group.
#define CTRL_REG5_DEFAULT				CTRL_REG5_REBOOT_DIABLE | CTRL_REG5_FIFO_DISABLE | CTRL_REG5_HPFILTER_DISABLE




//############################################################
//# Class CL3GD20: class for the L3GD20 modle (gyro)
//############################################################
class CL3GD20: public CAxesDeviceBase
{
	public:

		//####################################################
		//# CONSTRUCTION & INITIALISATION
		//####################################################
		CL3GD20(uint8_t nDevAddr);

		~CL3GD20();

		// Configure the device and carry out any other necessary setup tasks.
		virtual void begin();

		//####################################################
		//# USEFUL DEBUGGING FUNCTIONS
		//####################################################
		
		// Get the register name from the value e.g."WHO_AM_I".
		virtual const char *getRegName(const uint8_t nRegAddr);

		//####################################################
		//# CHANGE DEVICE SETTINGS
		//####################################################

		// Put the device into power down mode.
		virtual void setPowerDownMode()
		{
			// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
			// +----+----+----+----+----+----+----+----+
			// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
			// +----+----+----+----+----+----+----+----+
			// | DR | DR | BW | BW | PW | ZE | YE | XE |
			// +----+----+----+----+----+----+----+----+
			writeRegBit(false, CTRL_REG1, (uint8_t)0b11110111);
		};

		// Put the device into normal mode.
		virtual void setNormalMode()
		{
			// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
			// +----+----+----+----+----+----+----+----+
			// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
			// +----+----+----+----+----+----+----+----+
			// | DR | DR | BW | BW | PW | ZE | YE | XE |
			// +----+----+----+----+----+----+----+----+
			writeRegBit(false, CTRL_REG1, (uint8_t)0b00001000);
			enableAxes(true, true, true);
		};

		// Put the device into sleep mode.
		virtual void setSleepMode()
		{
			// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
			// +----+----+----+----+----+----+----+----+
			// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
			// +----+----+----+----+----+----+----+----+
			// | DR | DR | BW | BW | PW | ZE | YE | XE |
			// +----+----+----+----+----+----+----+----+
			writeRegBit(false, CTRL_REG1, (uint8_t)0b00001000);
			enableAxes(false, false, false);
		};

		// Change the data sampling rate.
		virtual void setDataRate(uint8_t nValue /* An appropriate define from above. */);

		// Change the device band width.
		virtual void setBandWidth(uint8_t nValue /* An appropriate define from above. */);

		// Enable and disable X axis.
		virtual void enableXAxis(const bool bEnable)
		{
			// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
			// +----+----+----+----+----+----+----+----+
			// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
			// +----+----+----+----+----+----+----+----+
			// | DR | DR | BW | BW | PW | ZE | YE | XE |
			// +----+----+----+----+----+----+----+----+
			writeRegBit(bEnable, CTRL_REG1, (uint8_t)0b00000001);
		};

		// Enable and disable Y axis.
		virtual void enableYAxis(const bool bEnable)
		{
			// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
			// +----+----+----+----+----+----+----+----+
			// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
			// +----+----+----+----+----+----+----+----+
			// | DR | DR | BW | BW | PW | ZE | YE | XE |
			// +----+----+----+----+----+----+----+----+
			writeRegBit(bEnable, CTRL_REG1, (uint8_t)0b00000010);
		};

		// Enable and disable Z axis.
		virtual void enableZAxis(const bool bEnable)
		{
			// CTRL_REG1: data rate, bandwidth, power and axes enable settings.
			// +----+----+----+----+----+----+----+----+
			// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
			// +----+----+----+----+----+----+----+----+
			// | DR | DR | BW | BW | PW | ZE | YE | XE |
			// +----+----+----+----+----+----+----+----+
			writeRegBit(bEnable, CTRL_REG1, (uint8_t)0b00000100);
		};

		// Change the devices scale setting.
		virtual void setScale(uint8_t nValue /* An appropriate define from above. */);

		//####################################################
		//# GET AXES DATA
		//####################################################

		// Get X axis data.
		virtual double getX()
		{
			return CAxesDeviceBase::getAxis(OUT_X_H, OUT_X_L);
		};

		// Get Y axis data.
		virtual double getY()
		{
			return CAxesDeviceBase::getAxis(OUT_Y_H, OUT_Y_L);
		};

		// Get Z axis data.
		virtual double getZ()
		{
			return CAxesDeviceBase::getAxis(OUT_Z_H, OUT_Z_L);
		};

		//####################################################
		//# INTERRUPTS
		//####################################################

		// Enable interrupts in INT1 pin and set the interrupt polarity
		void enableInt1(const bool bEnable, const bool bLowToHigh);

		// Set interrupt polarity: high to low or low to high
		void setIntPol(const bool bLowToHigh);

		// Interrupts, set with setIntX(...) etc trigger interrupts one at a time.
		void setORInts();

		// Interrupts, set with setIntX(...) etc can trigger interrupts simultaneously.
		void setANDInts();

		// Set or remove an interrupt to trigger on high or low rotational speed on the X axis
		// bHigh: 0 = interrupt triggered on low rotational speed, 1 = interrupt triggered on high rotational speed
		// bEnable: 0 = disable interrupt specified by bHigh, 1 = enable interrupt specified by bHigh
		// nThreshold: threshold value for interrupt triggering.
		// nDuration: 0 = interrupts are triggered as soon as rotational speed exceeds threshold, >0 = interrupts are
		//            generated only if rotational speed exceeds threshold for this period of time
		void setIntX(const bool bHighOrLowInt, const bool bEnable, const int16_t nThreshold, const uint8_t nDuration = 0, const bool bLatch = false);

		// Set or remove an interrupt to trigger on high or low rotational speed on the Y axis
		// bHigh: 0 = interrupt triggered on low rotational speed, 1 = interrupt triggered on high rotational speed
		// bEnable: 0 = disable interrupt specified by bHigh, 1 = enable interrupt specified by bHigh
		// nThreshold: threshold value for interrupt triggering.
		// nDuration: 0 = interrupts are triggered as soon as rotational speed exceeds threshold, >0 = interrupts are
		//            generated only if rotational speed exceeds threshold for this period of time
		void setIntY(const bool bHighOrLowInt, const bool bEnable, const int16_t nThreshold, const uint8_t nDuration = 0, const bool bLatch = false);

		// Set or remove an interrupt to trigger on high or low rotational speed on the Z axis
		// bHigh: 0 = interrupt triggered on low rotational speed, 1 = interrupt triggered on high rotational speed
		// bEnable: 0 = disable interrupt specified by bHigh, 1 = enable interrupt specified by bHigh
		// nThreshold: threshold value for interrupt triggering.
		// nDuration: 0 = interrupts are triggered as soon as rotational speed exceeds threshold, >0 = interrupts are
		//            generated only if rotational speed exceeds threshold for this period of time
		void setIntZ(const bool bHighOrLowInt, const bool bEnable, const int16_t nThreshold, const uint8_t nDuration = 0, const bool bLatch = false);

		// Clear all X, Y & Z interrupts
		void removeXYZInts();

		// Did a high value on the X axis trigger the interrupt
		bool isXHighInt();

		// Did a low value on the X axis trigger the interrupt
		bool isXLowInt();

		// Did a high value on the Y axis trigger the interrupt
		bool isYHighInt();

		// Did a low value on the Y axis trigger the interrupt
		bool isYLowInt();

		// Did a high value on the Z axis trigger the interrupt
		bool isZHighInt();

		// Did a low value on the Z axis trigger the interrupt
		bool isZLowInt();

		// Was any interrupt triggered.
		bool isInt();

	protected:

		//####################################################
		//# HELPER FUNCTIONS
		//####################################################

		//####################################################
		//# DATA MEMBERS
		//####################################################

};

#endif
