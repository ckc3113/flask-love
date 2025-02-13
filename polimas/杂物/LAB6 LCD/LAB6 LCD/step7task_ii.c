#include "fileheader.h"
#include "delay18.h"

void __init(void)
{
uart_initialize();
lcd_initialize();	
}
		
void main(void)
{
lcd_goto(0);send_data('A');
lcd_goto(35);send_data('B');
while(1);
}

	