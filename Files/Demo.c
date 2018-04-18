#include "DS3232.h"

int main(void)
{
	DateTime_t t;
	
	RTC_Setup();

	t.Second = 55;
	t.Minute = 59;
	t.Hour = 23;
	t.Day = Sunday;
	t.Date = 31;
	t.Month = December;
	t.Year = 2016;
	RTC_Set(t);
	
	while (1 == 1)
	{
		t = RTC_Get();
		if (RTC_Status() == RTC_Ok)
		{
			//Do something
		}
	}

	return 0;
}
