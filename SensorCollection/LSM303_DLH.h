/***************************************************************************
  This is a library for the LSM303 Accelerometer and magnentometer/compass

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#ifndef __LSM303_DLH_H__
#define __LSM303_DLH_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Sensor.h>
#include <Wire.h>


//================================================================================================ 
// CHIP ID
// -------

#define LSM303_ID                     0b11010100

//================================================================================================ 


//================================================================================================
// I2C ADDRESSES
// -------------

#define ADDR_ACCEL_SDO_HIGH         0b00110010 //0x32
#define ADDR_ACCEL_SDO_LOW          0b00110000 //0x30
#define ADDR_ACCEL_READ				0b00000001 //0x01
#define ADDR_ACCEL_WRITE			0b00000000 //0x00
#define ADDR_MAG_WRITE         		0b00111100 //0x3C  
#define ADDR_MAG_READ          		0b00111101 //0x3D

//================================================================================================


//================================================================================================
// ACCELERATOR REGISTERS
// ---------------------

typedef enum
{
	ACCEL_BLANK    		= 0x00,
	CTRL_REG1_A         = 0x20,
	CTRL_REG2_A         = 0x21,
	CTRL_REG3_A         = 0x22,
	CTRL_REG4_A         = 0x23, 
	CTRL_REG5_A         = 0x24,
	CTRL_REG6_A         = 0x25,
	REFERENCE_A         = 0x26, 
	STATUS_REG_A        = 0x27,
	OUT_X_L_A           = 0x28,
	OUT_X_H_A           = 0x29,
	OUT_Y_L_A           = 0x2A,
	OUT_Y_H_A           = 0x2B,
	OUT_Z_L_A           = 0x2C,
	OUT_Z_H_A           = 0x2D,
	FIFO_CTRL_REG_A     = 0x2E,
	FIFO_SRC_REG_A      = 0x2F,
	INT1_CFG_A          = 0x30,
	INT1_SOURCE_A       = 0x31,
	INT1_THS_A          = 0x32,
	INT1_DURATION_A     = 0x33,
	INT2_CFG_A          = 0x34,
	INT2_SOURCE_A       = 0x35,
	INT2_THS_A          = 0x36,
	INT2_DURATION_A     = 0x37,
	CLICK_CFG_A         = 0x38,
	CLICK_SRC_A         = 0x39,
	CLICK_THS_A         = 0x3A,
	TIME_LIMIT_A        = 0x3B,
	TIME_LATENCY_A      = 0x3C,
	TIME_WINDOW_A       = 0x3D
} e_accel_registers;
   
//================================================================================================

   
//================================================================================================
// MAGNETOMETER REGISTERS
// ----------------------
typedef enum
{
	MAG_BLANK         	= 0x00,
	CRA_REG_M           = 0x00,
	CRB_REG_M           = 0x01,
	MR_REG_M            = 0x02,
	OUT_X_H_M           = 0x03,
	OUT_X_L_M           = 0x04,
	OUT_Z_H_M           = 0x05,
	OUT_Z_L_M           = 0x06,
	OUT_Y_H_M           = 0x07,
	OUT_Y_L_M           = 0x08,
	SR_REG_M	        = 0x09,
	IRA_REG_M           = 0x0A,
	IRB_REG_M           = 0x0B,
	IRC_REG_M           = 0x0C,
	TEMP_OUT_H_M        = 0x31,
	TEMP_OUT_L_M        = 0x32
} e_mag_registers;

//================================================================================================


//================================================================================================
// CTRL_REG1 (20h)
//
// ┌──────┬──────┬──────┬──────┬───────┬──────┬──────┬──────┐
// │ ODR3 │ ODR2 │ ODR1 │ ODR0 │ LPen  │ Zen  │ Yen  │ Xen  │
// └──────┴──────┴──────┴──────┴───────┴──────┴──────┴──────┘
//
// ODR3 - ODR0: Data rate selection. Default value: 0000
//              ┌──────┬──────┬──────┬──────┬─────────────────────────────────────────────────┐
//              │ ODR3 │ ODR2 │ ODR1 │ ODR0 │ Power mode and ODR selection                    │
//              ├──────┼──────┼──────┼──────┼─────────────────────────────────────────────────┤
//              │  0   │  0   │  0   │  0   │ Power-down mode                                 │
//              │  0   │  0   │  0   │  1   │ Normal / low-power mode (1 Hz)                  │
//              │  0   │  0   │  1   │  0   │ Normal / low-power mode (10 Hz)                 │
//              │  0   │  0   │  1   │  1   │ Normal / low-power mode (25 Hz)                 │
//              │  0   │  1   │  0   │  0   │ Normal / low-power mode (50 Hz)                 │
//              │  0   │  1   │  0   │  1   │ Normal / low-power mode (100 Hz)                │
//              │  0   │  1   │  1   │  0   │ Normal / low-power mode (200 Hz)                │
//              │  0   │  1   │  1   │  1   │ Normal / low-power mode (2400 Hz)               │
//              │  1   │  0   │  0   │  0   │ Low-power mode (1.620 kHz)                      │
//              │  1   │  0   │  0   │  1   │ Normal (1.344 kHz) / low-power mode (5.376 kHz) │
//              └──────┴──────┴──────┴──────┴─────────────────────────────────────────────────┘
//        LPen: Low-power mode enable. Default value: 0
//              (0: normal mode, 1: low-power mode)
//         Zen: Z-axis enable. Default value: 1
//             (0: Z-axis disabled, 1: Z-axis enabled)
//         Yen: Y-axis enable. Default value: 1
//             (0: Y-axis disabled, 1: Y-axis enabled)
//         Xen: X-axis enable. Default value: 1
//              (0: X-axis disabled, 1: X-axis enabled)
//================================================================================================

typedef enum
{
	CTRL_REG1_A_POWER_DOWN			= 0b00000000,
	CTRL_REG1_A_NORMAL_1Hz			= 0b00010000,
	CTRL_REG1_A_NORMAL_10Hz			= 0b00100000,
	CTRL_REG1_A_NORMAL_25Hz			= 0b00110000,
	CTRL_REG1_A_NORMAL_50Hz			= 0b01000000,
	CTRL_REG1_A_NORMAL_100Hz		= 0b01010000,
	CTRL_REG1_A_NORMAL_200Hz		= 0b01100000,
	CTRL_REG1_A_LOW_POWER			= 0b10000000,
	CTRL_REG1_A_NORMAL_LOW_POWER	= 0b10010000
} e_ctrl_reg1_power_mode;

#define CTRL_REG1_A_LOW_POWER_ENABLE	0b00001000

#define CTRL_REG1_X_A_ENABLE			0b00000001
#define CTRL_REG1_Y_A_ENABLE			0b00000010
#define CTRL_REG1_Z_A_ENABLE			0b00000100

#define CTRL_REG1_A_DEFAULT				0b01010111

//================================================================================================
       
// CTRL_REG2 (21h)
//
// ┌────────┬────────┬────────┬────────┬────────┬─────────┬────────┬────────┐
// │  HPM1  │  HPM0  │ HPCF2  │ HPCF1  │  FDS   │ HPCLICK │ HPIS2  │ HPIS1  │
// └────────┴────────┴────────┴────────┴────────┴─────────┴────────┴────────┘
//
//   HPM1 = HPM0: High-pass filter mode selection. Default value: 00
//                ┌──────┬──────┬──────────────────────────────────────────────┐
//                │ HPM1 │ HPM0 │ High-pass filter mode                        │
//                ├──────┼──────┼──────────────────────────────────────────────┤
//                │  0   │  0   │  Normal mode (reset reading HP_RESET_FILTER) │
//                │  0   │  1   │  Reference signal for filtering              │
//                │  1   │  0   │  Normal mode                                 │
//                │  1   │  1   │  Autoreset on interrupt event                │
//                └──────┴──────┴──────────────────────────────────────────────┘
// HPCF2 - HPCF1: High-pass filter cutoff frequency selection
//           FDS: Filtered data selection. Default value: 0
//                (0: internal filter bypassed, 1: data from internal filter sent to output register and FIFO)
//        HPCLICK: High-pass filter enabled for click function
//                (0: filter bypassed, 1: filter enabled)
//          HPIS2: High-pass filter enabled for AOI function on Interrupt 2
//                (0: filter bypassed, 1: filter enabled)
//          HPIS1: High-pass filter enabled for AOI function on Interrupt 1
//                (0: filter bypassed, 1: filter enabled)
//
//================================================================================================

typedef enum
{
	CTRL_REG2_A_HPF_NORMAL_RESET				= 0b00000000,
	CTRL_REG2_A_HPF_NORMAL_REFERENCE			= 0b01000000,
	CTRL_REG2_A_HPF_NORMAL						= 0b10000000,
	CTRL_REG2_A_HPF_NORMAL_AUTO_RESET_INT		= 0b11000000
} e_ctrl_reg2_hpf_mode;

typedef enum
{
	CTRL_REG2_A_HPF_CUTOFF_FREQ1				= 0b00000000,
	CTRL_REG2_A_HPF_CUTOFF_FREQ2				= 0b00010000,
	CTRL_REG2_A_HPF_CUTOFF_FREQ3				= 0b00100000,
	CTRL_REG2_A_HPF_CUTOFF_FREQ4				= 0b00110000
} e_ctrl_reg2_hpf_cutoff;

#define CTRL_REG2_A_HPF_DATA_ENABLED				0b00001000
#define CTRL_REG2_A_HPF_CLICK_ENABLED				0b00000100
#define CTRL_REG2_A_HPF_AOI_INT2_ENABLED			0b00000010
#define CTRL_REG2_A_HPF_AOI_INT1_ENABLED			0b00000001

#define CTRL_REG2_A_DEFAULT							0b1001100

//================================================================================================

	
//================================================================================================    
// CTRL_REG3 (22h)
//
// ┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬───────────┐
// │  I1_CLICK  │  I1_AOI1   │  I1_AOI2   │  I1_DRDY1  │  I1_DRDY2  │   I1_WTM   │ I1_OVERRUN │ -         │
// └────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴───────────┘
//
//   I1_CLICK: CLICK interrupt on INT1. Default value 0
//             (0: disable, 1: enable)
//    I1_AOI1: AOI1 interrupt on INT1. Default value 0
//             (0: disable, 1: enable)
//    I1_AOI2: AOI2 interrupt on INT1. Default value 0
//             (0: disable, 1: enable)
//   I1_DRDY1: DRDY1 interrupt on INT1. Default value 0
//             (0: disable, 1: enable)
//   I1_DRDY2: DRDY2 interrupt on INT1. Default value 0
//             (0: disable, 1: enable)
//     I1_WTM: FIFO watermark interrupt on INT1. Default value 0
//             (0: disable, 1: enable)
// I1_OVERRUN: FIFO overrun interrupt on INT1. Default value 0
//             (0: disable, 1: enable)
//
//================================================================================================

#define CTRL_REG3_A_INT1_CLICK			0b10000000
#define CTRL_REG3_A_INT1_AOT1			0b01000000
#define CTRL_REG3_A_INT1_AOT2			0b00100000
#define CTRL_REG3_A_INT1_DRDY1			0b00010000
#define CTRL_REG3_A_INT1_DRDY2			0b00001000
#define CTRL_REG3_A_INT1_WTM			0b00000100
#define CTRL_REG3_A_INT1_OVERRUN		0b00000010

#define CTRL_REG3_A_DEFAULT				0b00000000

//================================================================================================


//================================================================================================    
// CTRL_REG4 (23h)
//
// ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
// │ BDU │ BLE │ FS1 │ FS0 │ HR  │  0  │  0  │ SIM │
// └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
//
//        BDU: Block data update. Default value: 0
//            (0: continuous update, 1: output registers not updated until MSB and LSB have been read
//        BLE: Big/little endian data selection. Default value 0.
//            (0: data LSB @ lower address, 1: data MSB @ lower address)
//  FS1 - FS0: Full-scale selection. Default value: 00
//             (00: ±2 g, 01: ±4 g, 10: ±8 g, 11: ±16 g)
//         HR: High-resolution output mode: Default value: 0
//             (0: high-resolution disable, 1: high-resolution enable)
//        SIM: SPI serial interface mode selection. Default value: 0
//             (0: 4-wire interface, 1: 3-wire interface).
//
//================================================================================================ 

#define CTRL_REG4_A_BDU_LATCHED			0b10000000
#define CTRL_REG4_A_BLE_ENABLED			0b01000000

typedef enum
{
	CTRL_REG4_A_FS_2G					= 0b00000000,
	CTRL_REG4_A_FS_4G 					= 0b00010000,
	CTRL_REG4_A_FS_8G					= 0b00100000,
	CTRL_REG4_A_FS_16G					= 0b00110000
} e_scale_type;

#define CTRL_REG4_A_HR_ENABLED			0b00001000
#define CTRL_REG4_A_SPI_3WIRE_ENBLED	0b00000001

#define CTRL_REG4_A_DEFAULT				0b00010000

//================================================================================================ 


//================================================================================================ 
// CTRL_REG5 (24h)   
//
// ┌────────────┬──────────┬──────────┬──────────┬──────────┬────────────┬──────────┬──────────┐
// │    BOOT    │ FIFO_EN  │    -     │    -     │ LIR_INT1 │  D4D_INT1  │ LIR_INT2 │ D4D_INT2 │
// └────────────┴──────────┴──────────┴──────────┴──────────┴────────────┴──────────┴──────────┘
//     BOOT: Reboot memory content. Default value: 0
//          (0: normal mode, 1: reboot memory content)
//  FIFO_EN: FIFO enable. Default value: 0
//          (0: FIFO disable, 1: FIFO enable)
// LIR_INT1: Latch interrupt request on INT1_SRC register, with INT1_SRC register cleared by reading INT1_SRC itself. Default value: 0.
//          (0: interrupt request not latched, 1: interrupt request latched)
// D4D_INT1: 4D enable: 4D detection is enabled on INT1 when 6D bit on INT1_CFG is set to 1.
// LIR_INT2: Latch interrupt request on INT2_SRC register, with INT2_SRC register cleared by reading INT2_SRC itself. Default value: 0.
//          (0: interrupt request not latched, 1: interrupt request latched)
// D4D_INT2: 4D enable: 4D detection is enabled on INT2 when 6D bit on INT2_CFG is set to 1.
//
//================================================================================================ 

#define CTRL_REG5_A_BOOT			0b10000000
#define CTRL_REG5_A_FIFO			0b01000000
#define CTRL_REG5_A_LIR_INT1		0b00001000
#define CTRL_REG5_A_4D_INT1			0b00000100
#define CTRL_REG5_A_LIR_INT2		0b00000010
#define CTRL_REG5_A_4D_INT2		    0b00000001

#define CTRL_REG5_A_DEFAULT			0b00000000

//================================================================================================ 


//================================================================================================ 
// CTRL_REG6 (24h)   
//
// ┌────────────┬────────────┬────────────┬───────────┬────────────┬────────────┬────────────┬────────────┐
// │ I2_CLICKen │  I2_INT1   │  I2_INT2   │  BOOT_I1  │   P2_ACT   │     -      │ H_LACTIVE  │     -      │
// └────────────┴────────────┴────────────┴───────────┴────────────┴────────────┴────────────┴────────────┘
//
// I2_CLICKen: CLICK interrupt on PAD2. Default value 0.
// 				(0: disable, 1: enable)
//    I2_INT1: Interrupt 1 on PAD2. Default value 0.
// 				(0: disable, 1: enable)
//    I2_INT2: Interrupt 2 on PAD2. Default value 0.
// 				(0: disable, 1: enable)
//    BOOT_I1: Reboot memory content on PAD2. Default value: 0
// 				(0: disable, 1: enable)
//     P2_ACT: Active function status on PAD2. Default value 0
// 				(0: disable, 1: enable)
//   H_LACTIVE: Interrupt active high, low. Default value 0.
//  			(0: active high, 1: active low)
//
//================================================================================================ 

#define CTRL_REG6_A_CLICK_INT_PAD2_ENABLE			0b10000000
#define CTRL_REG6_A_CLICK_INT1_PAD2_ENABLE			0b01000000
#define CTRL_REG6_A_CLICK_INT2_PAD2_ENABLE			0b00100000
#define CTRL_REG6_A_CLICK_REBOOT_PAD2_ENABLE		0b00010000
#define CTRL_REG6_A_CLICK_AFS_PAD2_ENABLE			0b00001000
#define CTRL_REG6_A_INT_ACTIVE_LOW_ENABLE			0b00000010

#define CTRL_REG6_A_DEFAULT							0b00000000

//================================================================================================ 


//================================================================================================ 
// STATUS_REG_A (27h)
//
// ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
// │ ZYXOR │  ZOR  │  YOR  │  XOR  │ ZYXDA │  ZDA  │  YDA  │  XDA  │
// └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
// ZYXOR: X-, Y-, and Z-axis data overrun. Default value: 0
//		  (0: no overrun has occurred, 1: a new set of data has overwritten the previous data)
//   ZOR: Z-axis data overrun. Default value: 0
//		  (0: no overrun has occurred, 1: new data for the Z-axis has overwritten the previous data)
//   YOR: Y-axis data overrun. Default value: 0
// 		  (0: no overrun has occurred, 1: new data for the Y-axis has overwritten the previous data)
//   XOR: X-axis data overrun. Default value: 0
//		  (0: no overrun has occurred, 1: new data for the X-axis has overwritten the previous data)
// ZYXDA: X-, Y-, and Z-axis new data available. Default value: 0
//			(0: a new set of data is not yet available, 1: a new set of data is available)
//   ZDA: Z-axis new data available. Default value: 0
//		  (0: new data for the Z-axis is not yet available, 1: new data for the Z-axis is available)
//   YDA: Y-axis new data available. Default value: 0
//		  (0: new data for the Y-axis is not yet available, 1: new data for the Y-axis is available)
//   XDA: X-axis new data available. Default value: 0
//        (0: new data for the X-axis is not yet available, 1: new data for the X-axis is available)

typedef enum
{
	STATUS_REG_BLANK				= 0b00000000,
	STATUS_REG_A_XDA				= 0b00000001,
	STATUS_REG_A_YDA				= 0b00000010,
	STATUS_REG_A_ZDA				= 0b00000100,
	STATUS_REG_A_ZYXDA				= 0b00001000,
	STATUS_REG_A_XOR				= 0b00010000,
    STATUS_REG_A_YOR				= 0b00100000,
	STATUS_REG_A_ZOR				= 0b01000000,
	STATUS_REG_A_ZYXOR				= 0b10000000
} e_status_reg_a;

//================================================================================================ 


//================================================================================================ 
// FIFO_CTRL_REG_A (2Eh)
//
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ FM1  │ FM0  │  TR  │ FTH4 │ FTH3 │ FTH2 │ FTH1 │ FTH0 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘

//   FM1 - FM0: FIFO mode selection. Default value: 00 (see Table 38)
//              ┌──────┬──────┬─────────────────────────┐
//              │ FM1  │ FM0  │ FIFO mode configuration │
//              ├──────┼──────┼─────────────────────────┤
//              │  0   │  0   │  Bypass mode            │
//              │  0   │  1   │  FIFO mode              │
//              │  1   │  0   │  Stream mode            │
//              │  1   │  1   │  Trigger mode           │
//              └──────┴──────┴─────────────────────────┘
//          TR: Trigger selection. Default value: 0
//              0: trigger event linked to trigger signal on INT1, 1: trigger event linked to trigger signal on INT2
// FTH4 - FTH0: Default value: 00000

typedef enum
{
	FIFO_CTRL_REG_A_BYPASS			= 0b00000000,
	FIFO_CTRL_REG_A_FIFO			= 0b01000000,
	FIFO_CTRL_REG_A_STREAM			= 0b10000000,
	FIFO_CTRL_REG_A_TRIGGER			= 0b11000000
} e_fifo_ctrl_reg_mode;

typedef enum
{
	FIFO_CTRL_REG_A_TRIGGER_INT1	=0b00000000,
	FIFO_CTRL_REG_A_TRIGGER_INT2	=0b00100000
} e_fifo_ctrl_reg_trigger;

#define FIFO_CTRL_REG_A_DEFAULT		0b00000000

//================================================================================================ 


//================================================================================================ 
// FIFO_SRC_REG_A (2Fh)
//
// ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
// │    WTM    │ OVRN_FIFO │   EMPTY   │   FSS4    │   FSS3    │   FSS2    │   FSS1    │   FSS0    │
// └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘

#define FIFO_SRC_REG_A_BLANK			0b00000000
#define FIFO_SRC_REG_A_EMPTY			0b00100000
#define FIFO_SRC_REG_A_OVRN				0b01000000
#define FIFO_SRC_REG_A_WTM				0b10000000
       
//================================================================================================ 


//================================================================================================ 
// INT1_CFG_A (30h) / INT2_CFG_A (34h)
//
// ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
// │  AOI   │   6D   │  ZHIE  │  ZLIE  │  YHIE  │  YLIE  │  XHIE  │  XLIE  │
// │        │	     │  ZUPE  │ ZDOWNE │  YUPE  │ YDOWNE │  XUPE  │ XDOWNE │
// └────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘
//
//         AOI: AND/OR combination of interrupt events. Default value: 0 (refer to Table 42)
//          6D: 6-direction detection function enabled. Default value: 0 (refer to Table 42)
//              ┌─────┬─────┬─────────────────────────────────────┐
//              │ AOI │ 6D  │ Interrupt mode                      │
//              ├─────┼─────┼─────────────────────────────────────┤
//              │  0  │	 0  │ OR combination of interrupt events  │
//              │  0  │  1  │ 6-direction movement recognition    │
//              │  1  │	 0  │ AND combination of interrupt events │
//              │  1  │  1  │ 6-direction position recognition    │
//              └─────┴─────┴─────────────────────────────────────┘
//   ZHIE/ZUPE: Enable interrupt generation on Z high event or on direction recognition. Default value: 0 
//              (0: disable interrupt request, 1: enable interrupt request)
// ZLIE/ZDOWNE: Enable interrupt generation on Z low event or on direction recognition. Default value: 0 
//              (0: disable interrupt request, 1: enable interrupt request)
//   YHIE/YUPE: Enable interrupt generation on Y high event or on direction recognition. Default value: 0 
//              (0: disable interrupt request, 1: enable interrupt request.)
// YLIE/YDOWNE: Enable interrupt generation on Y low event or on direction recognition. Default value: 0 
//              (0: disable interrupt request, 1: enable interrupt request.)
//   XHIE/XUPE: Enable interrupt generation on X high event or on direction recognition. Default value: 0 
//              (0: disable interrupt request, 1: enable interrupt request.)
// XLIE/XDOWNE: Enable interrupt generation on X low event or on direction recognition. Default value: 0 
//              (0: disable interrupt request, 1: enable interrupt request.)
//
// INT1_THS_A (32h) / INT2_THS_A (36h)
//
// ┌────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ 0  │ THS6 │ THS5 │ THS4 │ THS3 │ THS2 │ THS1 │ THS0 │
// └────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Threshold value for INT1 - if the axis reading is below this value then the interrupt won't be 
// generated.
//
// INT1_DURATION_A (33h) / INT2_DURATION_A (37h)
// ┌────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ 0  │  D6  │  D5  │  D4  │  D3  │  D2  │  D1  │  D0  │
// └────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Duration time for INT1 - if the axis reading is below the threshold value for less than this
// duration then the interrupt won't be generated.


typedef enum
{
	INT_CFG_A_INT_OR					= 0b00000000,
	INT_CFG_A_INT_6D_MOV				= 0b01000000,
	INT_CFG_A_INT_AND					= 0b10000000,
	INT_CFG_AINT_6D_MOV_DIR				= 0b11000000
} e_int_cfg;

#define INT_CFG_A_Z_HIGH				0b00100000
#define INT_CFG_A_Z_LOW					0b00010000
#define INT_CFG_A_Y_HIGH				0b00001000
#define INT_CFG_A_Y_LOW					0b00000100
#define INT_CFG_A_X_HIGH				0b00000010
#define INT_CFG_A_X_LOW					0b00000001

#define INT1_CFG_A_DEFAULT				0b00000000
#define INT2_CFG_A_DEFAULT			0b00000000

//================================================================================================ 


//================================================================================================ 
// INT1_SRC_A (31h) / INT2_SRC_A (35h)
//
// ┌────┬────┬────┬────┬────┬────┬────┬────┐
// │ 0  │ IA │ ZH │ ZL │ YH │ YL │ XH │ XL │
// └────┴────┴────┴────┴────┴────┴────┴────┘
//
// IA: Interrupt active. Default value: 0
//     (0: no interrupt has been generated, 1: one or more interrupts have been generated)
// ZH: Z high. Default value: 0
//     (0: no interrupt, 1: Z high event has occurred)
// ZL: Z low. Default value: 0
//     (0: no interrupt, 1: Z low event has occurred)
// YH: Y high. Default value: 0
//     (0: no interrupt, 1: Y high event has occurred)
// YL: Y low. Default value: 0
//     (0: no interrupt, 1: Y low event has occurred)
// XH: X high. Default value: 0
//     (0: no interrupt, 1: X high event has occurred)
// XL: X low. Default value: 0
//    (0: no interrupt, 1: X low event has occurred)

#define INT_SRC_A_X_BLANK 					0b00000000
#define INT_SRC_A_X_LOW 					0b00000001
#define INT_SRC_A_X_HIGH					0b00000010
#define INT_SRC_A_Y_LOW						0b00000100
#define INT_SRC_A_Y_HIGH					0b00001000
#define INT_SRC_A_Z_LOW						0b00010000
#define INT_SRC_A_Z_HIGH					0b00100000
#define INT_SRC_A_INT_ACTIVE				0b01000000

//================================================================================================ 


//================================================================================================ 
// CLICK_CFG_A (38h)
//
// ┌────┬────┬────┬────┬────┬────┬────┬────┐
// │ 0  │ 0  │ ZD │ ZS │ YD │ YS │ XD │ XS │
// └────┴────┴────┴────┴────┴────┴────┴────┘
//
// ZD: Enable interrupt double-click on Z-axis. Default value: 0
//     (0: disable interrupt request, 1: enable interrupt request on measured acceleration value higher than preset threshold)
// ZS: Enable interrupt single-click on Z-axis. Default value: 0
//     (0: disable interrupt request, 1: enable interrupt request on measured acceleration value higher than preset threshold)
// YD: Enable interrupt double-click on Y-axis. Default value: 0
//     (0: disable interrupt request, 1: enable interrupt request on measured acceleration value higher than preset threshold)
// YS: Enable interrupt single-click on Y-axis. Default value: 0
//     (0: disable interrupt request, 1: enable interrupt request on measured acceleration value higher than preset threshold)
// XD: Enable interrupt double-click on X-axis. Default value: 0
//    (0: disable interrupt request, 1: enable interrupt request on measured acceleration value higher than preset threshold)
// XS: Enable interrupt single-click on X-axis. Default value: 0
//    (0: disable interrupt request, 1: enable interrupt request on measured acceleration value higher than preset threshold)
//
// CLICK_THS_A (3Ah)
//
// ┌────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ 0  │ THS6 │ THS5 │ THS4 │ THS3 │ THS2 │ THS1 │ THS0 │
// └────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Threshold value for click interrupt - if the axis reading is below this value then the interrupt won't be 
// generated. 1 LSB = full-scale / 128 where ODR is the data rateetting in CTRL_REG1 (1Hz, 1.344Hz, 1.62Hz, 5.376Hz, 10Hz, 25Hz, 50Hz, 100Hz, 200Hz or 400Hz)
//
//
// TIME_LIMIT_A (3Bh)
//
// ┌────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ 0  │ TLI6 │ TLI5 │ TLI4 │ TLI3 │ TLI2 │ TLI1 │ TLI0 │
// └────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Specifies the minimum duration that the acceleration value on the selected axes must exceed the threshold value above for
// click event to be signaled. 1 LSB = 1/ODR where ODR is the data rateetting in CTRL_REG1 (1Hz, 1.344Hz, 1.62Hz, 5.376Hz, 10Hz, 25Hz, 50Hz, 100Hz, 200Hz or 400Hz)
//
//
// TIME_LATENCY_A (3Ch)
//
// ┌────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ 0  │ TLA6 │ TLA5 │ TLA4 │ TLA3 │ TLA2 │ TLA1 │ TLA0 │
// └────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Specifies the minimum time interval that must elapse after a single click event, before a double click event can be signalled.
// 1 LSB = 1/ODR where ODR is the data rateetting in CTRL_REG1 (1Hz, 1.344Hz, 1.62Hz, 5.376Hz, 10Hz, 25Hz, 50Hz, 100Hz, 200Hz or 400Hz)
//
//
// TIME_WINDOW_A (3Dh)
//
// ┌────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ 0  │ TW6  │ TW5  │ TW4  │ TW3  │ TW2  │ TLW  │ TW0  │
// └────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Specifies the time window after the latency period, within which a single click event must occur for a double click event to be signaled.
// 1 LSB = 1/ODR where ODR is the data rateetting in CTRL_REG1 (1Hz, 1.344Hz, 1.62Hz, 5.376Hz, 10Hz, 25Hz, 50Hz, 100Hz, 200Hz or 400Hz)
//

#define CLICK_CFG_A_X_SC					0b00000001
#define CLICK_CFG_A_X_DC 					0b00000010
#define CLICK_CFG_A_Y_SC					0b00000100
#define CLICK_CFG_A_Y_DC					0b00001000
#define CLICK_CFG_A_Z_SC					0b00010000
#define CLICK_CFG_A_Z_DC					0b00100000

#define CLICK_CFG_A_DEFAULT					0b00000000
#define CLICK_THS_A_DEFAULT                 0b00000000
#define TIME_LIMIT_A_DEFAULT                0b00000000
#define TIME_LATENCY_A_DEFAULT              0b00000000
#define TIME_WINDOW_A_DEFAULT               0b00000000

//================================================================================================ 


//================================================================================================ 
// CLICK_SRC_A (39h)
//
// ┌────────┬─────────┬────────┬────────┬────────┬────────┬────────┬────────┐
// │   -    │   1A    │ DCLICK │ SCLICK │  SIGN  │   Z    │   Y    │   X    │
// └────────┴─────────┴────────┴────────┴────────┴────────┴────────┴────────┘
//
//     IA: Interrupt active. Default value: 0
//         (0: no interrupt has been generated, 1: one or more interrupts have been generated)
// DCLICK: Double-click enable. Default value: 0 (0: double-click detection disable, 1: double-click detection enable)
// SCLICK: Single-click enable. Default value: 0 (0: Single-click detection disable, 1: single-click detection enable)
//   Sign: Click sign. 0: positive detection, 1: negative detection
//      Z: Z-click detection. Default value: 0
//         (0: no interrupt, 1: Z high event has occurred)
//      Y: Y-click detection. Default value: 0
//         (0: no interrupt, 1: Y high event has occurred)
//      X: X-click detection. Default value: 0
//        (0: no interrupt, 1: X high event has occurred)

#define CLICK_SRC_A_BLANK					0b00000000
#define CLICK_SRC_A_X						0b00000001
#define CLICK_SRC_A_Y						0b00000010
#define CLICK_SRC_A_Z						0b00000100
#define CLICK_SRC_A_SIGN					0b00001000
#define CLICK_SRC_A_SC						0b00010000
#define CLICK_SRC_A_DC						0b00100000
#define CLICK_SRC_A_IA						0b01000000

//================================================================================================ 


//================================================================================================ 
// CRA_REG_M (00h)
//
// ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
// │ TEMP_EN │    0    │    0    │   DO2   │   DO1   │   DO0   │    0    │    0    │
// └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
//
// TEMP_EN: Temperature sensor enable. (0: temperature sensor disabled (default), 1: temperature sensor enabled)
// DO2 - DO0: Data output rate bits. These bits set the rate at which data is written to all three data output registers (refer to Table 72). Default value: 100
//              ┌─────┬─────┬─────┬──────────────────────────────┐
//              │ DO2 │ DO1 │ DO0 │ Minimum data output rate (Hz)│
//              ├─────┼─────┼─────┼──────────────────────────────┤
//              │  0  │	 0  │  0  │ 0.75                         │
//              │  0  │  0  │  1  │ 1.5                          │
//              │  0  │	 1  │  0  │ 3.0                          │
//              │  0  │  1  │  1  │ 7.5                          │
//              │  1  │  0  │  0  │ 15                           │
//              │  1  │  0  │  1  │ 30                           │
//              │  1  │  1  │  0  │ 75                           │
//              │  1  │  1  │  1  │ 220                          │
//              └─────┴─────┴─────┴──────────────────────────────┘

typedef enum
{
  CRA_REG_M_RATE_0_75                      = 0b00000000,
  CRA_REG_M_RATE_1_5                       = 0b00000100,
  CRA_REG_M_RATE_3_0                       = 0b00001000,
  CRA_REG_M_RATE_7_5                       = 0b00001100,
  CRA_REG_M_RATE_15                        = 0b00010000,
  CRA_REG_M_RATE_30                        = 0b00010100,
  CRA_REG_M_RATE_75                        = 0b00011000,
  CRA_REG_M_RATE_220                       = 0b00011100
} e_cra_reg_m_rate;	

#define CRA_REG_M_RATE_DEFAULT				0b00010100

//================================================================================================ 


//================================================================================================ 
// CRB_REG_M (01h)
//
// ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
// │ GN2 │ GN1 │ GN0 │  0  │  0  │  0  │  0  │  0  │
// └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
//
// Gain configuration bits. The gain configuration is common for all channels
// ┌─────┬─────┬─────┬────────────────────┬─────────────┬─────────────┬──────────────────┐
// │ GN2 │ GN1 │ GN0 │ Sensor input field │ Gain X,Y,Z  │ Gain Z      │ Output range     │
// │     │     │     │ range (gauss)      │ (LSB/gauss) │ (LSB/gauss) │                  │
// ├─────┼─────┼─────┼────────────────────┼─────────────┼─────────────┼──────────────────┤
// │  0  │	0  │  1  │ ± 1.3              │	1100        │ 980         │                  │
// │  0  │	1  │  0  │ ± 1.9              │	855         │ 760         │                  │
// │  0  │	1  │  1  │ ± 2.5              │	670         │ 600         │ 0xF800–0x07FF    │
// │  1  │	0  │  0  │ ± 4.0              │	450         │ 400         │ (-2048 to +2047) │
// │  1  │	0  │  1  │ ± 4.7              │	400         │ 355         │                  │
// │  1  │	1  │  0  │ ± 5.6              │	330         │ 295         │                  │
// │  1  │	1  │  1  │ ± 8.1              │	230         │ 205         │                  │
// └─────┴─────┴─────┴────────────────────┴─────────────┴─────────────┴──────────────────┘

typedef enum
{
  CRA_REG_M_GAIN_1_3                      = 0b00100000,
  CRA_REG_M_GAIN_1_9                      = 0b01000000,
  CRA_REG_M_GAIN_2_5                      = 0b01100000,
  CRA_REG_M_GAIN_4_0                      = 0b10000000,
  CRA_REG_M_GAIN_4_7                      = 0b10100000,
  CRA_REG_M_GAIN_5_6                      = 0b11000000,
  CRA_REG_M_GAIN_8_1                      = 0b11100000
} e_cra_reg_m_gain;	

#define CRB_REG_M_GAIN_DEFAULT				0b10000000

//================================================================================================ 


//================================================================================================ 
// MR_REG_M (02h)
//
// ┌────┬────┬────┬────┬────┬────┬─────┬─────┐
// │ 0  │ 0  │ 0  │ 0  │ 0  │ 0  │ MD1 │ MD0 │
// └────┴────┴────┴────┴────┴────┴─────┴─────┘
//
// Magnetic sensor operating mode
// ┌─────┬─────┬──────────────────────────────┐
// │ MD1 │ MD0 │ Mode                         │
// ├─────┼─────┼──────────────────────────────┤
// │  0  │	0  │ Continuous-conversion mode   │
// │  0  │  1  │ Single-conversion mode       │
// │  1  │	0  │ Sleep mode                   │
// │  1  │  1  │ Sleep mode                   │
// └─────┴─────┴──────────────────────────────┘
//

typedef enum
{
	MR_REG_M_CONTINUOUS                    = 0b00000000,
	MR_REG_M_SINGLE                        = 0b00000001,
	MR_REG_M_SLEEP1                        = 0b00000010,
	MR_REG_M_SLEEP2                        = 0b00000011
} e_mag_reg_m_mode;

#define MR_REG_M_DEFAULT					0b00000000

//================================================================================================ 


//================================================================================================ 
// Class CLSM303_DLH_Accelerator
//================================================================================================ 

class CLSM303_DLH_Accelerator : public CSensor
{
	public:
		// Construction, destruction and initialisation
		CLSM303_DLH_Accelerator(int32_t nSensorID = -1);
		virtual ~CLSM303_DLH_Accelerator();
		virtual void begin(uint8_t nSDOPinNum, uint8_t nSDOPinState);

		// Device readings
		CVector& getAxes();
		
		// Device settings
		void setCtrlReg1(e_ctrl_reg1_power_mode ePowerMode, bool bLowPwerEnabled = false, bool bXAxis = true, bool bYAxis = true, bool bZAxis = true);
		void setCtrlReg(e_ctrl_reg2_hpf_mode eHPFilterMode, e_ctrl_reg2_hpf_cutoff eHPFCutOff, bool bHPFOnData, bool bHPFOnClick, bool bHPFAOIInt1, bool bHPFAOIInt2);
		void setCtrlReg3(bool bClick, bool bAOT1, bool bAOT2, bool bDRDY1, bool bDRDY2, bool bWTM, bool bOverrun);
		void setCtrlReg4(bool bBDU, bool bBLE, e_scale_type eScale, bool bHighRes, bool bSPI3Wire);
		void setCtrlReg5(bool bBoot, bool bFIFO, bool bLirInt1, bool b4DInt1, bool bLitInt2, bool b4DInt2);
		void setCtrlReg6(bool bIntPad2, bool bInt1Pad2, bool bInt2Pad2, bool bRebootPad2, bool bAFSPad2, bool bIntActiveLow);
		void setIntReference(uint8_t nRefVal)
		{
			writeReg(REFERENCE_A, nRefVal);
		};
		void setFIFOCtrlReg(e_fifo_ctrl_reg_mode eFIFOMode, e_fifo_ctrl_reg_trigger eTriggerMode);
		void setIntCfg(bool bForInt1, e_int_cfg eIntMode, bool bXHigh, bool bYHigh, bool bZHigh, uint8_t nThreshold = 0, uint8_t nDuration = 0);
		void setClicikClickInt(bool bXDoubleClick, bool bYDoubleClick, bool bZDoubleClick, uint8_t nThreshold = 0x14 /*2g*/, uint8_t nTimeLimit = 0x0A /*10ms*/, uint8_t nLatency = 0x64 /*100ms*/, uint8_t nWindow = 0x64 /*100ms*/);
		void setIntEnable(bool bDataReady, bool bSingleTap, bool bDoubleTap, bool bFreeFall, 
							bool bActivity = false, bool bInactivity = false, bool bWaterMark = false, bool bOverrun = false);
		void setIntMap(bool bDataReady, bool bSingleTap, bool bDoubleTap, bool bFreeFall, 
						bool bActivity = false, bool bInactivity = false, bool bWaterMark = false, bool bOverrun = false);
		
		// Device status
		bool getDevStatus(uint8_t nFlags)
		{
			return getStatus(REFERENCE_A, nFlags);
		};
		bool getFIFOSrc(uint8_t nFlags)
		{
			return getStatus(FIFO_SRC_REG_A, nFlags);
		};
		bool getClicSrc(uint8_t nFlags)
		{
			return getStatus(CLICK_SRC_A, nFlags);
		};
		bool getIntSrc(bool bForInt1, uint8_t nFlags = 0);

	private:
	
		static float m_arraySensitivity[4];
		e_scale_type m_eScale;

		// Helper functions
		void writeReg(e_accel_registers eRegAddr, uint8_t nValue)
		{
			CSensor::writeReg((uint8_t)eRegAddr, nValue);
		};
		uint8_t readReg8(e_accel_registers eRegAddr)
		{
			return CSensor::readReg8((uint8_t)eRegAddr);
		};
		void writeReg(e_accel_registers eHighRegAddr, e_accel_registers eLowRegAddr, uint16_t nValue)
		{
			CSensor::writeReg((uint8_t)eHighRegAddr, (uint8_t)eLowRegAddr, nValue);
		};
		uint8_t readReg16(e_accel_registers eHighRegAddr, e_accel_registers eLowRegAddr)
		{
			return CSensor::readReg16((uint8_t)eHighRegAddr, (uint8_t)eLowRegAddr);
		};
		void readRaw(CVector vectData);
};


