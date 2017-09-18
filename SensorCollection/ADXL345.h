
#ifndef __ADXL345_h
#define __ADXL345_h

#include <Arduino.h>
#include "Sensor.h"

#define ADDR_SDO_LO			(uint8_t)0x53 //CADXL345 address (the SEVEN bit device address) the W/R bit is tacked on by the Wire.h library
#define ADDR_SDO_HIGH		(uint8_t)0x1D //CADXL345 address (the SEVEN bit device address) the W/R bit is tacked on by the Wire.h library


// Register Addresses
typedef enum
{
	THRESH_TAP				= 0x1D,
	OFSX					= 0x1E,
	OFSY					= 0x1F,
	OFSZ					= 0x20,
	DUR						= 0x21,
	LATENT					= 0x22,
	WINDOW					= 0x23,
	THRESH_ACT				= 0x24,
	THRESH_INACT			= 0x25,
	TIME_INACT				= 0x26,
	ACT_INACT_CTL			= 0x27,
	THRESH_FF				= 0x28,
	TIME_FF					= 0x29,
	TAP_AXES				= 0x2A,
	ACT_TAP_STATUS			= 0x2B,
	BW_RATE					= 0x2C,
	POWER_CTL				= 0x2D,
	INT_ENABLE				= 0x2E,
	INT_MAP					= 0x2F,
	INT_SOURCE				= 0x30,
	DATA_FORMAT				= 0x31,
	DATAX0					= 0x32,                                                      
	DATAX1					= 0x33,
	DATAY0					= 0x34,
	DATAY1					= 0x35,
	DATAZ0					= 0x36,
	DATAZ1					= 0x37,
	FIFO_CTL				= 0x38,
	FIFO_STATUS				= 0x39
} e_reg_addr;


//===================================================================================================================================================
// DEVID (00h)
// ┌───┬───┬───┬───┬───┬───┬───┬───┐
// │ 1 │ 1 │ 1 │ 0 │ 0 │ 1 │ 0 │ 1 │
// └───┴───┴───┴───┴───┴───┴───┴───┘
//===================================================================================================================================================


//===================================================================================================================================================
// TAP_AXES (29h)
//
// ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
// │     0     │     0     │     0     │     0     │ SUPPRESS  │ X_TAP_EN  │ Y_TAP_EN  │ Z_TAP_EN  │
// └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
//
// SUPPRESS: Setting the suppress bit suppresses double tap detection if acceleration greater than the value in THRESH_TAP is present between taps. 
//           See the Tap Detection section for more details.
// X_TAP_EN: A setting of 1 enables x-axis participation in tap detection. A setting of 0 excludes the selected axis from participation in tap 
//           detection.
// Y_TAP_EN: A setting of 1 enables y-axis participation in tap detection. A setting of 0 excludes the selected axis from participation in tap 
//           detection.
// Z_TAP_EN: A setting of 1 enables z-axis participation in tap detection. A setting of 0 excludes the selected axis from participation in tap 
//           detection.

#define TAP_AXES_SUPPRESS_DOUBLE_TAP				0b00001000
#define TAP_AXES_X_ENABLE							0b00000100
#define TAP_AXES_Y_ENABLE							0b00000010
#define TAP_AXES_Z_ENABLE							0b00000001

#define TAP_AXES_DEFAULT								0b00000000

//===================================================================================================================================================


//===================================================================================================================================================
// ACT_TAP_STATUS (2Bh)
//
// ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
// │     0     │   ACT_X   │   ACT_Y   │   ACT_Z   │  ASLEEP   │   TAP_X   │   TAP_Y   │   TAP_Z   │
// └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
// 
//  ACT_X: Activity event on x-axis
//  ACT_Y: Activity event on y-axis
//  ACT_Z: Activity event on z-axis
// ASLEEP: A setting of 1 in the asleep bit indicates that the part is asleep, and a setting of 0 indicates that the part is not asleep. See the 
//         Register 0x2D—POWER_CTL (Read/Write) section for more information on autosleep mode.
//  TAP_X: Tap event occured on x-axis
//  TAP_Y: Tap event occured on y-axis
//  TAP_Z: Tap event occured on z-axis

