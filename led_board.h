/* 
 * File:   led_board.h
 * Author: jossi006
 *
 * Created on November 27, 2023, 8:53 PM
 */

#ifndef LED_BOARD_H
#define	LED_BOARD_H

#ifdef	__cplusplus
extern "C" {
#endif

void setup_led_board(void);
void write_color(uint8_t r, uint8_t g, uint8_t b);
void write_solid_color(uint8_t g, uint8_t r, uint8_t b);
void write_board();
void reset_board();
void set_individual_color(uint8_t row, uint8_t col, uint8_t g, uint8_t r, uint8_t b);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_BOARD_H */

