#include <p18cxxx.h>	//include the PIC18F model of PIC header file
#include "usart_head.h"
#include "define.h"
void uart_initialize(void)
{
	unsigned char dummy = 0;
	TXSTAbits.BRGH = 0;			// Select low speed baud rate, if we are using SPBRGH.
	BAUDCONbits.BRG16 = 1;		//Uses SPBRGH and SPBRG for baudrate generation
	SPBRGH = ( (_XTAL_FREQ / 16 / UART_BAUD) - 1 ) /256;	//calculate and assign the higher byte for SPBRGH
	SPBRG = ((_XTAL_FREQ / 16 / UART_BAUD) - 1)%256 ;	// Configure the baud rate.
	RCSTAbits.SPEN = 1;	// Enable serial port.
	RCSTAbits.CREN = 1;	// Enable reception.
	TXSTAbits.TXEN = 1;	// Enable transmission.
	dummy = RCREG;		// dummy read, to clear the receive buffer
}