#define ACT_TAP_STATUS_TAP_Z					0b00000001
#define ACT_TAP_STATUS_TAP_Y					0b00000010
#define ACT_TAP_STATUS_TAP_X					0b00000100
#define ACT_TAP_STATUS_ASLEEP					0b00001000
#define ACT_TAP_STATUS_ACT_Z					0b00010000
#define ACT_TAP_STATUS_ACT_Y					0b00100000
#define ACT_TAP_STATUS_ACT_X					0b01000000

//===================================================================================================================================================


//===================================================================================================================================================
// BW_RATE(2Ch)
//
// ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
// │     0     │     0     │     0     │ LOW POWER │    DR3    │    DR2    │    DR1    │    DR0    │
// └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
// 
// LOW POWER: A setting of 0 in the LOW_POWER bit selects normal operation, and a setting of 1 selects reduced power operation, which has somewhat 
//            higher noise
// DR0 - DR3: Data sampling rate
//            ┌─────┬─────┬─────┬─────┬──────────────────┐
//            │ DR3 │ DR2 │ DR1 │ DR0 │ Sample data rate │
//            ├─────┼─────┼─────┼─────┼──────────────────┤
//            │  0  │  1  │  1  │  0  │    6.25 Hz       │
//            │  0  │  1  │  1  │  1  │   12.50 Hz       │
//            │  1  │  0  │  0  │  0  │   25.00 Hz       │
//            │  1  │  0  │  0  │  1  │   50.00 Hz       │
//            │  1  │  0  │  1  │  0  │  100.00 Hz       │
//            │  1  │  0  │  1  │  1  │  200.00 Hz       │
//            │  1  │  1  │  0  │  0  │  400.00 Hz       │
//            │  1  │  1  │  0  │  1  │  800.00 Hz       │
//            │  1  │  1  │  1  │  0  │ 1600.00 Hz       │
//            │  1  │  1  │  1  │  1  │ 3200.00 Hz       │
//            └─────┴─────┴─────┴─────┴──────────────────┘
typedef enum
{
	DATARATE_3200_HZ			 = 0b00001111,
	DATARATE_1600_HZ			 = 0b00001110,
	DATARATE_800_HZ 			 = 0b00001101,
	DATARATE_400_HZ 			 = 0b00001100,
	DATARATE_200_HZ 			 = 0b00001011,
	DATARATE_100_HZ 			 = 0b00001010,
	DATARATE_50_HZ  			 = 0b00001001,
	DATARATE_25_HZ  			 = 0b00001000,
	DATARATE_12_5_HZ			 = 0b00000111,
	DATARATE_6_25HZ 			 = 0b00000110,
	DATARATE_3_13_HZ			 = 0b00000101,
	DATARATE_1_56_HZ			 = 0b00000100,
	DATARATE_0_78_HZ			 = 0b00000011,
	DATARATE_0_39_HZ			 = 0b00000010,
	DATARATE_0_20_HZ			 = 0b00000001,
	DATARATE_0_10_HZ			 = 0b00000000
} e_data_rate;

#define BW_RATE_DEFAULT			0b00001001

//===================================================================================================================================================


