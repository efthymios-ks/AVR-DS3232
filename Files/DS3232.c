#include "DS3232.h"

//----- Auxiliary data ------//
#define __I2C_SLA_W(Address)	(Address<<1)
#define __I2C_SLA_R(Address)		((Address<<1) | (1<<0))
#define __RTC_tmToY2k(Year)		(Year + 2000)
#define __RTC_y2kToTm(Year)		(Year - 2000)

uint8_t __RTC_Buffer[__RTC_Buffer_Length];
#if (RTC_Error_Checking != 0)
	enum RTC_Status_t __RTC_Status;
#endif

//---------------------------//

//----- Prototypes ----------------------------//
static uint8_t FlagWrite(const uint8_t Address, const uint8_t Flag, const uint8_t Value);
static uint8_t FlagRead(const uint8_t Address, const uint8_t Flag);
static uint8_t Dec2bcd(const uint8_t Value);
static uint8_t Bcd2dec(const uint8_t Value);
//---------------------------------------------//

//----- Functions -------------//
//Setup RTC. 
void RTC_Setup(void)
{
	__I2C_Setup();
	#if (RTC_Error_Checking != 0)
		__RTC_Status = RTC_OK;
	#endif

}

//Get RTC status. 
#if (RTC_Error_Checking != 0)
	enum RTC_Status_t RTC_Status(void)
	{
		return (__RTC_Status);
	}
#endif

//Write byte to RTC. 
void RTC_WriteByte(const uint8_t Address, uint8_t Data)
{
	RTC_WriteBlock(Address, &Data, 1);
}

//Read byte from RTC. 
uint8_t RTC_ReadByte(const uint8_t Address)
{
	uint8_t data;

	RTC_ReadBlock(Address, &data, 1);

	return data;
}

//Write data block to RTC.
void RTC_WriteBlock(const uint8_t StartAddress, uint8_t *Data, const uint8_t Length)
{	
	uint8_t i;
	#if (RTC_Error_Checking != 0)
		uint8_t status;
	#endif

	#if (RTC_Error_Checking != 0)
		do
		{
	#endif
			//Transmit START signal
			__I2C_Start();
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MT_START_TRANSMITTED) && (status != MT_REP_START_TRANSMITTED))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif

			//Transmit SLA+W
			__I2C_Transmit(__I2C_SLA_W(__RTC_I2C_Address));
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MT_SLA_W_TRANSMITTED_ACK) && (status != MT_SLA_W_TRANSMITTED_NACK))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif

			//Transmit write address
			__I2C_Transmit(StartAddress);
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MT_DATA_TRANSMITTED_ACK) && (status != MT_DATA_TRANSMITTED_NACK))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif

			//Transmit DATA
			for (i = 0 ; i < Length ; i++)
			{
				__I2C_Transmit(Data[i]);
				#if (RTC_Error_Checking != 0)
					status = __I2C_Status();
					if ((status != MT_DATA_TRANSMITTED_ACK) && (status != MT_DATA_TRANSMITTED_NACK))
					{
						__RTC_Status = RTC_ERROR;
						break;
					}
				#endif
			}
		
			#if (RTC_Error_Checking != 0)
				__RTC_Status = RTC_OK;
			#endif

	#if (RTC_Error_Checking != 0)
		}
		while (0);
	#endif

	//Transmit STOP signal
	__I2C_Stop();
}

//Read data block from RTC. 
void RTC_ReadBlock(const uint8_t StartAddress, uint8_t *Data, const uint8_t Length)
{
	uint8_t i;
	#if (RTC_Error_Checking != 0)
		uint8_t status;
	#endif

	#if (RTC_Error_Checking != 0)
		do
		{
	#endif
			//Transmit START signal
			__I2C_Start();
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MR_START_TRANSMITTED) && (status != MR_REP_START_TRANSMITTED))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif
		
			//Transmit RTC address (write) (SLA+W)
			__I2C_Transmit(__I2C_SLA_W(__RTC_I2C_Address));
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MT_SLA_W_TRANSMITTED_ACK) && (status != MT_SLA_W_TRANSMITTED_NACK))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif

			//Transmit read address
			__I2C_Transmit(StartAddress);
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MT_DATA_TRANSMITTED_ACK) && (status != MT_DATA_TRANSMITTED_NACK))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif

			//Transmit START signal
			__I2C_Start();
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MR_START_TRANSMITTED) && (status != MR_REP_START_TRANSMITTED))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif
				
			//Transmit RTC address (read) (SLA+R)
			__I2C_Transmit(__I2C_SLA_R(__RTC_I2C_Address));
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MR_SLA_R_TRANSMITTED_ACK) && (status != MR_SLA_R_TRANSMITTED_NACK))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif

			//Receive DATA
			for (i = 0 ; i < (Length - 1) ; i++)
			{
				Data[i] = __I2C_ReceiveACK();
				#if (RTC_Error_Checking != 0)
					status = __I2C_Status();
					if ((status != MR_DATA_RECEIVED_ACK) && (status != MR_DATA_RECEIVED_NACK))
					{
						__RTC_Status = RTC_ERROR;
						break;
					}
				#endif
			}
			Data[i] = __I2C_ReceiveNACK();
			#if (RTC_Error_Checking != 0)
				status = __I2C_Status();
				if ((status != MR_DATA_RECEIVED_ACK) && (status != MR_DATA_RECEIVED_NACK))
				{
					__RTC_Status = RTC_ERROR;
					break;
				}
			#endif
				

			#if (RTC_Error_Checking != 0)
				__RTC_Status = RTC_OK;
			#endif

	#if (RTC_Error_Checking != 0)
		}
		while (0);
	#endif

	//Transmit STOP signal
	__I2C_Stop();
}

