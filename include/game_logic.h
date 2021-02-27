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

void terminarPrograma(GAME_ENV *game_environment); 

void createBoard(GAME_ENV *game_environment);

int processUserMove(int userMove);
  
void transporMatrix(int **matrix);

void inverterMatrix(int **matrix);

void rotacionarMatrix90Graus(int **matrix);

void moverTabuleiroParaEsquerda(int ** matrix, GAME_ENV *game_environment);
  
void executarMovimento(int **matrix, int direcao, GAME_ENV *game_environment);

void registrarHighScore(GAME_ENV * game_environment);

void pegaHighScore(GAME_ENV * game_environment);

int createRandomSquare(GAME_ENV * game_environment);

void startGameEnvironment(GAME_ENV *game_environment);

int testIfMoveChangedBoard(GAME_ENV *game_environment, int direcao);

void startGameEnvironment(GAME_ENV *game_environment);

void manageFimDeJogo(GAME_ENV *game_environment);

int testIfGameIsLost(GAME_ENV *game_environment);

int testIfGameIsWon(GAME_ENV *game_environment);

void runGameLoop(GAME_ENV * game_environment);

void runGameLoop(GAME_ENV * game_environment);

#endif
