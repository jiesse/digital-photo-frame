/*
 *  2010.2.25
 *  mouse.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>

#include "common.h"

/* ********** .h */
typedef struct 
{
	int x;
	int y;
	int button;
}mouse_event_t;
		
extern int mouse_open(char *device_name, int *fd);
extern int mouse_parse(const u8_t *buf, mouse_event_t* mevent);

extern int  fb_savecursor (fb_info fb_inf,int x,int y);
extern int fb_restorecursor(fb_info fb_inf, int x, int y);
extern int fb_drawcursor(fb_info fb_inf, int x, int y);
extern int test_mouse(fb_info fb_inf);

/* **************** end .h */
extern int pipe_fd[2];


#define C_WIDTH  10
#define C_HEIGHT 17
#define T___     0XFFFFFFFF
#define BORD     0x0
#define X___     0xFFFF
static unsigned long cursor_pixel[C_WIDTH * C_HEIGHT] = {
  BORD, T___, T___, T___, T___, T___, T___, T___, T___, T___,
  BORD, BORD, T___, T___, T___, T___, T___, T___, T___, T___,
  BORD, X___, BORD, T___, T___, T___, T___, T___, T___, T___,
  BORD, X___, X___, BORD, T___, T___, T___, T___, T___, T___,
  BORD, X___, X___, X___, BORD, T___, T___, T___, T___, T___,
  BORD, X___, X___, X___, X___, BORD, T___, T___, T___, T___,
  BORD, X___, X___, X___, X___, X___, BORD, T___, T___, T___,
  BORD, X___, X___, X___, X___, X___, X___, BORD, T___, T___,
  BORD, X___, X___, X___, X___, X___, X___, X___, BORD, T___,
  BORD, X___, X___, X___, X___, X___, X___, X___, X___, BORD,
  BORD, X___, X___, X___, X___, X___, BORD, BORD, BORD, BORD,
  BORD, X___, X___, BORD, X___, X___, BORD, T___, T___, T___,
  BORD, X___, BORD, T___, BORD, X___, X___, BORD, T___, T___,
  BORD, BORD, T___, T___, BORD, X___, X___, BORD, T___, T___,
  T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
  T___, T___, T___, T___, T___, BORD, X___, X___, BORD, T___,
  T___, T___, T___, T___, T___, T___, BORD, BORD, T___, T___
};
static unsigned long save_cursor[C_WIDTH * C_HEIGHT];

int test_mouse(fb_info fb_inf)
{
	int mfd;
	mouse_open(NULL, &mfd);

	int m_x = fb_inf.w / 2;
	int m_y = fb_inf.h / 2;
    fb_drawcursor(fb_inf, m_x, m_y);

	u8_t buf[8];
    unsigned int mouce_msg[3];
    
	mouse_event_t mevent;

    int flag = 0;

    //for(i = 0; i < 1000; ++i){
    while(1) {
        //mouse_parse(buf,&mevent);
        //fb_drawcursor(fb_inf, m_x, m_y);
		int n = read(mfd, buf, 8);
        if(flag == 1)
        {
        flag = 0;
        fb_drawcursor(fb_inf, m_x, m_y);
        continue;
        
       }
        if (n != -1){
			
			if((buf[0] & 0x7) ==1)
            {
                flag = 10;
                mouce_msg[0] =buf[0] & 0x7;
                mouce_msg[1] =m_x;
                mouce_msg[2] =m_y;
                write(pipe_fd[1],&mouce_msg,sizeof(mouce_msg));
                //m_x +=C_WIDTH;
                //m_y +=C_HEIGHT;
			    fb_restorecursor(fb_inf, m_x, m_y);
                usleep(500000);
                fb_drawcursor(fb_inf, m_x, m_y);
                //fb_restorecursor(fb_inf, m_x, m_y);
                continue;
            }
            //if(0)
            /*
            if(flag > 1)
            {
                flag--;
			mouse_parse(buf,&mevent);
			m_x += mevent.x;
			m_y += mevent.y;
            m_x = m_x>0?m_x:0;
            m_y = m_y>0?m_y:0;
            
            m_x = m_x<(fb_inf.w-C_WIDTH)?m_x:(fb_inf.w-C_WIDTH);
            m_y = m_y<(fb_inf.h-C_HEIGHT)?m_y:(fb_inf.h-C_HEIGHT);



            mouce_msg[0] =buf[0] & 0x7;
            mouce_msg[1] =m_x;
            mouce_msg[2] =m_y;

            write(pipe_fd[1],&mouce_msg,sizeof(mouce_msg));
                //fb_drawcursor(fb_inf, m_x, m_y);
                continue;
                
            }
            */
               
			mouse_parse(buf,&mevent);
            //printf("dx:%d\tdy:%d\n", mevent.x, mevent.y);
            //printf("mx:%d\tmy:%d\n", m_x, m_y);

			fb_restorecursor(fb_inf, m_x, m_y);
		
			m_x += mevent.x;
			m_y += mevent.y;
            m_x = m_x>0?m_x:0;
            m_y = m_y>0?m_y:0;
            
            m_x = m_x<(fb_inf.w-C_WIDTH)?m_x:(fb_inf.w-C_WIDTH);
            m_y = m_y<(fb_inf.h-C_HEIGHT)?m_y:(fb_inf.h-C_HEIGHT);



            mouce_msg[0] =buf[0] & 0x7;
            mouce_msg[1] =m_x;
            mouce_msg[2] =m_y;

            //printf("%d\n",mouce_msg[0]);
            //printf("%d\n",mouce_msg[1]);
            //printf("%d\n",mouce_msg[2]);
            write(pipe_fd[1],&mouce_msg,sizeof(mouce_msg));
            //usleep(1000);
			fb_drawcursor(fb_inf, m_x, m_y);
            
            //fb_restorecursor(fb_inf, m_x, m_y);
            //fb_drawcursor(fb_inf, m_x, m_y);
        }
		
		usleep(10000);
    }
	return 0;
}

