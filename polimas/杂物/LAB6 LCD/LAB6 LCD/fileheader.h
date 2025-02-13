#include <p18cxxx.h>	//include the PIC18F model of PIC header file
//#include <p18f4550.h>
#include <delays.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <usart.h>
#include "usart_head.c"
#include "lcd8bit.c"
//#include "lcd4bit.c"

/*******************************************************************************
* DEVICE CONFIGURATION WORDS                                                   *
*******************************************************************************/
#if defined (__18F4520) 		//if this test file is compiled for PIC18F4520
	//Configuration bits for PIC18F4520
	#pragma config OSC = HS		// Maximum Fosc is 40MHz, so if 20MHz external crystal is used, need to select HS
	//#pragma config OSC = HSPLL	// Maximum Fosc is 40MHz, so HSPLL is select, please change external crystal to 10MHz
	//#pragma config OSC = INTIO67	// Using internal 8MHz Oscillator, RA6 and RA7 as normal IO pin
	#pragma config FCMEN = ON 	// Fail-Safe Clock Monitor Enable bit
	#pragma config IESO = ON		// Internal/External Oscillator Switchover Enable bit
	#pragma config PWRT = ON		// Power Up Timer Enable bit 
	#pragma config BOREN = OFF 	// Brown-out Reset Enable bits
	#pragma config WDT = OFF 		// Watch-Dog Timer Enable bit
	#pragma config CCP2MX = PORTC	//CCP2 at RC1
	#pragma config PBADEN = ON	// PORTB analog pin as analog input after Reset	
	#pragma config LPT1OSC = OFF	// Timer 1 configured for high power operation 
	#pragma config MCLRE = ON		// MCLR pin enable, RE3 input disabled
	#pragma config STVREN = ON	//Stack overflow will cause Reset
	#pragma config LVP = OFF 		// Low voltage programming disable
	#pragma config XINST = OFF		// Instruction set extension and Indexed Addressing mode enabled 
	#pragma config DEBUG = OFF	// Debug disable, RB6 and RB7 as normal IO

#elif defined (__18F4550) 	//if this test file is compiled for PIC18F4550
	//Configuration bits for PIC18F4550
	//#pragma config FOSC = HS		// Maximum Fosc is 48MHz, so if 20MHz external crystal is used, need to select HS
	#pragma config FOSC = HSPLL_HS		// Maximum Fosc is 48MHz, so HSPLL is select, 20MHz can be used, but need to select correct CPUDIV
	//#pragma config FOSC = INTOSC_HS	// Using internal Oscillator, HS use for USB
	#pragma config PLLDIV = 5		//20MHz divide by 5 to obtain 4MHz to enter PLL and become 96MHz
	#pragma config CPUDIV = OSC1_PLL2	//Primary Oscillator source is 96MHz/2
	#pragma config USBDIV = 2		//USB peripheral clock source from 96MHz / 2
	#pragma config FCMEN = ON 	// Fail-Safe Clock Monitor Enable bit
	#pragma config IESO = ON		// Internal/External Oscillator Switchover Enable bit
	#pragma config PWRT = ON		// Power Up Timer Enable bit 
	#pragma config BOR = OFF 	// Brown-out Reset Enable bits
	#pragma config WDT = OFF 		// Watch-Dog Timer Enable bit
	#pragma config VREGEN = ON	//Enable USB Voltage Regulator
	#pragma config CCP2MX = ON	//CCP2 at RC1
	#pragma config PBADEN = OFF	// PORTB analog pin as analog input after Reset	
	#pragma config LPT1OSC = OFF	// Timer 1 configured for high power operation 
	#pragma config MCLRE = ON		// MCLR pin enable, RE3 input disabled
	#pragma config STVREN = ON	//Stack overflow will cause Reset
	#pragma config LVP = OFF 		// Low voltage programming disable
	#pragma config XINST = OFF		// Instruction set extension and Indexed Addressing mode enabled 			
	#pragma config DEBUG = OFF	// Debug disable, RB6 and RB7 as normal IO
	#pragma config WRTB = ON		//Boot Block Write Protect Enable
#endif


union{
	unsigned char BYTE;
	struct{
		unsigned b0: 1;
		unsigned b1: 1;
		unsigned b2: 1;	
		unsigned b3: 1;	
		unsigned nib: 4;	
	}bits;
}FLAGs;

