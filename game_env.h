#ifndef __GAME_ENV_H__
#define __GAME_ENV_H__
#endif

#ifndef __NCURSES_H
#include <ncurses.h>
#endif

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
