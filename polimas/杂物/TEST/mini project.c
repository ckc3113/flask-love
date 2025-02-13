#include "fileheader.h"
#include "delay18.h"
#include "define.h"
#include "lcd8bit.h"

#define SW1 PORTBbits.RB0
#define SW2 PORTBbits.RB1
#define LED1 PORTAbits.RA1
#define LED2 PORTBbits.RB4
#define LED3 PORTBbits.RB2
#define LED4 PORTBbits.RB3
#define BUZZER PORTBbits.RB7
#define IN1 PORTBbits.RB5
#define IN2 PORTBbits.RB6

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
    LED1 = 0; 
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;

    lcd_clr();
    lcd_puts("Obtain");
    lcd_goto(20);
    lcd_puts("Cloth Weight");
    delay_ms(1000);
    lcd_clr();

    TRISBbits.TRISB5 = 0; // Set IN1 as output
    TRISBbits.TRISB6 = 0; // Set IN2 as output
    
    while (1)
    {
        lcd_goto(0);
        lcd_puts("Select Wash Type:");
        lcd_goto(20);
        lcd_puts("1.Low 2.High");

        if (!SW1) 
        {
            lcd_clr();
            lcd_puts("Low Wash");
            delay_ms(300);
            lcd_clr();
            lcd_goto(0);
            lcd_puts("Starting");
            lcd_goto(20);
            lcd_puts("Low Wash");
            
            LED3 = 1;
            delay_ms(1000);
            LED3 = 0;

            lcd_clr();
            lcd_puts("Washing");
            LED2 = 1; // Turn on LED2 for washing
            LED1 = 0; // Turn off LED1 for washing
            
            IN1 = 1; 
            IN2 = 0; 
            delay_ms(500); 
            IN1 = 0; 
            IN2 = 1;
            delay_ms(500);
            
            BUZZER = 1;
            BUZZER = 0;
            BUZZER = 1;
            delay_ms(300);

            // Turn off LED2 after washing
            LED2 = 0;

            lcd_clr();
            lcd_puts("Rinse");
            LED1 = 1; // Turn on LED1 for rinse
            BUZZER = 0;
            
            // Activate motor for rinse
            IN1 = 1; 
            IN2 = 0; 
            delay_ms(2000); // Motor runs for rinse
            IN1 = 0; 
            IN2 = 0; // Turn off motor after rinse

            LED1 = 0; // Turn off LED1 after rinse

            LED4 = 1;
            lcd_clr();
            lcd_puts("Washing Complete!");
            delay_ms(2000);
            lcd_clr();
            LED4 = 0;
        }
        else if (!SW2) // Button 2 pressed (Intense Wash)
        {
            lcd_clr();
            lcd_puts("High Wash");
            delay_ms(700);

            lcd_clr();
            lcd_goto(0);
            lcd_puts("Starting");
            lcd_goto(20);
            lcd_puts("High Wash");
  
            LED3 = 1;
            delay_ms(1000);
            LED3 = 0;

            lcd_clr();
            lcd_puts("Washing");
            LED2 = 1; // Turn on LED2 for washing
            LED1 = 0; // Turn off LED1 for washing
            
            IN1 = 0; 
            IN2 = 1;
            delay_ms(500); 
            IN1 = 1; 
            IN2 = 0;
            delay_ms(500);
            IN1 = 0; 
            IN2 = 1;
            delay_ms(1000);

            BUZZER = 1;
            BUZZER = 0;
            BUZZER = 1;
            delay_ms(700);

            // Turn off LED2 after washing
            LED2 = 0;

            lcd_clr();
            lcd_puts("Rinse");
            LED1 = 1; // Turn on LED1 for rinse
            BUZZER = 0;
            
            // Activate motor for rinse
            IN1 = 1; 
            IN2 = 0; 
            delay_ms(2000); // Motor runs for rinse
            IN1 = 0; 
            IN2 = 0; // Turn off motor after rinse
LED1 = 0; // Turn off LED1 after rinse
LED4 = 1;
            lcd_clr();
            lcd_puts("Washing Complete!");
            delay_ms(2000);
            lcd_clr();
            LED4 = 0;
        }
    }
}