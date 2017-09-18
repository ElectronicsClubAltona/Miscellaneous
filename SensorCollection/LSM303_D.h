#ifndef __LSM303_D_H__
#define __LSM303_D_H__

#include <Arduino.h> // for uint8_t data type
#include <Sensor.h>


#define LSM303D_ADDR_SDO_LOW	0b00011110 //0x1E
#define LSM303D_ADDR_SDO_HIGH	0b00011101 //0x1D


//================================================================================================
// REGISTER ADDRESSES
// -------------------

typedef enum 
{
	BLANK			  = 0x00,
	TEMP_OUT_L        = 0x05,
	TEMP_OUT_H        = 0x06,
	STATUS_M          = 0x07,
	OUT_X_L_M 		  = 0x08,
	OUT_X_H_M         = 0x09,
	OUT_Y_L_M         = 0x0A,
	OUT_Y_H_M         = 0x0B,
	OUT_Z_L_M         = 0x0C,
	OUT_Z_H_M         = 0x0D,
	WHO_AM_I   		  = 0x0F,
	INT_CTRL_M        = 0x12,
	INT_SRC_M         = 0x13,
	INT_THS_L_M       = 0x14,
	INT_THS_H_M       = 0x15,
	OFFSET_X_L_M      = 0x16,
	OFFSET_X_H_M      = 0x17,
	OFFSET_Y_L_M      = 0x18,
	OFFSET_Y_H_M      = 0x19,
	OFFSET_Z_L_M      = 0x1A,
	OFFSET_Z_H_M      = 0x1B,
	REFERENCE_X       = 0x1C,
	REFERENCE_Y       = 0x1D,
	REFERENCE_Z       = 0x1E,
	CTRL0             = 0x1F,
	CTRL1             = 0x20,
	CTRL2             = 0x21,
	CTRL3             = 0x22,
	CTRL4             = 0x23,
	CTRL5             = 0x24,
	CTRL6             = 0x25,
	CTRL7             = 0x26,
	STATUS_A          = 0x27,
	OUT_X_L_A         = 0x28,
	OUT_X_H_A         = 0x29,
	OUT_Y_L_A         = 0x2A,
	OUT_Y_H_A         = 0x2B,
	OUT_Z_L_A         = 0x2C,
	OUT_Z_H_A         = 0x2D,
	FIFO_CTRL         = 0x2E,
	FIFO_SRC          = 0x2F,
	IG_CFG1           = 0x30,
	IG_SRC1           = 0x31,
	IG_THS1           = 0x32,
	IG_DUR1           = 0x33,
	IG_CFG2           = 0x34,
	IG_SRC2           = 0x35,
	IG_THS2           = 0x36,
	IG_DUR2           = 0x37,
	CLICK_CFG         = 0x38,
	CLICK_SRC         = 0x39,
	CLICK_THS         = 0x3A,
	TIME_LIMIT        = 0x3B,
	TIME_LATENCY      = 0x3C,
	TIME_WINDOW       = 0x3D,
	ACT_THS           = 0x3E,
	ACT_DUR           = 0x3F
} reg_addr;

//================================================================================================

	
//================================================================================================
// STATUS_M (07h)
//
// ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
// │ ZYXMOR │  ZMOR  │  YMOR  │  XMOR  │ ZYXMDA │  ZMDA  │  YMDA  │  XMDA  │
// │ Tempor │        │        │        │ Tempda │        │        │        │
// └────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘
//
// 
// ZYXMOR/Tempor: Magnetic X, Y and Z-axis and temperature data overrun. Default value: 0
//                (0: no overrun has occurred; 1: a new set of data has overwritten the previous data)
//                Temperature data overrun if T_ONLY bit in CTRL7 (26h) is set to ‘1’. Default value: 0.
//          ZMOR: Z-axis data overrun. Default value: 0
//                (0: no overrun has occurred; 1: new data for the Z-axis has overwritten the previous data)
//          YMOR: Y-axis data overrun. Default value: 0
//                (0: no overrun has occurred; 1: new data for the Y-axis has overwritten the previous data)
//          XMOR: X-axis data overrun. Default value: 0
//                (0: no overrun has occurred; 1: new data for the X-axis has overwritten the previous data)
// ZYXMDA/Tempda: X, Y and Z-axis and temperature new data available. Default value: 0
//                (0: a new set of data is not yet available; 1: a new set of data is available)
//                Temperature new data available if the T_ONLY bit in CTRL7 (26h) is set to ‘1’.
//          ZMDA: Z-axis new data available. Default value: 0
//                (0: new data for the Z-axis is not yet available; 1: new data for the Z-axis is available)
//          YMDA: Y-axis new data available. Default value: 0
//                (0: new data for the Y-axis is not yet available; 1: new data for the Y-axis is available)
//          XMDA: X-axis new data available. Default value: 0
//                (0: new data for the X-axis is not yet available; 1: new data for the X-axis is available)

#define STATUS_M_OVERRUN		0b10000000
#define STATUS_M_Z_OVERRUN		0b01000000
#define STATUS_M_Y_OVERRUN		0b00100000
#define STATUS_M_Z_OVERRUN      0b00010000
#define STATUS_M_AVAILABLE		0b00001000
#define STATUS_M_Z_AVAILABLE	0b00000100
#define STATUS_M_Y_AVAILABLE    0b00000010
#define STATUS_M_X_AVAILABLE    0b00000001

//================================================================================================


//===================================================================================================================================================
// WHO_A: M_I: Device identification register.
//
// ┌───┬───┬───┬───┬───┬───┬───┬───┐
// │ 0 │ 1 │ 0 │ 0 │ 1 │ 0 │ 0 │ 1 │
// └───┴───┴───┴───┴───┴───┴───┴───┘
//
//===================================================================================================================================================


//===================================================================================================================================================
// INT_CTRL_M: Interrupt control register for magnometer.

// ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
// │ XMIEN │ YMIEN │ ZMIEN │ PP_OD │ IEA   │ MIEL  │ 4D    │ MIEN  │
// └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
// XMIEN: Enable interrupt recognition on X─axis for magnetic data. Default value: 0.
//        (0: disable interrupt recognition; 1: enable interrupt recognition)
// YMIEN: Enable interrupt recognition on Y─axis for magnetic data. Default value: 0.
//        (0: disable interrupt recognition; 1: enable interrupt recognition)
// ZMIEN: Enable interrupt recognition on Z─axis for magnetic data. Default value: 0.
//        (0: disable interrupt recognition; 1: enable interrupt recognition)
// PP_OD: Interrupt pin configuration. Default value: 0.
//        (0: push─pull; 1: open drain)
//   IEA: Interrupt polarity. Default value: 0.
//        (0: interrupt active─low; 1: interrupt active─high)
//  MIEL: Latch interrupt request on INT_SRC_M (13h) register. Default value: 0.
//        (0: interrupt request not latched; 1: interrupt request latched)
//        Once the MIEL is set to ‘1’, the interrupt is cleared by reading the INT_SRC_M (13h) register.
//    4D: 4D enable: 4D detection on acceleration data is enabled when 6D bit in IG_CFG1 (30h) is set to 1. Default value: 0.
//  MIEN: Enable interrupt generation for magnetic data. Default value: 0.
//        (0: disable interrupt generation; 1: enable interrupt generation)
//
// INT_THS_L_M (14h) / INT_THS_H_M (15h)
//
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ THS7 │ THS6 │ THS5 │ THS4 │ THS3 │ THS2 │ THS1 │ THS0 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │  0   │THS14 │THS13 │THS12 │THS11 │THS10 │ THS9 │ THS8 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Threshold value for magentic interrupt generation - if an axes value is less than or greater than this threshold then and 
// interrupt will be generated.

typedef enum
{
	INT_CTRL_M_Z							= 0b001000000,
	INT_CTRL_M_Y							= 0b010000000,
	INT_CTRL_M_X							= 0b100000000
} int_crl_m_axes;

#define INT_CTRL_M_OPEN_DRAIN_ENABLE		0b00010000
#define INT_CTRL_M_ACTIVE_HIGH				0b00001000
#define INT_CTRL_M_LATCH_ENABLE				0b00000100
#define INT_CTRL_M_4D_ENABLE				0b00000010
#define INT_CTRL_M_ENABLE					0b00000001

#define INT_CTRL_M_DEFAULT					0b00001000
//===================================================================================================================================================


//===================================================================================================================================================
// INT_SRC_M: Interrupt status register for magnometer.
//
// ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
// │ M_PTH_X │ M_PTH_Y │ M_PTH_Z │ M_NTH_X │ M_NTH_Y │ M_NTH_Z │ MROI    │ MINT    │
// └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
// M_PTH_X: Magnetic value on X─axis exceeds the threshold on the positive side. Default value: 0.
// M_PTH_Y: Magnetic value on Y─axis exceeds the threshold on the positive side. Default value: 0.
// M_PTH_Z: Magnetic value on Z─axis exceeds the threshold on the positive side. Default value: 0.
// M_NTH_X: Magnetic value on X─axis exceeds the threshold on the negative side. Default value: 0.
// M_NTH_Y: Magnetic value on Y─axis exceeds the threshold on the negative side. Default value: 0.
// M_NTH_Z: Magnetic value on Z─axis exceeds the threshold on the negative side. Default value: 0.
//    MROI: Internal measurement range overflow on magnetic value. Default value: 0.
//    MINT: Magnetic interrupt event. The magnetic field value exceeds the threshold. Default value: 0.

#define	INT_SRC_M_MAG_INT					0b00000001
#define	INT_SRC_M_MAG_RANGE_OVERFLOW		0b00000010
#define	INT_SRC_M_MAG_INT_Z_NEG				0b00000100
#define	INT_SRC_M_MAG_INT_Y_NEG				0b00001000
#define	INT_SRC_M_MAG_INT_X_NEG				0b00010000
#define	INT_SRC_M_MAG_INT_Z_POS				0b00100000
#define	INT_SRC_M_MAG_INT_Y_POS				0b01000000
#define	INT_SRC_M_MAG_INT_X_POS				0b10000000

//===================================================================================================================================================


//===================================================================================================================================================
// CTRL0: Device configuration register.
//
// ┌──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┐
// │ BOOT     │ FIFO_EN  │ FTH_EN   │ 0        │ 0        │ HP_Click │ HPIS1    │ HPIS2    │
// └──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
//     BOOT: Reboot memory content. Default value: 0
//           (0: normal mode; 1: reboot memory content)
//  FIFO_EN: FIFO enable. Default value: 0
//           (0: FIFO disable; 1: FIFO enable)
//   FTH_EN: FIFO programmable threshold enable. Default value: 0
//           (0: disable; 1: enable)
// HP_Click: High─pass filter enabled for click function. Default value: 0
//           (0: filter bypassed; 1: filter enabled)
//    HPIS1: High─pass filter enabled for interrupt generator 1. Default value: 0
//           (0: filter bypassed; 1: filter enabled)
//    HPIS2: High─pass filter enabled for interrupt generator 2. Default value: 0
//           (0: filter bypassed; 1: filter enabled)

#define CTRL0_HPF_INT2						0b00000001
#define CTRL0_HPF_INT1						0b00000010
#define CTRL0_HPF_CLICK_ENABLE				0b00000100
#define CTRL0_FIFO_THRESH_ENABLE			0b00100000
#define CTRL0_FIFO_ENABLE					0b01000000
#define CTRL0_BOOT_ENABLE					0b10000000

#define CTRL0_DEFAULT						0b00000000

//===================================================================================================================================================


