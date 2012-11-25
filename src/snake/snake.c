#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>

#include "defs.h"
#include "snake.h"
#include "list.h"

//list_head g_snake_list;
snake_t g_snake;
snake_n *g_feed;

void snake_timer()
{
    list_head *plist;
    list_head *plist_tmp;
    snake_n *pnode;
    snake_n *pnode1, *pnode2;
    point newpos;

    if ( 0 == list_empty(&g_snake.list))
    {
        
        // get last snake node
        plist = g_snake.list.prev;
        pnode = list_entry(plist, snake_n, list);

        // get first snake node and the after one
        pnode1 = list_entry(g_snake.list.next, snake_n, list);
        pnode2 = list_entry(g_snake.list.next->next, snake_n, list);

        // calc the relative position
        newpos.x = pnode1->pos.x - pnode2->pos.x;
        newpos.y = pnode1->pos.y - pnode2->pos.y;

        // calc the next pos of the snake head from user keydown
        switch (g_snake.dir)
        {
            case SNAKE_DIR_W:
                if (newpos.x)
                {
                    newpos.x = pnode1->pos.x;
                    newpos.y = pnode1->pos.y - 1;
                }
                else
                {
                    newpos.x += pnode1->pos.x;
                    newpos.y += pnode1->pos.y;
                }
                break;
            case SNAKE_DIR_S:
                if (newpos.x)
                {
                    newpos.x = pnode1->pos.x;
                    newpos.y = pnode1->pos.y + 1;
                }
                else
                {
                    newpos.x += pnode1->pos.x;
                    newpos.y += pnode1->pos.y;
                }
                break;
            case SNAKE_DIR_A:
                if (newpos.y)
                {
                    newpos.x = pnode1->pos.x - 1;
                    newpos.y = pnode1->pos.y;
                }
                else
                {
                    newpos.x += pnode1->pos.x;
                    newpos.y += pnode1->pos.y;
                }
                break;
            case SNAKE_DIR_D:
                if (newpos.y)
                {
                    newpos.x = pnode1->pos.x + 1;
                    newpos.y = pnode1->pos.y;
                }
                else
                {
                    newpos.x += pnode1->pos.x;
                    newpos.y += pnode1->pos.y;
                }
                break;
            default:
                newpos.x = pnode1->pos.x + newpos.x;
                newpos.y = pnode1->pos.y + newpos.y;
                break;
        }
        // reset user keydown
        g_snake.dir = SNAKE_DIR_U;

        // if the next snake head position already exist
        list_for_each(plist_tmp, &g_snake.list)
        {
            pnode2 = list_entry(plist_tmp, snake_n, list);
            if (pnode2->pos.x == newpos.x && pnode2->pos.y == newpos.y)
            {
                // stop timer
                snake_settimer(0);

                snake_ch(pnode->pos.x, pnode->pos.y, SNAKE_BODY_CH);
                list_add_tail(plist, &g_snake.list);
                snake_die();
                return;
            }
        }

        // if out of border
        if (newpos.x <= (SNAKE_REC_WBASE) ||
            newpos.x >= (SNAKE_REC_WBASE + SNAKE_REC_WIDTH - 1) ||
            newpos.y <= (SNAKE_REC_HBASE) ||
            newpos.y >= (SNAKE_REC_HBASE + SNAKE_REC_HEIGHT - 1))
        {
            // stop timer
            snake_settimer(0);

            snake_ch(pnode->pos.x, pnode->pos.y, SNAKE_BODY_CH);
            list_add_tail(plist, &g_snake.list);
            snake_die();
            return;
        }

        // if eat a feed
        if (newpos.x == g_feed->pos.x && newpos.y == g_feed->pos.y)
        {
            list_add(&g_feed->list, &g_snake.list);
            while (R_ERROR == snake_random_feed());
            g_snake.score++;
            snake_score(g_snake.score);
            return;
        }

        // delete the tail snake node
        list_delete(plist);
        snake_ch(pnode->pos.x, pnode->pos.y, SNAKE_DEFAULT_CH);

        // renew the tail snake node pos
        pnode->pos.x = newpos.x;
        pnode->pos.y = newpos.y;

        // redraw the snake node
        
        list_add(plist, &g_snake.list);
        snake_ch(pnode->pos.x, pnode->pos.y, SNAKE_BODY_CH);

    }
}

void snake_sig_handler(int sig)
{
    if(sig == SIGINT){
        // clear screen
        printf("\033[2J\033[1;1H");
        snake_echo();
        snake_uninit(&g_snake);
        printf("thanks, snake end\n");
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
            snake_ch(SNAKE_REC_WBASE + i, SNAKE_REC_HBASE + j, SNAKE_FRAME_CH);
        }
    }

    for (i = 1; i < SNAKE_REC_WIDTH - 1; i++)
    {
        for (j = 1; j < SNAKE_REC_HEIGHT - 1; j++)
        {
            snake_ch(SNAKE_REC_WBASE + i, SNAKE_REC_HBASE + j, SNAKE_DEFAULT_CH);
        }
    }

    for (i = 0; i < SNAKE_STAT_WIDTH - 1; i++)
    {
        for (j = 1; j < SNAKE_REC_HEIGHT - 1; j++)
        {
            snake_ch(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + i, SNAKE_REC_HBASE + j, SNAKE_DEFAULT_CH);
        }
    }
}

