/*
Filename: lcd.h
Date: 25/08/2014
Author: King Diaw
Email: kingdiawehsut@gmail.com
Revision: 1
Depedency: lcd.h
*/

#include "lcd4bit.h"

void lcd_initialize(void)
{
	LCD_E_TRIS=0;
	LCD_RS_TRIS=0;
	LCD_DATA_TRIS=0b00000000;
	LCD_DATA_TRIS &= 0x0f;		// Set data line as output
   	LCD_DATA &= 0x0f;			// clear previous higher nibble on data line	
   	LCD_DATA |= (0b00100000 & 0xf0);		// put higher nibble	
	e_pulse();
	send_control(0b00101000);			// (2)2nd Function Set - 4-bit, 2 lines, 5X7 (entry 6 T6)
 	send_control(0b00001100); 			// (3)Display & cursor on/off (entry 4 T6)
	send_control(0b00000110);			// (4)Entry mode - inc addr, no shift (entry 3 T6)
}

void send_data(unsigned char data)
{
										
	LCD_DATA=data&0xF0;
	LCD_RS=1;							//set rs into data mode 
	e_pulse();
	Delay1KTCYx(12);

	LCD_DATA=(data<<4)&0xF0;
	LCD_RS=1;
	e_pulse();
	Delay1KTCYx(12);
}

void send_control(unsigned char data)
{
	LCD_RS=0;										//clear rs into config mode 
	LCD_DATA=(data&0xF0);
	e_pulse();
	Delay1KTCYx(12);
	LCD_RS=0;
	LCD_DATA=((data<<4)&0xF0);
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
	 	data=data-16;					// | |16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31| |
		send_control(0xc0+data);		// -----------------------------------------------------	
	}
}

void lcd_clr(void)
{
 	send_control(0x01);
	Delay1KTCYx(12);	
}