//===================================================================================================================================================
// CTRL1: Device configuration register.
//
// ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬──────┐
// │ AODR3 │ AODR2 │ AODR1 │ AODR0 │ BDU   │ AZEN  │ AYEN  │ AXEN │
// └───────┴───────┴───────┴───────┴───────┴───────┴───────┴──────┘
// AODR3─ AODR0: Acceleration data─rate selection. Default value: 0000
//                (0000: Power─down mode; Others: Refer to Table 36)
//                ┌───────┬───────┬───────┬───────┬────────────────────────────┐
//                │ AODR3 │ AODR2 │ AODR1 │ AODR0 │ Power mode and sample rate │
//                ├───────┼───────┼───────┼───────┼────────────────────────────┤
//                │ 0     │ 0     │ 0     │ 0     │ Power─down mode            │
//                │ 0     │ 0     │ 0     │ 1     │ 3.125 Hz                   │
//                │ 0     │ 0     │ 1     │ 0     │ 6.25 Hz                    │
//                │ 0     │ 0     │ 1     │ 1     │ 12.5 Hz                    │
//                │ 0     │ 1     │ 0     │ 0     │ 25 Hz                      │
//                │ 0     │ 1     │ 0     │ 1     │ 50 Hz                      │
//                │ 0     │ 1     │ 1     │ 0     │ 100 Hz                     │
//                │ 0     │ 1     │ 1     │ 1     │ 200 Hz                     │
//                │ 1     │ 0     │ 0     │ 0     │ 400 Hz                     │
//                │ 1     │ 0     │ 0     │ 1     │ 800 Hz                     │
//                │ 1     │ 0     │ 1     │ 0     │ 1600 Hz                    │
//                └───────┴───────┴───────┴───────┴────────────────────────────┘
//          BDU: Block data update for acceleration and magnetic data. Default value: 0
//               (0: continuous update; 1: output registers not updated until MSB and LSB have been read)
//         AZEN: Acceleration Z─axis enable. Default value: 1
//               (0: Z─axis disabled; 1: Z─axis enabled)
//         AYEN: Acceleration Y─axis enable. Default value: 1
//               (0: Y─axis disabled; 1: Y─axis enabled)
//         AXEN: Acceleration X─axis enable. Default value: 1
//               (0: X─axis disabled; 1: X─axis enabled)
//
// OFFSET_X_L_M (16h) / OFFSET_X_L_M (17h)
//
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ OFX7 │ OFX6 │ OFX5 │ OFX4 │ OFX3 │ OFX2 │ OFX1 │ OFX0 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │OFX15 │OFX14 │OFX13 │OFX12 │OFX11 │OFX10 │ OFX9 │ OFX8 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Offset value for acceleration X axis - this is subtracted from the measured value in the registers OUT_X_L_A and OUT_X_H_A
//
// OFFSET_Y_L_M (16h) / OFFSET_Y_L_M (17h)
//
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ OFY7 │ OFY6 │ OFY5 │ OFY4 │ OFY3 │ OFY2 │ OFY1 │ OFY0 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │OFY15 │OFY14 │OFY13 │OFY12 │OFY11 │OFY10 │ OFY9 │ OFY8 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Offset value for acceleration Y axis - this is subtracted from the measured value in the registers OUT_Y_L_A and OUT_Y_H_A
//
// OFFSET_Z_L_M (16h) / OFFSET_Z_L_M (17h)
//
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ OFZ7 │ OFZ6 │ OFZ5 │ OFZ4 │ OFZ3 │ OFZ2 │ OFZ1 │ OFZ0 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │OFZ15 │OFZ14 │OFZ13 │OFZ12 │OFZ11 │OFZ10 │ OFZ9 │ OFZ8 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// Offset value for acceleration Z axis - this is subtracted from the measured value in the registers OUT_Z_L_A and OUT_Z_H_A

typedef enum
{
	CTRL1_POWER_DOWN					= 0b00000000,
	CTRL1_3_125_HZ						= 0b00000000,
	CTRL1_6_25_HZ						= 0b00000000,
	CTRL1_12_5_HZ						= 0b00000000,
	CTRL1_25_HZ							= 0b00000000,
	CTRL1_50_HZ							= 0b00000000,
	CTRL1_100_HZ						= 0b00000000,
	CTRL1_200_HZ						= 0b00000000,
	CTRL1_400_HZ						= 0b00000000,
	CTRL1_800_HZ						= 0b00000000,
	CTRL1_1600_HZ						= 0b00000000,
} e_ctrl1_power_mode;

#define CTRL1_BDU_ENABLE				0b00001000
#define CTRL1_X_ENABLE					0b00000001
#define CTRL1_Y_ENABLE					0b00000010
#define CTRL1_Z_ENABLE					0b00000100

#define CTRL1_DEFAULT					0b01010111

//===================================================================================================================================================


//===================================================================================================================================================
// CTRL2: Device configuration register.
//
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ ABW1 │ ABW0 │ AFS2 │ AFS1 │ AFS0 │  0   │ AST  │ SIM  │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// ABW1 ─ ABW0: Accelerometer anti─alias filter bandwidth. Default value: 00
//              ┌──────┬──────┬─────────────────────────────┐
//              │ ABW1 │ ABW0 │ Anti─alias filter bandwidth │
//              ├──────┼──────┼─────────────────────────────┤
//              │ 0    │ 0    │ 773 Hz                      │
//              │ 0    │ 1    │ 194 Hz                      │
//              │ 1    │ 0    │ 362 Hz                      │
//              │ 1    │ 1    │ 50 Hz                       │
//              └──────┴──────┴─────────────────────────────┘
// AFS2 ─ AFS0: Acceleration full─scale selection. Default value: 000
//              ┌──────┬──────┬──────┬─────────────────────────┐
//              │ AFS2 │ AFS1 │ AFS0 │ Acceleration full scale │
//              ├──────┼──────┼──────┼─────────────────────────┤
//              │ 0    │ 0    │ 0    │±2 g                     │
//              │ 0    │ 0    │ 1    │±4 g                     │
//              │ 0    │ 1    │ 0    │±6 g                     │
//              │ 0    │ 1    │ 1    │±8 g                     │
//              │ 1    │ 0    │ 0    │±16 g                    │
//              └──────┴──────┴──────┴─────────────────────────┘
//         AST: Acceleration self─test enable. Default value: 0
//              (0: self─test disabled; 1: self─test enabled)
//         SIM: SPI serial interface mode selection. Default value: 0
//              (0: 4─wire interface; 1: 3─wire interface)

