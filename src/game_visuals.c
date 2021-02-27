/* Implementação do jogo 2048 em C para Computação 1 (CC)
   Feito por :
   Leonardo Santiago DRE 120036072
   Miguel Uchoa      DRE 120036412
   Caio Monteiro     DRE 120036373
*/



#ifndef __NCURSES_H
#include <ncurses.h>
#endif

#ifndef __STDLIB_H__
#include <stdlib.h>
#endif

#ifndef __STRING_H
#include <string.h>
#endif

/* Importa todos os comandos e funções para a lógica do jogo.
   importa junto a estrutura GAME_ENV.
*/
#ifndef GAME_LOGIC_H
#include "../include/game_logic.h"
#endif

/* Define as cores para a tela ncurses. */
#define BACKGROUND_COLOR 100
#define TILE_VAZIA_COLOR 101
#define DEFEAT_COLOR 102
#define VICTORY_COLOR 103

/* Define o tamanho dos blocos e quantidade de blocos. */
#define SQUARE_WIDTH 11
#define SQUARE_HEIGHT 7
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4


/* Função que acha a potencia de 2 do número, que será o color-id dele.
   Como todos os números são múltiplos de 2, só precisamos dividir até achar 1.
*/
int getColorPair(int number) {
  int potencia = 0;
  if (number == 0) return TILE_VAZIA_COLOR;
  while (number > 1) {
    number /= 2;
    potencia++;
  }
  return potencia;
}

/* Função que desenha todo o tabuleiro, blocos do tabuleiro e tela de pontuação usando ncurses. */
void drawBoard(GAME_ENV *game_environment) {
  int index, numberAtTile, charLength, colorPairIndex;
  char *to_char_buffer = malloc(sizeof * to_char_buffer * 50); /* Colocamos 50 characteres só para ter certeza, apesar de não conhecermos ninguém que chegaria em 10 dígitos no 2048. */
  charLength = 0;
  for (index = 0; index < game_environment->height * game_environment->width; index++) {
    werase(game_environment->gameBoard[index]); /* Apagar o bloco que está no índice do tabuleiro. */
    wbkgd(game_environment->gameBoard[index], 0); /* Resetar a cor de fundo para que possamos trocá-la. */
    numberAtTile = game_environment->gamePositions[index]; /* O valor que está no bloco é registrado em numberAtTile. */
    if(numberAtTile > 0) { /* Apenas mostramos um valor num bloco se este for diferente de 0. */
      sprintf(to_char_buffer, "%d", numberAtTile);
      charLength = (int) strlen(to_char_buffer) / 2; /* Centralizar o número mostrado no bloco.*/
    }
    else {
      sprintf(to_char_buffer, "%c", ' '); /* Se o valor no bloco for 0, printamos um espaço em branco. */
    }
    colorPairIndex = getColorPair(numberAtTile); /* Pegamos o índice da cor baseado no valor do bloco. */ 
    wbkgd(game_environment->gameBoard[index], COLOR_PAIR(colorPairIndex)); /* Colore o bloco de acordo com o valor mostrado. */ 
										
    wattron(game_environment->gameBoard[index], COLOR_PAIR(BACKGROUND_COLOR)); 
    box(game_environment->gameBoard[index], 0, 0);  /* Desenha o contorno do bloco. */
    wattroff(game_environment->gameBoard[index], COLOR_PAIR(BACKGROUND_COLOR));
    
    wattron(game_environment->gameBoard[index], A_BOLD); 
    mvwaddstr(game_environment->gameBoard[index], SQUARE_HEIGHT/ 2, SQUARE_WIDTH / 2 - charLength , to_char_buffer); /* printa o número no meio do bloco. */
    wattroff(game_environment->gameBoard[index], A_BOLD); 

    wrefresh(game_environment->gameBoard[index]); /* printa todas as modificações na tela do terminal. */
  }
  free(to_char_buffer);
  
}