//================================================================================================ 
// Class CLSM303_DLH_Accelerator
//================================================================================================ 

class CLSM303_DLH_Magnetometer : public CSensor
{
	public:
		// Construction, destruction and initialisation
		CLSM303_DLH_Magnetometer(int32_t nSensorID = -1);
		virtual ~CLSM303_DLH_Magnetometer();
		virtual void begin(uint8_t nSDOPinNum, uint8_t nSDOPinState);
	  
		// Device settings
		void setMagMode(e_mag_reg_m_mode eMagMode)
		{
			CSensor::writeReg(MR_REG_M, (uint8_t)eMagMode);
		};
		void setMagGain(e_cra_reg_m_gain eMagGain);
		void setMagRate(e_cra_reg_m_rate eMagRate)
		{
			CSensor::writeReg(CRA_REG_M, (uint8_t)eMagRate);
		};

		// Interface
		float getTemperature()
		{
			return CSensor::calculateTemperature(TEMP_OUT_H_M, TEMP_OUT_L_M, 8);
		};
		CVector& getAxes();
		float getHeading(CVector vectAccel, CVector vectFrom = CVector(1.0, 0.0, 0.0));

		// Device status
		
  private:
		int32_t         		m_nSensorID;
		uint8_t					m_nDevWriteAddr, m_nDevReadAddr;
		e_cra_reg_m_gain 		m_eGain;
		static float 			m_arrayGainXY[7], m_arrayGainZ[7];
    
		// Helper functions
		void writeReg(e_mag_registers eRegAddr, uint8_t nValue)
		{
			CSensor::writeReg((uint8_t)eRegAddr, nValue);
		};
		uint8_t readReg8(e_mag_registers eRegAddr)
		{
			return CSensor::readReg8((uint8_t)eRegAddr);
		};
		void writeReg(e_mag_registers eHighRegAddr, e_mag_registers eLowRegAddr, uint16_t nValue)
		{
			CSensor::writeReg((uint8_t)eHighRegAddr, (uint8_t)eLowRegAddr, nValue);
		};
		uint8_t readReg16(e_mag_registers eHighRegAddr, e_mag_registers eLowRegAddr)
		{
			return CSensor::readReg16((uint8_t)eHighRegAddr, (uint8_t)eLowRegAddr);
		};
		void readRaw(CVector& vectData);
};

#endif
