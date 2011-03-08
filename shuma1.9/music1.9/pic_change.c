#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "common.h"


void cartoon_pic(fb_info fb_inf)
{
    //display_screen("fengjing.jpg", fb_inf, 10);
    //display_screen("1.jpg", fb_inf, 0);
    //display_screen("2.jpg", fb_inf, 1);
    //display_screen("3.jpg", fb_inf, 2);
    //display_screen("4.jpg", fb_inf, 3);
    //display_screen("5.jpg", fb_inf, 4);
    //display_screen("6.jpg", fb_inf, 5);
    //display_screen("7.jpg", fb_inf, 6);
    //display_screen("8.jpg", fb_inf, 7);
    //display_screen("9.jpg", fb_inf, 8);
    //sleep(2);
    //display_part("bianxing.jpg",0,0, fb_inf);
      //display_part("qingtianzhu.jpg", fb_7nf.w/2, 0, fb_inf);
      //display_part("anniukkk.jpg",100,200, fb_inf);
      //sleep(1);
}

int display_game(char *filename, fb_info old_inf,int k, int pic)
{

	fb_info jpeg_inf;
	fb_info fb_inf;
    int x = 0;
    int y = 0;


    //fb_inf.h = (int)(old_inf.h/1.46);
    //fb_inf.w = (int)(old_inf.w/1.46);

    fb_inf.h = old_inf.h/1.83;
    fb_inf.w = old_inf.w/2.438;

	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *bufs32 = rgb24to32(scale_buf, fb_inf);
    

	int i, j;
    if(k == 9)
    {
        //x = (int)(old_inf.w/4.76);
        //y = (int)(old_inf.h/15.36);

        for (j = 0; j < fb_inf.w; ++j){
        for(i = 0; i < fb_inf.h; ++i){
        fb_pixel(old_inf, x+j, y+i, bufs32[j + i * fb_inf.w]);
        }
        }
    }
    if(k == 19)
    {
        int x = fb_inf.w/3*4;
        int y = 0;
        //int x = 0;

        for (j = 0; j < fb_inf.w; ++j){
        for(i = 0; i < fb_inf.h; ++i){
        fb_pixel(old_inf, x+j, y+i, bufs32[j + i * fb_inf.w]);
        }
        }
    }
    if(k < 9)
    {
        if(pic == 9)
        {
            //x = fb_inf.w/3*4 + fb_inf.w/3*(k%3);
            x = fb_inf.w/3*(k%3);
            y = fb_inf.h/3*(k/3);
            
            for (j = 0; j < fb_inf.w/3; ++j){
            for(i = 0; i < fb_inf.h/3; ++i){
                //fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
                //fb_pixel(old_inf, x+j, y+i, bufs32[fb_inf.w/3*(pic%3)+j + (i+fb_inf.h/3*(pic/3)) * fb_inf.w]);
                fb_pixel(old_inf, x+j, y+i, 0xffffff);
                
            }
            }
        }
        if(pic != 9)
        {
            //x = fb_inf.w/3*4 + fb_inf.w/3*(k%3);
            x = fb_inf.w/3*(k%3);
            y = fb_inf.h/3*(k/3);
            
            for (j = 0; j < fb_inf.w/3; ++j){
            for(i = 0; i < fb_inf.h/3; ++i){
                //fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
                fb_pixel(old_inf, x+j, y+i, bufs32[fb_inf.w/3*(pic%3)+j + (i+fb_inf.h/3*(pic/3)) * fb_inf.w]);
                //fb_pixel(old_inf, x+j, y+i, 0xffffff);
                
            }
            }
        }
    }
    if(k>9 && k<19)
    {
        k = k-10;
        x = fb_inf.w/3*4 + fb_inf.w/3*(k%3);
        y = fb_inf.h/3*(k/3);
        if(pic != 9)
        {
            
            for (j = 0; j < fb_inf.w/3; ++j){
            for(i = 0; i < fb_inf.h/3; ++i){
                //fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
            fb_pixel(old_inf, x+j, y+i, bufs32[fb_inf.w/3*(pic%3)+j + (i+fb_inf.h/3*(pic/3)) * fb_inf.w]);
            }
            }
        }
        else
        {
            for (j = 0; j < fb_inf.w/3; ++j){
            for(i = 0; i < fb_inf.h/3; ++i){
                //fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
            fb_pixel(old_inf, x+j, y+i, 0xffffff);
            }
            }

        }
    }
    
    //if(k != 20 && k!=21)
    /*
    if(k == 22)
    {
        //x = (int)(old_inf.w/4.76 + fb_inf.w/3*(k%3));
        //y = (int)(old_inf.h/15.36 + fb_inf.h/3*(k/3));
        
        for (j = 0; j < fb_inf.w/3; ++j){
        for(i = 0; i < fb_inf.h/3; ++i){
            //fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
        fb_pixel(old_inf, x+j, y+i, bufs32[3*j + 3*i * fb_inf.w]);
        }
        }
    }
    */
	free(buf24);
	free(scale_buf);
    free(bufs32);
	
	return 0;
}
void game(fb_info fb_inf)
{
    int i;
    //display_game("fengjing.jpg", fb_inf, 21);

    for (i = 0; i < 9; i++) 
    {
        //display_game("fengjing.jpg", fb_inf, i);
    sleep(1);
    }
    
}

