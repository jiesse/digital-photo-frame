#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "common.h"

#define DEBUG 0
#if DEBUG
#endif
u8_t save[768][1024 * 4];
u8_t *buf24;
u8_t *scale_buf;
extern unsigned int msg[4];
extern u32_t *buf32;
/*
void fb_save(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
    {
        memcpy(save[i], fb_inf.fbmem + ((starty + i) * fb_inf.w + startx) * 4, width * 4);
    }
}
void fb_recover(fb_info fb_inf, int width, int hight, int startx, int starty)
{
    int i;
    for(i = 0; i < hight; i++)
    {
        memcpy(fb_inf.fbmem + ((starty + i) * fb_inf.w + startx) * 4, save[i], width * 4);
    }
}
int display_jpeg(char *filename, fb_info fb_inf, int width, int hight)
{

	fb_info jpeg_inf;
    fb_info pic_inf = fb_inf;
    pic_inf.w = width;
    pic_inf.h = hight;
	
	u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
	u8_t * scale_buf = scale24(buf24, pic_inf, jpeg_inf);
	u32_t *buf32 = rgb24to32(scale_buf, pic_inf);

	int i, j;
	for(i = 0; i < pic_inf.h; ++i){
		for (j = 0; j < pic_inf.w; ++j){
			fb_pixel(fb_inf, j, i, buf32[j + i * pic_inf.w]);
		}
	}

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}*/
void load_image(char *filename,fb_info fb_inf, int width, int hight)
{
    fb_info jpeg_inf;
    fb_info pic_inf = fb_inf;
    pic_inf.w = width;
    pic_inf.h = hight;
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, pic_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, pic_inf);
}
void unload_image(void)
{
	free(buf24);
	free(scale_buf);
	free(buf32);
}
int display_retangle(char *filename, fb_info fb_inf, int width, int hight, int startx, int starty)
{
    //fb_info jpeg_inf;
    fb_info pic_inf = fb_inf;
    pic_inf.w = width;
    pic_inf.h = hight;
	int i, j;


	for(i = 0; i < pic_inf.h; ++i){
		for (j = 0; j < pic_inf.w; ++j){
			fb_pixel(fb_inf, j + startx, i + starty, buf32[j + i * pic_inf.w]);
		}
	}
	return 0;
}
void delay(int z)
{
    int i;
    while(z--)
        for (i = 0; i <100000; i++);
}
int grain(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);

	int i, j, count = 0, temp, tcount = 0;
    int assigned[3072] = {0};
    srand((unsigned)time(NULL));
    while(count < 3072)
    {
        temp = rand() % 3072;
        tcount++;
        if(!assigned[temp])
        {
            assigned[temp] = 1;
            for(i = temp/64 * 16; i < temp/64 * 16 + 16; ++i)
            {
                for (j = temp%64 * 16; j < temp%64 * 16 + 16; ++j)
                {
                    fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
                }
            }
            count++;
            delay(1);
        if(msg[0] == 2)
            return 1;
        }
        if(count > 2500)
        { 
            for(i = 0; i < fb_inf.h; ++i)
            {
                for (j = 0; j < fb_inf.w; ++j)
                {
                    fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
                }
            }
        if(msg[0] == 2)
            return 1;
            break;
        }
        if(tcount == 10000)
        {
            srand((unsigned)time(NULL));
            tcount = 0;
        }
    }

	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
