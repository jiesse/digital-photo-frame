
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "common.h"

#define DEBUG 0
#if DEBUG
#endif
u32_t *buf32;
int xiaoshi(fb_info fb_inf)
{
    int i,j;
    int n  =0;
    int ki;
    u32_t corlor;

    int x1 = fb_inf.w*100/1024;
    int y1 = fb_inf.h*400/1024;
    //u8_t *pos ; 
    srand(time(NULL));
for (ki = 0; ki < 2; ki++) 
{
	for (j = x1; j < fb_inf.h; ++j){
       	for(i = y1; i < fb_inf.w; ++i){
            if( *((int*)((u8_t*)fb_inf.fbmem + (fb_inf.w * j + i) * fb_inf.bpp/8)))   
            {
                usleep(200);
                fb_pixel(fb_inf,i,j, 0x0);

                n =rand()%50+1;
                if(j+n >= fb_inf.h) 
                {
                    //fb_pixel(fb_inf,fb_inf.w,fb_inf.h, 0x0);
                    //return 0;
                }
                if(j+n < fb_inf.h)
                {
                   corlor =rand()%0xffffff;
                   fb_pixel(fb_inf,i,j+n, corlor);
                }
            }
		}
	}
}    
    return 0;
}

int display_muspic(char *filename, fb_info old_inf)
{

	fb_info jpeg_inf;
	fb_info fb_inf;
    int x = 0;
    int y = 0;


    fb_inf.h = (int)(old_inf.h/2.5);
    fb_inf.w = (int)(old_inf.w/5);

	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *bufs32 = rgb24to32(scale_buf, fb_inf);
    

	int i, j;
        x = (int)(old_inf.w/7*5);
        y = (int)(old_inf.h/10*1);
        
        for (j = 0; j < fb_inf.w; ++j){
        for(i = 0; i < fb_inf.h; ++i){
            //fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
        fb_pixel(old_inf, x+j, y+i, bufs32[j + i * fb_inf.w]);
        }
        }
	free(buf24);
	free(scale_buf);
    free(bufs32);
	
	return 0;
}
int display_kaiji(fb_info fb_inf)
{
    int i,j;

	for (j = 0; j < fb_inf.w; ++j){
       	for(i = 0; i < fb_inf.h; ++i){
			fb_pixel(fb_inf,j, i, 0x0);
		}
	}
    
    return 0;
}
int display_music_jpeg(fb_info fb_inf,int x1,int y1,int x2,int y2)
{

	fb_info jpeg_inf;
    char path[20] = "music9.jpg";
	
	u8_t *buf24 = decode_jpeg(path, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    buf32 = rgb24to32(scale_buf, fb_inf);
    

	int i, j;
	for (j = x1; j < x2; ++j){
       	for(i = y1; i < y2; ++i){
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
	}
	free(buf24);
	free(scale_buf);
    free(buf32);
    usleep(300000);

	buf24 = decode_jpeg("music8.jpg", &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    buf32 = rgb24to32(scale_buf, fb_inf);

	for (j = x1; j < x2; ++j){
       	for(i = y1; i < y2; ++i){
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
	}
    
	free(buf24);
	free(scale_buf);
    free(buf32);
	
	return 0;
}
int display_part(char *filename,int x, int y, fb_info fb_inf)
{

	fb_info jpeg_inf;
	fb_info new_inf;

	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    new_inf.w = jpeg_inf.w*fb_inf.w/1024;
    new_inf.h = jpeg_inf.h*fb_inf.h/768;
	u8_t * scale_buf = scale24(buf24, new_inf, jpeg_inf);
	u32_t *bufe32 = rgb24to32(scale_buf, new_inf);

	int i, j;
	for (j = 0; j < new_inf.w; ++j){
       	for(i = 0; i < new_inf.h; ++i){
        if(bufe32[j + i*new_inf.w] > 0x020000)
			fb_pixel(fb_inf, x+j, y+i, bufe32[j + i * new_inf.w]);
		}
	}
	free(scale_buf);
	free(buf24);
	free(bufe32);
	return 0;
}

int display_jpeg(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    buf32 = rgb24to32(scale_buf, fb_inf);
    

	int i, j;
	for (j = 0; j < fb_inf.w; ++j){
       	for(i = 0; i < fb_inf.h; ++i){
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
	}
    
    //for (j = 0; j < jpeg_inf.w/2; ++j){
    //for(i = 0; i < jpeg_inf.h/2; ++i){
    ////fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
    //fb_pixel(fb_inf, j, i, buf32[2*j + 2*i * fb_inf.w]);
    //}
    //usleep(2000);
    //}
	free(buf24);
	free(scale_buf);
    free(buf32);
	
	return 0;
}
int display_screen(char *filename, fb_info old_inf,int k)
{

	fb_info jpeg_inf;
	fb_info fb_inf;
    int x = 0;
    int y = 0;


    fb_inf.h = (int)(old_inf.h/1.46);
    fb_inf.w = (int)(old_inf.w/1.46);

	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *bufs32 = rgb24to32(scale_buf, fb_inf);
    

	int i, j;
    if(k == 10)
    {
        x = (int)(old_inf.w/4.76);
        y = (int)(old_inf.h/15.36);

        for (j = 0; j < fb_inf.w; ++j){
        for(i = 0; i < fb_inf.h; ++i){
        fb_pixel(old_inf, x+j, y+i, bufs32[j + i * fb_inf.w]);
        }
        }
    }
    
    if(k != 10)
    {
        x = (int)(old_inf.w/4.76 + fb_inf.w/3*(k%3));
        y = (int)(old_inf.h/15.36 + fb_inf.h/3*(k/3));
        
        for (j = 0; j < fb_inf.w/3; ++j){
        for(i = 0; i < fb_inf.h/3; ++i){
            //fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
        fb_pixel(old_inf, x+j, y+i, bufs32[3*j + 3*i * fb_inf.w]);
        }
        }
    }
	free(buf24);
	free(scale_buf);
    free(bufs32);
	
	return 0;
}