//===================================================================================================================================================
// POWER_CTL (0Dh)
//
// ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
// │     0     │     0     │   LINK    │   ASLEEP  │  MEASURE  │   SLEEP   │  WAKEUP1  │  WAKEUP0  │
// └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
//
//              LINK: A setting of 1 in the link bit with both the activity and inactivity functions enabled delays the start of the activity function 
//                    until inactivity is detected. After activity is detected, inactivity detection begins, preventing the detection of activity. This 
//                    bit serially links the activity and inactivity functions. When this bit is set to 0, the inactivity and activity functions are 
//                    concurrent. Additional information can be found in the Link Mode section.
//                    When clearing the link bit, it is recommended that the part be placed into standby mode and then set back to measurement mode with 
//                    a subsequent write. This is done to ensure that the device is properly biased if sleep mode is manually disabled; otherwise, the 
//                    first few samples of data after the link bit is cleared may have additional noise, especially if the device was asleep when the 
//                    bit was cleared.
//            ASLEEP: If the link bit is set, a setting of 1 in the AUTO_SLEEP bit sets the ADXL345 to switch to sleep mode when inactivity is detected 
//                    (that is, when acceleration has been below the THRESH_INACT value for at least the time indicated by TIME_INACT). A setting of 0 
//                     disables automatic switching to sleep mode. See the description of the sleep bit in this section for more information.
//                     When clearing the AUTO_SLEEP bit, it is recommended that the part be placed into standby mode and then set back to measure-ment 
//                     mode with a subsequent write. This is done to ensure that the device is properly biased if sleep mode is manually disabled; otherwise, 
//                     the first few samples of data after the AUTO_SLEEP bit is cleared may have additional noise, especially if the device was asleep when 
//                     the bit was cleared.
//           MEASURE: A setting of 0 in the measure bit places the part into standby mode, and a setting of 1 places the part into measurement mode. The 
//					  ADXL345 powers up in standby mode with minimum power consumption.
//             SLEEP: A setting of 0 in the sleep bit puts the part into the normal mode of operation, and a setting of 1 places the part into sleep mode. 
//                    Sleep mode suppresses DATA_READY, stops transmission of data to FIFO, and switches the sampling rate to one specified by the wakeup 
//                    bits. In sleep mode, only the activity function can be used.
//                    When clearing the sleep bit, it is recommended that the part be placed into standby mode and then set back to measurement mode with a 
//                    subsequent write. This is done to ensure that the device is properly biased if sleep mode is manually disabled; otherwise, the first few 
//                    samples of data after the sleep bit is cleared may have additional noise, especially if the device was asleep when the bit was cleared.
// WAKEUP0 - WAKEUP1: These bits control the frequency of readings in sleep mode.
//                    ┌─────────┬─────────┬──────────────────┐
//                    │ WAKEUP1 │ WAKEUP0 │ Sample data rate │
//                    ├─────────┼─────────┼──────────────────┤
//                    │    0    │    0    │ 8 Hz             │
//                    │    0    │    1    │ 4 Hz             │
//                    │    1    │    0    │ 2 Hz             │
//                    │    1    │    1    │ 1 Hz             │
//                    └─────────┴─────────┴──────────────────┘


#define POWER_CTL_LINK			0b00100000
#define POWER_CTL_AUTO_SLEEP	0b00010000
#define POWER_CTL_MEASURE		0b00001000
#define POWER_CTL_SLEEP			0b00000100

typedef enum
{
	POWER_CTL_SLEEP_DATA_RATE_8HZ		 = 0b00000000,
	POWER_CTL_SLEEP_DATA_RATE_4HZ		 = 0b00000001,
	POWER_CTL_SLEEP_DATA_RATE_2HZ		 = 0b00000010,
	POWER_CTL_SLEEP_DATA_RATE_1HZ		 = 0b00000011
}e_sleep_data_rate;

#define POWER_CTL_DEFAULT		0b00001000

//===================================================================================================================================================


//===================================================================================================================================================
// DATA_FORMAT (31h)
// 
// ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
// │ SELF TEST │    SPI    │INT INVERT │     0     │ FULL RES  │  JUSTFY   │  RANGE1   │  RANGE0   │
// └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
// 
//     SELF_TEST: A setting of 1 in the SELF_TEST bit applies a self-test force to the sensor, causing a shift in the output data. A value of 0 disables 
//                the self-test force.
//           SPI: A value of 1 in the SPI bit sets the device to 3-wire SPI mode, and a value of 0 sets the device to 4-wire SPI mode.
//    INT_INVERT: A value of 0 in the INT_INVERT bit sets the interrupts to active high, and a value of 1 sets the interrupts to active low.
//      FULL_RES: When this bit is set to a value of 1, the device is in full resolution mode, where the output resolution increases with the g range 
//                set by the range bits to maintain a 4 mg/LSB scale factor. When the FULL_RES bit is set to 0, the device is in 10-bit mode, and the 
//                range bits determine the maximum g range and scale factor.
//       JUSTIFY: A setting of 1 in the justify bit selects left (MSB) justified mode, and a setting of 0 selects right justified mode with sign 
//                extension.
// RANGE1-RANGE0: These bits set the g range.
//                ┌─────────┬─────────┬──────────────────┐
//                │ WAKEUP1 │ WAKEUP0 │ Sample data rate │
//                ├─────────┼─────────┼──────────────────┤
//                │    0    │    0    │ 2g               │
//                │    0    │    1    │ 4g               │
//                │    1    │    0    │ 8g               │
//                │    1    │    1    │ 16g              │
//                └─────────┴─────────┴──────────────────┘

