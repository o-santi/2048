#
# makefile for 2048
# so pra facilitar make build e make clean
# nada muito serio

unix: 2048.c
	gcc 2048.c -o 2048 -Wall -pedantic -lcurses
win: 2048.c
	gcc 2048.c -o 2048.exe -Wall -pedantic -lncurses
clean:
	$(RM) 2048 2048.exe
