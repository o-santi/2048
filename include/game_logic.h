/* Implementação do jogo 2048 em C para Computação 1 (CC)
   Feito por :
   Leonardo Santiago DRE 120036072
   Miguel Uchoa      DRE 120036412
   Caio Monteiro     DRE 120036373
*/
#ifndef __GAME_ENV_H__
#include "game_env.h"
#endif

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H



int getArrayLength(int * array);

void endProgram(GAME_ENV *game_environment); 

void createBoard(GAME_ENV *game_environment);

int processUserMove(int userMove);
  
void transposeMatrix(int **matrix);

void invertMatrix(int **matrix);

void rotateMatrix90Degrees(int **matrix);

void moveBoardtoLeft(int ** matrix, GAME_ENV *game_environment);
  
void executeMove(int **matrix, int direcao, GAME_ENV *game_environment);

void saveHighScore(GAME_ENV * game_environment);

void fetchHighScore(GAME_ENV * game_environment);

int createRandomSquare(GAME_ENV * game_environment);

void startGameEnvironment(GAME_ENV *game_environment);

int testIfMoveChangedBoard(GAME_ENV *game_environment, int direcao);

void startGameEnvironment(GAME_ENV *game_environment);

void manageEndGame(GAME_ENV *game_environment);

int testIfGameIsLost(GAME_ENV *game_environment);

int testIfGameIsWon(GAME_ENV *game_environment);

void runGameLoop(GAME_ENV * game_environment);

void runGameLoop(GAME_ENV * game_environment);

#endif
