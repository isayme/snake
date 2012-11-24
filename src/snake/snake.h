#ifndef __SNAKE_H
#define __SNAKE_H

#include "list.h"

typedef struct _snake_n{
    char x;
    char y;
    list_head list;
}snake_n;

typedef struct _snake
{
    int len;

    list_head list;
}snake_t;

#define SNAKE_DEFAULT_LEN 4

#define SNAKE_REC_WBASE 5
#define SNAKE_REC_HBASE 5
#define SNAKE_REC_WIDTH 20
#define SNAKE_REC_HEIGHT 15
#define SNAKE_STAT_WIDTH 10

// define snake game level macros
#define SNAKE_LEVEL_BASE 450
#define SNAKE_LEVEL_DIFF 50
#define SNAKE_LEVEL_ONE     SNAKE_LEVEL_BASE
#define SNAKE_LEVEL_TWO     (SNAKE_LEVEL_ONE - SNAKE_LEVEL_DIFF)
#define SNAKE_LEVEL_THREE   (SNAKE_LEVEL_TWO - SNAKE_LEVEL_DIFF)
#define SNAKE_LEVEL_FOUR    (SNAKE_LEVEL_THREE - SNAKE_LEVEL_DIFF)
#define SNAKE_LEVEL_FIVE    (SNAKE_LEVEL_FOUR - SNAKE_LEVEL_DIFF)
#define SNAKE_LEVEL_SIX     (SNAKE_LEVEL_FIVE - SNAKE_LEVEL_DIFF)
#define SNAKE_LEVEL_SEVEN   (SNAKE_LEVEL_SIX - SNAKE_LEVEL_DIFF)
#define SNAKE_LEVEL_EIGHT   (SNAKE_LEVEL_SEVEN - SNAKE_LEVEL_DIFF)
#define SNAKE_LEVEL_NINE    (SNAKE_LEVEL_EIGHT - SNAKE_LEVEL_DIFF)

int snake_init(snake_t *snake, int len, int timer);

void snake_ch(int x, int y, char ch);
void snake_str(int x, int y, char *str);
void snake_frame();
int snake_settimer(int ms);
void snake_score(int score);
void snake_level(int level);
void snake_keys();

int snake_get_ch();

void snake_timer();
void snake_sig_handler(int sig);

#endif