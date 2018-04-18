# AVR DS3232 Library  

---

**Description:**  
A library to control DS3232-based RTC using AVR microcontrollers. This library may work with DS3231 as well. Give it a try.
+ Functions include:  
  + Setting up.  
  + Checking status.  
  + Writing single byte.  
  + Reading single byte.  
  + Setting time.  
  + Reading time.  
  + Setting and reading any operation-related flag.
  + Setting alarms in any mode.
  + Reading temperature.  
   _Read the bottom of "DS3232.h" to see all functions available._  
+ A buffer stored in RAM is used to complete the time read and write operations (7 bytes).  
+ The RTC is interfaced using the TWI (I2C) protocol at 400KHz. You can use your own or my included TWI library.  
+ There is an error checking option while while tranceiving data via TWI protocol. If you are not using my library, you have to either define the codes or disable error checking.
+ My custom I/O macros are required and are included.  

---  

**Memory usage:**  
+ Program: 0 bytes  
+ Data: 8 bytes (default)
  + RTC buffer: 7 bytes (default)
  + RTC status: 1 byte  
  
**Compiler:**  
AVR-GCC  
  
**Optimization Level:**  
Optimize (-O1)  
  
---
  
![picture alt](https://raw.githubusercontent.com/efthymios-ks/AVR-DS3232-Library/master/Demonstration%20(1).PNG)
![picture alt](https://raw.githubusercontent.com/efthymios-ks/AVR-DS3232-Library/master/Demonstration%20(2).PNG)
  
--- 
