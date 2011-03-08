#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "start.h"

#define BLOCK_SIZE 40
#define MIDDLE_X 290
#define MAX_OFFSET 6

typedef struct{
    int line[4];
    int total_line;
    int total_score;
} score;

char assigned[16][19] = {{0,0}};

const char shape[11][4] = {{4,5,9,13}, {10,12,13,14}, {4,8,12,13}, {8,9,10,12}, 
                           {5,9,8,12}, {8,9,13,14}, {4,8,9,13}, {9,10,12,13},
                           {8,9,12,13}, {0,4,8,12}, {12,13,14,15}};
extern u8_t *buf24;
extern u8_t *scale_buf;

extern unsigned int msg[4];
u32_t *block_blue, *block_green, *block_red;
u32_t *back_ground;
u32_t *block_color[3];

int x_ori = 0, y_ori = 0;
int x_now = 0, y_now = 0;
int draw_flag = 0;
int x_offset = 0;
int y_offset = 0;
int color = 0;
int next_flag = 0;
int block_num = 0;
int clear_flag = 0;
int left_flag = 0;
int right_flag = 0;
int change_flag = 0;
int handle_flag = 0;
int verify_flag = 0;
int t_score = 0;
int block_next = 0;
int color_next = 0;
int put_flag = 0;

score scr;

pthread_mutex_t pmutex = PTHREAD_MUTEX_INITIALIZER;

u32_t save[768][1024];
u32_t eros_save[768][1024];
u32_t score_save[100][100];
u32_t next_save[200][200];

