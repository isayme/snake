#ifndef __SNAKE_H
#define __SNAKE_H

#include "list.h"

typedef struct _point{
    int x;
    int y;
}point;

typedef struct _snake_n{
    point pos;
    list_head list;
}snake_n;

typedef struct _snake
{
    int level;
    int score;
#define SNAKE_DIR_U 0
#define SNAKE_DIR_W 1
#define SNAKE_DIR_S 2
#define SNAKE_DIR_A 3
#define SNAKE_DIR_D 4
    unsigned char dir;
    list_head list;
}snake_t;

#define SNAKE_DEFAULT_LEN 4

#define SNAKE_REC_WBASE 5
#define SNAKE_REC_HBASE 5
#define SNAKE_REC_WIDTH 25
#define SNAKE_REC_HEIGHT 15
#define SNAKE_STAT_WIDTH 10

// define snake game level macros
#define SNAKE_LEVEL_MIX 0
#define SNAKE_LEVEL_MAX 9

#define SNAKE_LEVEL_BASE 450
#define SNAKE_LEVEL_DIFF 50

// define snake chars
#define SNAKE_BODY_CH '*'
#define SNAKE_FRAME_CH '#'
#define SNAKE_DEFAULT_CH ' '

#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL | ICANON)

// snake init
int snake_init(snake_t *snake);
int snake_uninit(snake_t *snake);
// print a char
void snake_ch(int x, int y, char ch);
// print string
void snake_str(int x, int y, char *str);

// draw the frame of the game
void snake_frame();
// set timer for move frequency
int snake_settimer(int ms);
// renew score
void snake_score(int score);
// renew level
void snake_level(int level);
// show help keys and version info
void snake_keys();
// set stdin no echo
void snake_noecho();
// restore stdin mode
void snake_echo();

// random a feed
int snake_random_feed();

// die info
void snake_die();

// timer process function
void snake_timer();
// signal process function
void snake_sig_handler(int sig);

#endif