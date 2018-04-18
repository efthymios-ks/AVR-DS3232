#ifndef DS3232_H_INCLUDED
#define DS3232_H_INCLUDED
/*
||
||  Filename:	 		DS3232.h
||  Title: 			    DS3232 Driver
||  Author: 			Efthymios Koktsidis
||	Email:				efthymios.ks@gmail.com
||  Compiler:		 	AVR-GCC
||	Description:
||	This library can drive a DS3232 RTC.
||
*/

//----- Headers ------------//
#include <inttypes.h>
#include <util/delay.h>

#include "IO_Macros.h"
#include "TWI.h"
#include "DS3232_Settings.h"
//--------------------------//

//----- Auxiliary data ---------------------------//
//RTC I2C Address
#define __RTC_I2C_Address		0x68

//RTC Auxiliary Transfer Buffer
#define __RTC_Buffer_Length		7

//RTC Addresses
#define __RTC_Address_Seconds			0x00
#define __RTC_Address_Minutes			0x01
#define __RTC_Address_Hours				0x02
#define __RTC_Address_Day				0x03
#define __RTC_Address_Date				0x04
#define __RTC_Address_Month				0x05
#define __RTC_Address_Year				0x06
#define __RTC_Address_Alarm1_Seconds	0x07
#define __RTC_Address_Alarm1_Minutes	0x08
#define __RTC_Address_Alarm1_Hour		0x09
#define __RTC_ADDR_Alarm1_DayDate		0x0A
#define __RTC_Address_Alarm2_Minutes	0x0B
#define __RTC_Address_Alarm2_Hours		0x0C
#define __RTC_Address_Alarm2_DayDate	0x0D
#define __RTC_Address_Control			0x0E
#define __RTC_Address_Status			0x0F
#define __RTC_Address_Aging				0x10
#define __RTC_Address_Temperature_MSB	0x11
#define __RTC_Address_Temperature_LSB	0x12

//RTC Internal SRAM
#define __RTC_SRAM_Address_Start	0x14
#define __RTC_SRAM_Size			236

//Alarm Control Bits
#define __RTC_A1M1				7
#define __RTC_A1M2				7
#define __RTC_A1M3				7
#define __RTC_A1M4				7
#define __RTC_A2M2				7
#define __RTC_A2M3				7
#define __RTC_A2M4				7 

//Control Register Bits
#define __RTC_EOSC				7
#define __RTC_BBSQW				6
#define __RTC_CONV				5
#define __RTC_RS2				4
#define __RTC_RS1				3
#define __RTC_INTCN				2
#define __RTC_A2IE				1
#define __RTC_A1IE				0

//Status Register Bits
#define __RTC_OSF				7
#define __RTC_BB32KHZ			6
#define __RTC_CRATE1			5
#define __RTC_CRATE0			4
#define __RTC_EN32KHZ			3
#define __RTC_BSY				2
#define __RTC_A2F				1
#define __RTC_A1F				0

//Other
#define __RTC_HR1224			6	//Hours register 12 or 24 hour mode (24 hour mode==0)
#define __RTC_CENTURY			7	//Century bit in Month register
#define __RTC_DYDT				6	//Day-Date flag bit in alarm Day/Date registers

#if (RTC_Error_Checking != 0)
	enum RTC_Status_t
	{
		RTC_ERROR,
		RTC_OK
	};
#endif

enum WeekDay_t
{
	Monday		= 1,
	Tuesday		= 2,
	Wednesday	= 3,
	Thursday	= 4,
	Friday		= 5,
	Saturday	= 6,
	Sunday		= 7
};

enum Month_t
{
	January		= 1,
	February	= 2,
	March		= 3,
	April		= 4,
	May			= 5,
	June		= 6,
	July		= 7,
	August		= 8,
	September	= 9,
	October		= 10,
	November	= 11,
	December	= 12
};

//Alarm number
enum Alarm_Number_t
{
	Alarm_1				= __RTC_A1IE,
	Alarm_2				= __RTC_A2IE
};

//Alarm masks
enum Alarm_Type_t
{
	Alarm1_Every_Seconds	= 0x0F,
	Alarm1_Match_Seconds	= 0x0E,
	Alarm1_Match_Minutes	= 0x0C,	//Match minutes and seconds
	Alarm1_Match_Hours	=	 0x08,	//Match hours, minutes and seconds
	Alarm1_Match_Date		= 0x00,	//Match date, hours, minutes and seconds
	Alarm1_Match_Day		= 0x10,	//Match day, hours, minutes and seconds
	Alarm2_Every_Minute		= 0x8E,
	Alarm2_Match_Minutes	= 0x8C,	//Match minutes
	Alarm2_Match_Hours		= 0x88,	//Match hours and minutes
	Alarm2_Match_Date		= 0x80,	//Match date, hours and minutes
	Alarm2_Match_Day		= 0x90	//Match day, hours and minutes
};

//Square-wave output frequency (TS2, RS1 bits)
enum SQWAVE_FREQ_t
{
	RTC_SQWAVE_1_HZ			= 0, 
	RTC_SQWAVE_1024_HZ		= (1<<__RTC_RS1), 
	RTC_SQWAVE_4096_HZ		= (1<<__RTC_RS2),
	RTC_SQWAVE_8192_HZ		= (1<<__RTC_RS2) | (1<<__RTC_RS1),
	RTC_SQWAVE_NONE
};

//Temperature convert sample rate
enum TCXO_Sample_Rate_t
{
	RTC_Sample_Rate_64		= 0,
	RTC_Sample_Rate_128		= (1<<__RTC_CRATE0),
	RTC_Sample_Rate_256		= (1<<__RTC_CRATE1),
	RTC_Sample_Rate_512		= (1<<__RTC_CRATE1) | (1<<__RTC_CRATE0)
};

typedef struct
{
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	enum WeekDay_t Day;
	uint8_t Date;
	enum Month_t Month;
	uint16_t Year;
}DateTime_t;
//------------------------------------------------//

//----- Prototypes ------------------------------------------------------------//
void RTC_Setup(void);
#if (RTC_Error_Checking != 0)
	enum RTC_Status_t RTC_Status(void);
#endif

void RTC_WriteByte(const uint8_t Address, uint8_t Data);
uint8_t RTC_ReadByte(const uint8_t Address);

void RTC_WriteBlock(const uint8_t StartAddress, uint8_t *Data, const uint8_t Length);
void RTC_ReadBlock(const uint8_t StartAddress, uint8_t *Data, const uint8_t Length);

void RTC_Set(DateTime_t Value);
DateTime_t RTC_Get(void);

uint8_t RTC_OnBatteryOperate(const uint8_t Enable);
uint8_t RTC_OnBatteryAlarm(const uint8_t Enable);
uint8_t RTC_OnBattery32kHZ(const uint8_t Enable);

void RTC_SetSquareWave(enum SQWAVE_FREQ_t Frequency);
void RTC_AlarmSet(enum Alarm_Type_t Alarm, uint8_t DayDate, uint8_t Hours, uint8_t Minutes, uint8_t Seconds);
uint8_t RTC_AlarmInterrupt(enum Alarm_Number_t Alarm, const uint8_t Enable);
uint8_t RTC_AlarmCheck(enum Alarm_Number_t Alarm);

uint8_t RTC_OscillatorStopped(uint8_t ClearOSF);
uint8_t RTC_32KHZ(const uint8_t Enable);

void RTC_TemperatureSetRate(enum TCXO_Sample_Rate_t Rate);
void RTC_TemperatureForceConvert(void);
double RTC_TemperatureRead(const uint8_t Force);
//-----------------------------------------------------------------------------//
#endif