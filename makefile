#
# makefile for 2048
# so pra facilitar make build e make clean
# nada muito serio

build: 2048.c
	gcc 2048.c -o 2048 -Wall -pedantic -lcurses
clean:
	$(RM) 2048