#define DATA_FORMAT_SELF_TEST			0b10000000
#define DATA_FORMAT_SPI_3WIRE			0b01000000
#define DATA_FORMAT_INT_INVERT			0b00100000
#define DATA_FORMAT_FULL_RES			0b00001000
#define DATA_FORMAT_JUSTIFY				0b00000100

typedef enum
{
	DATA_FORMAT_SCALE_2G				= 0b00000000,
	DATA_FORMAT_SCALE_4G				= 0b00000001,
	DATA_FORMAT_SCALE_8G				= 0b00000010,
	DATA_FORMAT_SCALE_16G				= 0b00000011

} e_scale;

#define DATA_FORMAT_DEFAULT				0b00001001

//===================================================================================================================================================


//===================================================================================================================================================
// FIFO_CTL(38h)
//
// ┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┐
// │ FIFO_MODE1 │ FIFO_MODE0 │  TRIGGER   │ FIFO_SAMP4 │ FIFO_SAMP3 │ FIFO_SAMP2 │ FIFO_SAMP1 │ FIFO_SAMP0 │
// └────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘
// 
//     FIFO_MODE0-FIFO_MODE1: These bits set the FIFO mode
//              		      ┌────────────┬────────────┬──────────────────┐
//             		          │ FIFO_MODE1 │ FIFO_MODE0 │ Mode             │
//                            ├────────────┼────────────┼──────────────────┤
//                            │     0      │     0      │ Bypass           │
//                            │     0      │     1      │ FIFO             │
//                            │     1      │     0      │ Stream           │
//                            │     1      │     1      │ Trigger          │
//                            └────────────┴────────────┴──────────────────┘
//                            Bypass: FIFO is bypassed
//                              FIFO: FIFO collects up to 32 values and then stops collecting data, collecting new data only when FIFO is not full
//                            Stream: FIFO holds the last 32 data values. When FIFO is full, the oldest data is overwritten with newer data.
//                           Trigger: When triggered by the trigger bit, FIFO holds the last data samples before the trigger event and then continues 
//                                    to collect data until full. New data is collected only when FIFO is not full.
//                   TRIGGER: A value of 0 in the trigger bit links the trigger event of trigger mode to INT1, and a value of 1 links the trigger 
//                      event to INT2.
// FIFO_SAMPLE0-FIFO_SAMPLE4: The function of these bits depends on the FIFO mode selected (see Table 20). Entering a value of 0 in the samples bits 
//                            immediately sets the watermark status bit in the INT_SOURCE register, regardless of which FIFO mode is selected. Undesirable 
//                            operation may occur if a value of 0 is used for the samples bits when trigger mode is used.
//              		      ┌───────────┬─────────────────────────────────────────────────────────────────────────────────────────┐
//             		          │ FIFO_MODE │ Function of FIFO_SAMPLE                                                                 │
//                            ├───────────┼─────────────────────────────────────────────────────────────────────────────────────────┤
//                            │ Bypass    │ None                                                                                    │
//                            │ FIFO      │ Specifies how many FIFO entries are needed to trigger a watermark interrupt.            │
//                            │ Stream    │ Specifies how many FIFO entries are needed to trigger a watermark interrupt.            │
//                            │ Trigger   │ Specifies how many FIFO samples are retained in the FIFO buffer before a trigger event. │
//                            └───────────┴─────────────────────────────────────────────────────────────────────────────────────────┘

