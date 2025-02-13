/*
Filename: lcd.h
Date: 25/08/2014
Author: King Diaw
Email: kingdiawehsut@gmail.com
Revision: 1
Depedency: lcd.h
*/

#include "lcd8bit.h"
#include "define.h"

void lcd_initialize(void)
{
	LCD_DATA_TRIS=0b00000000;
	LCD_RS_TRIS = 0;
	LCD_E_TRIS = 0;
	send_control(0x38);		// to configure the LCD for 2-line, 5x7 font and 8-bit operation mode
 	send_control(0x0C); 	// for Display On and Cursor Off
	send_control(0x01);		// to Clear Display screen
	send_control(0x06);		//to increment cursor
	send_control(0x80);		//to set cursor position at first block of the first line of LCD.
}

void send_data(unsigned char data)
{
	LCD_RS=1;		//set rs into data mode 									
	LCD_DATA=data;							
	e_pulse();
	Delay1KTCYx(12);
}

void send_control(unsigned char data)
{
	LCD_RS=0;										//clear rs into config mode 
	LCD_DATA=data;							
	e_pulse();
	Delay1KTCYx(12);
}	


void e_pulse(void)
{
	LCD_E=1;
	Delay1KTCYx(12);
	LCD_E=0;
	Delay1KTCYx(12);
}

void lcd_puts(const rom char *pointer)
{
     while(*pointer){    		// WHILE NOT NULL CHARACTER
    	send_data(*pointer); 
		pointer++; 
		}  
}

void lcd_goto(unsigned char data)		//set the location of the lcd cursor
{										//if the given value is (0-15) the 
 	if(data<16)							//cursor will be at the upper line
	{									//if the given value is (20-35) the 
	 	send_control(0x80+data);			//cursor will be at the lower line
	}									//location of the lcd cursor(2X16):
	else								// -----------------------------------------------------
	{									// | |00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15| |
	 	data=data-20;					// | |20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35| |
		send_control(0xc0+data);		// -----------------------------------------------------	
	}
}

void lcd_clr(void)
{
 	send_control(0x01);
	Delay1KTCYx(12);	
}

