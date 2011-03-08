/*
 * libmad - MPEG audio decoder library
 * Copyright (C) 2000-2004 Underbit Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: minimad.c,v 1.4 2004/01/23 09:41:32 rob Exp $
 */


# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <sys/soundcard.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <malloc.h>
#include <string.h>
# include "mad.h"

#undef putchar
/*
 * This is perhaps the simplest example use of the MAD high-level API.
 * Standard input is mapped into memory via mmap(), then the high-level API
 * is invoked with three callbacks: input, output, and error. The output
 * callback converts MAD's high-resolution PCM samples to 16 bits, then
 * writes them to standard output in little-endian, stereo-interleaved
 * format.
 */
/***********************************************************/
#define BUFSIZE 4096
struct buffer {
    FILE  *fp;                    /*file pointer*/
    unsigned int  flen;           /*file length*/
    unsigned int  fpos;           /*current position*/
    unsigned char fbuf[BUFSIZE];  /*buffer*/
    unsigned int  fbsize;         /*indeed size of buffer*/
};
typedef struct buffer mp3_file;

extern int *mus;
extern int music_flag[4];
int music_num;
int kkk;
int flag;
extern pthread_mutex_t counter_mutex;


/***********************************************************/
int soundfd;                 /*soundcard file*/

unsigned int prerate =0;
int writedsp(int c)
{
    return write(soundfd, (char *)&c, 1);
}

void set_dsp()
{
    //int rate = 441000;
    int format = AFMT_S16_LE;
    int channels = 2;

    soundfd = open("/dev/dsp", O_WRONLY);
    //ioctl(soundfd, SNDCTL_DSP_SPEED, &rate);
    ioctl(soundfd, SNDCTL_DSP_SETFMT, &format);
    ioctl(soundfd, SNDCTL_DSP_CHANNELS, &channels);
}

//static int decode(unsigned char const *, unsigned long);
/********************************************************************/
static int decode(mp3_file *mp3fp);
/********************************************************************/
int musicplay(void)
{
/*
    struct stat stat;
    void *fdm;
    int fd;

    fd = open("/home/jiesse/src-teach/hetang.mp3",O_RDONLY);
    if(fd < 0)
    {

        printf("error read music file\n");
    }

    if (fstat(fd, &stat) == -1 ||
            stat.st_size == 0)
        return 2;

    fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (fdm == MAP_FAILED)
        return 3;

    set_dsp();
    decode(fdm, stat.st_size);

    if (munmap(fdm, stat.st_size) == -1)
        return 4;

    close(soundfd);

    close(fd);
    return 0;
*/
/**********************************************************************/
 long flen, fsta, fend;
 //int  dlen;
  mp3_file *mp3fp;
  char music_name[12] = {0};
  //int k;

  mp3fp = (mp3_file *)malloc(sizeof(mp3_file));
  //mp3fp->fp = fopen("/home/yaomoon/study/dpf-30/music/dyhhd.mp3", "r");
  set_dsp();
  while(*(mus + 1) == 1)
  {
      //printf("shunxu\n");
      if(music_num == 0)
      music_num = 1;
      sprintf(music_name,"%d.mp3",music_num);
      mp3fp->fp = fopen(music_name, "r");

      fsta = ftell(mp3fp->fp);
      fseek(mp3fp->fp, 0, SEEK_END);
      fend = ftell(mp3fp->fp);
      flen = fend - fsta;
      fseek(mp3fp->fp, 0, SEEK_SET);
      fread(mp3fp->fbuf, 1, BUFSIZE, mp3fp->fp);
      mp3fp->fbsize = BUFSIZE;
      mp3fp->fpos   = BUFSIZE;
      mp3fp->flen = flen;

      
      if(!decode(mp3fp))
      {
          music_num++;
          if(music_num == 6)
          music_num = 1;
      }
      else
      {
      }
      *(mus + 2) = music_num;
    fclose(mp3fp->fp);
  }
  while(*(mus + 1) == 2)
  {
      //printf("suiji\n");
      music_num = rand()%5 + 1;
      *(mus + 2) = music_num;
      sprintf(music_name,"%d.mp3",music_num);
      mp3fp->fp = fopen(music_name, "r");

      fsta = ftell(mp3fp->fp);
      fseek(mp3fp->fp, 0, SEEK_END);
      fend = ftell(mp3fp->fp);
      flen = fend - fsta;
      fseek(mp3fp->fp, 0, SEEK_SET);
      fread(mp3fp->fbuf, 1, BUFSIZE, mp3fp->fp);
      mp3fp->fbsize = BUFSIZE;
      mp3fp->fpos   = BUFSIZE;
      mp3fp->flen = flen;

      
      if(!decode(mp3fp))
      {
      }
      else
      {
      }
    fclose(mp3fp->fp);
  }


  close(soundfd);

  return 0;
  /*********************************************************************/
}

/*
 * This is a private message structure. A generic pointer to this structure
 * is passed to each of the callback functions. Put here any data you need
 * to access from within the callbacks.
 */
