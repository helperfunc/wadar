OBJS	= proc.o salsa.o utils.o wadar.o wavelib/src/conv.o wavelib/src/cwt.o wavelib/src/cwtmath.o wavelib/src/hsfft.o wavelib/src/real.o wavelib/src/wavefilt.o wavelib/src/wavefunc.o wavelib/src/wavelib.o wavelib/src/wtmath.o
SOURCE	= proc.c salsa.c utils.c wadar.c wavelib/src/conv.c wavelib/src/cwt.c wavelib/src/cwtmath.c wavelib/src/hsfft.c wavelib/src/real.c wavelib/src/wavefilt.c wavelib/src/wavefunc.c wavelib/src/wavelib.c wavelib/src/wtmath.c
HEADER	= wavelib/header/wavelib.h wavelib/header/wauxlib.h proc.h salsa.h utils.h wadar.h wavelib/src/cwt.h wavelib/src/cwtmath.h wavelib/src/hsfft.h wavelib/src/real.h wavelib/src/wavefilt.h wavelib/src/wavefunc.h wavelib/src/wtmath.h
OUT	= wadar
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) -lfftw3f -lfftw3 -lm -lcurl

salsa.o: salsa.c
	$(CC) $(FLAGS) salsa.c -lfftw3f -lfftw3 -lm -lcurl

proc.o: proc.c
	$(CC) $(FLAGS) proc.c -lfftw3f -lfftw3 -lm -lcurl

wadar.o: wadar.c
	$(CC) $(FLAGS) wadar.c -lfftw3f -lfftw3 -lm -lcurl

deploy: all
	cp $(OUT) ../b1/chipotle-radar/

clean:
	rm -f $(OBJS) $(OUT)
