#include <xc.h>
#include "maze.h"
#include "led_board.h"
#include "mpu6050.h"

const int8_t wall[16][16] = { //create matrix for wall so ball doesn't overwrite
{1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //row 0
{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}, //row 1
{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}, //row 2
{1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1}, //row 3
{1, 0, 0, 1, 0, 0, 0, 1, 0 ,0, 1, 1, 1, 1, 0, 1}, //row 4
{1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1}, //row 5
{1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1}, //row 6
{1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, //row 7
{1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, //row 8
{1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1}, //row 9
{1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, //row 10
{1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1}, //row 11
{1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1}, //row 12
{1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1}, //row 13
{1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1}, //row 14
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1}, //row 15
};

void setup_maze(void) {
    uint8_t green[16][16] = {
        {MAZE_COLOR_GREEN, BALL_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                0,                0,                0,                0,                MAZE_COLOR_GREEN, 0,                0,                0,                0,                0,                0,                0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                0,                0,                0,                0,                MAZE_COLOR_GREEN, 0,                0,                0,                0,                0,                0,                0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                MAZE_COLOR_GREEN, 0,                0,                0,                MAZE_COLOR_GREEN, 0,                0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                0,                0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                0,                0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                0,                0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                0,                0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                0,                0,                0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                0,                0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                0,                0,                0,                0,                0,                MAZE_COLOR_GREEN, 0,                0,                0,                0,                0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                0,                0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                0,                0,                0,                0,                MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, 0,                0,                MAZE_COLOR_GREEN, 0,                0,                0,                MAZE_COLOR_GREEN, 0,                0,                0,                0,                0,                0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN},
        {MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, MAZE_COLOR_GREEN, 0,                MAZE_COLOR_GREEN, MAZE_COLOR_GREEN}
    }
    uint8_t red[16][16] = {
        {MAZE_COLOR_RED,   BALL_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                0,                0,                0,                0,                MAZE_COLOR_RED,   0,                0,                0,                0,                0,                0,                0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                0,                0,                0,                0,                MAZE_COLOR_RED,   0,                0,                0,                0,                0,                0,                0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                MAZE_COLOR_RED,   0,                0,                0,                MAZE_COLOR_RED,   0,                0,                MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                0,                0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                0,                0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                0,                0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                0,                0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                0,                0,                0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                0,                0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                0,                0,                0,                0,                0,                MAZE_COLOR_RED,   0,                0,                0,                0,                0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                0,                0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                0,                0,                0,                0,                MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   0,                0,                MAZE_COLOR_RED,   0,                0,                0,                MAZE_COLOR_RED,   0,                0,                0,                0,                0,                0,                MAZE_COLOR_RED,   MAZE_COLOR_RED},
        {MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   MAZE_COLOR_RED,   0,                MAZE_COLOR_RED,   MAZE_COLOR_RED}  
    }
    uint8_t blue[16][16] = {
        {MAZE_COLOR_BLUE,  BALL_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                0,                0,                0,                0,                MAZE_COLOR_BLUE,  0,                0,                0,                0,                0,                0,                0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                0,                0,                0,                0,                MAZE_COLOR_BLUE,  0,                0,                0,                0,                0,                0,                0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                MAZE_COLOR_BLUE,  0,                0,                0,                MAZE_COLOR_BLUE,  0,                0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                0,                0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                0,                0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                0,                0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                0,                0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                0,                0,                0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                0,                0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                0,                0,                0,                0,                0,                MAZE_COLOR_BLUE,  0,                0,                0,                0,                0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                0,                0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                0,                0,                0,                0,                MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  0,                0,                MAZE_COLOR_BLUE,  0,                0,                0,                MAZE_COLOR_BLUE,  0,                0,                0,                0,                0,                0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE},
        {MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE,  0,                MAZE_COLOR_BLUE,  MAZE_COLOR_BLUE}
    }
    struct led_board colors;
    colors.green = green;
    colors.red = red;
    colors.blue = blue;
    set_board(colors);
}

void update_ball(void) {
    static char ball_row = 0;
    static char ball_col = 1;
    
    if(ball_row == 15 && ball_col == 13){
        setup_congrats();
        return; 
    }
    
    float x = get_degX();
    float y = get_degY();
    set_individual_color(ball_row, ball_col, 0x00, 0x00, 0x00);
    if (x > 20.0 && ball_col < 15 && !wall[ball_row][ball_col+1]) {
        ball_col++;
    } else if (x < -20.0 && ball_col > 0 && !wall[ball_row][ball_col-1]) {
        ball_col--;
    }
    if (y > 20.0 && ball_row > 0 && !wall[ball_row-1][ball_col]) {
        ball_row--;
    } else if (y < -20.0 && ball_row < 15 && !wall[ball_row+1][ball_col]) {
        ball_row++;
    }
    set_individual_color(ball_row, ball_col, BALL_COLOR_GREEN, BALL_COLOR_RED, BALL_COLOR_BLUE);
}

void setup_congrats(void) {
    //congratulations message for successfully solving the maze
    //something like if ball =[16][15] then this part runs
    reset_board();
    set_individual_color(0, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(0, 5, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(5, 5, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(1, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(2, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(8, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(9, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(11, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(2, 1, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(5, 1, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 1, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(0, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(1, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(2, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(3, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(4, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(5, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(11, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 2, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 3, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(0, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(1, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(2, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(3, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(4, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(5, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(8, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(9, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(11, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 4, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(0, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(1, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(2, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(3, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(4, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(5, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(8, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(9, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(11, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 6, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(8, 7, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 7, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(0, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(1, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(2, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(3, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(4, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(5, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(8, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(9, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(11, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 8, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(4, 9, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(5, 9, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(0, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(1, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(2, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(3, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(4, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(5, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(8, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(9, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(11, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 10, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(9, 11, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(0, 0, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 11, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(9, 12, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 12, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(11, 12, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 12, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(7, 14, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(8, 14, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(9, 14, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(10, 14, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
    set_individual_color(12, 14, MAZE_COLOR_GREEN, MAZE_COLOR_RED, MAZE_COLOR_BLUE);
}