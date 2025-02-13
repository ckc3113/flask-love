#include "fileheader.h"
#include "delay18.h"

void __init(void)
{
uart_initialize();
lcd_initialize();	
}
		
void main(void)
{
lcd_goto(7);send_data('A');
lcd_goto(27);send_data('B');
while(1);
}

	