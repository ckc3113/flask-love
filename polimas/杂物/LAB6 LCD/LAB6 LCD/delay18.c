#include <p18cxxx.h>	//include the PIC18F model of PIC header file
#include <delays.h>
#include "delay18.h"
#include "define.h"

void delay_10us(unsigned int ui_10microsecond)
{
	for( ; ui_10microsecond > 0; ui_10microsecond --)
	{
		#if (_XTAL_FREQ == 20000000)
		Delay10TCYx(5);	//50 instruction cycle with 20MHz oscillator is 10us
		#elif (_XTAL_FREQ == 48000000)
		Delay10TCYx(12);	//120 instruction cycle with 48MHz oscillator is 10us
		#endif
	}	
}


void delay_ms(unsigned int ui_millisecond)
{
	for( ; ui_millisecond > 0; ui_millisecond --)
	{
		#if (_XTAL_FREQ == 20000000)
		Delay1KTCYx(5);	//5K instruction cycle with 20MHz oscillator is 1ms
		#elif (_XTAL_FREQ == 48000000)
		Delay1KTCYx(12);	//12K instruction cycle with 48MHz oscillator is 1ms
		#endif
	}		
}	


