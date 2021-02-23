#ifndef __GAME_ENV_H__
#include "game_env.h"
#endif

int getColorPair(int number);

void drawBoard(GAME_ENV *game_environment);

void drawUiScreen(GAME_ENV *game_environment);

void showEndGameScreen(GAME_ENV *game_environment);

void blitToScreen(GAME_ENV *game_environment);

void initColorPairs(void);
