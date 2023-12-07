/* 
 * File:   maze.h
 * Author: jossi006
 *
 * Created on December 2, 2023, 4:17 PM
 */

#ifndef MAZE_H
#define	MAZE_H

#include "led_board.h"
#include "mpu6050.h"

#define MAZE_COLOR_GREEN 0x00
#define MAZE_COLOR_RED 0x00
#define MAZE_COLOR_BLUE 0x10
#define BALL_COLOR_GREEN 0x00
#define BALL_COLOR_RED 0xA0
#define BALL_COLOR_BLUE 0xA0

void setup_maze(void);
void update_ball(void);
void setup_congrats(void);


#endif	/* MAZE_H */

