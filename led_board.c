/*
 * Author: Ethan Jossi
 * Created: 12/7/2023
*/

#include <xc.h>
#include <stdint.h>
#include "led_board_asm.h"

// ROW, COLUMN indexing is used
uint8_t green[16][16];
uint8_t red[16][16];
uint8_t blue[16][16];

/*
 * Sets up the LED board by setting the data out pin (RA0)
 * to digital output and initializing it to low. 
 * Parameters: void
 * Returns: void
 */
void setup_led_board(void) {
    AD1PCFGbits.PCFG0 = 0; // Sets RA0 to digital
    TRISA &= 0xFFFE; // Sets pin RA0 to output
    LATAbits.LATA0 = 0; // Sets RA0 to low
}

/*
 * Takes in a 24 bit GRB color value. Is used by the write_board and write_solid_color functions
 * to write to the display. Cannot be used to write to an individual
 * LED since it does not output the reset command.
 * Parameters: uint8_t g - green value, uint8_t r - red value, uint8_t b - blue value
 */
void write_color(uint8_t g, uint8_t r, uint8_t b) {
    uint8_t i;
    // Write GREEN
    for (i = 0; i < 8; i++) {
        if ((g & 0x80) == 0x00) { // The first bit is 0
            write_0();
        } else { // The first bit is 1
            write_1();
        }
        g = g << 1; // Go to the next bit
    }
    // Write RED
    for (i = 0; i < 8; i++) {
        if ((r & 0x80) == 0x00) { // The first bit is 0
            write_0();
        } else { // The first bit is 1
            write_1();
        }
        r = r << 1; // Go to the next bit
    }
    // Write BLUE
    for (i = 0; i < 8; i++) {
        if ((b & 0x80) == 0x00) { // The first bit is 0
            write_0();
        } else { // The first bit is 1
            write_1();
        }
        b = b << 1; // Go to the next bit
    }
}


void write_solid_color(uint8_t g, uint8_t r, uint8_t b) {
    for (int i = 0; i < 256; i++) {
        write_color(g, r, b);
    }
    reset_50();
}

void write_board() {
    int row, col; // THIS MUST BE INT!!!
    for (row = 0; row < 16; row++) {
        for (col = 15; col >= 0; col--) {
            write_color(green[row][col], red[row][col], blue[row][col]);
        }
        row++;
        for (col = 0; col < 16; col++) {
            write_color(green[row][col], red[row][col], blue[row][col]);
        }
    }
    reset_50();
}

void reset_board() {
    for (uint8_t row = 0; row < 16; row++) {
        for (uint8_t col = 0; col < 16; col++) {
            green[row][col] = 0x00;
            red[row][col] = 0x00;
            blue[row][col] = 0x00;
        }
    }
}

void set_individual_color(uint8_t row, uint8_t col, uint8_t g, uint8_t r, uint8_t b) {
    green[row][col] = g;
    red[row][col] = r;
    blue[row][col] = b;
}