// show current score
void snake_score(int score)
{
    char buf[SNAKE_STAT_WIDTH];

    sprintf(buf, "  %03d", score);
    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 3, buf);
}

// show current level
void snake_level(int level)
{
    char buf[SNAKE_STAT_WIDTH];

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

void snake_noecho()
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHOFLAGS;
    term.c_cc[VTIME] = 0;
    term.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

}
void snake_echo()
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHOFLAGS;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

}

void snake_die()
{
    /*
    |-----------------|
    | you dead. press |
    | ctrl + c to end |
    |-----------------|
    */
    snake_str(SNAKE_REC_WBASE + 3, SNAKE_REC_HBASE +  SNAKE_REC_HEIGHT / 2 - 2, 
        "|-----------------|");
    snake_str(SNAKE_REC_WBASE + 3, SNAKE_REC_HBASE +  SNAKE_REC_HEIGHT / 2 - 1, 
        "| you dead. press |");
    snake_str(SNAKE_REC_WBASE + 3, SNAKE_REC_HBASE +  SNAKE_REC_HEIGHT / 2 - 0, 
        "| ctrl + c to end |");
    snake_str(SNAKE_REC_WBASE + 3, SNAKE_REC_HBASE +  SNAKE_REC_HEIGHT / 2 + 1, 
        "|-----------------|");

}

int snake_random_feed()
{
    static point pos;
    list_head *plist;
    snake_n *pnode;

    srandom(time(NULL) + pos.x);
    pos.x = SNAKE_REC_WBASE + 1 + (pos.x + random()) % (SNAKE_REC_WIDTH - 2);
    srandom(time(NULL) + pos.y);
    pos.y = SNAKE_REC_HBASE + 1 + (pos.y + random()) % (SNAKE_REC_HEIGHT - 2);

    // if the next snake head position already exist
    list_for_each(plist, &g_snake.list)
    {
        pnode = list_entry(plist, snake_n, list);
        if (pnode->pos.x == pos.x && pnode->pos.y == pos.y)
        {
            return R_ERROR;
        }
    }

    g_feed = (snake_n*)malloc(sizeof(snake_n));
    g_feed->pos.x = pos.x;
    g_feed->pos.y = pos.y;
    snake_ch(pos.x, pos.y, SNAKE_BODY_CH);

    return R_OK;
}
int snake_init(snake_t *snake)
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

    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 2, " score");
    snake_score(g_snake.score);
    snake_str(SNAKE_REC_WBASE + SNAKE_REC_WIDTH + 1, SNAKE_REC_HBASE + 5, " level");
    snake_level(g_snake.level);
    snake_keys();

    for (i = 0; i < g_snake.score; i++)
    {
        pnode = (snake_n*)malloc(sizeof(snake_n));
        if (NULL == pnode)
        {
            continue;
        }
        pnode->pos.x = SNAKE_REC_WBASE + SNAKE_REC_WIDTH / 2 - g_snake.score / 2 
        + i;
        pnode->pos.y = SNAKE_REC_HBASE + SNAKE_REC_HEIGHT / 2;
        list_add(&pnode->list, &snake->list);
    }

    list_for_each(plist, &snake->list)
    {
        pnode = list_entry(plist, snake_n, list);
        snake_ch(pnode->pos.x, pnode->pos.y, SNAKE_BODY_CH);
    }

    snake_noecho();
    while (R_ERROR == snake_random_feed());

    snake_settimer(SNAKE_LEVEL_BASE - SNAKE_LEVEL_DIFF * g_snake.level);

    return R_OK;
}

int snake_uninit(snake_t *snake)
{
    list_head *plist;
    snake_n *pnode;

    if (NULL == snake)
    {
        return R_ERROR;
    }

    list_for_each(plist, &snake->list)
    {
        pnode = list_entry(plist, snake_n, list);
        free(pnode);
    }

    if (NULL != g_feed)
    {
        free(g_feed);
    }
    return R_OK;
}

int main(int argc, char **argv)
{
    int ch;
    int pause = 0;

    g_snake.level = 0;
    g_snake.score = SNAKE_DEFAULT_LEN;

    while (-1 != (ch = getopt(argc, argv, "l:")))
    {
        switch (ch)
        {
            case 'l':
                g_snake.level = atoi(optarg);
                if (g_snake.level < SNAKE_LEVEL_MIX || g_snake.level > SNAKE_LEVEL_MAX)
                {
                    printf("level must between [0-9].\n");
                    return R_ERROR;
                }
                break;
            default:
                printf("not supported argument [%c].\n", ch);
                break;
        }
    }
    
    snake_init(&g_snake);

    while(EOF != (ch = getchar()))
    {
        switch(ch)
        {
            case 'w':
                g_snake.dir = SNAKE_DIR_W;
                break;
            case 's':
                g_snake.dir = SNAKE_DIR_S;
                break;
            case 'a':
                g_snake.dir = SNAKE_DIR_A;
                break;
            case 'd':
                g_snake.dir = SNAKE_DIR_D;
                break;
            case 'p':
                if (0 == pause)
                {
                    pause = 1;
                    snake_settimer(0);
                }
                else
                {
                    pause = 0;
                    snake_settimer(SNAKE_LEVEL_BASE - SNAKE_LEVEL_DIFF * g_snake.level);
                }
                break;
            default:
                break;
        }
    }

    return 0;
}