typedef enum
{
	FIFO_CTL_FIFO_MODE_BYPASS			= 0b00000000,
	FIFO_CTL_FIFO_MODE_FIFO				= 0b01000000,
	FIFO_CTL_FIFO_MODE_STREAM			= 0b10000000,
	FIFO_CTL_FIFO_MODE_TRIGGER			= 0b11000000
} e_fifo_mode;

#define FIFO_CTL_TRIGGER				0b00100000

#define FIFO_CTL_DEFAULT				0b00000000

//===================================================================================================================================================


//===================================================================================================================================================
// FIFO_STATUS (39h)
//
// ┌────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┬────────────┐
// │ FIFO_TRIG  │     0      │  STATUS5   │  STATUS4   │  STATUS3   │  STATUS2   │  STATUS1   │  STATUS0   │
// └────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘
//
//       FIFO_TRIG: A 1 in the FIFO_TRIG bit corresponds to a trigger event occurring, and a 0 means that a FIFO trigger event has not occurred.
// STATUS0-STATUS5: These bits report how many data values are stored in FIFO. Access to collect the data from FIFO is provided through the DATAX, 
//                  DATAY, and DATAZ registers. FIFO reads must be done in burst or multiple-byte mode because each FIFO level is cleared after 
//                  any read (single- or multiple-byte) of FIFO. FIFO stores a maximum of 32 entries, which equates to a maximum of 33 entries 
//                  available at any given time because an additional entry is available at the output filter of the device.

#define FIFO_STATUS_TRIG				0b10000000

//===================================================================================================================================================


//================================================================================================ 
// INT_ENABLE (2E)
// 
// ┌──────────┬───────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┐
// │DATA_READY│SINGLE_TAP │DOUBLE_TAP│ ACTIVITY │INACTIVITY│FREE_FALL │WATER_MARK│ OVERRUN  │
// └──────────┴───────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
// 
// DATA_READY: If set to 1 then an interrupt is generated when there is data in the FIFO buffer.
// SINGLE_TAP: If set to 1 then an interrupt is generated when a single tap event occurs.
// DOUBLE_TAP: If set to 1 then an interrupt is generated when a double tap event occurs.
//   ACTIVITY: If set to 1 then an interrupt is generated when an activity occurs.
// INACTIVITY: If set to 1 then an interrupt is generated when no events occur.
//  FREE_FALL: If set to 1 then an interrupt is generated when a  free fall event occurs.
// WATER_MARK: If set to 1 then an interrupt is generated when a the data in the FIFO buffer 
//             reaches the watermark.
//    OVERRUN: If set to 1 then an interrupt is generated when a in the FIFO buffer is overwritten.

// INT_MAP (2F)
//
// ┌──────────┬───────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┐
// │DATA_READY│SINGLE_TAP │DOUBLE_TAP│ ACTIVITY │INACTIVITY│FREE_FALL │WATER_MARK│ OVERRUN  │
// └──────────┴───────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
//
// Same definitions as for INT_ENABLE,
// If the bit is set to 0 then the interrupt is directed to INT1.
// If the bit is set to 1 then the interrupt is directed to INT2.

// INT_SOURCE (30)
// 
// ┌──────────┬───────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┐
// │DATA_READY│SINGLE_TAP │DOUBLE_TAP│ ACTIVITY │INACTIVITY│FREE_FALL │WATER_MARK│ OVERRUN  │
// └──────────┴───────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
//
// DATA_READY: If set to 1 then data is ready interrupt has been generated.
// SINGLE_TAP: If set to 1 then a single tap interrupt has been generated.
// DOUBLE_TAP: If set to 1 then a double tap interrupt has been generated.
//   ACTIVITY: If set to 1 then an activity interrupt has been generated.
// INACTIVITY: If set to 1 then an inactivity interrupt has been generated.
//  FREE_FALL: If set to 1 then a free fall interrupt has been generated.
// WATER_MARK: If set to 1 then a watermark interrupt has been generated
//    OVERRUN: If set to 1 then an overrun interrupt has been generated.

