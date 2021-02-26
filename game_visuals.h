#ifndef GAME_ENV_H
#include "game_env.h"
#endif

#ifndef GAME_VISUALS_H
#define GAME_VISUALS_h

int getColorPair(int number);

void drawBoard(GAME_ENV *game_environment);

void drawUiScreen(GAME_ENV *game_environment);

void showEndGameScreen(GAME_ENV *game_environment);

void blitToScreen(GAME_ENV *game_environment);

void initColorPairs(void);
#endif