typedef enum
{
	CTRL2_AAF_BANDWIDTH_733_HZ			= 0b00000000,
	CTRL2_AAF_BANDWIDTH_194_HZ			= 0b01000000,
	CTRL2_AAF_BANDWIDTH_362_HZ			= 0b10000000,
	CTRL2_AAF_BANDWIDTH_50_HZ			= 0b11000000
} ctrl2_aaf_bandwidth;

typedef enum
{
	CTRL2_ACCEL_BADWIDTH_2G				= 0b00000000,
	CTRL2_ACCEL_BADWIDTH_4G				= 0b00001000,
	CTRL2_ACCEL_BADWIDTH_6G				= 0b00010000,
	CTRL2_ACCEL_BADWIDTH_8G				= 0b00011000,
	CTRL2_ACCEL_BADWIDTH_16G			= 0b00100000
} ctrl2_accel_scale;

#define CTRL2_ACCEL_SELF_TEST			0b00000010
#define CTRL2_ACCEL_SPI_3WIRE_ENABLE	0b00000001

#define CTRL2_DEFAULT					0b00010000

//===================================================================================================================================================

//===================================================================================================================================================
// CTRL3: Device configuration register.
//
// ┌─────────────┬─────────────┬─────────────┬─────────────┬─────────────┬─────────────┬─────────────┬─────────────┐
// │ INT1_BOOT   │ INT1_Click  │ INT1_IG1    │ INT1_IG2    │ INT1_IGM    │ INT1_DRDY_A │ INT1_DRDY_M │ INT1_EMPTY  │
// └─────────────┴─────────────┴─────────────┴─────────────┴─────────────┴─────────────┴─────────────┴─────────────┘
//   INT1_BOOT: Boot on INT1 enable. Default value: 0
//              (0: disable; 1: enable)
//  INT1_Click: Click generator interrupt on INT1. Default value: 0
//              (0: disable; 1: enable)
//    INT1_IG1: Inertial interrupt generator 1 on INT1. Default value: 0
//              (0: disable; 1: enable)
//    INT1_IG2: Inertial interrupt generator 2 on INT1. Default value: 0
//              (0: disable; 1: enable)
//    INT1_IGM: Magnetic interrupt generator on INT1. Default value: 0
//              (0: disable; 1: enable)
// INT1_DRDY_A: Accelerometer data─ready signal on INT1. Default value: 0
//              (0: disable; 1: enable)
// INT1_DRDY_M: Magnetometer data─ready signal on INT1. Default value: 0
//              (0: disable; 1: enable)
//  INT1_EMPTY: FIFO empty indication on INT1. Default value: 0
//              (0: disable; 1: enable)

// CTRL4: Device configuration register.
//
// ┌──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┐
// │ INT2_Click   │ INT2_IG1     │ INT2_IG2     │ INT2_IGM     │ INT2_DRDY_A  │ INT2_DRDY_M  │ INT2_Overrun │ INT2_FTH     │
// └──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┘
//   INT2_Click: Click generator interrupt on INT2. Default value: 0 
//               (0: disable; 1: enable)
//     INT2_IG1: Inertial interrupt generator 1 on INT2. Default value: 0 
//               (0: disable; 1: enable)
//     INT2_IG2: Inertial interrupt generator 2 on INT2. Default value: 0 
//               (0: disable; 1: enable)
//     INT2_IGM: Magnetic interrupt generator on INT2. Default value: 0 
//               (0: disable; 1: enable)
//  INT2_DRDY_A: Accelerometer data─ready signal on INT2. Default value: 0 
//               (0: disable; 1: enable)
//  INT2_DRDY_M: Magnetometer data─ready signal on INT2. Default value: 0 
//               (0: disable; 1: enable)
// INT2_Overrun: FIFO overrun interrupt on INT2. Default value: 0 
//               (0: disable; 1: enable)
//     INT2_FTH: FIFO threshold interrupt on INT2. Default value: 0 
//               (0: disable; 1: enable)		

#define CTRL_INT_BOOT					0b10000000
#define CTRL_INT_CLICK					0b01000000
#define CTRL_INT_IG1					0b00100000
#define CTRL_INT_IG2					0b00010000
#define CTRL_INT_MAGNETIC				0b00001000
#define CTRL_INT_ACCEL_DR				0b00000100
#define CTRL_INT_MAG_DR					0b00000010
#define CTRL_INT_FIFO_EMPTY				0b00000001

#define CTRL3_DEFAULT					0b00000000
#define CTRL4_DEFAULT					0b00000000

//===================================================================================================================================================


//===================================================================================================================================================
// CTRL5: Device configuration register.
//
// ┌─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
// │ TEMP_EN │ M_RES1  │ M_RES0  │ M_ODR2  │ M_ODR1  │ M_ODR0  │ LIR2    │ LIR1    │
// └─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
//         TEMP_EN: Temperature sensor enable. Default value: 0
//                  (0: temperature sensor disabled; 1: temperature sensor enabled)
//   M_RES1─M_RES0: Magnetic resolution selection. Default value: 00
//                  (00: low resolution, 11: high resolution)
// M_ODR2 ─ M_ODR0: Magnetic data rate selection. Default value: 110
//                  ┌───────┬───────┬───────┬────────────────────────────────────────────────────────────────────────────────────────────┐
//                  │ MODR2 │ MODR1 │ MODR0 │ Sample rate (ODR)                                                                          │
//                  ├───────┼───────┼───────┼────────────────────────────────────────────────────────────────────────────────────────────┤
//                  │ 0     │ 0     │ 0     │ 3.125 Hz                                                                                   │
//                  │ 0     │ 0     │ 1     │ 6.25 Hz                                                                                    │
//                  │ 0     │ 1     │ 0     │ 12.5 Hz                                                                                    │
//                  │ 0     │ 1     │ 1     │ 25 Hz                                                                                      │
//                  │ 1     │ 0     │ 0     │ 50 Hz                                                                                      │
//                  │ 1     │ 0     │ 1     │ 100 Hz ─ Available only for accelerometer ODR > 50 Hz or accelerometer in power─down mode. │
//                  │       │       │       │          (refer to CTRL1, AODR setting).                                                   │
//                  │ 1     │ 1     │ 0     │ Do not use                                                                                 │
//                  │ 1     │ 1     │ 1     │ Reserved                                                                                   │
//                  └───────┴───────┴───────┴────────────────────────────────────────────────────────────────────────────────────────────┘
//            LIR2: Latch interrupt request on INT2_SRC register, with INT2_SRC register cleared by reading INT2_SRC itself. Default value: 0.
//                  (0: interrupt request not latched; 1: interrupt request latched)
//            LIR1: Latch interrupt request on INT1_SRC register, with INT1_SRC register cleared by reading INT1_SRC itself. Default value: 0.
//                  (0: interrupt request not latched; 1: interrupt request latched)

