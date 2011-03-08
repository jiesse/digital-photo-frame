# 2010.2.22

#DIR = ./out/
#TAR_DIR = ./out/

src = $(wildcard *.c)
obj = $(patsubst %.c, $(DIR)%.o, $(src))
target = $(TAR_DIR)main

CC = gcc
#CC = arm-linux-gcc

CFLAGS = -Wall -g -c -I../jpeg-8/ -I../freetype-2.3.11/include
LDFLAGS = -L../jpeg-8/.libs/ -ljpeg -L../freetype-2.3.11/objs/.libs -lfreetype -lpthread -L /home/yaomoon/study/dpf-30/libmad-0.15.0b/.libs/ -lmad

#CFLAGS = -Wall -g -c -I /home/jiesse/jpeglib/include -I /home/jiesse/freetype/include/freetype2/freetype/
#LDFLAGS = -L /home/jiesse/jpeglib/lib/ -ljpeg -L /home/jiesse/freetype/lib -lfreetype -lpthread -L /home/jiesse/musiclib/lib/ -lmad



all: $(target)

$(target): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
$(DIR)%.o:%.c
	$(CC) -o $@ $(CFLAGS) $< 

clean:
	-rm -f $(target)
	-rm -f $(obj)
	-rm -f *~

.PHONY: clean
test:
	@echo $(src
