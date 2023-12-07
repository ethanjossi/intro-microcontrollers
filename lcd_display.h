/* 
 * File:   lcd_display.h
 * Author: jossi006
 *
 * Created on November 21, 2023, 10:30 AM
 */

#ifndef LCD_DISPLAY_H
#define	LCD_DISPLAY_H

#include <xc.h>

// Constants needed for ST7032
#define ADDRESS_ST7032_WRITE 0b01111100
#define CB_INIT 0b00000000
#define CB_SINGLE_CHAR 0b01000000
#define CB_REPEATED_CHAR 0b11000000
#define DB_SHIFT_LEFT 0b00011000
#define DB_SHIFT_RIGHT 0b00011100
#define CB_SHIFT 0b00000000
#define CB_CURSOR 0b00000000
#define CONTRAST 0b100000

// Function Declarations
void setup_I2C(void);
void delay_ms(long ms);
void lcd_cmd(unsigned char contr, unsigned char command);
void lcd_init(void);
void lcd_setCursor(unsigned char x, unsigned char y);
void lcd_printChar(char myChar);
void lcd_printStr(const char s[]);

#endif	/* LCD_DISPLAY_H */