#define CTRL5_TEMP_ENABLE			0b10000000

typedef enum
{
	CTRL5_MAG_LOW_RES					= 0b00000000,
	CTRL5_MAG_HIGH_RES					= 0b01100000
} e_ctrl5_mag_resolution;

typedef enum
{
	CTRL5_MAG_DATA_RATE_3_125_HZ		= 0b00000000,
	CTRL5_MAG_DATA_RATE_6_25_HZ			= 0b00000000,
	CTRL5_MAG_DATA_RATE_12_5_HZ			= 0b00000000,
	CTRL5_MAG_DATA_RATE_25_HZ			= 0b00000000,
	CTRL5_MAG_DATA_RATE_50_HZ			= 0b00000000,
	CTRL5_MAG_DATA_RATE_100_HZ			= 0b00000000,
} e_ctrl5_mag_data_rate;

#define CTRL5_INT1_LATCH				0b00000001
#define CTRL5_INT2_LATCH				0b00000010

#define CTRL5_DEFAULT					0b11110000

//===================================================================================================================================================


//===================================================================================================================================================
// CTRL6: Device configuration register.
//
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ 0    │ MFS1 │ MFS0 │ 0    │ 0    │ 0    │ 0    │ 0    │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// MFS1─MFS0: Magnetic full─scale selection. Default value: 01
//            ┌──────┬──────┬─────────────────────┐
//            │ MFS1 │ MFS0 │ Magnetic full scale │
//            ├──────┼──────┼─────────────────────┤
//            │ 0    │ 0    │ ±2 gauss            │
//            │ 0    │ 1    │ ±4 gauss            │
//            │ 1    │ 0    │ ±8 gauss            │
//            │ 1    │ 1    │ ±12 gauss           │
//            └──────┴──────┴─────────────────────┘

typedef enum
{
	CTRL6_MAG_SCALE_2G				= 0b00000000,
	CTRL6_MAG_SCALE_4G				= 0b00100000,
	CTRL6_MAG_SCALE_8G				= 0b01000000,
	CTRL6_MAG_SCALE_16G				= 0b01100000
} e_mag_scale;

#define CTRL6_DEFAULT				0b00100000

//===================================================================================================================================================


//===================================================================================================================================================
// CTRL7: Device configuration register.
//
// ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
// │ AHPM1  │ AHPM0  │ AFDS   │ T_ONLY │ 0      │ MLP    │ MD1    │ MD0    │
// └────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘
//
// AHPM1 ─ AHPM0: High─pass filter mode selection for acceleration data. Default value: 00
//                ┌───────┬───────┬─────────────────────────────────────────────────────────────────────────────────────────────────┐
//                │ AHPM1 │ AHPM0 │ High─pass filter mode                                                                           │
//                ├───────┼───────┼─────────────────────────────────────────────────────────────────────────────────────────────────┤
//                │ 0     │ 0     │ Normal mode (reset X, Y and Z─axis, reading respective REFERENCE_X (1Ch), REFERENCE_Y (1Dh) and │
//                │       │       │ REFERENCE_Z (1Eh) registers)                                                                    │
//                │ 0     │ 1     │ Reference signal for filtering                                                                  │
//                │ 1     │ 0     │ Normal mode                                                                                     │
//                │ 1     │ 1     │ Auto─reset on interrupt event                                                                   │
//                └───────┴───────┴─────────────────────────────────────────────────────────────────────────────────────────────────┘
//          AFDS: Filtered acceleration data selection. Default value: 0
//                (0: internal filter bypassed; 1: data from internal filter sent to output register and FIFO)
//        T_ONLY: Temperature sensor only mode. Default value: 0. If this bit is set to ‘1’, the temperature sensor is on while the magnetic sensor is off.
//           MLP: Magnetic data low─power mode. Default value: 0. If this bit is ‘1’, the M_ODR [2:0] is set to 3.125 Hz independently from the MODR settings.
//                Once the bit is set to ‘0’, the magnetic data rate is configured by the MODR bits in the CTRL5 (24h) register.
//       MD1─MD0: Magnetic sensor mode selection. Default 10
//                ┌─────┬─────┬────────────────────────────┐
//                │ MD1 │ MD0 │ Magnetic sensor mode       │
//                ├─────┼─────┼────────────────────────────┤
//                │ 0   │ 0   │ Continuous─conversion mode │
//                │ 0   │ 1   │ Single─conversion mode     │
//                │ 1   │ 0   │ Power─down mode            │
//                │ 1   │ 1   │ Power─down mode            │
//                └─────┴─────┴────────────────────────────┘

typedef enum
{
	CTRL7_HPF_MODE_NORMAL_RESET			= 0b00000000,
	CTRL7_HPF_MODE_REFERENCE			= 0b01000000,
	CTRL7_HPF_MODE_NORMAL				= 0b10000000,
	CTRL7_HPF_MODE_AUTO_RESET			= 0b11000000
} e_hpf_mode;