#define INT_DATA_READY					0b10000000
#define INT_SINGLE_TAP					0b01000000
#define INT_DOUBLE_TAP					0b00100000
#define INT_ACTIVITY					0b00010000
#define INT_INACTIVITY					0b00001000
#define INT_FREE_FALL					0b00000100
#define INT_WATER_MARK					0b00000010
#define INT_OVERRUN						0b00000001

#define INT_DEFAULT						0b00000000

//================================================================================================ 


//================================================================================================ 
// ACT_INACT_CTL (27h)
//
// ┌─────────┬─────────┬──────────┬──────────┬─────────┬─────────┬─────────┬─────────┐
// │ ACT     │ ACT_X   │  ACT_Y   │  ACT_Z   │ INACT   │ INACT_X │ INACT_Y │ INACT_Z │
// │ ac/dc   │ enable  │  enable  │  enable  │ ac/dc   │ enable  │ enable  │ enable  │
// └─────────┴─────────┴──────────┴──────────┴─────────┴─────────┴─────────┴─────────┘
// ACT AC/DC and INACT AC/DC Bits
//		A setting of 0 selects dc-coupled operation, and a setting of 1 enables ac-coupled operation. 
//		In dc-coupled operation, the current acceleration magnitude is compared directly with 
//		THRESH_ACT and THRESH_INACT to determine whether activity or inactivity is detected.
//		In ac-coupled operation for activity detection, the acceleration value at the start of 
//		activity detection is taken as a reference value. New samples of acceleration are then 
//		compared to this reference value, and if the magnitude of the difference exceeds the 
//		THRESH_ACT value, the device triggers an activity interrupt.
//		Similarly, in ac-coupled operation for inactivity detection, a reference value is used for 
//		comparison and is updated whenever the device exceeds the inactivity threshold. After the 
//		reference value is selected, the device compares the magnitude of the difference between the 
//		reference value and the current acceleration with THRESH_INACT. If the difference is less 
//		than the value in THRESH_INACT for the time in TIME_INACT, the device is considered inactive 
//		and the inactivity interrupt is triggered.
//
// ACT_x Enable Bits and INACT_x Enable Bits
//		A setting of 1 enables x-, y-, or z-axis participation in detecting activity or inactivity. 
//		A setting of 0 excludes the selected axis from participation. If all axes are excluded, the 
//		function is disabled.

#define ACT_INACT_CTL_ACT_AC			0b10000000
#define ACT_INACT_CTL_ACT_X				0b01000000
#define ACT_INACT_CTL_ACT_Y				0b00100000
#define ACT_INACT_CTL_ACT_Z				0b00010000
#define ACT_INACT_CTL_INACT_AC			0b00001000
#define ACT_INACT_CTL_INACT_X			0b00000100
#define ACT_INACT_CTL_INACT_Y			0b00000010
#define ACT_INACT_CTL_INACT_Z			0b00000001

#define ACT_INACT_CTL_DEFAULT			0b00000000

//================================================================================================ 


