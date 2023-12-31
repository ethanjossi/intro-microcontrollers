/* 
 * File:   led_board.h
 * Author: jossi006
 *
 * Created on November 27, 2023, 8:53 PM
 */

#ifndef LED_BOARD_H
#define	LED_BOARD_H

// Constants
#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 16

// Color format definition using structs
// ROW, COLUMN indexing is used
// green, red, and blue represent the colors for each pixel
struct led_board
{
    uint8_t green[MATRIX_HEIGHT][MATRIX_WIDTH];
    uint8_t red[MATRIX_HEIGHT][MATRIX_WIDTH];
    uint8_t blue[MATRIX_HEIGHT][MATRIX_WIDTH];
};

// Function Definitions
void setup_led_board(void);
void write_color(uint8_t r, uint8_t g, uint8_t b);
void write_solid_color(uint8_t g, uint8_t r, uint8_t b);
void write_board();
void reset_board();
void set_individual_color(uint8_t row, uint8_t col, uint8_t g, uint8_t r, uint8_t b);
void set_board(struct led_board colors);

#endif	/* LED_BOARD_H */