void fb_save(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
        memcpy(save[i], fb_inf.fbmem + ((starty + i) * fb_inf.w + startx) , width*4 );
}
void fb_recover(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
        memcpy(fb_inf.fbmem + ((starty + i) * fb_inf.w + startx), save[i], width*4 );
}
void eros_s(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
        memcpy(eros_save[i], fb_inf.fbmem + ((starty + i) * fb_inf.w + startx), width*4 );
}
void eros_r(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
        memcpy(fb_inf.fbmem + ((starty + i) * fb_inf.w + startx) , eros_save[i], width*4 );
}
void score_s(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
        memcpy(score_save[i], fb_inf.fbmem + ((starty + i) * fb_inf.w + startx) , width*4 );
}
void score_r(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
        memcpy(fb_inf.fbmem + ((starty + i) * fb_inf.w + startx) , score_save[i], width*4);
}
void next_s(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
        memcpy(next_save[i], fb_inf.fbmem + ((starty + i) * fb_inf.w + startx) , width*4 );
}
void next_r(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
        memcpy(fb_inf.fbmem + ((starty + i) * fb_inf.w + startx) , next_save[i], width*4 );
}
void decode_block(fb_info fb_inf)
{
	fb_info jpeg_inf;
    fb_inf.w = BLOCK_SIZE;
    fb_inf.h = BLOCK_SIZE;
	
	buf24 = decode_jpeg("green.jpg", &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	block_green = rgb24to32(scale_buf, fb_inf);
    free(buf24);
    free(scale_buf);

	buf24 = decode_jpeg("red.jpg", &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	block_red = rgb24to32(scale_buf, fb_inf);
    free(buf24);
    free(scale_buf);

	buf24 = decode_jpeg("blue.jpg", &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	block_blue = rgb24to32(scale_buf, fb_inf);
    free(buf24);
    free(scale_buf);

    fb_inf.w = BLOCK_SIZE * 16;
    fb_inf.h = BLOCK_SIZE;
	buf24 = decode_jpeg("back.jpg", &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	back_ground = rgb24to32(scale_buf, fb_inf);
    free(buf24);
    free(scale_buf);

    block_color[0] = block_blue;
    block_color[1] = block_green;
    block_color[2] = block_red;
    srand((unsigned)time(NULL));
}
void draw_block(fb_info fb_inf, int x, int y)
{
    int i, j;
    for(i = 0; i < BLOCK_SIZE; ++i)
        for (j = 0; j < BLOCK_SIZE; ++j)
            fb_pixel(fb_inf, j + x, i + y, block_color[color][j + i * BLOCK_SIZE]);
}
void draw_shape(fb_info fb_inf, int num, int x, int y)
{
    int i, temp;
    fb_save(fb_inf, BLOCK_SIZE * 4, BLOCK_SIZE * 4, x, y);
    x_ori = x; y_ori = y;
    pthread_mutex_lock(&pmutex);
    for (i = 0; i < 4; i++) 
    {
        temp = shape[num][i];
        draw_block(fb_inf, x + temp % 4 * BLOCK_SIZE, y + (temp) / 4 * BLOCK_SIZE);
    }
    pthread_mutex_unlock(&pmutex);
    clear_flag = 0;
}
void draw_next(fb_info fb_inf, int num, int x, int y)
{
    int i, temp;
    pthread_mutex_lock(&pmutex);
    for (i = 0; i < 4; i++) 
    {
        temp = shape[num][i];
        draw_block(fb_inf, x + temp % 4 * BLOCK_SIZE, y + (temp) / 4 * BLOCK_SIZE);
    }
    pthread_mutex_unlock(&pmutex);
}
int predicate()
{
    int i, j, temp = 0, cont = 0;
    for(i = 0; i < 19; i++)
    {
        for(j = 0; j < 16; j++)
        {
            if(assigned[j][i])
                temp++;
        }
        if(temp == 16)
            scr.line[cont++] = i;
        temp = 0;
    }
    scr.total_line = cont;
    switch(cont)
    {
        case 1 : scr.total_score = 100; break;
        case 2 : scr.total_score = 300; break;
        case 3 : scr.total_score = 700; break;
        case 4 : scr.total_score = 1500; break;
    }
    return cont;
}
int next_calc()
{
    switch(block_num)
    {
        case 0 : return 1; break;
        case 1 : return 2; break;
        case 2 : return 3; break;
        case 3 : return 0; break;
        case 4 : return 5; break;
        case 5 : return 4; break;
        case 6 : return 7; break;
        case 7 : return 6; break;
        case 8 : return 8;break;
        case 9 : return 10; break;
        case 10 : return 9; break;
    }
    return 0;
}
void *x_timer()
{
    usleep(100000);
    draw_flag = 0;
    return NULL;
}
void *c_timer()
{
    usleep(500000);
    draw_flag = 0;
    return NULL;
}
void *y_timer(fb_info *fb_inf)
{
    int i;
    while(1)
    {
        for(i = 0; i < 1000; i++)
        {
        //////////////////////////
            usleep(300);
            if(msg[0] == 3)
            {
                y_offset++;
                if(!clear_flag)
                    fb_recover(*fb_inf, BLOCK_SIZE * 4, BLOCK_SIZE * 4, x_ori, y_ori);
                draw_shape(*fb_inf, block_num, MIDDLE_X + BLOCK_SIZE * x_offset, BLOCK_SIZE * y_offset);
                usleep(100000);
            }
        }
        y_offset++;
        if(!clear_flag)
        fb_recover(*fb_inf, BLOCK_SIZE * 4, BLOCK_SIZE * 4, x_ori, y_ori);
        draw_shape(*fb_inf, block_num, MIDDLE_X + BLOCK_SIZE * x_offset, BLOCK_SIZE * y_offset);
    }
}
void clear_handle(fb_info fb_inf)
{
    int i, j, k;
    char buf[10];
    verify_flag = 1;
    eros_s(fb_inf, 640, BLOCK_SIZE * (scr.line[scr.total_line - 1] - scr.total_line), 51, BLOCK_SIZE);
    eros_r(fb_inf, 640, BLOCK_SIZE * (scr.line[scr.total_line - 1] - scr.total_line), 51, BLOCK_SIZE + BLOCK_SIZE * scr.total_line);

    for(k = 0; k < scr.total_line; k++)
        for(j = 0; j < 16; j++)
            assigned[j][scr.line[k]] = assigned[j][scr.line[k] - scr.total_line];

    for(i = 0; i < scr.line[0] - scr.total_line; i++)
        for(j = 0; j < 16; j++)
            assigned[j][scr.line[0] - i] = assigned[j][scr.line[0] - i - scr.total_line];

    score_r(fb_inf, 100, 100, 858, 477);
    t_score += scr.total_score;
    sprintf(buf, "%d", t_score);
	display_string(buf, 858, 477, fb_inf, 0xffffff);
    handle_flag = 0;
    verify_flag = 0;
}
void *judge(fb_info *fb_inf)
{
    int blank = 0, left = 0,  right = 0, change = 0;;
    int i;
    while(1)
    {
        for(i = 0; i < 4; i++)
        {
            if(assigned[MAX_OFFSET + x_offset + shape[block_num][i] % 4][y_offset + shape[block_num][i] / 4 + 1])
                blank++;
        }
        for(i = 0; i < 4; i++)
        {
            if(assigned[MAX_OFFSET + x_offset + shape[block_num][i] % 4 - 1][y_offset + shape[block_num][i] / 4])
                left++;
        }
        for(i = 0; i < 4; i++)
        {
            if(assigned[MAX_OFFSET + x_offset + shape[block_num][i] % 4 + 1][y_offset + shape[block_num][i] / 4])
                right++;
        }
        for(i = 0; i < 4; i++)
        {
            if(assigned[MAX_OFFSET + x_offset + shape[next_calc(block_num)][i] % 4][y_offset + shape[block_num][i] / 4])
                change++;
        }
        if(y_offset == 15 || blank)
        {
            for(i = 0; i < 4; i++)
            {
                assigned[MAX_OFFSET + x_offset + shape[block_num][i] % 4][y_offset + shape[block_num][i] / 4] = 1;
            }
            clear_flag = 1;
            next_flag = 0;
            x_offset = 0;
            y_offset = 0;
        }
        if(predicate() && !handle_flag && !verify_flag)
        {
            handle_flag = 1;
            verify_flag = 1;
            clear_handle(*fb_inf);
        }
        blank = 0;
        if(left)  left_flag = 1;  else      left_flag = 0;
        if(right) right_flag = 1; else      right_flag = 0;
        if(change) change_flag = 1; else      change_flag = 0;
        left = 0; right = 0; change = 0;
        usleep(100);
    }
}
int test(fb_info fb_inf)
{
    pthread_t pid[6] = {0};
    int tmp = 0;
    int gm = 0;
    int i = 0;

    display_jpeg("tetris.jpg", fb_inf);
    decode_block(fb_inf);
    
    fb_save(fb_inf, BLOCK_SIZE * 4, BLOCK_SIZE * 4, 0, 0);
    score_s(fb_inf, 100, 100, 858, 477);

//##########
    pthread_create(&pid[0],NULL,(void *)y_timer,&fb_inf);
    pthread_create(&pid[1],NULL,(void *)judge,&fb_inf);
//##########
	display_string("000", 858, 477, fb_inf, 0xffffff);
    next_s(fb_inf, 200, 200, 810, 12);
    block_next = rand() % 11;
    color_next = rand() % 3;
    display_part("gameover.jpg",fb_inf.w/1.3,fb_inf.h/1.2, fb_inf);
    display_part("nextgame.jpg",fb_inf.w/1.3,fb_inf.h/1.4, fb_inf);
    while(1)
    {
        gm = judge_game2(fb_inf);
        if(gm == 2)
        {
            for (i = 0; i < 5; i++) 
            {
                if(pid[i])
                pthread_cancel(pid[i]);
            }
            return 1;
        }
        if(gm == 1)
        {
            for (i = 0; i < 5; i++) 
            {
                if(pid[i])
                pthread_cancel(pid[i]);
            }
            return 0;
        }

        if(!next_flag)
        {
            next_r(fb_inf, 200, 200, 810, 12);
            color = color_next;
            color_next = rand() % 3;
            block_num = block_next;
            block_next = rand() % 11;
            next_flag = 1;
            draw_next(fb_inf, block_next, 810, 12);
        }
        if(msg[0] == 1 && !draw_flag)
        {
            if(!left_flag && x_offset > -MAX_OFFSET)
            {
                x_offset--;
                draw_flag = 1;
//##########
                pthread_create(&pid[2],NULL, x_timer, NULL);
                if(!clear_flag)
                    fb_recover(fb_inf, BLOCK_SIZE * 4, BLOCK_SIZE * 4, x_ori, y_ori);
                draw_shape(fb_inf, block_num, MIDDLE_X + BLOCK_SIZE * x_offset, BLOCK_SIZE * y_offset);
                left_flag = 0;
            }
        }
        else if(msg[0] == 2 && !draw_flag)
        {
            switch(block_num)
            {
                case 1  :
                case 3  :
                case 5  :
                case 7  : tmp = 1; break;
                case 9  : tmp = 3; break;
                case 10 : tmp = 0; break;
                default : tmp = 2; break;
            }
            if(!right_flag && x_offset < tmp + MAX_OFFSET)
            {
                x_offset++;
                draw_flag = 1;
                if(!clear_flag)
                    fb_recover(fb_inf, BLOCK_SIZE * 4, BLOCK_SIZE * 4, x_ori, y_ori);
//##########
                pthread_create(&pid[3],NULL ,x_timer,NULL);
                draw_shape(fb_inf, block_num, MIDDLE_X + BLOCK_SIZE * x_offset, BLOCK_SIZE * y_offset);
                right_flag = 0;
            }
        }
        else if(msg[0] == 4 && !draw_flag)
        {
            if(!change_flag)
            {
                block_num = next_calc();
                draw_flag = 1;
                if(!clear_flag)
                    fb_recover(fb_inf, BLOCK_SIZE * 4, BLOCK_SIZE * 4, x_ori, y_ori);
//##########
                pthread_create(&pid[4],NULL ,c_timer,NULL);
                draw_shape(fb_inf, block_num, MIDDLE_X + BLOCK_SIZE * x_offset, BLOCK_SIZE * y_offset);
                change_flag = 0;
            }
        }
        usleep(100);
    }
    return 0;
}
