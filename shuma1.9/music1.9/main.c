#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>

#include "common.h"
#include "ccc.h" 
#include "start.h"
#include "pic_change.h" 
#include "texiao.h" 

#include "mad.h" 
#define MAXLINE 80

int test(fb_info fb_inf);
int pipe_fd[2];
int *mus;
unsigned int msg[4] = {0,300,0};
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *read_pipe(void*arg)
{
    int n;
    for(;;)
    {
        n = read(pipe_fd[0],&msg,20);
    }
}
char *zitixianshi[]={"回到过去","那些逝去的日子","一去不复返了","80后的回忆","一辈子不会忘记","THE END","制作者","第二组","姚明月 高金龙 范振平 李晓龙"};

int music_flag[3];

int main(int argc, char *argv[]) 
{
    int i;
    int z, n;
    int fd;
    int fk, fmu;
    char buf[3] = "aa";
    long col = 0xffffff;
    int position;
    pid_t pida, pidb;
    fb_info fb_inf;
    pthread_t pid;

    srand(time(NULL));

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++

    fmu = open("music_flag",O_RDWR|O_CREAT, 0644);
    n = lseek(fmu, 13 - 1, SEEK_SET);
    if(n < 0)
    {
        perror("failed");
    }
    write(fmu, buf, 1); 
    mus = mmap(0, 13, PROT_READ|PROT_WRITE, MAP_SHARED, fmu, 0); 
    *(mus + 1) = 1;
    *(mus + 2) = 1;


    //+++++++++++++++++++++++++++++++++++++++++++++++++++++


    if (init_fb(&fb_inf) < 0){
        fprintf(stderr, "Error initial framebuffer\n");
        return 1;
    }
    if(pipe(pipe_fd)<0)
    {
        perror("pipe"); 
        exit(1);
    }
    fd = fork();
    if(fd == 0)
    {
        close(pipe_fd[0]);
        test_mouse(fb_inf);

    }
    if(fd > 0)
    {
        pida = fd;
        //texiao(fb_inf);
        //test(fb_inf);
        //while(1)
        //{
        //}
        fk = fork();
        if(fk == 0)
        {
            musicplay();

        }
        if(fk > 0)
        {
            pidb = fk;
            close(pipe_fd[1]);
            pthread_create(&pid,NULL,read_pipe,NULL);

            display_kaiji(fb_inf);
            if (init_ft("shaonv.ttf.ttf", 0) != 0){
                fprintf(stderr, "Error initial font\n")	;
                return 1;
            }

            position = fb_inf.w*100/1024 ;
            for (z = 0; z < 5; z++) 
            {
                display_string(zitixianshi[z], position, (z+1)*position, fb_inf, 0xffffff);
                sleep(1);
            }
            while(col > 0x30303)
            {
                for (z = 0; z < 5; z++) 
                {
                    display_string(zitixianshi[z], position, (z+1)*position , fb_inf, col);
                }
                usleep(100000);
                col -= 0x60606;
            }
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if 1
            start(fb_inf);
            //texiao(fb_inf);
            //while(1)
            //{
            //}
            judge_navi(fb_inf);
            while(navi_flag)
            {
                judge_navi(fb_inf);
                //scan
                while(navi_flag == 1)
                {
                    display_screen("beijing.jpg", fb_inf,10);
                    cartoon_pic(fb_inf);
                    judge_scan(fb_inf);

                    judge_next(fb_inf);
                }
                //huandeng
                if(navi_flag == 2)
                {
                    texiao(fb_inf);
                    start(fb_inf);
                    judge_navi(fb_inf);
                }
                if(navi_flag == 3)
                {
                    display_jpeg("music8.jpg",fb_inf);
                    judge_music(fb_inf);
                    start(fb_inf);
                    judge_navi(fb_inf);
                }
                //game
                if(navi_flag == 4)
                {
                    while(1)
                    {
                        if(test(fb_inf))
                        break;
                        print_game(fb_inf);
                        if(judge_game(fb_inf))
                        break;
                    }
                    start(fb_inf);
                }
                if(navi_flag == 5)
                {
                    navi_flag = 0;
                }
            }
#endif
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //guanji

            display_kaiji(fb_inf);

            for (z = 5; z < 9; z++) 
            {


                if(z == 5) display_string(zitixianshi[z], position*5, (z-4)*position*1.5, fb_inf, 0xffffff);
                else display_string(zitixianshi[z], position*4, (z-4)*position*1.5, fb_inf, 0xffffff);
            }

            col = 0xede7e7;
            for(i = 0; i < 200; i++) 
            {
                for (z = 5; z < 9; z++) 
                {


                    if(z == 5) display_string(zitixianshi[z], position*5, (z-4)*position*1.5, fb_inf, col);
                    else display_string(zitixianshi[z], position*4, (z-4)*position*1.5, fb_inf, col);
                }
                sleep(1/10);
                col -= 0xc0909;

            }
            xiaoshi(fb_inf);
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            kill(pida,SIGINT);
            kill(pidb,SIGINT);
            
            wait(NULL);
            wait(NULL);
            munmap(fb_inf.fbmem, fb_inf.w * fb_inf.h * fb_inf.bpp / 8);
        }
    }
    return 0;
}

