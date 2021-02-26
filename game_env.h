#ifndef __NCURSES_H
#include <ncurses.h>
#endif

#ifndef GAME_ENV_H
#define GAME_ENV_H

struct _GAME_ENV{
  int actualScore;
  int highScore;
  int gameStatus;
  int rounds;
  int width;
  int height;
  int * gamePositions;
  WINDOW ** gameBoard;
};

typedef struct _GAME_ENV GAME_ENV;
#endif