#define CTRL7_FILTER_ENABLE				  0b00100000
#define CTRL7_TEMPERATURE_ONLY			  0b00010000
#define CTRL7_MAG_LOW_POWER_ENABLE		  0b00000100

typedef enum
{
	CTRL7_MAG_DATA_MODE_CONTINUOUS		= 0b00000000,
	CTRL7_MAG_DATA_MODE_SINGLE			= 0b00000001,
	CTRL7_MAG_DATA_MODE_POWER_DOWN1		= 0b00000010,
	CTRL7_MAG_DATA_MODE_POWER_DOWN2		= 0b00000001
} e_mag_data_mode;

#define CTRL7_DEFAULT					0b00000000

//===================================================================================================================================================


//===================================================================================================================================================
// STATUS_A: Acceleration data overrun / available status register.
//
// ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
// │ ZYXAOR │ ZAOR   │ YAOR   │ XAOR   │ ZYXADA │ ZADA   │ YADA   │ XADA   │
// └────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘
// ZYXAOR: Acceleration X, Y and Z─axis data overrun. Default value: 0
//         (0: no overrun has occurred; 1: a new set of data has overwritten the previous data)
//   ZAOR: Acceleration Z─axis data overrun. Default value: 0
//         (0: no overrun has occurred; 1: new data for the Z─axis has overwritten the previous data)
//   YAOR: Acceleration Y─axis data overrun. Default value: 0
//         (0: no overrun has occurred; 1: new data for the Y─axis has overwritten the previous data)
//   XAOR: Acceleration X─axis data overrun. Default value: 0
//         (0: no overrun has occurred; 1: new data for the X─axis has overwritten the previous data)
// ZYXADA: Acceleration X, Y and Z─axis new value available. Default value: 0
//         (0: a new set of data is not yet available; 1: a new set of data is available)
//   ZADA: Acceleration Z─axis new value available. Default value: 0
//         (0: new data for the Z─axis is not yet available; 1: new data for the Z─axis is available)
//   YADA: Acceleration Y─axis new value available. Default value: 0
//         (0: new data for the Y─axis is not yet available; 1: new data for the Y─axis is available)
//   XADA: Acceleration X─axis new value available. Default value: 0
//         (0: new data for the X─axis is not yet available; 1: new data for the X─axis is available)

#define STATUS_A_XYZ_OVERRUN			0b10000000
#define STATUS_A_Z_OVERRUN				0b01000000
#define STATUS_A_Y_OVERRUN				0b00100000
#define STATUS_A_X_OVERRUN				0b00010000
#define STATUS_A_XYZ_AVAILABLE			0b00001000
#define STATUS_A_Z_AVAILABLE			0b00000100
#define STATUS_A_Y_AVAILABLE			0b00000010
#define STATUS_A_X_AVAILABLE			0b00000001

//===================================================================================================================================================


//===================================================================================================================================================
// FIFO_CTRL: FIFO buffer configuration register.
//
// ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
// │ FM2  │ FM1  │ FM0  │ FTH4 │ FTH3 │ FTH2 │ FTH1 │ FTH0 │
// └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
// FM2 ─ FM0: FIFO mode selection. Default value: 000
//            ┌─────┬─────┬─────┬───────────────────────┐
//            │ FM2 │ FM1 │ FM0 │ FIFO mode             │
//            ├─────┼─────┼─────┼───────────────────────┤
//            │ 0   │ 0   │ 0   │ Bypass mode           │
//            │ 0   │ 0   │ 1   │ FIFO mode             │
//            │ 0   │ 1   │ 0   │ Stream mode           │
//            │ 0   │ 1   │ 1   │ Stream─to─FIFO mode   │ 
//            │ 1   │ 0   │ 0   │ Bypass─to─Stream mode │
//            └─────┴─────┴─────┴───────────────────────┘
// FTH4 ─ FTH0: FIFO threshold level. Default value: 00000

typedef enum
{
		FIFO_CTRL_BYPASS				= 0b00000000,
		FIFO_CTRL_FIFO					= 0b00000001,
		FIFO_CTRL_STREAM				= 0b00000010,
		FIFO_CTRL_STREAM_FIFO			= 0b00000011,
		FIFO_CTRL_BYPASS_STREAM			= 0b00000100
} e_fifo_ctrl;

#define FIFO_CTRL_DEFAULT 				0b00000000

//===================================================================================================================================================


//===================================================================================================================================================
// FIFO_SRC: FIFO status register.
//
// ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
// │ FTH   │ OVRN  │ EMPTY │ FSS4  │ FSS3  │ FSS2  │ FSS1  │ FSS0  │
// └───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
//
// FTH: FIFO threshold status. FTH bit is set to ‘1’ when FIFO content exceeds threshold level.
// OVRN: FIFO overrun status. OVRN bit is set to ‘1’ when FIFO buffer is full.
// EMPTY: Empty status. EMPTY bit is set to ‘1’ when all FIFO samples have been read and FIFO is empty.
// FSS4 ─ FSS0: FIFO stored data level. Contain the current number of unread FIFO levels.

#define FIFO_THRESHOLD					0b10000000
#define FIFO_OVERRUN					0b01000000
#define FIFO_EMPTY						0b00100000

//===================================================================================================================================================


//===================================================================================================================================================
// IG_CFG1 / IG_CFG2: Inertial interrupt generator 1 and 2 configuration registers.
//
// ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
// │ AOI    │ 6D     │ ZHIE   │ ZLIE   │ YHIE   │ YLIE   │ XHIE   │ XLIE   │
// │        │        │ ZUPE   │ ZDOWNE │ YUPE   │ YDOWNE │ XUPE   │ XDOWNE │
// └────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘
// Content of this register is loaded at boot. Write operation at this address is possible only after system boot.
//
//           AOI: And/Or combination of interrupt events. Default value: 0.
//            6D: 6─direction detection function enabled. Default value: 0.
//                ┌─────┬─────┬─────────────────────────────────────┐
//                │ AOI │ 6D  │ Interrupt mode                      │
//                ├─────┼─────┼─────────────────────────────────────┤
//                │  0  │  0  │ OR combination of interrupt events  │
//                │  0  │  1  │ 6─direction movement recognition    │
//                │  1  │  0  │ AND combination of interrupt events │
//                │  1  │  1  │ 6─direction position recognition    │
//                └─────┴─────┴─────────────────────────────────────┘
//                Difference between AOI─6D = ‘01’ and AOI─6D = ‘11’. AOI─6D = ‘01’ is movement recognition. An interrupt is generated when orientation 
//                moves from an unknown zone to a known zone. The interrupt signal stays for a duration ODR.
//                AOI─6D = ‘11’ is direction recognition. An interrupt is generated when orientation is inside a known zone. The interrupt signal stays 
//                until orientation is inside the zone.

