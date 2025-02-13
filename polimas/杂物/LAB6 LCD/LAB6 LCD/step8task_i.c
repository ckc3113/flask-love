#include "fileheader.h"
#include "delay18.h"

void __init(void)
{
uart_initialize();
lcd_initialize();	
}
		
void main(void)
{
lcd_goto(0);
lcd_puts("Hai My Name is");
lcd_goto(20);
lcd_puts("E.S.King Diaw");
while(1);
}

	