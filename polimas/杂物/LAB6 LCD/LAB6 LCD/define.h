#define	_XTAL_FREQ		48000000	//if PIC18F4550 is defined, default configuration bits will
									
// UART baud rate
#define UART_BAUD		9600

#define TRUE 1
#define FALSE 0

/* definitions of PINs connected to 2 x 16 LCD*/
#define LCD_DATA	PORTD
#define LCD_DATA_TRIS TRISD
/* --------------------------------------*/
#define LCD_E   	LATAbits.LATA3   	
#define LCD_E_TRIS  TRISAbits.TRISA3

#define LCD_RS   	LATAbits.LATA2   	
#define LCD_RS_TRIS TRISAbits.TRISA2

#define SW1			PORTBbits.RB0
#define	SW2			PORTBbits.RB1

//#define LCD_RW    	PORTDbits.RD1   
//#define TRIS_LCD_RW TRISDbits.TRISD1

