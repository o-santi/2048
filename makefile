# makefile for 2048
# so pra facilitar make build e make clean
# nada muito serio

CC=gcc
CFLAGS=-Wall -pedantic -lncurses -ansi

default: build

build: 
		$(CC) -o 2048 src/game_logic.c src/game_visuals.c $(CFLAGS)

clean:
		$(RM) 2048 2048.exe high_score.txt
