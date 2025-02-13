#line 1 "miniproject new.c"
#line 1 "miniproject new.c"





#line 7 "miniproject new.c"
#line 8 "miniproject new.c"
#line 9 "miniproject new.c"
#line 10 "miniproject new.c"
#line 11 "miniproject new.c"
#line 12 "miniproject new.c"
#line 13 "miniproject new.c"
#line 14 "miniproject new.c"
#line 15 "miniproject new.c"

void obtainClothWeight(void);
void wash(int washType);
void rinse(int rinseType);
void dry(void);

void __init(void)
{
    uart_initialize();
    lcd_initialize();
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB7 = 0;
ADCON1 = 0x0F;
}

void main(void)
{
    TRISBbits.TRISB0 = 1; 
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB7 = 0;
    TRISAbits.TRISA1 = 0;
    PORTAbits.RA1  = 0; 
    PORTBbits.RB4  = 0;
    PORTBbits.RB2  = 0;
    PORTBbits.RB3  = 0;

    lcd_clr();
    lcd_puts("Obtain");
    lcd_goto(20);
    lcd_puts("Cloth Weight");
    delay_ms(1000);
    lcd_clr();

    TRISBbits.TRISB5 = 0; 
    TRISBbits.TRISB6 = 0; 
    
    while (1)
    {
        lcd_goto(0);
        lcd_puts("Select Wash Type:");
        lcd_goto(20);
        lcd_puts("1.Low 2.High");

        if (!PORTBbits.RB0 ) 
        {
            lcd_clr();
            lcd_puts("Low Wash");
            delay_ms(300);
            lcd_clr();
            lcd_goto(0);
            lcd_puts("Starting");
            lcd_goto(20);
            lcd_puts("Low Wash");
            
            PORTBbits.RB2  = 1;
            delay_ms(1000);
            PORTBbits.RB2  = 0;

            lcd_clr();
            lcd_puts("Washing");
            PORTBbits.RB4  = 1; 
            PORTAbits.RA1  = 0; 
            
            PORTBbits.RB5  = 1; 
            PORTBbits.RB6  = 0; 
            delay_ms(500); 
            PORTBbits.RB5  = 0; 
            PORTBbits.RB6  = 1;
            delay_ms(500);
            
            PORTBbits.RB7  = 1;
            PORTBbits.RB7  = 0;
            PORTBbits.RB7  = 1;
            delay_ms(300);

            
            PORTBbits.RB4  = 0;

            lcd_clr();
            lcd_puts("Rinse");
            PORTAbits.RA1  = 1; 
            PORTBbits.RB7  = 0;
            
            
            PORTBbits.RB5  = 1; 
            PORTBbits.RB6  = 0; 
            delay_ms(2000); 
            PORTBbits.RB5  = 0; 
            PORTBbits.RB6  = 0; 

            PORTAbits.RA1  = 0; 

            PORTBbits.RB3  = 1;
            lcd_clr();
            lcd_puts("Washing Complete!");
            delay_ms(2000);
            lcd_clr();
            PORTBbits.RB3  = 0;
        }
        else if (!PORTBbits.RB1 ) 
        {
            lcd_clr();
            lcd_puts("High Wash");
            delay_ms(700);

            lcd_clr();
            lcd_goto(0);
            lcd_puts("Starting");
            lcd_goto(20);
            lcd_puts("High Wash");
  
            PORTBbits.RB2  = 1;
            delay_ms(1000);
            PORTBbits.RB2  = 0;

            lcd_clr();
            lcd_puts("Washing");
            PORTBbits.RB4  = 1; 
            PORTAbits.RA1  = 0; 
            
            PORTBbits.RB5  = 0; 
            PORTBbits.RB6  = 1;
            delay_ms(500); 
            PORTBbits.RB5  = 1; 
            PORTBbits.RB6  = 0;
            delay_ms(500);
            PORTBbits.RB5  = 0; 
            PORTBbits.RB6  = 1;
            delay_ms(1000);

            PORTBbits.RB7  = 1;
            PORTBbits.RB7  = 0;
            PORTBbits.RB7  = 1;
            delay_ms(700);

            
            PORTBbits.RB4  = 0;

            lcd_clr();
            lcd_puts("Rinse");
            PORTAbits.RA1  = 1; 
            PORTBbits.RB7  = 0;
            
            
            PORTBbits.RB5  = 1; 
            PORTBbits.RB6  = 0; 
            delay_ms(2000); 
            PORTBbits.RB5  = 0; 
            PORTBbits.RB6  = 0; 
PORTAbits.RA1  = 0; 
PORTBbits.RB3  = 1;
            lcd_clr();
            lcd_puts("Washing Complete!");
            delay_ms(2000);
            lcd_clr();
            PORTBbits.RB3  = 0;
        }
    }
}