#include "xc.h"
#include "stdint.h"
#include <stdio.h>
#include <p24FJ64GA002.h>
// Use quotes when using your own library, and <> when using a standard library
//#include <stdlib.h>
#include "mpu6050.h"
#include "lcd_display.h"
#include "led_board.h"
#include "maze.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


// FUNCTION DECLARATIONS
int main(void);
void setup(void);
void loop(void);
void I2C_setup(void);
void interrupt_setup(void);
void update_display(void);
void __attribute__ ((interrupt, auto_psv)) _T1Interrupt(void);
void __attribute__ ((interrupt, auto_psv)) _T2Interrupt(void);

// FUNCTION DEFINITIONS

void __attribute__ ((interrupt, auto_psv)) _T1Interrupt(void) {
    _T1IF = 0;
    update_degrees();
    update_display();
}

void __attribute__ ((interrupt, auto_psv)) _T2Interrupt(void) {
    _T2IF = 0;
    update_ball();
    write_board();
}

void update_display(void) {
    static char adStrX[11];
    sprintf(adStrX, "X: %f", get_degX());
    lcd_printStr(adStrX);
    lcd_setCursor(0, 0);
    static char adStrY[11];
    sprintf(adStrY, "Y: %f", get_degY());
    lcd_printStr(adStrY);
    lcd_setCursor(0, 1);
}

void interrupt_setup(void) {
    // TMR1 - interrupt for gyroscope
    // Sets up for a 25ms period
    TMR1 = 3999;
    T1CON = 0;
    T1CONbits.TCKPS = 0b10;
    PR1 = 6249;
    _T1IF = 0;
    _T1IE = 1;
    // TMR2 - interrupt LED board and game calculation
    // Sets up for a 50 ms period
    TMR2 = 0;
    T2CON = 0;
    T2CONbits.TCKPS = 0b11;
    PR2 = 18749;
    _T2IF = 0;
    _T2IE = 1;
    
    T1CONbits.TON = 1;
    T2CONbits.TON = 1;
}

void I2C_setup(void) {
    // I2C SETUP
    I2C2CONbits.I2CEN = 0;
    I2C2BRG = 37; // Set Baud Rate to 400kHz
    I2C2CONbits.I2CEN = 1;
//    IEC3bits.MI2C2IE = 1;
    IFS3bits.MI2C2IF = 0;
}

void setup(void) {
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xFFFF;
    // Testing LED
//    TRISBbits.TRISB6 = 0;
//    LATBbits.LATB6 = 1;
    
    I2C_setup(); // Initializes I2C peripheral
    lcd_init(); // Setup the I2C display
    delay_ms(20);
    setup_mpu6050(); // Setup MPU6050
    delay_ms(20);
    setup_led_board();
    calibrate_gyro();
    delay_ms(5);
    
    reset_board();
    setup_maze();
    write_board();
    
    // Last thing we do is start the interrupts because these
    // Will interrupt the setup and power on delays in the earlier
    // part of the setup
    interrupt_setup();
}

void loop(void) {
}

int main(void) {
    setup();
    while(1) {
        loop();
    }
}