//================================================================================================ 
// THRESH_TAP (1Dh)
// 		The THRESH_TAP register is eight bits and holds the threshold value for tap interrupts. 
//		The data format is unsigned, so the magnitude of the tap event is compared with the value 
//		in THRESH_TAP. The scale factor is 62.5 mg/LSB (that is, 0xFF = +16 g). A value of 0 may 
//		result in undesirable behavior if tap/ double tap interrupts are enabled.
//
// DUR (21h)
//		The DUR register is eight bits and contains an unsigned time value representing the 
//		maximum time that an event must be above the THRESH_TAP threshold to qualify as a tap 
//		event. The scale factor is 625 μs/LSB. A value of 0 disables the tap/double tap functions.
//
// LATENT (22h)
//		The latent register is eight bits and contains an unsigned time value representing the 
//		wait time from the detection of a tap event to the start of the time window (defined by 
//		the window register) during which a possible second tap event can be detected. The scale 
//		factor is 1.25 ms/LSB. A value of 0 disables the double tap function.
//
// WINDOW (23h)
//		The window register is eight bits and contains an unsigned time value representing the 
//		amount of time after the expiration of the latency time (determined by the latent register) 
//		during which a second valid tap can begin. The scale factor is 1.25 ms/LSB. A value of 0 
// 		disables the double tap function.
//
// THRESH_ACT (24h)
//		The THRESH_ACT register is eight bits and holds the threshold value for detecting activity. 
//		The data format is unsigned, so the magnitude of the activity event is compared with the 
//		value in the THRESH_ACT register. The scale factor is 62.5 mg/LSB. A value of 0 may result 
//		in undesirable behavior if the activity interrupt is enabled.
//
// THRESH_INACT (25h)
//		The THRESH_INACT register is eight bits and holds the threshold value for detecting 
//		inactivity. The data format is unsigned, so the magnitude of the inactivity event is 
//		compared with the value in the THRESH_INACT register. The scale factor is 62.5 mg/LSB. 
//		A value of 0 mg may result in undesirable behavior if the inactivity interrupt is enabled.
//
// TIME_INACT (26h)
//		The TIME_INACT register is eight bits and contains an unsigned time value representing 
//		the amount of time that acceleration must be less than the value in the THRESH_INACT 
//		register for inactivity to be declared. The scale factor is 1 sec/LSB. Unlike the other 
//		interrupt functions, which use unfiltered data (see the Threshold section), the 
//		inactivity function uses filtered output data. At least one output sample must be 
//		generated for the inactivity interrupt to be triggered. This results in the function 
//		appearing unresponsive if the TIME_INACT register is set to a value less than the time 
//		constant of the output data rate. A value of 0 results in an interrupt when the output 
//		data is less than the value in the THRESH_INACT register.
//
// THRESH_FF (28h)
// 		The THRESH_FF register is eight bits and holds the threshold value, in unsigned format, 
//		for free-fall detection. The root-sum-square (RSS) value of all axes is calculated and 
//		compared with the value in THRESH_FF to determine if a free-fall event occurred. The 
//		scale factor is 62.5 mg/LSB. Note that a value of 0 mg may result in undesirable behavior 
//		if the free-fall interrupt is enabled. Values between 300 mg and 600 mg (0x05 to 0x09) 
//		are recommended.
//
// TIME_FF (29h)
// 		The TIME_FF register is eight bits and stores an unsigned time value representing the 
//		minimum time that the RSS value of all axes must be less than THRESH_FF to generate a 
//		free-fall interrupt. The scale factor is 5 ms/LSB. A value of 0 may result in undesirable 
//		behavior if the free-fall interrupt is enabled. Values between 100 ms and 350 ms 
//		(0x14 to 0x46) are recommended.
//
//================================================================================================ 


class CADXL345: public CSensor
{
	public:

		// Construction and initialisation
		CADXL345(int32_t nSensorID);
		virtual ~CADXL345();
		virtual void begin(uint8_t nSDOPinNum, uint8_t nSDOPinState);

		// Device settings
		void setDataRate(e_data_rate eDataRate)
		{
			writeReg(BW_RATE, (uint8_t)eDataRate);
		};
		void setPowerCtrl(bool bLink, bool bAutoSleep, bool bMeasure, bool bSleep, e_sleep_data_rate eSleepDataRate);
		void setDataFormat(bool bSelfTest, bool bSPI3Wire, bool bIntInvert, bool bFullRes, bool bJustify, e_scale eScale);
		void setFIFOCtrl(e_fifo_mode eFIFOMode, bool bTrigger, uint8_t nFIFOSize);

		// Device status
		bool getFIFOStatus(uint8_t& nFIFOValues);
		
