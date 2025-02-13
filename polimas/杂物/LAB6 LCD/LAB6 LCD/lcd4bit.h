/*
Filename: lcd.h
Date: 25/08/2014
Author: King Diaw
Email: kingdiawehsut@gmail.com
Revision: 1
*/

void e_pulse(void);
void send_data(unsigned char data);
void send_control(unsigned char data);
void lcd_initialize(void);
void lcd_puts(const rom char *pointer);
void lcd_goto(unsigned char data);
void lcd_clr(void);