//Set datetime. 
void RTC_Set(DateTime_t Value)
{
	//Prepare transmission buffer
	__RTC_Buffer[0] = Dec2bcd(Value.Second);
	__RTC_Buffer[1] = Dec2bcd(Value.Minute);
	__RTC_Buffer[2] = Dec2bcd(Value.Hour);
	__RTC_Buffer[3] = Dec2bcd(Value.Day);
	__RTC_Buffer[4] = Dec2bcd(Value.Date);
	__RTC_Buffer[5] = Dec2bcd(Value.Month);
	__RTC_Buffer[6] = Dec2bcd(__RTC_y2kToTm(Value.Year));

	//Transmit buffer
	RTC_WriteBlock(__RTC_Address_Seconds, __RTC_Buffer, __RTC_Buffer_Length);
}

//Get datetime. 
DateTime_t RTC_Get(void)
{
	DateTime_t value;

	//Read DATA into buffer
	RTC_ReadBlock(__RTC_Address_Seconds, __RTC_Buffer, __RTC_Buffer_Length);
		
	//Build time_t variable
	value.Second = Bcd2dec(__RTC_Buffer[0]);
	value.Minute = Bcd2dec(__RTC_Buffer[1]);
	value.Hour = Bcd2dec(__RTC_Buffer[2] & 0x3F);
	value.Day = Bcd2dec(__RTC_Buffer[3]);
	value.Date = Bcd2dec(__RTC_Buffer[4]);
	value.Month = Bcd2dec(__RTC_Buffer[5]);
	value.Year = __RTC_tmToY2k(Bcd2dec(__RTC_Buffer[6]));

	return value;
}

//Operate on battery. 
uint8_t RTC_OnBatteryOperate(const uint8_t Enable)
{
	return (FlagWrite(__RTC_Address_Control, __RTC_EOSC, Enable));
}

//Alarm on battery. 
uint8_t RTC_OnBatteryAlarm(const uint8_t Enable)
{
	return (FlagWrite(__RTC_Address_Control, __RTC_BBSQW, Enable));
}

//32KHz wave on battery. 
uint8_t RTC_OnBattery32kHZ(const uint8_t Enable)
{
	return (FlagWrite(__RTC_Address_Status, __RTC_BB32KHZ, Enable));
}

//Set square wave frequency. 
void RTC_SetSquareWave(enum SQWAVE_FREQ_t Frequency)
{
	//Read control register
	uint8_t reg = RTC_ReadByte(__RTC_Address_Control);

	//Clear RS bits
	BitClear(reg, __RTC_RS1);
	BitClear(reg, __RTC_RS2);
	
	if (Frequency != RTC_SQWAVE_NONE)
	{
		//Enable Square Wave on INT/SQW pin
		BitClear(reg, __RTC_INTCN);
		//Set corresponding bits
		reg |= Frequency;
	}

	//Write control register
	RTC_WriteByte(__RTC_Address_Control, reg);
}

