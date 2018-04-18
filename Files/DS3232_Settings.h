#ifndef DS3232_SETTINGS_H_INCLUDED
#define DS3232_SETTINGS_H_INCLUDED
/*
||
||  Filename:	 		DS3232_Settings.h
||  Title: 			    DS3232 Driver Settings
||  Author: 			Efthymios Koktsidis
||	Email:				efthymios.ks@gmail.com
||  Compiler:		 	AVR-GCC
||	Description:
||	Settings for the DS3232 RTC.
||
*/

//----- Configuration -------------//
//Specify whether error checking while transceiving data via I2C should be done.
//No error checking:	0
//Error checking:		Any other value
#define RTC_Error_Checking		1

//Match auxiliary functions to your defined I2C functions
//Setup TWI peripheral at 400KHz
#define __I2C_Setup()			TWI_Setup()
//Transmit START signal
#define __I2C_Start()			TWI_BeginTransmission()
//Transmit STOP signal
#define __I2C_Stop()			TWI_EndTransmission()
//Transmit DATA
#define __I2C_Transmit(data)	TWI_Transmit(data)
//Receive DATA + Return ACK
#define __I2C_ReceiveACK()		TWI_ReceiveACK()
//Receive DATA + Return NACK
#define __I2C_ReceiveNACK()		TWI_ReceiveNACK()
//Return TWI Status Register (If you do not need error checking, leave it as is or delete it)
#define __I2C_Status()			TWI_Status()
//---------------------------------//
#endif