#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "common.h"
#include "pic_change.h"

extern int *mus;
extern unsigned int msg[4];
extern int music_flag[4];
int navi_flag;

int flag1 = 1;
int pic_num = 0;
int flag_scan = 12;

char musnam[15][256] = {"1大海","2老男孩","3李雷和韩梅梅","4同桌的你","5星星点灯"};
char mumode[3][256] = {"顺序播放","随机播放"};
extern pthread_mutex_t counter_mutex;
void start(fb_info fb_inf)
{
    display_jpeg("kkkkkkkkkkk2.jpg", fb_inf);
}
int judge_music(fb_info fb_inf)
{
    int x, y;
    char muspic[12] = {0};
    int k;
    int xz = fb_inf.w/1024;
    int yz = fb_inf.h/768;
    int m;

    k = 0;
    m = 2;
    while(1)
    {
        x = msg[1];
        y = msg[2];
        
        if(k != *(mus + 2))
        {
        clean_string(musnam[k-1], fb_inf.w/3, fb_inf.h/10, fb_inf, 0x0);
        //clean_string(musnam[k-1], fb_inf.w/2, fb_inf.h/2, fb_inf, 0x0);
        k = *(mus + 2);
        sprintf(muspic,"music%d.jpg",*(mus + 2));
        display_muspic(muspic, fb_inf);
	    display_string(musnam[*(mus + 2) - 1], fb_inf.w/3, fb_inf.h/10, fb_inf, 0x0);
        //sleep(1);
        //clean_string(musnam[k-1], fb_inf.w/3, fb_inf.h/10, fb_inf, 0x0);
        }
        if(m != *(mus + 1))
        {
        clean_string(mumode[m-1], fb_inf.w/3, fb_inf.h/10*8, fb_inf, 0x0);
        //clean_string(musnam[k-1], fb_inf.w/2, fb_inf.h/2, fb_inf, 0x0);
        m = *(mus + 1);
	    display_string(mumode[*(mus + 1) - 1], fb_inf.w/3, fb_inf.h/10*8, fb_inf, 0x0);
        //sleep(1);
        //clean_string(musnam[k-1], fb_inf.w/3, fb_inf.h/10, fb_inf, 0x0);
        }
        if(1 == msg[0])
        {
            //msg[0] = 0;

            //shang
            
            if(258*xz < x && x < xz*409 && 269*yz < y && y < 333*yz ) 
            {
                *mus = 2;
                
                display_music_jpeg(fb_inf,258*xz,269*yz,409*xz,333*yz) ;
            }

            //xia
            if(530*xz<x && x<675*xz && 270*yz<y && y<333*yz ) 
            {
                *mus = 1;
                display_music_jpeg(fb_inf,530*xz,270*yz,675*xz,333*yz) ;
            }
            //shunxu
            if(242*xz<x && x<452*xz && 435*yz<y && y<497*yz )
            {
                *(mus + 1) = 1;
                display_music_jpeg(fb_inf,242*xz,435*yz,453*xz,505*yz) ;
            }
            //suiji

            if(518*xz<x && x<728*xz && 437*yz<y && y<498*yz ) 
            {
                *(mus + 1) = 2;
                display_music_jpeg(fb_inf,518*xz,437*yz,728*xz,505*yz) ;
            }
            //fanhui
            if(787*xz<x && x<912*xz && 626*yz<y && y<683*yz )
            {
                display_music_jpeg(fb_inf,787*xz,626*yz,912*xz,690*yz) ;
                return 0;
            }
        }
    }
    return 0;
}

