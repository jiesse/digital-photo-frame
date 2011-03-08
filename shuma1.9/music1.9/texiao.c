
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "common.h"
#include "ymqqqq.h"

#define TIM 100000

//#define DEBUG 0
//#if DEBUG
//#endif
//u32_t *buf32;
extern unsigned int msg[4];


int texiao1_rive(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    fb_info jpeg_inf2;
    fb_info new_inf;
    int x = 0;
    //int y = 0;
    new_inf.h = fb_inf.h; 
    new_inf.w = fb_inf.w/2; 
    int z = 0;
    int m = 0;

    u8_t *buf24 = decode_jpeg("zhezhao.jpg", &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, new_inf, jpeg_inf);
    u32_t *bufe32 = rgb24to32(scale_buf, new_inf);

    u8_t *buf242 = decode_jpeg(filename, &jpeg_inf2);
    u8_t * scale_buf2 = scale24(buf242, fb_inf, jpeg_inf2);
    u32_t *bufe322 = rgb24to32(scale_buf2, fb_inf);

    int i, j;
    for(i = 0; i < new_inf.h; ++i){
        m = i*fb_inf.w;
        for (j = 0; j < 20; ++j){
            fb_pixel(fb_inf, x+j, i, bufe322[x+j + m]);
        }
    }
    for (x = 0; x < fb_inf.w; x = x+10) 
    {
        for(i = 0; i < new_inf.h; ++i){
            z = i*new_inf.w;
            m = i*fb_inf.w;
            for (j = 0; j < new_inf.w; ++j){
                if(bufe32[j + z] < 0x080000 )
                    fb_pixel(fb_inf, x+j, i, bufe322[x+j + m]);
            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(TIM);
    }

    free(buf24);
    free(bufe32);
    free(scale_buf2);
    free(scale_buf);
    free(buf242);
    free(bufe322);
    return 0;

}
int texiao2_sector(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    fb_info jpeg_inf2;
    fb_info new_inf;
    int x = 0;
    //int y = 0;
    new_inf.h = fb_inf.h; 
    new_inf.w = fb_inf.w; 

    u8_t *buf24 = decode_jpeg("zhezhao2.jpg", &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, new_inf, jpeg_inf);
    u32_t *bufe32 = rgb24to32(scale_buf, new_inf);

    u8_t *buf242 = decode_jpeg(filename, &jpeg_inf2);
    u8_t * scale_buf2 = scale24(buf242, fb_inf, jpeg_inf2);
    u32_t *bufe322 = rgb24to32(scale_buf2, fb_inf);

    int i, j;
    for (x = 0; x < 0x0ffffff; x = x+0x100000) 
    {
        for (j = 0; j < new_inf.w; ++j){
            for(i = 0; i < new_inf.h; ++i){
                if(bufe32[j + i*new_inf.w] < x )
                    fb_pixel(fb_inf, j, i, bufe322[j + i * new_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(TIM);
    }
        for (j = 0; j < new_inf.w; ++j){
            for(i = 0; i < new_inf.h; ++i){
                    fb_pixel(fb_inf, j, i, bufe322[j + i * new_inf.w]);
            }
        }

    free(buf24);
    free(bufe32);
    free(scale_buf2);
    free(scale_buf);
    free(buf242);
    free(bufe322);
    return 0;

}
int texiao3_rain(char *filename, fb_info fb_inf)
{
    int knum = 41;
    int num;
    int i, j;
    int kk[42] = {0};
    int km[42] = {0};


    fb_info jpeg_inf;
    int h = fb_inf.h;
    int w = fb_inf.w;
    int k = 0;

    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

    srand(time(NULL));

    for (k = 0; k < 41; k++) 
    {
        kk[k] = k;
    }
    for (k = 0; k < 41; k++) 
    {
        num = rand()%knum;
        km[k] = kk[num];
        for (i = num; i < knum-1; i++) 
        {
            kk[i] = kk[i+1];
        }
        knum--; 
    }
    for (k = 0; k < 41; k++) 
    {
        kk[k] = rand()%10 + 1;
    }
    for(i = 0; i < fb_inf.h*2; ++i)
    {
        for (k = 0; k < 41; k++) 
        {
            if((i > h/10*kk[k])&&(i -h/10*kk[k] < h))
            {
                for (j = w/40*km[k]; j < w/40*(km[k]+1); ++j)
                {
                    fb_pixel(fb_inf, j, i-h/10*kk[k], buf32[j + (i-h/10*kk[k]) * fb_inf.w]);
                }
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
int texiao4_spark(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    fb_info jpeg_inf2;
    fb_info new_inf;
    int x = 0;
    //int y = 0;
    new_inf.h = fb_inf.h; 
    new_inf.w = fb_inf.w; 
    int z = 0;

    u8_t *buf24 = decode_jpeg("huohua.jpg", &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, new_inf, jpeg_inf);
    u32_t *bufe32 = rgb24to32(scale_buf, new_inf);

    u8_t *buf242 = decode_jpeg(filename, &jpeg_inf2);
    u8_t * scale_buf2 = scale24(buf242, fb_inf, jpeg_inf2);
    u32_t *bufe322 = rgb24to32(scale_buf2, fb_inf);

    int i, j;
        for (j = 0; j < new_inf.w; ++j){
            for(i = 0; i < new_inf.h; ++i){
                    fb_pixel(fb_inf, j, i, 0x0);
            }
        }
    for (x = 0x1000000; x >= 0x200000; x = x-0x50000) 
    {
        for(i = 0; i < new_inf.h; ++i){
            z = i*new_inf.w;
            for (j = 0; j < new_inf.w; ++j){
                if(bufe32[j + z] > x )
                    fb_pixel(fb_inf, j, i, bufe322[j + z]);
            }
        }
        usleep(TIM);
        if(msg[0] == 2)
            return 1;
    }
        for(i = 0; i < new_inf.h; ++i){
            z = i*new_inf.w;
            for (j = 0; j < new_inf.w; ++j){
                    fb_pixel(fb_inf, j, i, bufe322[j + z]);
            }
        }

    free(buf24);
    free(bufe32);
    free(scale_buf2);
    free(scale_buf);
    free(buf242);
    free(bufe322);
    return 0;

}
int texiao5(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    fb_info jpeg_inf2;
    fb_info new_inf;
    int x = 0;
    //int y = 0;
    new_inf.h = fb_inf.h; 
    new_inf.w = fb_inf.w; 

    u8_t *buf24 = decode_jpeg("zhecai.jpg", &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, new_inf, jpeg_inf);
    u32_t *bufe32 = rgb24to32(scale_buf, new_inf);

    u8_t *buf242 = decode_jpeg(filename, &jpeg_inf2);
    u8_t * scale_buf2 = scale24(buf242, fb_inf, jpeg_inf2);
    u32_t *bufe322 = rgb24to32(scale_buf2, fb_inf);

    int i, j;
    for (x = 0; x < 0x1100000; x = x+0x100000) 
    {
        for (j = 0; j < new_inf.w; ++j){
            for(i = 0; i < new_inf.h; ++i){
                if(bufe32[j + i*new_inf.w] < x )
                    fb_pixel(fb_inf, j, i, bufe322[j + i * new_inf.w]);
            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(TIM);
        //usleep(200000);
    }

    free(buf24);
    free(bufe32);
    free(scale_buf2);
    free(scale_buf);
    free(buf242);
    free(bufe322);
    return 0;

}
int texiao6_heart(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    fb_info jpeg_inf2;
    fb_info new_inf;

    u8_t *buf24 = decode_jpeg("xingxing.jpg", &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *bufe32 = rgb24to32(scale_buf, fb_inf);

    u8_t *buf242 = decode_jpeg(filename, &jpeg_inf2);
    u8_t * scale_buf2 = scale24(buf242, fb_inf, jpeg_inf2);
    u32_t *bufe322 = rgb24to32(scale_buf2, fb_inf);

    int i, j;
    int k;
    int x, y;
    int z = 0;
    int m = 0;
    for (k = 1; k < 50 ; k++) 
    {
        new_inf.h = fb_inf.h*k*0.02; 
        new_inf.w = fb_inf.w*k*0.02; 

        scale_buf = scale24(buf24, new_inf, jpeg_inf); 
        bufe32 = rgb24to32(scale_buf, new_inf);

        x = fb_inf.w/2 - new_inf.w/2;
        y = fb_inf.h/2 - new_inf.h/2;

        for(i = y; i < y + new_inf.h; ++i){
            z = (i-y)*new_inf.w -x;
            m = i*fb_inf.w;
            for (j = x; j < x +new_inf.w; ++j){
                if(bufe32[j + z] < 0x800000 )
                    fb_pixel(fb_inf, j, i, bufe322[j + m]);

            }
        }
        usleep(TIM/k);
        //usleep((int)(1000/k/2));
        if(msg[0] == 2)
            return 1;
        free(bufe32);
        free(scale_buf);
    }
    for (k = 50 ; k < 80 ; k++) 
    {
        new_inf.h = fb_inf.h*k*0.02; 
        new_inf.w = fb_inf.w*k*0.02; 

        scale_buf = scale24(buf24, new_inf, jpeg_inf); 
        bufe32 = rgb24to32(scale_buf, new_inf);

        x = new_inf.w/2 - fb_inf.w/2;
        y = new_inf.h/2 - fb_inf.h/2;

        for(i = 0; i < fb_inf.h; ++i){
            z = x+(i+y)*new_inf.w;
            m = i*fb_inf.w;
            for (j = 0; j < fb_inf.w; ++j){
                if(bufe32[j + z] < 0x800000 )
                    fb_pixel(fb_inf, j, i, bufe322[j + m]);

            }
        }
        //usleep((int)(1000/k));
        usleep(TIM/k/3);
        if(msg[0] == 2)
            return 1;
        free(bufe32);
        free(scale_buf);
    }
    for (j = 0; j < fb_inf.w; ++j){
    m = i*fb_inf.w;
    for(i = 0; i < fb_inf.h; ++i){
    fb_pixel(fb_inf, j, i, bufe322[j + m]);
    
   }
   }

    free(buf24);
    free(scale_buf2);
    free(buf242);
    free(bufe322);
    return 0;

}
int texiao7_circle(char *filename, fb_info fb_inf)
{
    int i, j;
    int k = 0;


    fb_info jpeg_inf;
    int h = fb_inf.h;
    int w = fb_inf.w;

    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);


    while(k<640)
    {
         for (j = 0; j < h; j++) 
                for (i = 0; i < w; i++) 
                   if(((i-w/2)*(i-w/2)+(j-h/2)*(j-h/2)>=k*k)&&((i-w/2)*(i-w/2)+(j-h/2)*(j-h/2)<(k+1)*(k+1)))
                     fb_pixel(fb_inf, i, j, buf32[i + j * fb_inf.w]);
         k++;
        if(msg[0] == 2)
            return 1;
    }

    free(buf24);
    free(scale_buf);
    free(buf32);

    return 0;
}
int texiao8_square(char *filename, fb_info fb_inf)
{
    int i, j;
    int k;


    fb_info jpeg_inf;
    int h = fb_inf.h;
    int w = fb_inf.w;

    u8_t *buf24 = decode_jpeg(filename, &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, fb_inf, jpeg_inf);
    u32_t *buf32 = rgb24to32(scale_buf, fb_inf);

   for (k = 0; k < h/2; k++) 
   {
       for (j = h/2-k; j <= h/2+k; j++) 
       {
            fb_pixel(fb_inf, w/2+k, j, buf32[w/2+k + j * fb_inf.w]);
            fb_pixel(fb_inf, w/2-k, j, buf32[w/2-k + j * fb_inf.w]);
            //dis_px(512+k,j,p,kkkk);
            //dis_px(512-k,j,p,kkkk);
       }
       for (i = w/2-k; i <= w/2+k; i++) 
       {
            fb_pixel(fb_inf, i, h/2+k, buf32[i + (h/2+k)* fb_inf.w]);
            fb_pixel(fb_inf, i, h/2-k, buf32[i + (h/2-k) * fb_inf.w]);
            //dis_px(i,384+k,p,kkkk);
            //dis_px(i,384-k,p,kkkk);
       }
        if(msg[0] == 2)
            return 1;
        //usleep(3000);
        usleep(TIM/5);
   }
   for (i = h/2; i < w/2; i++) 
   {
       for (j = 0; j < h; j++) 
       {
            fb_pixel(fb_inf, i+w/2, j, buf32[i+w/2 + j * fb_inf.w]);
            fb_pixel(fb_inf, w/2-i, j, buf32[w/2-i + j * fb_inf.w]);
            //dis_px(i+512,j,p,kkkk);
            //dis_px(512-i,j,p,kkkk);
       }
        if(msg[0] == 2)
            return 1;
        //usleep(3000);
        usleep(TIM/5);
   }


    free(buf24);
    free(scale_buf);
    free(buf32);

    return 0;
}
int texiao9_wave(char *filename, fb_info fb_inf)
{
    fb_info jpeg_inf;
    fb_info jpeg_inf2;
    fb_info new_inf;
    int x = 0;
    //int y = 0;
    int z = 0;
    int m = 0;
    new_inf.h = fb_inf.h; 
    new_inf.w = fb_inf.w/2; 

    u8_t *buf24 = decode_jpeg("shuiwen4.jpg", &jpeg_inf);
    u8_t * scale_buf = scale24(buf24, new_inf, jpeg_inf);
    u32_t *bufe32 = rgb24to32(scale_buf, new_inf);

    u8_t *buf242 = decode_jpeg(filename, &jpeg_inf2);
    u8_t * scale_buf2 = scale24(buf242, fb_inf, jpeg_inf2);
    u32_t *bufe322 = rgb24to32(scale_buf2, fb_inf);

    int i, j;
    for(i = 0; i < fb_inf.h; ++i){
        z = i*fb_inf.w;
        for (j = 10; j < fb_inf.w; ++j){
            fb_pixel(fb_inf, j, i, bufe322[j-10 + z]);
        }
    }
    for (x = 0; x < fb_inf.w - 100; x = x+10) 
    {
        for(i = 0; i < new_inf.h; ++i){
            z = i*fb_inf.w;
            m = i*new_inf.w;
            for (j = 10; j < new_inf.w; ++j){
                if(bufe32[j + m] < 0x080000 )
                    fb_pixel(fb_inf, x+j, i, bufe322[x+j + z]);
                else
                fb_pixel(fb_inf, x+j, i, bufe322[x+j-10 + z]);

            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(TIM);
    }
    for (x = 0; x < fb_inf.w - 100; x = x+10) 
    {
        for(i = 0; i < new_inf.h; ++i){
            z = i*fb_inf.w;
            m = i*new_inf.w;
            for (j = 10; j < new_inf.w; ++j){
                if(bufe32[j + m] < 0x080000 )
                    fb_pixel(fb_inf, x+j, i, bufe322[x+j + z]);
                else
                    fb_pixel(fb_inf, x+j, i, bufe322[x+j-10 + z]);

            }
        }
        if(msg[0] == 2)
            return 1;
        usleep(TIM);
        //usleep(10000);
    }

    free(buf24);
    free(bufe32);
    free(scale_buf2);
    free(scale_buf);
    free(buf242);
    free(bufe322);
    return 0;

}
int rand_num(int k)
{
    unsigned int num = k;
    while(num == k)
    num = rand()%9 + 1;
    return num;
}
int rand_num2(int k)
{
    unsigned int num = k;
    while(num == k)
    num = rand()%6 + 1;
    return num;
}

int texiao(fb_info fb_inf)
{
    char filename[25] = {0};
    char filename2[25] = "fengjing.jpg";
    unsigned int k = 0;
    //int ret;
    #if 1
    while(1)
    {
    #if 1
    srand(time(NULL));

    //display_jpeg("kkkkkkkkkkk2.jpg", fb_inf);

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(grain(filename,fb_inf))
    break;

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(fly_left_right(filename,fb_inf))
    break;

    //k = rand_num(k);
    //sprintf(filename,"%d.jpg",k);
    //fly_multi(filename, fb_inf);
    //printf("33333333333333\n");

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(disp_expand(filename, fb_inf))
    break;
    //printf("44444444444444\n");

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(random_grating(filename, fb_inf))
    break;
    //printf("55555555555\n");

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
   if( scan(filename, fb_inf))
    break;

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(tremor(filename, fb_inf))
    break;


    srand(time(NULL));



    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(push(filename, fb_inf))
    break;

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(shutter(filename, fb_inf))
    break;

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(vertical_grating(filename, fb_inf))
    break;

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(mosaic(filename, fb_inf))
    break;

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(vacum(filename, filename2, fb_inf))
    break;

    k = rand_num(k);
    sprintf(filename,"%d.jpg",k);
    if(rotate(filename, filename2, fb_inf))
    break;

    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
    if(texiao1_rive(filename, fb_inf))
    break;


    srand(time(NULL));



    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
    if(texiao2_sector(filename, fb_inf))
    break;

    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
    if(texiao3_rain(filename, fb_inf))
    break;

    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
    if(texiao4_spark(filename, fb_inf))
    break;

    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
    if(texiao5(filename, fb_inf))
    break;

    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
    if(texiao6_heart(filename, fb_inf))
    break;

    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
   if( texiao7_circle(filename, fb_inf))
    break;
#endif

    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
    if(texiao8_square(filename, fb_inf))
    break;

    k = rand_num2(k);
    sprintf(filename,"%d.jpg",k);
    if(texiao9_wave(filename, fb_inf))
    break;

    }
    #endif

    return 0;
}