		bool getActivityTapStatus(uint8_t nBitFlags);
		bool getActivityStatusX()
		{
			return getActivityTapStatus(ACT_TAP_STATUS_ACT_X);
		};
		bool getActivityStatusY()
		{
			return getActivityTapStatus(ACT_TAP_STATUS_ACT_Y);
		};
		bool getActivityStatusZ()
		{
			return getActivityTapStatus(ACT_TAP_STATUS_ACT_Z);
		};
		bool getTapStatusX()
		{
			return getActivityTapStatus(ACT_TAP_STATUS_TAP_X);
		};
		bool getTapStatusY()
		{
			return getActivityTapStatus(ACT_TAP_STATUS_TAP_Y);
		};
		bool getTapStatusZ()
		{
			return getActivityTapStatus(ACT_TAP_STATUS_TAP_Z);
		};
		bool getSleepStatus()
		{
			return getActivityTapStatus(ACT_TAP_STATUS_ASLEEP);
		};

		// Interrupt settings
		
		void setIntEnable(bool bDataReady, bool bSingleTap, bool bDoubleTap, bool bFreeFall, 
							bool bActivity, bool bInactivity, bool bWaterMark, bool bOverrun);
							
		void setIntMap(bool bDataReadyInt1, bool bSingleTapInt1, bool bDoubleTapInt1, bool bFreeFallInt1, 
						bool bActivityInt1, bool bInactivityInt1, bool bWaterMarkInt1, bool bOverrunInt1);

		void setTapInterrupt(void (* pSingleTapISR)(), uint8_t nArduinoSingleTapIntNo, void (* pDoubleTapISR)(), uint8_t nArduinoDoubleTapIntNo, double nTapThreshold = 4.0 * 9.8, 
								double dTapDuration = 30.0, double nTapLatency = 80.0, double dTapWindow = 318.0, bool bX = true, bool bY = true, bool bZ = true);
								
		void setSingleTapInterrupt(void (* pISR)(), uint8_t nArduinoIntNo, bool bINT2 = false, double dTapThreshold = 4.0 * 9.8, double dTapDuration = 30.0, 
									bool bX = true, bool bY = true, bool bZ = true);

		void setFreeFallInterrupt(void (* pSingleTapISR)(), uint8_t nArduinoIntNo, bool bInt2 = false, double dThreshold = 600.0, double dTime = 100.0);
		
		// Tap settings
		void setTapAxes(bool bDCDisable, bool bXEnable, bool bYEnable, bool bZEnable);
		
		void setTap(double dThreshold_gs, double dDuration_ms, double nLatency_ms, double nWindow_ms);

		// Free fall settings
		void setFreeFall(double dThreshold = 600.0 /*mg (300mg to 600mg recommended)*/, double dTime = 350.0 /*ms (100ms to 300ms recommended)*/);
		
		
		// Activity settings
		void setThresholdActivity(double dThreshold/*g's*/)
		{
			writeReg(THRESH_ACT, (uint8_t)round((dThreshold * 1000) / 62.5 /*mg/LSB*/));
		};
		
		void setThresholdInactivity(double dThreshold/*g's*/)
		{
			writeReg(THRESH_INACT, (uint8_t)round((dThreshold * 1000) / 62.5 /*mg/LSB*/));
		};
		
		void setTimeInactivity(double dTime/*s*/)
		{
			writeReg(THRESH_INACT, (uint8_t)round((dTime * 1000) / 1.0 /*s/LSB*/));
		};
		
		void setActInact(bool bActAC, bool bActX, bool bActY, bool bActZ, bool bInactAC, bool bInactX, bool bInactY, bool bInactZ);
		
	protected:
		
		// Helper functions
		void writeReg(e_reg_addr eRegAddr, uint8_t nVal)
		{
			CSensor::writeReg((uint8_t)eRegAddr, nVal);
		};
		void writeReg(e_reg_addr eRegAddrHigh, e_reg_addr eRegAddrLow, uint16_t nVal)
		{
			CSensor::writeReg((uint8_t)eRegAddrHigh, (uint8_t)eRegAddrLow, nVal);
		};
		uint8_t readReg8(e_reg_addr eRegAddr)
		{
			return CSensor::readReg8((uint8_t)eRegAddr);
		};
		uint16_t readReg16(e_reg_addr eRegAddrHigh, e_reg_addr eRegAddrLow)
		{
			return CSensor::readReg16((uint8_t)eRegAddrHigh, (uint8_t)eRegAddrLow);
		};
};

#endif