int mouse_open(char *device_name, int *fd)
{
	if (NULL == device_name)
		device_name = "/dev/input/mice";
	
	*fd = open(device_name, O_RDONLY | O_NONBLOCK);
	if(*fd < 0){
		fprintf(stderr, "Open %s:%s\n", device_name, strerror(errno));
		exit(1);
	}
	
	return 0;
}

int mouse_parse(const u8_t *buf, mouse_event_t* mevent)
{
	switch(buf[0] & 0x7){
	case 1:						/* left */
		mevent->button = 1;
		break;
	case 2:
		mevent->button = 2;		/* right */
		break;
	case 4:
		mevent->button = 3;		/* middle */
		break;
	default:
		break;
	}
#if 0
	mevent->x = buf[1];
	mevent->y = buf[2];
#else
	mevent->x =  buf[1] - ((buf[0] & 0x10) ? 256 : 0);
	mevent->y = -buf[2] + ((buf[0] & 0x20) ? 256 : 0); 
#endif
	return 0;
}

/* cursor */

static int fb_pixel_m(fb_info fb_inf, int x, int y, u32_t color)
{
/* 16 bits */
	//u8_t *pos = (u8_t*)fb_inf.fbmem + (fb_inf.w * y + x) * fb_inf.bpp/8;
	
	fb_inf.fbmem[fb_inf.w * y + x] = color;

	//switch (fb_inf.bpp){
    //case 32:				  /* 32 bits */
    //*(pos + 3) = color >> 24;
    //case 24:				  /* 24 bits */
    //*(pos + 2) = color >> 16;
    //case 16:	  		       /* 16 bits */
    //*(pos + 1) = color >> 8;
    //case 8:			      /* 8 bits */
    //*pos = color;
    //return 0;
    //default:
    //return -1;
    //}
    //return -1;
    return 0;
}

int fb_restorecursor(fb_info fb_inf, int x, int y)
{
	int i, j;
	u32_t color;
	for (j = 0;j<C_HEIGHT;j++){
		for (i = 0;i<C_WIDTH;i++){
			color = save_cursor[i + j * C_WIDTH];
			fb_pixel_m(fb_inf,x+i,y+j,color);
		}
	}
	
	return 0;
}
int  fb_savecursor (fb_info fb_inf,int x,int y)
{
	int i,j;
	u32_t color;

	for (j = 0;j<C_HEIGHT;j++)
	{
		for (i = 0;i<C_WIDTH;i++)
		{
  			//color = *(u32_t*)(fb_inf.fbmem + ((y + j) * fb_inf.w + x + i) * fb_inf.bpp/8);
  			color = fb_inf.fbmem[(y + j) * fb_inf.w + x + i];
			save_cursor[i + j * C_WIDTH] = color;
		}
	}
	return 0;
}

int  fb_drawcursor(fb_info fb_inf, int x, int y)
{
	int i,j;
	unsigned int color;

	fb_savecursor (fb_inf,x,y);

	for (j = 0;j<C_HEIGHT;j++)
	{
		for (i = 0;i<C_WIDTH;i++)
		{
			color = cursor_pixel[i + j * C_WIDTH];
			if (color != T___)
				fb_pixel_m(fb_inf,x+i,y+j,color);
		}
	}
	return 0;
}

