#
# makefile for 2048
# so pra facilitar make build e make clean
# nada muito serio

build: 
	gcc -o 2048 game_logic.c game_visuals.c -Wall -pedantic -lncurses

clean:
	$(RM) 2048 2048.exe