int navigation(fb_info fb_inf)
{
    int w = fb_inf.w;
    int h = fb_inf.h;
    if(msg[1] < w/7 && msg[2]<h/1.5 && 1 == flag1)
    {

        display_part("daohang.jpg",w/20,h/38, fb_inf);
        flag1 = 2;
        //display_part("annu.jpg",0,0, fb_inf);
    }
    if((msg[1] > w/7 || msg[2] > h/1.5) && 2==flag1)
    {

        display_part("fugaitaohang.jpg",0,0, fb_inf);
        flag1 = 1;
        //display_part("annu.jpg",0,0, fb_inf);
    }
    if(msg[0] ==1)
    {
        if(msg[1] < w/9 && msg[2] > h/25.36 && msg[2] < h/7.6)
        {
            navi_flag = 1;
            return 1;
        }
        if(msg[1] < w/9 && msg[2] > h/7.6 && msg[2] < h/3.8)
        {
            navi_flag = 2;
            return 1;
        }
        if(msg[1] < w/9 && msg[2] > h/3.8 && msg[2] < h/2.56)
        {
            navi_flag = 3;
            return 1;
        }
        if(msg[1] < w/9 && msg[2] > h/2.56 && msg[2] < h/1.9)
        {
            navi_flag = 4;
            return 1;
        }
        if(msg[1] < w/9 && msg[2] > h/1.9 && msg[2] < h/1.53)
        {
            navi_flag = 5;
            return 1;
        }
        //if(msg[1] < w/10 && msg[2] > w/15.36 && msg[2] < w/9.4)
    }
    return 0;
}
int judge_navi(fb_info fb_inf)
{
    int flag = 0;
    while(!flag)
    {
        flag = navigation(fb_inf);
    }
    return 0;
}
int scan_menu(fb_info fb_inf)
{
    int m = 0;
    int k = 0;
    int i;
    int flag = 0;

    //int w = fb_inf.w;
    //int h = fb_inf.h;
    int sw = (int)(fb_inf.w/1.46);
    int sh = (int)(fb_inf.h/1.46);
    int x = 0;
    int y = 0;
    char filename[6] = {0};

    while(!flag)
    {
        flag = navigation(fb_inf);
        x = msg[1] - (int)(fb_inf.w/4.76);
        y = msg[2] - (int)(fb_inf.h/15.36);
        if(x*y<0 || x > sw || y > sh)
        {}
        else
        {
            for (k = 2; k >= 0; k--) 
            {
                if(x > k*sw/3)
                    break;
            }
            for (m = 2; m >= 0; m--) 
            {
                if(y > m*sh/3)
                    break;
            }
            k = m*3 + k;
            if(k >= 0 && k < 9)
            {
                if(flag_scan != k)
                {
                    display_screen("beijing.jpg", fb_inf, 10);
                    sprintf(filename,"%d.jpg",k+1);
                    display_screen(filename, fb_inf, k);
                    flag_scan = k;
                }
            }
            else
                display_screen("beijing.jpg", fb_inf, 10);
        }
        if(msg[0] ==1)
        {
            x = msg[1] - (int)(fb_inf.w/4.76);
            y = msg[2] - (int)(fb_inf.h/15.36);
            if(x*y<0 || x > sw || y > sh)
            {}
            else
            {
                for (k = 2; k >= 0; k--) 
                {
                    if(x > k*sw/3)
                        break;
                }
                for (m = 2; m >= 0; m--) 
                {
                    if(y > m*sh/3)
                        break;
                }
                for (i = 0; i < 9; i++) 
                {
                    if(m*3+k == i)
                    {
                        pic_num = i+1;
                        sprintf(filename,"%d.jpg",i+1);
                        display_screen(filename, fb_inf, 10);
                        return 0;
                    }
                }

            }
        }
    }
    return 0;
}
int judge_scan(fb_info fb_inf)
{
    //int flag = 0;
    scan_menu(fb_inf);
    display_part("next2.jpg",fb_inf.w/1.6,fb_inf.h/1.6, fb_inf);
    display_part("next1.jpg",fb_inf.w/2.4,fb_inf.h/1.6, fb_inf);
    //sleep(2);
    return 0;
}
int judge_next(fb_info fb_inf)
{
    int flag = 0;
    int w = fb_inf.w;
    int h = fb_inf.h;
    char filename[6] = {0};

    while(!flag)
    {
        flag = navigation(fb_inf);
        if(msg[0] ==1)
        {
            while(msg[0] == 1);
            if(msg[1] > w/1.6 && msg[1] < w/1.4 && msg[2] > h/1.6 && msg[2] < h/1.4)
            {
                pic_num++;
                if(pic_num == 10)
                    pic_num = 1;
                sprintf(filename,"%d.jpg",pic_num);
                display_screen(filename, fb_inf, 10);
                display_part("next2.jpg",fb_inf.w/1.6,fb_inf.h/1.6, fb_inf);
                display_part("next1.jpg",fb_inf.w/2.4,fb_inf.h/1.6, fb_inf);
            }
            if(msg[1] > w/2.4 && msg[1] < w/2.0 && msg[2] > h/1.6 && msg[2] < h/1.4)
            {
                pic_num--;
                if(pic_num ==0)
                    pic_num = 9;
                sprintf(filename,"%d.jpg",pic_num);
                display_screen(filename, fb_inf, 10);
                display_part("next2.jpg",fb_inf.w/1.6,fb_inf.h/1.6, fb_inf);
                display_part("next1.jpg",fb_inf.w/2.4,fb_inf.h/1.6, fb_inf);
            }
            //display_part("next2.jpg",fb_inf.w/1.6,fb_inf.h/1.6, fb_inf);
            //display_part("next1.jpg",fb_inf.w/2.4,fb_inf.h/1.6, fb_inf);
        }
    }
    return 0;
}