/***************************************************************
struct buffer {
    unsigned char const *start;
    unsigned long length;
};
***************************************************************/
/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */
/*
    static
enum mad_flow input(void *data,
        struct mad_stream *stream)
{
    struct buffer *buffer = data;

    if (!buffer->length)
        return MAD_FLOW_STOP;

    mad_stream_buffer(stream, buffer->start, buffer->length);

    buffer->length = 0;

    return MAD_FLOW_CONTINUE;
}
*/

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

    static inline
signed int scale(mad_fixed_t sample)
{
    /* round */
    sample += (1L << (MAD_F_FRACBITS - 16));

    /* clip */
    if (sample >= MAD_F_ONE)
        sample = MAD_F_ONE - 1;
    else if (sample < -MAD_F_ONE)
        sample = -MAD_F_ONE;

    /* quantize */
    return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

   static
   enum mad_flow output(void *data,
   struct mad_header const *header,
   struct mad_pcm *pcm)
   {
   unsigned int nchannels, nsamples;
   mad_fixed_t const *left_ch, *right_ch;
   unsigned int rate;


// pcm->samplerate contains the sampling frequency 
rate= pcm->samplerate;

nchannels = pcm->channels;
nsamples  = pcm->length;
left_ch   = pcm->samples[0];
right_ch  = pcm->samples[1];

if(rate != prerate)
{
ioctl(soundfd, SNDCTL_DSP_SPEED, &rate);
prerate = rate;
}
while (nsamples--) {
signed int sample;

// output sample(s) in 16-bit signed little-endian PCM 

sample = scale(*left_ch++);
writedsp((sample >> 0) & 0xff);
writedsp((sample >> 8) & 0xff);

if (nchannels == 2) {
sample = scale(*right_ch++);
writedsp((sample >> 0) & 0xff);
writedsp((sample >> 8) & 0xff);
}
}

return MAD_FLOW_CONTINUE;
}
//*************************************************************
//*************************************************************
//*************************************************************
//*************************************************************
//*************************************************************
//*************************************************************
//input

int kk = 0;
    static
enum mad_flow input(void *data,
        struct mad_stream *stream)
{
    mp3_file *mp3fp;
    int      ret_code;
    int      unproc_data_size;    /*the unprocessed data's size*/
    int      copy_size;

    mp3fp = (mp3_file *)data;
    if(mp3fp->fpos < mp3fp->flen)
    {
        //kk++;
        //printf("%d\n",kk);
        //if(kk == 180)
        //{
        //kk = 0;
        //ret_code = MAD_FLOW_STOP;
        //printf(" input return %d\n",ret_code);
        //return ret_code;
        //}
        if(*mus == 1)
        {
            *mus = 0;
            music_num++;
            if(music_num == 6)
            music_num = 1;
            ret_code = MAD_FLOW_BREAK;
            return ret_code;
        }
        if(*mus == 2)
        {
            *mus = 0;
            music_num--;
            if(music_num == 0)
            music_num = 5;
            ret_code = MAD_FLOW_BREAK;
            return ret_code;
        }
        
        unproc_data_size = stream->bufend - stream->next_frame;
        memcpy(mp3fp->fbuf, mp3fp->fbuf+mp3fp->fbsize-unproc_data_size, unproc_data_size);
        copy_size = BUFSIZE - unproc_data_size;
        if(mp3fp->fpos + copy_size > mp3fp->flen)
        {
            copy_size = mp3fp->flen - mp3fp->fpos;
        }
        fread(mp3fp->fbuf+unproc_data_size, 1, copy_size, mp3fp->fp);
        mp3fp->fbsize = unproc_data_size + copy_size;
        mp3fp->fpos  += copy_size;

        mad_stream_buffer(stream, mp3fp->fbuf, mp3fp->fbsize);
        ret_code = MAD_FLOW_CONTINUE;
    }
    else
    {
        ret_code = MAD_FLOW_STOP;
    }

    return ret_code;

}
/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */

    static
enum mad_flow error(void *data,
        struct mad_stream *stream,
        struct mad_frame *frame)
{
    //struct buffer *buffer = data;

    //fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
    //stream->error, mad_stream_errorstr(stream),
    //stream->this_frame - buffer->start);

    /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

    return MAD_FLOW_CONTINUE;
}

/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */

    
//int decode(unsigned char const *start, unsigned long length)
 int decode(mp3_file *mp3fp)
{
    //struct buffer buffer;
    struct mad_decoder decoder;
    int result;

    /* initialize our private message structure */

    //buffer.start  = start;
    //buffer.length = length;

    /* configure input, output, and error functions */

    //mad_decoder_init(&decoder, &buffer,
    //input, 0 /* header */, 0 /* filter */, output,
    //error, 0 /* message */);
     mad_decoder_init(&decoder, mp3fp,
               input, 0 /* header */, 0 /* filter */, output,
                       error, 0 /* message */);

    /* start decoding */

    result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

    /* release the decoder */

    mad_decoder_finish(&decoder);
    //printf("decode return %d\n",result);

    return result;
}

