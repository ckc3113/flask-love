#include <xc.h>
#include <pic18f4550.h>

// Configuration Bits
#pragma config FOSC = HS // HS oscillator
#pragma config PLLDIV = 5 // 20 MHz input
#pragma config CPUDIV = OSC1_PLL2 // 48 MHz PLL Src
#pragma config FCMEN = OFF // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF // Oscillator Switchover mode disabled
#pragma config PWRT = OFF // Power-up Timer disabled
#pragma config BOR = OFF // Brown-out Reset disabled
#pragma config BORV = 3 // Brown-out Reset Voltage 2.0V
#pragma config VREGEN = OFF // USB Voltage Regulator disabled
#pragma config WDT = OFF // Watchdog Timer disabled
#pragma config WDTPS = 32768 // Watchdog Timer Postscale 1:32768
#pragma config MCLRE = ON // MCLR pin enabled; RE3 input pin disabled
#pragma config LPT1OSC = OFF // Timer1 configured for higher power operation
#pragma config PBADEN = OFF // PORTB<4:0> pins are configured as digital I/O on Reset
#pragma config CCP2MX = PORTC // CCP2 input/output is multiplexed with RC1
#pragma config LVP = OFF // Single-Supply ICSP disabled
#pragma config XINST = OFF // Extended Instruction Set disabled
#pragma config STVREN = ON // Stack full/underflow will cause Reset
#pragma config CP0 = OFF // Block 0 (000800-003FFFh) not code-protected
#pragma config CP1 = OFF // Block 1 (004000-007FFFh) not code-protected
#pragma config CP2 = OFF // Block 2 (008000-00BFFFh) not code-protected
#pragma config CP3 = OFF // Block 3 (00C000-00FFFFh) not code-protected
#pragma config CPB = OFF // Boot block (000000-0007FFh) not code-protected
#pragma config CPD = OFF // Data EEPROM not code-protected
#pragma config WRT0 = OFF // Block 0 not write-protected
#pragma config WRT1 = OFF // Block 1 not write-protected
#pragma config WRT2 = OFF // Block 2 not write-protected
#pragma config WRT3 = OFF // Block 3 not write-protected
#pragma config WRTC = OFF // Configuration registers not write-protected
#pragma config WRTB = OFF // Boot block not write-protected
#pragma config WRTD = OFF // Data EEPROM not write-protected
#pragma config EBTR0 = OFF // Block 0 not protected from table reads executed in other blocks
#pragma config EBTR1 = OFF // Block 1 not protected from table reads executed in other blocks
#pragma config EBTR2 = OFF // Block 2 not protected from table reads executed in other blocks
#pragma config EBTR3 = OFF // Block 3 not protected from table reads executed in other blocks
#pragma config EBTRB = OFF // Boot block not protected from table reads executed in other blocks

// Define Input Pins
#define WATER_LEVEL_PIN RB0
#define TEMPERATURE_PIN RB1
#define DETERGENT_LEVEL_PIN RB2
#define LOAD_SIZE_PIN RB3
#define WASH_CYCLE_NORMAL RB4
#define WASH_CYCLE_DELICATE RB5

// Define Output Pins
#define WATER_PUMP RC0
#define AGITATOR RC1
#define SPIN_MOTOR RC2

void main(void) {
    // Initialize Input Pins
    TRISB = 0b00001111; // RB0-RB3 as input
    TRISC = 0b00000000; // All pins of PORTC as output

    while(1) {
        // Read input sensors
        if (WATER_LEVEL_PIN == 0) {
            // Water level is low
            // Alert the user
        }

        if (TEMPERATURE_PIN == 0) {
            // Temperature is low
            // Adjust the wash cycle accordingly
        }

        if (DETERGENT_LEVEL_PIN == 0) {
            // Detergent level is low
            // Alert the user
        }

        if (LOAD_SIZE_PIN == 0) {
            // Load size is small
            // Adjust the wash cycle accordingly
        }

        // Perform wash cycle based on user selection
        if (WASH_CYCLE_NORMAL == 0) {
            // Normal wash cycle
            WATER_PUMP = 1; // Turn on water pump
            AGITATOR = 1; // Turn on agitator
            __delay_ms(5000); // Wash for 5 seconds
            WATER_PUMP = 0; // Turn off water pump
            AGITATOR = 0; // Turn off agitator
            SPIN_MOTOR = 1; // Start spinning
            __delay_ms(3000); // Spin for 3 seconds
            SPIN_MOTOR = 0; // Stop spinning
        } else if (WASH_CYCLE_DELICATE == 0) {
            // Delicate wash cycle
            WATER_PUMP = 1; // Turn on water pump
            __delay_ms(3000); // Wash for 3 seconds
            WATER_PUMP = 0; // Turn off water pump
            SPIN_MOTOR = 1; // Start spinning
            __delay_ms(2000); // Spin for 2 seconds
            SPIN_MOTOR = 0; // Stop spinning
        }
    }
}