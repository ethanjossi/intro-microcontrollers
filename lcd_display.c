/*
 * Author: Ethan Jossi
 * Created: 12/7/2023
 * Generic C file to interface with the ST7032 LCD display.
 * Contains the necessary code to the setup the ST7032, write a string to the
 * display, write a character, and adjust the positioning of the text. Scrolling
 * effects possible with additional code. 
*/

#include <xc.h>
#include "lcd_display.h"

/* This function takes in a command and a control byte
 * to send to the device over I2C. Each time it sends a
 * start and stop byte. It also sends a fixed address
 * and write that are hardcoded into the function
 * Return: void
 * Parameters: 
 * unsigned char contr - specific control byte for the display, a series of predefined control byte are given in the header file
 * unsigned char command - the command to issue the device, a series of predefined commands are given in the header file
 */
void lcd_cmd(unsigned char contr, unsigned char command) {
    IFS3bits.MI2C2IF = 0;
    // Start bit
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    // Address byte
    I2C2TRN = ADDRESS_ST7032_WRITE;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    // Control Byte
    I2C2TRN = contr;
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    // Data Byte
    I2C2TRN = command;
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    // Stop Bit
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1);
}

/* This function is designed to set up the LCD display
 * using a series of lcd_cmd() calls. It uses a CB_INIT
 * control byte and other specific commands to setup the
 * device properly. The contrast can be adjusted by 
 * uncommenting several lines. It also waits approximately
 * 200ms for the power supply to steady out
 * Return: void
 * Parameters: void
 */
void lcd_init(void) {
    delay_ms(50);
    lcd_cmd(CB_INIT, 0b00111000);    // Function set, normal instruction mode
    lcd_cmd(CB_INIT, 0b00111001);    // Function set, extended instruction mode
    lcd_cmd(CB_INIT, 0b00010100);    // Interval OSC
//    lcd_cmd(CB_INIT, 0b01110000 | (CONTRAST & 0b00001111));    // Contrast C3-C0
//    lcd_cmd(CB_INIT, 0b01011100 | (CONTRAST >> 4));    // Ion, Bon, C5-C4
    lcd_cmd(CB_INIT, 0b01110000); // contrast C3-C0
    lcd_cmd(CB_INIT, 0b01011110); // Contrast C4-C5
    lcd_cmd(CB_INIT, 0b01101100);    // Follower Control
    delay_ms(200);
    lcd_cmd(CB_INIT, 0b00111000);    // Function set, normal instruction mode
    lcd_cmd(CB_INIT, 0b00001100);    // Display on
    lcd_cmd(CB_INIT, 0b00000001);    // Clear Display
    delay_ms(2);
}

/* This prints a string to the display with only one start
 * and stop bit. It accomplishes this by setting the CO bit to
 * 1 so the LCD knows another data byte is coming. It writes the
 * string only on the top line of the display, and the string may
 * rolloff so other configuration may be needed depending on the
 * length of the string.
 * Return: void
 * Parameters:
 * const char s[] - a string to print to the display. The char array must end in the special end character '\0'
 */
void lcd_printStr(const char s[]) {
    IFS3bits.MI2C2IF = 0;
    // Start bit
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    // Address byte
    I2C2TRN = ADDRESS_ST7032_WRITE;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    int index = 0;
    while (s[index+1] != '\0') {
        // Control Byte
        I2C2TRN = CB_REPEATED_CHAR;
        while (IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        // Data Byte
        I2C2TRN = s[index];
        while (IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        index++;
    }
    // Last two bytes with CO = 0
    // Control Byte
    I2C2TRN = CB_SINGLE_CHAR;
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    // Data Byte
    I2C2TRN = s[index];
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    // Stop Bit
    I2C2CONbits.PEN = 1;
    while (I2C2CONbits.PEN == 1);
}

/* Delays a certain number of ms
 * Return: void
 * Parameters:
 * long int ms - the number of milliseconds to delay
 */
void delay_ms(long ms) {
    for (long i = 0; i < ms; i++) {
        asm("repeat #15993");
        asm("nop");
    }
}

/* Sets up the I2C protocol. Enables the 
 * baud rate to 100kHz and clears and enables interrupts.
 * Return: void
 * Parameters: void
 */
void setup_I2c(void) {
    // Set Baud Rate to 100kHz
    I2C2CONbits.I2CEN = 0;
    I2C2BRG = 157;
    I2C2CONbits.I2CEN = 1;
    // Clear and enable interrupt flag
//    IEC3bits.MI2C2IE = 1;
    IFS3bits.MI2C2IF = 0;
}

/* Sets the cursor to a specific place on the LCD display
 * This essentially allows certain portions of the text to 
 * show up on the display depending on the DRAM array filled
 * on the LCD device.
 * Return: void
 * Parameters:
 * unsigned char col - the column to set the cursor to
 * unsigned char row - the row to set the cursor to
 */
void lcd_setCursor(unsigned char col, unsigned char row) {
    unsigned char command = col+0x40*row+0x80;
    lcd_cmd(CB_CURSOR, command);
}

/* Prints a single char to the LCD display. Used for testing
 * primarily. Uses the lcd_cmd() function.
 * Return type: void
 * Parameters: 
 * char myChar - the single char to print to the display
 */
void lcd_printChar(char myChar) {
    lcd_cmd(CB_SINGLE_CHAR, myChar);
}