//   ZHIE / ZUPE: Enable interrupt generation on Z high event or on direction recognition. Default value: 0 
//                (0: disable interrupt request; 1: enable interrupt request)
// ZLIE / ZDOWNE: Enable interrupt generation on Z low event or on direction recognition. Default value: 0 
//                (0: disable interrupt request; 1: enable interrupt request)
//   YHIE / YUPE: Enable interrupt generation on Y high event or on direction recognition. Default value: 0 
//                (0: disable interrupt request; 1: enable interrupt request.)
// YLIE / YDOWNE: Enable interrupt generation on Y low event or on direction recognition. Default value: 0 
//                (0: disable interrupt request; 1: enable interrupt request.)
//   XHIE / XUPE: Enable interrupt generation on X high event or on direction recognition. Default value: 0 
//                (0: disable interrupt request; 1: enable interrupt request.)
// XLIE / XDOWNE: Enable interrupt generation on X low event or on direction recognition. Default value: 0 
//                (0: disable interrupt request; 1: enable interrupt request.)

typedef enum
{
	IG_CFG_OR						= 0b00000000,
	IG_CFG_MOVE_RECOG				= 0b01000000,
	IG_CFG_AND						= 0b10000000,
	IG_CFG_POS_RECOG				= 0b11000000
} e_ig_cfg_int_mode;

#define IG_CFG1_Z_HIGH				0b00100000
#define IG_CFG1_Z_LOW				0b00010000
#define IG_CFG1_Y_HIGH				0b00001000
#define IG_CFG1_Y_LOW				0b00000100
#define IG_CFG1_X_HIGH				0b00000010
#define IG_CFG1_X_LOW				0b00000001

#define IG_CFG_DEFAULT				0b00000000

//===================================================================================================================================================


//===================================================================================================================================================
// IG_SRC1 / IG_SRC2: Inertial interrupt generator 1 and 2 status registers.
//
// ┌────┬────┬────┬────┬────┬────┬────┬────┐
// │ 0  │ IA │ ZH │ ZL │ YH │ YL │ XH │ XL │
// └────┴────┴────┴────┴────┴────┴────┴────┘
// IA: Interrupt status. Default value: 0
//     (0: no interrupt has been generated; 1: one or more interrupts have been generated)
// ZH: Z high. Default value: 0
//     (0: no interrupt; 1: Z high event has occurred)
// ZL: Z low. Default value: 0
//     (0: no interrupt; 1: Z low event has occurred)
// YH: Y high. Default value: 0
//     (0: no interrupt; 1: Y high event has occurred)
// YL: Y low. Default value: 0
//    (0: no interrupt; 1: Y low event has occurred)
// XH: X high. Default value: 0
//    (0: no interrupt; 1: X high event has occurred)
// XL: X low. Default value: 0
//    (0: no interrupt; 1: X low event has occurred)

#define IG_SRC_INT_STATUS			0b01000000
#define IG_SRC_INT_Z_HIGH			0b00100000
#define IG_SRC_INT_Z_LOW			0b00010000
#define IG_SRC_INT_Y_HIGH			0b00001000
#define IG_SRC_INT_Y_LOW			0b00000100
#define IG_SRC_INT_X_HIGH			0b00000010
#define IG_SRC_INT_X_LOW			0b00000001

//===================================================================================================================================================


//===================================================================================================================================================
// CLICK_CFG: Click interrupt generator configuration register.
//
// ┌────┬────┬────┬────┬────┬────┬────┬────┐
// │ ─  │ ─  │ ZD │ ZS │ YD │ YS │ XD │ XS │
// └────┴────┴────┴────┴────┴────┴────┴────┘
// ZD: Enable interrupt double─click on Z─axis. Default value: 0
//     (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold)
// ZS: Enable interrupt single─click on Z─axis. Default value: 0
//     (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold)
// YD: Enable interrupt double─click on Y─axis. Default value: 0
//     (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold)
// YS: Enable interrupt single─click on Y─axis. Default value: 0
//    (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold)
// XD: Enable interrupt double─click on X─axis. Default value: 0
//     (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold)
// XS: Enable interrupt single─click on X─axis. Default value: 0
//     (0: disable interrupt request; 1: enable interrupt request on measured accel. value higher than preset threshold)

#define CLICK_CFG_DC_Z				0b00100000
#define CLICK_CFG_SC_Z				0b00010000
#define CLICK_CFG_DC_Y				0b00001000
#define CLICK_CFG_SC_Y				0b00000100
#define CLICK_CFG_DC_X				0b00000010
#define CLICK_CFG_SC_X				0b00000001

#define CLICK_CFG_DEFAULT			0b00000000

//===================================================================================================================================================


//===================================================================================================================================================
// CLICK_SRC: Click interrupt source status register.
//
// ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┬────────┐
// │ ─      │ IA     │ DClick │ SClick │ Sign   │ Z      │ Y      │ X      │
// └────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘
//     IA: Interrupt active. Default value: 0
//         (0: no interrupt has been generated; 1: one or more interrupts have been generated)
// DClick: Double─click enable. Default value: 0
//         (0: double─click detection disable; 1: double─click detection enable)
// SClick: Single─click enable. Default value: 0
//         (0: single─click detection disable; 1: single─click detection enable)
//   Sign: Click sign. 0: positive detection; 1: negative detection
//      Z: Z─click detection. Default value: 0
//         (0: no interrupt; 1: Z high event has occurred)
//      Y: Y─click detection. Default value: 0
//         (0: no interrupt; 1: Y high event has occurred)
//      X: X─click detection. Default value: 0
//         (0: no interrupt; 1: X high event has occurred)