//Set alarm.
void RTC_AlarmSet(enum Alarm_Type_t Alarm, uint8_t DayDate, uint8_t Hours, uint8_t Minutes, uint8_t Seconds)
{
	uint8_t addr;

	//Convert all decimal values to their corresponding BCD values
	Seconds = Dec2bcd(Seconds);
	Minutes = Dec2bcd(Minutes);
	Hours = Dec2bcd(Hours);
	DayDate = Dec2bcd(DayDate);	//Either Day or Date can be used. Not both.
	
	//Set Alarm Control Bits based on Alarm Mask Bits
	//Seconds
	if (BitCheck(Alarm, 0))
		BitSet(Seconds, __RTC_A1M1);
	//Minutes
	if (BitCheck(Alarm, 1))
		BitSet(Minutes, __RTC_A1M2);
	//Hours
	if (BitCheck(Alarm, 2))
		BitSet(Hours, __RTC_A1M3);
	//Day-Date value
	if (BitCheck(Alarm, 3))
		BitSet(DayDate, __RTC_A1M4);
	//Day-Date flag
	if (BitCheck(Alarm, 4))
		BitSet(DayDate, __RTC_DYDT);
	
	//Send data (on consecutive address)
	//Alarm 2
	if (BitCheck(Alarm, 7))
	{
		//Alarm 2 does not have seconds register
		addr = __RTC_Address_Alarm2_Minutes;
	}
	//Alarm 1
	else
	{
		//Alarm 1 does have a seconds register
		addr = __RTC_Address_Alarm1_Seconds;
		RTC_WriteByte(addr++, Seconds);
	}
	RTC_WriteByte(addr++, Minutes);
	RTC_WriteByte(addr++, Hours);
	RTC_WriteByte(addr++, DayDate);
}

//Set alarm interrupt pin 
uint8_t RTC_AlarmInterrupt(enum Alarm_Number_t Alarm, const uint8_t Enable)
{
	//Enable INTCN
	FlagWrite(__RTC_Address_Control, __RTC_INTCN, Enable);
		
	//Enable alarm
	return (FlagWrite(__RTC_Address_Control, Alarm, Enable));
}

//Check if alarm rung. 
uint8_t RTC_AlarmCheck(enum Alarm_Number_t Alarm)
{
	return (FlagWrite(__RTC_Address_Status, Alarm, False));
}

//Clear OSF and return if oscillator stopped. 
uint8_t RTC_OscillatorStopped(uint8_t ClearOSF)
{
	if (ClearOSF)
		ClearOSF = 0xFF;

	return (FlagWrite(__RTC_Address_Status, __RTC_OSF, !ClearOSF));
}

//Enable or disable 32KHz wave. 
uint8_t RTC_32KHZ(const uint8_t Enable)
{
	return (FlagWrite(__RTC_Address_Status, __RTC_EN32KHZ, Enable));
}

void RTC_TemperatureSetRate(enum TCXO_Sample_Rate_t Rate)
{
	//Read control register
	uint8_t reg = RTC_ReadByte(__RTC_Address_Status);

	//Clear RS bits
	BitClear(reg, __RTC_CRATE0);
	BitClear(reg, __RTC_CRATE1);
	
	//Set corresponding bits
	reg |= Rate;

	//Write control register
	RTC_WriteByte(__RTC_Address_Status, reg);
}

void RTC_TemperatureForceConvert(void)
{
	//Wait if busy
	while (FlagRead(__RTC_Address_Status, __RTC_BSY));
	
	FlagWrite(__RTC_Address_Control, __RTC_CONV, True);

	//Wait for conversion to be completed
	while (FlagRead(__RTC_Address_Control, __RTC_CONV));
}

double RTC_TemperatureRead(const uint8_t Force)
{
	union
	{
		uint16_t i;
		uint8_t b[2];
	}temp;
	
	//Force conversion
	if (Force)
		RTC_TemperatureForceConvert();

	//Read temperature
	RTC_ReadBlock(__RTC_Address_Temperature_MSB, temp.b, 2);

	//Build temperature
	
	#if (RTC_Error_Checking != 0)
		if (__RTC_Status == RTC_OK)
		{
	#endif
			//Swap high with low byte (the previous read received MSB first, so we need to swap)
			temp.i = (temp.i>>8) | (temp.i<<8);
			//Shift it 6 bits right to align it correctly
			temp.i >>= 6;
	#if (RTC_Error_Checking != 0)
		}
	#endif

	//RTC registers hold {temperature * 4}
	return (temp.i / 4.0);
}

static uint8_t FlagWrite(const uint8_t addr, const uint8_t flag, const uint8_t enable)
{
	uint8_t reg, r;
	
	//Read register
	reg = RTC_ReadByte(addr);

	//Isolate bit
	r = BitCheck(reg, flag);
	BitClear(reg, flag);

	//Save bit
	if (enable)
		BitSet(reg, flag);
	RTC_WriteByte(addr, reg);

	//Return bit
	return r;
}

static uint8_t FlagRead(const uint8_t addr, const uint8_t flag)
{
	return (BitCheck(RTC_ReadByte(addr), flag));	
}

static uint8_t Dec2bcd(const uint8_t n)
{
	//Works for 0 -> 98
	return (n + ((n * 26)>>8) * 6);
}

static uint8_t Bcd2dec(const uint8_t n)
{
	//Works for 0 -> 98
	return (n - 6 * (n / 16));
}
//-----------------------------//
