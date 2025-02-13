#line 1 "miniprojectaqil.c"
#line 1 "miniprojectaqil.c"




#pragma config FOSC = HS 
#pragma config PLLDIV = 5 
#pragma config CPUDIV = OSC1_PLL2 
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = OFF 
#pragma config BOR = OFF 
#pragma config BORV = 3 
#pragma config VREGEN = OFF 
#pragma config WDT = OFF 
#pragma config WDTPS = 32768 
#pragma config MCLRE = ON 
#pragma config LPT1OSC = OFF 
#pragma config PBADEN = OFF 
#pragma config CCP2MX = PORTC 
#pragma config LVP = OFF 
#pragma config XINST = OFF 
#pragma config STVREN = ON 
#pragma config CP0 = OFF 
#pragma config CP1 = OFF 
#pragma config CP2 = OFF 
#pragma config CP3 = OFF 
#pragma config CPB = OFF 
#pragma config CPD = OFF 
#pragma config WRT0 = OFF 
#pragma config WRT1 = OFF 
#pragma config WRT2 = OFF 
#pragma config WRT3 = OFF 
#pragma config WRTC = OFF 
#pragma config WRTB = OFF 
#pragma config WRTD = OFF 
#pragma config EBTR0 = OFF 
#pragma config EBTR1 = OFF 
#pragma config EBTR2 = OFF 
#pragma config EBTR3 = OFF 
#pragma config EBTRB = OFF 


#line 44 "miniprojectaqil.c"
#line 45 "miniprojectaqil.c"
#line 46 "miniprojectaqil.c"
#line 47 "miniprojectaqil.c"
#line 48 "miniprojectaqil.c"
#line 49 "miniprojectaqil.c"


#line 52 "miniprojectaqil.c"
#line 53 "miniprojectaqil.c"
#line 54 "miniprojectaqil.c"

void main(void) {
    
    TRISB = 0b00001111; 
    TRISC = 0b00000000; 

    while(1) {
        
        if (RB0  == 0) {
            
            
        }

        if (RB1  == 0) {
            
            
        }

        if (RB2  == 0) {
            
            
        }

        if (RB3  == 0) {
            
            
        }

        
        if (RB4  == 0) {
            
            RC0  = 1; 
            RC1  = 1; 
            __delay_ms(5000); 
            RC0  = 0; 
            RC1  = 0; 
            RC2  = 1; 
            __delay_ms(3000); 
            RC2  = 0; 
        } else if (RB5  == 0) {
            
            RC0  = 1; 
            __delay_ms(3000); 
            RC0  = 0; 
            RC2  = 1; 
            __delay_ms(2000); 
            RC2  = 0; 
        }
    }
}