int game1[10];
int game2[10];
int current_pic = 10;
int last_palce1;
int last_palce2;
int last_pic;
int judge_game2(fb_info fb_inf)
{
    int w = fb_inf.w;
    int h = fb_inf.h;
    if(msg[0] ==1)
    {
        //while(msg[0] == 1);
         //over
         if(msg[1] > w/1.3 && msg[1] < w/1.1 && msg[2] > h/1.2 && msg[2] < h/1.1)
         {
            return 2;
         }
         //next
         if(msg[1] > w/1.3 && msg[1] < w/1.1 && msg[2] > h/1.4 && msg[2] < h/1.3)
         {
            return 1;
         }
    }
    return 0;
}
int judge_game(fb_info fb_inf)
{
    int m = 0;
    int k = 0;
    int i;
    int gm;

    //int w = fb_inf.w;
    //int h = fb_inf.h;
    int sw = (int)(fb_inf.w/2.438);
    int sh = (int)(fb_inf.h/1.83);
    int x = 0;
    int y = 0;
    //char filename[6] = {0};
    while(1)
    {
        gm = judge_game2(fb_inf);
        if(gm == 2)
        return 1;
        if(gm == 1)
        return 0;

        if(msg[0] == 1)
        {
            x = msg[1];
            y = msg[2];
            if(y > sh)
            {
                display_game("5.jpg", fb_inf, last_palce1, last_pic);
                display_game("5.jpg", fb_inf, last_palce2, 9);
            }
            if(x*y<0 || x > sw || y > sh)
            {
            }
            else
            {
                for (k = 2; k >= 0; k--) 
                {
                    if(x > k*sw/3)
                        break;
                }
                for (m = 2; m >= 0; m--) 
                {
                    if(y > m*sh/3)
                        break;
                }
                for (i = 0; i < 9; i++) 
                {
                    if(m*3+k == i)
                    {
                        if(current_pic == 10)
                        {
                            //sprintf(filename,"5.jpg",i+1);
                            display_game("5.jpg", fb_inf, i, 9);
                            current_pic = game1[i];
                            last_pic = game1[i];
                            last_palce1 = i;
                        }
                        //return 0;
                    }
                }

            }

            x = msg[1] - sw/3*4;
            y = msg[2];
            if(x*y<0 || x > sw || y > sh)
            {}
            else
            {
                for (k = 2; k >= 0; k--) 
                {
                    if(x > k*sw/3)
                        break;
                }
                for (m = 2; m >= 0; m--) 
                {
                    if(y > m*sh/3)
                        break;
                }
                for (i = 0; i < 9; i++) 
                {
                    if(m*3+k == i)
                    {
                        if(current_pic != 10)
                        {
                            //sprintf(filename,"5.jpg",i+1);
                            display_game("5.jpg", fb_inf, i+10, current_pic);
                            current_pic = 10;
                            last_palce2 = i+10;
                        }
                        //return 0;
                    }
                }

            }
        }
    }
    return 0;
}
int print_game(fb_info fb_inf)
{
    int num[10] = {0};
    int rand_sum = 9;
    int i,rand_num, k;
    for (i = 0; i < 9; i++) 
    {
        num[i] = i;
    }
    for (i = 0; i < 9; i++) 
    {
        rand_num = rand()%rand_sum;
        game1[i] = num[rand_num];
        for (k = rand_num; k < rand_sum; k++) 
        {
            num[k] = num[k+1];
        }
        rand_sum--;
    }
    display_jpeg("fengjing.jpg", fb_inf);
    display_part("gameover.jpg",fb_inf.w/1.3,fb_inf.h/1.2, fb_inf);
    display_part("nextgame.jpg",fb_inf.w/1.3,fb_inf.h/1.4, fb_inf);
    for (i = 0; i < 9; i++) 
    {
        display_game("5.jpg", fb_inf, i, game1[i]);
        display_game("5.jpg", fb_inf, 10+i, 9);
        //sleep(1);
    }
    //display_game("fengjing.jpg", fb_inf, 19, game1[i]);

    return 0;

}

