/*
 * Author: Ethan Jossi
 * Created: 12/7/2023
 * This file is used to interface with a X by X board of WS2812 RGB LED pixels.
 * The library includes all necessary functions needed to control this board. 
 * IMPORTANT: the functions reset_board() and set_individual_color() do not apply
 * to the board. One must call the write_board() function to actually change the
 * board. This implemenation was used so that a set refresh rate for the board can
 * be set by calling write_board() every T seconds, but the pixels can be changed
 * anytime. Otherwise, chaning a series of X pixels would write to the board X number
 * of times. The CPU will be busy until write_color() is complete.
*/

#include <xc.h>
#include <stdint.h>
#include "led_board_asm.h"
#include "led_board.h"

// ROW, COLUMN indexing is used
// green, red, and blue represent the colors for each pixel
struct led_board
{
    uint8_t green[MATRIX_HEIGHT][MATRIX_WIDTH];
    uint8_t red[MATRIX_HEIGHT][MATRIX_WIDTH];
    uint8_t blue[MATRIX_HEIGHT][MATRIX_WIDTH];
};

struct led_board board;

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
 * Return Type: void
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

/*
 * This function write a solid color to the whole display. Takes
 * in a green, red, and blue value. write_board() function does NOT need to be called
 * Parameters: uint8_t green value, uint8_t red value, uint8_t blue value
 * Return Type: void
*/
void write_solid_color(uint8_t g, uint8_t r, uint8_t b) {
    for (int i = 0; i < (MATRIX_HEIGHT*MATRIX_WIDTH); i++) {
        write_color(g, r, b);
    }
    reset_50();
}

/*
 * This function applies the current colors set in the green, red, and blue matrices
 * To the led board by sending out a series of pulses. Typically takes less than 10ms
 * to fully execute. 
 * Parameters: none
 * Returns: void
*/
void write_board() {
    int row, col;
    for (row = 0; row < MATRIX_HEIGHT; row++) {
        for (col = MATRIX_WIDTH-1; col >= 0; col--) {
            write_color(board.green[row][col], board.red[row][col], board.blue[row][col]);
        }
        row++;
        for (col = 0; col < MATRIX_WIDTH; col++) {
            write_color(board.green[row][col], board.red[row][col], board.blue[row][col]);
        }
    }
    reset_50();
}

/*
 * Resets the entire board so that all the pixels are off. Does not call
 * write_board(). Only the green, red, and blue matricies are set to 0x00. 
 * Parameters: None
 * Returns: void
*/
void reset_board() {
    for (uint8_t row = 0; row < MATRIX_HEIGHT; row++) {
        for (uint8_t col = 0; col < MATRIX_WIDTH; col++) {
            board.green[row][col] = 0x00;
            board.red[row][col] = 0x00;
            board.blue[row][col] = 0x00;
        }
    }
}

/*
 * Sets an individual pixel to a color. Does not call write_board().
 * Parameters: uint8_t row, uint8_t col, uint8_t green red and blue values
 * Returns: void
*/
void set_individual_color(uint8_t row, uint8_t col, uint8_t g, uint8_t r, uint8_t b) {
    board.green[row][col] = g;
    board.red[row][col] = r;
    board.blue[row][col] = b;
}