#define CLICK_SRC_INT_ACTIVE			0b01000000
#define CLICK_SRC_INT_DC_ENABLED		0b00100000
#define CLICK_SRC_INT_SC_ENABLED		0b00010000
#define CLICK_SRC_INT_SC_NEG			0b00001000
#define CLICK_SRC_INT_ON_Z				0b00000100
#define CLICK_SRC_INT_ON_Y				0b00000010
#define CLICK_SRC_INT_ON_Z				0b00000001

//===================================================================================================================================================


class CLSM303_D: public CSensor
{
	public:

		// Construction, destruction and initialisation
		CLSM303_D(int32_t nSensorID = -1);
		virtual ~CLSM303_D();
		void begin(uint8_t nSDOPinNum, uint8_t nSDOPinState);

		// Device settings
		void setDefaultMode(void);
		void setIntCtrlM(int_crl_m_axes eAxes, bool bOpenDrain = false, bool bActiveHigh = true, uint16_t nThreshold = 0, bool bLatchEnable = false, bool b4DEnable = false);
		void setCtrl0HPF(bool bHPFInt2, bool bHPFInt1, bool bHPFClick, bool bFIFOThreshold, bool bFIFO, bool bBoot);
		void setCtrl1AccelData(e_ctrl1_power_mode ePower, bool bBDUEnable, bool bXAxis, bool bYAxis, bool bZAxis);
		void setCtrl2AccelScale(ctrl2_aaf_bandwidth eAAFBandWidth, ctrl2_accel_scale eAccelScale, bool bEnableSelfTest = false, bool bEnableSPI3WireEnable = false);
		void setCtrl34Int(bool nForInt1, bool bBoot, bool bClick, bool bIG1, bool bIG2, bool bMag, bool bAccelDR = false, bool bMagDR = false, bool bFIFOEmpty = false);
		void setCtrl5Mag(bool bTempEnable, e_ctrl5_mag_resolution eMagRes, e_ctrl5_mag_data_rate eMagDataRate, bool bInt1Latch = false, bool bInt2Latch = false);
		void setCtrl6MagData(e_mag_scale eDataRate)
		{
			writeReg(CTRL6, CTRL6_DEFAULT);
		};
		void setCtrl7MAgMode(e_hpf_mode eHPFMode, bool bFilterEnable, bool bTempOnly, bool bMagLowPower, e_mag_data_mode eMagDataMode, uint16_t nXAxisRef = 0, uint16_t nYAxisRef = 0, uint16_t nZAxisRef = 0);
		void setFIFOCtrl(e_fifo_ctrl eFIFOCtrl, uint8_t nFIFOThreshold);
		void setIgCfg(bool bForIG1, e_ig_cfg_int_mode eMode, bool nXHigh, bool nXLow, bool nYHigh, bool nYLow, bool nZHigh, bool nZLow);
		void setClickCfg(bool bDCXAxis, bool bSCXAxis, bool bDCYAxis, bool bSCYAxis, bool bDCZAxis, bool bSCZAxis);
		void setMagAxesOffset(uint16_t nXAxis, uint16_t nYAxis, uint16_t nZAxis);

		// Device status
		bool getMagneticStatus(uint8_t nBitFlags)
		{
			return getStatus(STATUS_M, nBitFlags);
		};
		bool getIntSrcM(uint8_t nBitFlags)
		{
			return getStatus(INT_SRC_M, nBitFlags);
		};
		bool getStatusA(uint8_t nBitFlags)
		{
			return getStatus(STATUS_A, nBitFlags);
		};
		
		bool getFIFOSrc(uint8_t nBitFlags)
		{
			return getStatus(FIFO_SRC, nBitFlags);
		};

		uint8_t getFIFOLevel()
		{
			return readReg8(FIFO_SRC) & 0b00011111;
		};

		bool getIGStatus(bool bForIG1, bool nXLow, bool nXHigh, bool nYLow, bool nYHigh, bool nZLow, bool nZHigh);
		
		bool getClickSrc(uint8_t nBitFlags)
		{
			return getStatus(CLICK_SRC, nBitFlags);
		};
		
		// Device readings
		CVector& getAxesA();
		CVector& getAxesM();
		void getAxes(CVector &vectMag, CVector &vectAccel);
		float getHeading(CVector vectFrom = CVector(1.0, 0.0, 0.0));
		float getTemperature()
		{
			return CSensor::calculateTemperature(TEMP_OUT_H, TEMP_OUT_L, 8);
		};

	private:
	
		// Used to index the conversion arrays below (with the bits right shifted 3 places)
		ctrl2_accel_scale m_eAccelScale;
		e_mag_scale m_MagScale;
		
		// Scale conversions from datasheet.
		static float m_arrayMagConversion[4];
		static float m_arrayAccelConversion[5];
		CMagCalibration m_calibMag;
		CAccelCalibration m_calibAccel;

		// Helper functions
		void writeReg(reg_addr eRegAddr, uint8_t nValue)
		{
			CSensor::writeReg((uint8_t)eRegAddr, nValue);
		};
		uint8_t readReg(reg_addr eRegAddr)
		{
			return CSensor::readReg8((uint8_t)eRegAddr);
		};
		void writeReg(reg_addr eHighRegAddr, reg_addr eLowRegAddr, uint16_t nValue)
		{
			CSensor::writeReg((uint8_t)eHighRegAddr, (uint8_t)eLowRegAddr, nValue);
		};
		uint16_t readReg(reg_addr eHighRegAddr, reg_addr eLowRegAddr)
		{
			return CSensor::readReg16((uint8_t)eHighRegAddr, (uint8_t)eLowRegAddr);
		};
				
};

#endif