int fly_left_right(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
    //float gama = fb_inf.w / fb_inf.h;
    int i, j, k;
    for(k = 0; k < fb_inf.w; k+=10)
    {
        for(i = 0; i < fb_inf.h; ++i)
        {
            for (j = fb_inf.w - k; j < fb_inf.w; ++j)
            {
                fb_pixel(fb_inf, j + k - fb_inf.w, i, buf32[j + i * fb_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
    }
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}

int fly_multi(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
    //float gama = fb_inf.w / fb_inf.h;
    int i, j, k;
    for(k = 0; k <= fb_inf.w / 2; k+=5)
    {
        for(i = 0; i <= fb_inf.h / 2; ++i)
        {
            for (j = fb_inf.w / 2 - k; j <= fb_inf.w / 2; ++j)
            {
                fb_pixel(fb_inf, j + k - fb_inf.w / 2, i, buf32[j + i * fb_inf.w]);
            }
        }
        for(i = fb_inf.h / 2; i <= fb_inf.h; ++i)
        {
            for (j = fb_inf.w - 1 ; j >= fb_inf.w - k; --j)
            {
                fb_pixel(fb_inf, j , i, buf32[j + i * fb_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
    }
    for(k = 0; k <= fb_inf.h / 2; k+=5)
    {
        for(i = fb_inf.h / 2 - k; i < fb_inf.h / 2; i++)
        {
            for(j = fb_inf.w / 2; j < fb_inf.w; j++)
                fb_pixel(fb_inf, j , i + k - fb_inf.h / 2, buf32[j + i * fb_inf.w]);
        }
        for(i = fb_inf.h - 1; i >= fb_inf.h - k; i--)
        {
            for(j = 0; j <= fb_inf.w / 2; j++)
                fb_pixel(fb_inf, j , i, buf32[j + i * fb_inf.w]);
        }
        if(msg[0] == 2)
            return 1;
    }
	for(i = 0; i < fb_inf.h; ++i){
		for (j = 0; j < fb_inf.w; ++j){
			fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		}
	}
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
int disp_expand(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
    int i, j, k, l;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
    for(l = 1; l <= 16; l++)
    {
        for(k = 0;k < fb_inf.w / 32 * fb_inf.h / 32; k++)
        {
            for(i = k / (fb_inf.w / 32) * 32 + 16 - l; i < k / (fb_inf.w / 32) * 32 + 16 + l; ++i)
            {
                for (j = k % (fb_inf.w / 32) * 32 + 16 - l; j < k % (fb_inf.w / 32) * 32 + 16 + l; ++j)
                {
                    fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
                }
            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(100000);
    }
	free(buf24);
	free(scale_buf);
	free(buf32);
	return 0;
}
int random_grating(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
    int i, j, counter_x = 0, counter_y = 0, counter_t = 0;
    int temp;
    char *assigned_x = malloc(fb_inf.w / 3);
    char *assigned_y = malloc(fb_inf.h / 3);
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
    srand((unsigned)time(NULL));
    while(1)
    {
        temp = rand() % (fb_inf.w / 3);
        counter_t++;
        if(!*(assigned_x + temp))
        {
            counter_x++;
            *(assigned_x + temp) = 1;
            for(i = 0; i < fb_inf.h; ++i)
            {
                for (j = temp * 3; j < temp * 3 + 3; ++j)
                {
                    fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
		        }
            }
            delay(10);
        }
        temp = rand() % (fb_inf.h / 3);
        counter_t++;
        if(!*(assigned_y + temp))
        {
            counter_y++;
            *(assigned_y +temp) = 1;
            for(i = temp * 3; i < temp * 3 + 3; i++)
            {
                for(j = 0; j < fb_inf.w; j++)
                {
                    fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
                }
            }
            delay(20);
        }
        if(counter_x > (fb_inf.w / 3 - 250) || counter_y > (fb_inf.h / 3 - 250))
        {
            for(i = 0; i < fb_inf.h; ++i)
            {
                for (j = 0; j < fb_inf.w; ++j)
                {
                    fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
                }
            }
            break;
        }

        if(msg[0] == 2)
            return 1;

        if(counter_t > 100000)
        {
            srand((unsigned)time(NULL));
            counter_t = 0;
        }
    }
	free(buf24);
	free(scale_buf);
	free(buf32);
    free(assigned_x);
    free(assigned_y);
	return 0;
}
int scan(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
    int i, j, k;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
    
    for(k = 0; k < fb_inf.w / 32; k++)
    {
        if(k)
        {
            for(i = 0; i < fb_inf.h; i++)
            {
                for(j = k * 32 - 32; j < k * 32; j++)
                    fb_pixel(fb_inf, j, i, 0x00);
            }
        }
        for(i = 0; i < fb_inf.h; i++)
        {
            for(j = k * 32; j < k * 32 + 32; j++)
                fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
        }
        if(msg[0] == 2)
            return 1;
        usleep(20000);
    }
    for(j = fb_inf.w; j > 0; j--)
    {
        for(i = 0; i < fb_inf.h; i++)
            fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
        delay(3);
    }
	free(buf24);
	free(scale_buf);
	free(buf32);
	return 0;
}
int tremor(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
    int i, j, k, l;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
    for(k = 0; k < fb_inf.h; k+=30)
    {
        for(i = fb_inf.h - k; i < fb_inf.h; ++i)
        {
            for (j = 0; j < fb_inf.w; ++j)
            {
                fb_pixel(fb_inf, j, i + k - fb_inf.h, buf32[j + i * fb_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
        //usleep(100 - k / 10);
        //usleep(1);
    }
    for(l = 5; l > 0; l--)
    {
        for(k = fb_inf.h; k > fb_inf.h - l * 20; k-=20)
        {
            for(i = fb_inf.h - k; i < fb_inf.h; ++i)
            {
                for (j = 0; j < fb_inf.w; ++j)
                {
                    fb_pixel(fb_inf, j, i + k - fb_inf.h, buf32[j + i * fb_inf.w]);
                }
            }
            usleep(1000 - l * 100);
            for(i = k; i < fb_inf.h; ++i)
            {
                for (j = 0; j < fb_inf.w; ++j)
                {
                    fb_pixel(fb_inf, j, i, 0x00);
                }
            }
        if(msg[0] == 2)
            return 1;
        }
        for(k = fb_inf.h - l * 20; k < fb_inf.h; k+=20)
        {
            for(i = fb_inf.h - k; i < fb_inf.h; ++i)
            {
                for (j = 0; j < fb_inf.w; ++j)
                {
                    fb_pixel(fb_inf, j, i + k - fb_inf.h, buf32[j + i * fb_inf.w]);
                }
            }
        if(msg[0] == 2)
            return 1;
            usleep(1000 - l * 100);
        }
    }
    
	free(buf24);
	free(scale_buf);
	free(buf32);
    return 0;
}
int push(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
    fb_info scale_inf;
    int i, j, k;
	scale_inf = fb_inf;
	buf24 = decode_jpeg(filename, &jpeg_inf);
    for(k = 95; k >= 0; k--)
    {
        scale_inf.w = fb_inf.w - (fb_inf.w / 100) * k;
        scale_inf.h = fb_inf.h - (fb_inf.h / 100) * k;
	    scale_buf = scale24(buf24, scale_inf, jpeg_inf);
	    buf32 = rgb24to32(scale_buf, scale_inf);
        for(i = 0; i < scale_inf.h; ++i)
        {
            for (j = 0; j < scale_inf.w; ++j)
            {
                fb_pixel(fb_inf, j + (fb_inf.w - scale_inf.w) / 2, i + (fb_inf.h - scale_inf.h) / 2, buf32[j + i * scale_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
        free(scale_buf);
        free(buf32);
        usleep(10000);
    }

	free(buf24);
    return 0;
}
int shutter(char *filename, fb_info fb_inf)
{

	fb_info jpeg_inf;
    int i, j, k;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
        for(i = 0; i < fb_inf.h / 20; ++i)
        {
            for(k = 0; k < 20; k++)
            {
                for (j = 0; j < fb_inf.w; ++j)
                {
                    fb_pixel(fb_inf, j, i+ fb_inf.h / 20 * k, buf32[j + (i + fb_inf.h / 20 * k) * fb_inf.w]);
                }
            }
        if(msg[0] == 2)
            return 1;
            usleep(10000);
        }

	free(buf24);
	free(scale_buf);
	free(buf32);
    return 0;
}
int vertical_grating(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
    //float gama = fb_inf.w / fb_inf.h;
    int i, j, k;
    for(k = 0; k <= fb_inf.h; k+=2)
    {
        for(i = fb_inf.h - k; i < fb_inf.h; ++i)
        {
            for (j = 0; j < fb_inf.w; ++j)
            {
                if(j % 2 >= 1)
                    fb_pixel(fb_inf, j, i + k - fb_inf.h, buf32[j + i * fb_inf.w]);
            }
        }
        for(i = 0; i < k; ++i)
        {
            for (j = 0; j < fb_inf.w; ++j)
            {
                if(j % 2 < 1)
                    fb_pixel(fb_inf, j, i - k + fb_inf.h, buf32[j + i * fb_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(10000);
    }
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
int mosaic(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
    int i, j, k;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
    for(k = 32; k > 1; k-=2)
    {
        for(i = 0; i < fb_inf.h; ++i)
        {
            for (j = 0; j < fb_inf.w; ++j)
            {
                fb_pixel(fb_inf, j, i, buf32[j + (i / k) * k * fb_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(40000 - 1000 * k);
    }
	free(buf24);
	free(scale_buf);
	free(buf32);
	
	return 0;
}
int vacum(char *filename, char *filename2, fb_info fb_inf)
{
	fb_info jpeg_inf;
    int i, j, k;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);

    for(i = 0; i < fb_inf.h; ++i)
    {
        for (j = 0; j < fb_inf.w; ++j)
        {
            fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
        }
    }
    for(k = 5; k < fb_inf.w; k+=10)
    {
        for(i = 0; i < fb_inf.h; ++i)
        {
            for (j = fb_inf.w - k; j < fb_inf.w; ++j)
            {
                if(j>(fb_inf.w - k))
                    fb_pixel(fb_inf, j, i, buf32[(fb_inf.w - k) + j / 20 + i * fb_inf.w]);
                else
                    fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(1000);
    }
	free(buf24);
	free(scale_buf);
	free(buf32);
	buf24 = decode_jpeg(filename2, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);
	
    for(k = fb_inf.w; k > 0; k-=10)
    {
        for(i = 0; i < fb_inf.h; ++i)
        {
            for (j = fb_inf.w - k; j < fb_inf.w; ++j)
            {
                if(j>(fb_inf.w - k))
                    fb_pixel(fb_inf, j, i, buf32[(fb_inf.w - k) + j / 20 + i * fb_inf.w]);
                else
                    fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
            }
        }
        usleep(1000);
        for(i = 0; i < fb_inf.h; ++i)
        {
            for (j = 0; j < fb_inf.w - k; ++j)
            {
                fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
            }
        }
    }
    for(i = 0; i < fb_inf.h; ++i)
    {
        for (j = 0; j < fb_inf.w; ++j)
        {
            fb_pixel(fb_inf, j, i, buf32[j + i * fb_inf.w]);
        }
    }

	free(buf24);
	free(scale_buf);
	free(buf32);
	return 0;
}
int rotate(char *filename,char *filename2, fb_info fb_inf)
{
	fb_info jpeg_inf;
    fb_info scale_inf;
    int i, j, k;
	scale_inf = fb_inf;
	buf24 = decode_jpeg(filename, &jpeg_inf);

    scale_inf.w -= 4;
    for(k = 10; k < (fb_inf.w - fb_inf.w % 10); k+=10)
    {
        scale_inf.w -= 10;
        scale_buf = scale24(buf24, scale_inf, jpeg_inf);
	    buf32 = rgb24to32(scale_buf, scale_inf);
        for(i = 0; i < scale_inf.h; ++i)
        {
            for (j = 0; j < scale_inf.w; ++j)
            {
                fb_pixel(fb_inf, k / 2 + j, i, buf32[j + i * scale_inf.w]);
            }
        }
        free(scale_buf);
        free(buf32);
        for(i = 0; i < fb_inf.h; ++i)
        {
            for (j = 0; j < k / 2 + 1; ++j)
            {
                fb_pixel(fb_inf, j, i, 0x00);
            }
            for (j = fb_inf.w - k / 2 - 4; j < fb_inf.w; ++j)
            {
                fb_pixel(fb_inf, j, i, 0x00);
            }
        }
        if(msg[0] == 2)
            return 1;
    }
    free(buf24);
	buf24 = decode_jpeg(filename2, &jpeg_inf);
    scale_inf.w = 10;
    for(k = (fb_inf.w - fb_inf.w % 10); k > 10; k-=10)
    {
        scale_inf.w += 10;
        scale_buf = scale24(buf24, scale_inf, jpeg_inf);
	    buf32 = rgb24to32(scale_buf, scale_inf);
        for(i = 0; i < scale_inf.h; ++i)
        {
            for (j = 0; j < scale_inf.w; ++j)
            {
                fb_pixel(fb_inf, k / 2 + j, i, buf32[j + i * scale_inf.w]);
            }
        }
        free(scale_buf);
        free(buf32);
        for(i = 0; i < fb_inf.h; ++i)
        {
            for (j = 0; j < k / 2; ++j)
            {
                fb_pixel(fb_inf, j, i, 0x00);
            }
            for (j = fb_inf.w + 100 - k / 2; j < fb_inf.w; ++j)
            {
                fb_pixel(fb_inf, j, i, 0x00);
            }
        }
    }
    free(buf24);
    return 0;
}
int cr_spiral(char *filename, fb_info fb_inf)
{
    char assigned[25] = {0};
    int i = 0, counter = 0;
    int xt = 5, yt = 5, total = 25, inc = 1;
    while(counter != 25)
    {
        i += inc;
        if(i == total / yt);
        {
            inc = xt;
        }
        if(!assigned[i])
        {
            assigned[i] = 1;
        }
        if(msg[0] == 2)
            return 1;
    }
    return 0;
}
/*int spiral(char *filename, fb_info fb_inf)
{
	fb_info jpeg_inf;
    int i, j, k, count_x = 0, count_y = 0;
    int temp;
	
	buf24 = decode_jpeg(filename, &jpeg_inf);
	scale_buf = scale24(buf24, fb_inf, jpeg_inf);
	buf32 = rgb24to32(scale_buf, fb_inf);

    while(1)
    {
        for(j = count_y; j < fb_inf.w - count_y; j++)
        {
            fb_pixel(fb_inf, j, (count_x % 2 == 0) ? count_x / 2: fb_inf.h - count_x / 2, buf32[j + i * fb_inf.w]);
        }
        count_x++;
        for(i = count_x; i < fb_inf.h - count_x; i++)
        {
            fb_pixel(fb_inf, (count_y % 2 == 0) ? fb_inf.w - count_y / 2: count_y / 2, i, buf32[j + i * fb_inf.w]);
        }
        count_y++;
        usleep(10000);
    }
	free(buf24);
	free(scale_buf);
	free(buf32);
	return 0;
}*/
/*int display_circle(char *filename, fb_info fb_inf, midx, midy, radis)
{
	fb_info jpeg_inf;
    fb_info pic_inf = fb_inf;
    pic_inf.w = width;
    pic_inf.h = hight;
	int i, j;
}*/
