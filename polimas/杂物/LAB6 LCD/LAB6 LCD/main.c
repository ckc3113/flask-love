
#include "fileheader.h"
#include "delay18.h"
#include "define.h"
#include "lcd8bit.h"

#define SW1 PORTBbits.RB0
#define SW2 PORTBbits.RB1
#define SW3 PORTBbits.RB2
#define EN PORTCbits.RC2
#define IN1 PORTBbits.RB5
#define IN2 PORTBbits.RB6
#define WATER_VALVE_INLET PORTBbits.RB7
#define WATER_VALVE_OUTLET PORTBbits.RB4
#define LED_WASHING PORTAbits.RA0
#define LED_RINSING PORTAbits.RA1
#define LED_DRYING PORTCbits.RC0
#define LED_DONE PORTCbits.RC1

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
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB7 = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
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

    lcd_clr();
    lcd_puts("MESIN BASUH");
    lcd_goto(20);
    lcd_puts("");
    delay_ms(500);
    lcd_clr();

    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;

while (1)
    {
        if (!SW3) 
        {
            lcd_goto(0);
            lcd_puts("1.LOW 2.HIGH");
            lcd_goto(20);
            lcd_puts("");

            if (!SW2) 
            {
                lcd_clr();
                lcd_puts("HIGH");
                delay_ms(1000);
                lcd_clr();
                lcd_goto(0);
                lcd_puts("OBTAIN WEIGHT");
                delay_ms(1000);
                obtainClothWeight();
                lcd_clr();
                lcd_puts("WASHING");
                LED_WASHING = 1;
                wash(2);
                LED_WASHING = 0;
                lcd_clr();
                lcd_puts("RINSING");
                LED_RINSING = 1;
                rinse(2);
                LED_RINSING = 0;
                lcd_clr();
                lcd_puts("DRYING");
                LED_DRYING = 1;
                dry();
                LED_DRYING = 0;
                lcd_clr();
                lcd_puts("DONE");
                LED_DONE = 1;
                delay_ms(1000);
                LED_DONE = 0;
                lcd_clr();
                lcd_puts("");
                delay_ms(200);
                lcd_clr();
                lcd_puts("TAKE OUT");
                delay_ms(500);
                lcd_clr();

            }
            else if (!SW1) 
            {
                lcd_clr();
                lcd_puts("LOW");
                delay_ms(1000);
                lcd_clr();
                lcd_goto(0);
                lcd_puts("OBTAIN WEIGHT");
                delay_ms(1000);
                obtainClothWeight();
                lcd_clr();
                lcd_puts("WASHING");
                LED_WASHING = 1;
                wash(1);
                LED_WASHING = 0;
                lcd_clr();
                lcd_puts("RINSING");
                LED_RINSING = 1;
                rinse(1);
                LED_RINSING = 0;
                lcd_clr();
                lcd_puts("DRYING");
                LED_DRYING = 1;
                dry();
                LED_DRYING = 0;
                lcd_clr();
                lcd_puts("DONE");
                LED_DONE = 1;
                delay_ms(1000);
                LED_DONE = 0;
                lcd_clr();
                lcd_puts("");
                delay_ms(200);
                lcd_clr();
                lcd_puts("TAKE OUT");
                delay_ms(500);
                lcd_clr();
            }
        }
    }
}

void obtainClothWeight(void)
{
    // Add your code here to obtain the weight of the clothes
}
void wash(int washType)
{
    int inletTime = washType == 1 ? 3000 : 7000;
    int outletTime = washType == 1 ? 3000 : 7000;
    WATER_VALVE_INLET = 1; 
    delay_ms(inletTime); 
    WATER_VALVE_INLET = 0; 
    IN1 = 1; 
    IN2 = 0;
    delay_ms(5000); 
    IN1 = 0; 
    IN2 = 1;
    delay_ms(5000);
    IN1 = 0; 
    IN2 = 0;
    WATER_VALVE_OUTLET = 1; 
    delay_ms(outletTime);
    WATER_VALVE_OUTLET = 0; 
}

void rinse(int rinseType)
{
    int inletTime = rinseType == 1 ? 3000 : 7000;
    int outletTime = rinseType == 1 ? 3000 : 7000;
    WATER_VALVE_INLET = 1; 
    delay_ms(inletTime); 
    WATER_VALVE_INLET = 0; 
    IN1 = 1; 
    IN2 = 0;
    delay_ms(5000); 
    IN1 = 0; 
    IN2 = 1;
    delay_ms(5000);
    IN1 = 0; 
    IN2 = 0;
    WATER_VALVE_OUTLET = 1; 
    delay_ms(outletTime);
    WATER_VALVE_OUTLET = 0; 
}

void dry(void)
{
    IN1 = 1; 
    IN2 = 0; 
    WATER_VALVE_OUTLET = 1; 
    delay_ms(10000); 
    IN1 = 0; 
    IN2 = 0;
    WATER_VALVE_OUTLET = 0; 
}