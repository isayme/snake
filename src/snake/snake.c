#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

#include "defs.h"
#include "snake.h"
#include "list.h"

//list_head g_snake_list;
snake_t g_snake;
list_head *plist;

void snake_timer()
{
    static int color = 0;

    printf("\033[%d;%dH", SNAKE_REC_WBASE + 5, SNAKE_REC_HBASE + 5);
    printf("\033[%d;%dm*\033[0m", 30 + color%9, 49);
    color++;
    fflush(stdout);


}

void snake_sig_handler(int sig)
{
    if(sig == SIGINT){
        // clear screen
        printf("\033[2J\033[1;1H");
        printf("snake end\n");
        exit(0);
    }
}

void snake_ch(int x, int y, char ch)
{
    printf("\033[%d;%dH", y, x);
    printf("%c", ch);
    fflush(stdout);
}

void snake_str(int x, int y, char *str)
{
    int i, len;

    len = strlen(str);
    for (i = 0; i < len; i++)
    {
        snake_ch(x + i, y, str[i]);
    }
}

void snake_frame()
{
    int i, j;

    for (i = 0; i < (SNAKE_REC_WIDTH + SNAKE_STAT_WIDTH); i++)
    {
        for (j = 0; j < SNAKE_REC_HEIGHT; j++)
        {
            snake_ch(SNAKE_REC_WBASE + i, SNAKE_REC_HBASE + j, '#');
        }
    }

    for (i = 1; i < SNAKE_REC_WIDTH - 1; i++)
    {
        for (j = 1; j < SNAKE_REC_HEIGHT - 1; j++)
        {
            snake_ch(SNAKE_REC_WBASE + i, SNAKE_REC_HBASE + j, ' ');
        }
    }

    for (i = 0; i < SNAKE_STAT_WIDTH - 1; i++)
    {
        for (j = 1; j < SNAKE_REC_HEIGHT - 1; j++)
        {
            snake_ch(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + i, SNAKE_REC_HBASE + j, ' ');
        }
    }
}

// show current score
void snake_score(int score)
{
    char buf[SNAKE_STAT_WIDTH];

    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 2, " score");
    sprintf(buf, "   %d", score);
    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 3, buf);
}

// show current level
void snake_level(int level)
{
    char buf[SNAKE_STAT_WIDTH];

    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 5, " level");
    sprintf(buf, "   %d", level);
    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 6, buf);
}

// show operation keys, version and author
void snake_keys()
{
    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 8, "   W  ");
    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 9, " A S D");
    
    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 11, "  v1.0");
    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 12, " iSayme");
}

// get a char form user
int snake_get_ch()
{
    return 0;
}

int snake_settimer(int ms)
{
    struct itimerval tick;

    memset(&tick, 0, sizeof(tick));  
  
    tick.it_value.tv_sec = 0; 
    tick.it_value.tv_usec = ms * 1000; 
    tick.it_interval.tv_sec = 0;  
    tick.it_interval.tv_usec = ms * 1000;  
 
    if (-1 == setitimer(ITIMER_REAL, &tick, NULL))
    {
        return R_ERROR;
    }

    return R_OK;
}

int snake_init(snake_t *snake, int len, int timer)
{
    int i;
    snake_n *pnode;
    list_head *plist;

    signal(SIGALRM,  snake_timer);
    signal(SIGINT, snake_sig_handler);

    INIT_LIST_HEAD(&g_snake.list);

    // clear screen
    printf("\033[2J");

    // hide cursor
    printf("\033[?25l");

    snake_frame();
    snake_score(0);
    snake_level(0);
    snake_keys();

    for (i = 0; i < len; i++)
    {
        pnode = (snake_n*)malloc(sizeof(snake_n));
        if (NULL == pnode)
        {
            continue;
        }
        pnode->x = SNAKE_REC_WBASE + SNAKE_REC_WIDTH / 2 - len / 2 + i;
        pnode->y = SNAKE_REC_HBASE + SNAKE_REC_HEIGHT / 2;
        list_add(&pnode->list, &snake->list);
    }

    list_for_each(plist, &snake->list)
    {
        pnode = list_entry(plist, snake_n, list);
        snake_ch(pnode->x, pnode->y, '*');
    }
    snake_settimer(timer);
}

int main(int argc, char **argv)
{
    int i, j;
    snake_n *node;

    snake_init(&g_snake, SNAKE_DEFAULT_LEN, SNAKE_LEVEL_ONE);

    while(1) sleep(10);

    return 0;
}