/* Escreve as informações pontuação, high score e a rodada do lado do tabuleiro do jogo. */
void drawUiScreen(GAME_ENV *game_environment) {
  WINDOW *ui_window;
  /* Criando strings para cada uma das informações a serem mostradas. */
  char *user_actual_score = calloc(sizeof(char) * 10, sizeof(char));
  char *user_high_score = calloc(sizeof(char) * 10, sizeof(char));
  char *round_count = calloc(sizeof(char) * 10, sizeof(char));

  char *score_string = calloc(sizeof(char) * 30, sizeof(char));
  char *high_score_string = calloc(sizeof(char) * 30, sizeof(char));
  char *round_string = calloc(sizeof(char) * 30, sizeof(char));
  
  ui_window = game_environment->gameBoard[game_environment->width * game_environment->height];
  /* Criando um bloco grande para servir como interface de usuário. */
  wbkgd(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  wattron(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  box(ui_window, 0, 0);
  wattroff(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  mvwaddstr(ui_window, 1, SQUARE_WIDTH * 3/2 - 4, "2048 em C"); /* Printar o título do jogo centralizado na interface de usuário. */
  
  strcpy(score_string, "Score: ");
  strcpy(high_score_string, "High Score: ");
  strcpy(round_string, "Round: ");
  
  sprintf(user_actual_score, "%d", game_environment->actualScore);
  sprintf(user_high_score, "%d", game_environment->highScore);
  sprintf(round_count, "%d", game_environment->rounds);

  /* Concatena o nome da informação com o valor da informação. */
  strcat(score_string, user_actual_score);
  strcat(high_score_string, user_high_score);
  strcat(round_string, round_count);
  
  /* Escreve as strings nas suas respectivas posições. */
  mvwaddstr(ui_window, 2, 1, score_string);
  mvwaddstr(ui_window, 3, 1, high_score_string);
  mvwaddstr(ui_window, 4, 1, round_string);

  mvwaddstr(ui_window, 6, 1, "'Q' para terminar o jogo.");
  mvwaddstr(ui_window, 7, 1, "WASD/setas para jogar.");
  
  /* Printa todas as informações definidas acima na tela do terminal. */
  wrefresh(ui_window);

  free(user_high_score);
  free(user_actual_score);
  free(round_count);
  
  free(score_string);
  free(high_score_string);
  free(round_string);
}

/* Função que printa a tela de fim de jogo (quando o jogador perde, ganha ou aperta 'q'). */
void showEndGameScreen(GAME_ENV *game_environment) {
  int index, color;
  WINDOW * board_window = malloc(sizeof(void*));
  WINDOW * ui_window = malloc(sizeof(void*));
  /* Se o jogador ganha, pintamos o fundo de verde, caso contrário, de vermelho. */
  color = game_environment->gameStatus == 1 ? VICTORY_COLOR : DEFEAT_COLOR; 
  for (index = 0; index < game_environment->width * game_environment->height; index++) {
    board_window = game_environment->gameBoard[index];
    wattron(board_window, COLOR_PAIR(color));
    box(board_window, 0, 0);
    wattron(board_window, COLOR_PAIR(color));
    if (game_environment->gamePositions[index] == 0) {
      wbkgd(board_window, COLOR_PAIR(color));
    }
    wrefresh(board_window);
  }
  /* Atualizamos a interface de usuário. */
  ui_window = game_environment->gameBoard[game_environment->width * game_environment->height];
  werase(ui_window);
  mvwaddstr(ui_window, 6, 1, "'Q' para fechar o jogo.");
  mvwaddstr(ui_window, 7, 1, "'R' para reiniciar o jogo.");
  wattron(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  box(ui_window, 0, 0);
  wattroff(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  wrefresh(ui_window);
}

/* Função que desenha o tabuleiro e a interface de usuário. */
void blitToScreen(GAME_ENV *game_environment) {
  drawBoard(game_environment);
  drawUiScreen(game_environment);
}

/* Função que define a paleta de cores para o jogo. 
   A tabela de cores está disponível em:
   https://jonasjacek.github.io/colors/
*/
void initColorPairs(void) { /* Ordem: (id, texto, fundo). */
  init_pair(1, 59, 188); /* Cores do 2. */
  init_pair(2, 59, 187); /* Cores do 4. */
  init_pair(3, 15, 180); /* Cores do 8. */
  init_pair(4, 15, 173); /* Cores do 16. */
  init_pair(5, 15, 174); /* Cores do 32. */
  init_pair(6, 15, 166); /* Cores do 64. */
  init_pair(7, 15, 186); /* Cores do 128. */
  init_pair(8, 15, 185); /* Cores do 256. */
  init_pair(9, 15, 184); /* Cores do 512. */
  init_pair(10, 15, 221); /* Cores do 1024. */
  init_pair(11, 15, 220); /* Cores do 2048. */
  init_pair(BACKGROUND_COLOR, 0, 102); 
  init_pair(TILE_VAZIA_COLOR, 0, 102);
  init_pair(DEFEAT_COLOR, 0, COLOR_RED);
  init_pair(VICTORY_COLOR, 0, COLOR_GREEN);
}

