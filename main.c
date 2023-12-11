#include "xc.h"
#include "stdint.h"
#include <stdio.h>
#include <p24FJ64GA002.h>
// Use quotes when using your own library, and <> when using a standard library
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

uint16_t rollovers = 0;
uint32_t previous_press = 0;

// FUNCTION DECLARATIONS
int main(void);
void setup(void);
void loop(void);
void I2C_setup(void);
void timer_init(void);
void IC_init(void);
void update_display(void);
void __attribute__ ((interrupt, auto_psv)) _T1Interrupt(void);
void __attribute__ ((interrupt, auto_psv)) _T2Interrupt(void);
void __attribute__ ((interrupt, auto_psv)) _T3Interrupt(void);
void __attribute__ ((interrupt, auto_psv)) _IC1Interrupt(void);

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

void __attribute__ ((interrupt, auto_psv)) _T3Interrupt(void) {
    _T3IF = 0;
    rollovers++;
}

void __attribute__ ((interrupt, auto_psv)) _IC1Interrupt(void) {
    _IC1IF = 0;
    uint32_t current_press = IC1BUF + 62500*rollovers;
    // If the time difference is greater than 20ms, it is not switch bounce
    if (current_press - previous_press > 1250) {
        calibrate_gyro();
    }
}

void update_display(void) {
    static char adStrX[11];
    sprintf(adStrX, "X: %f", get_degX_integer());
    lcd_printStr(adStrX);
    lcd_setCursor(0, 0);
    static char adStrY[11];
    sprintf(adStrY, "X: %f", get_degX_fractional());
    lcd_printStr(adStrY);
    lcd_setCursor(0, 1);
}

void IC_init(void) {
    AD1PCFG |= 0x0040;                       // Set RB4 to digital
    TRISBbits.TRISB4 = 1;                    // Set RB4 input (value 1)
    
    // Setup PPS
    __builtin_write_OSCCONL(OSCCON & 0xBF);  // Unlock PPS
    RPINR7bits.IC1R = 4;
    __builtin_write_OSCCONL(OSCCON | 0x40);  // Re-lock PPS
    
    CNPU1bits.CN1PUE = 1;                    // RB4:CN24 pull up resistor
    
    // Set up the Input Capture
    IC1CONbits.ICTMR = 0;                    // Use TMR3 for events
    IC1CONbits.ICM = 0b001;                  // Turn on capture for every falling edge
    _IC1IF = 0;
    _IC1IE = 1;                              // Enable IC1 Interrupts
}

void timer_init(void) {
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
    
    // TMR3 - used for the IC1 module
    // Period 1 second
    TMR3 = 100;
    T3CON = 0;
    T3CONbits.TCKPS = 0b11;
    PR3 = 62499;
    _T3IF = 0;
    _T3IE = 1;
    
    T1CONbits.TON = 1;
    T2CONbits.TON = 1;
    T3CONbits.TON = 1;
}

void I2C_setup(void) {
    // I2C SETUP
    I2C2CONbits.I2CEN = 0;
    // Set Baud Rate to 400kHz
    I2C2BRG = 37;
    I2C2CONbits.I2CEN = 1;
    // IEC3bits.MI2C2IE = 1;
    IFS3bits.MI2C2IF = 0;
}

void setup(void) {
    CLKDIVbits.RCDIV = 0;  // Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xFFFF;      // Only need digital pins
    
    // Delays are all necessary for the power on timings of the devices.
    I2C_setup();           // Initializes I2C peripherals
    lcd_init();            // Setup the I2C display
    IC_init();             // Input Capture 1 setup
    delay_ms(20);
    setup_mpu6050();       // Setup MPU6050
    delay_ms(20);
    setup_led_board();     // Setup WS2812 LED board
    calibrate_gyro();      // Calibrate the gyro before using
    delay_ms(5);
    
    reset_board();         // Reset the board
    setup_maze();          // Initialize the maze to the display
    write_board();         // Apply the maze to the display
    
    // Last thing we do is start the timers because these
    // will interrupt the setup and power on delays.
    timer_init();
}

void loop(void) {
    /*
     * Nothing needed here in the loop because everything is handled
     * by interrupts to happen at certain times and certain refresh
     * rates. Interrupt timing is EXTREMELY important though. 
    */
}

int main(void) {
    setup();
    while(1) {
        loop();
    }
}