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

#ifndef GAME_LOGIC_H
#include "game_logic.h"
#endif

#define BACKGROUND_COLOR 100
#define TILE_VAZIA_COLOR 101
#define DERROTA_COLOR 102
#define VITORIA_COLOR 103

#define SQUARE_WIDTH 11
#define SQUARE_HEIGHT 7
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4



int getColorPair(int number) {
  /* Acha a potencia de 2 do numero, que sera o color-id dele
     como todos os numeros são multiplos de 2, só precisamos dividir até achar 1
   */
  if (number == 0) {return TILE_VAZIA_COLOR;}
  int potencia;
  potencia = 0;
  while (number > 1) {
    number /= 2;
    potencia++;
  }
  return potencia;
}


void drawBoard(GAME_ENV *game_environment) {
  int index, numberAtTile, charLength, colorPairIndex;
  char *to_char_buffer = malloc(sizeof * to_char_buffer * 50); /* colocamos 50 chars so pra ter certeza, apesar de nao conhecer ninguem que chegaria em 10 digitos no 2048*/
  charLength = 0;
  for (index = 0; index < game_environment->height * game_environment->width; index++) {
    werase(game_environment->gameBoard[index]); /* erase the tile in window*/
    wbkgd(game_environment->gameBoard[index], 0); // reset the background color
    numberAtTile = game_environment->gamePositions[index]; /* number at the current tile*/
    if (numberAtTile > 0) { /* only print to buffer if it's not 0 */
      sprintf(to_char_buffer, "%d", numberAtTile); /* print it to buffer*/
      charLength = (int) strlen(to_char_buffer) / 2;
    }
    else {
      sprintf(to_char_buffer, "%c", ' '); /* printa espaco vazio para o buffer */
    }
    colorPairIndex = getColorPair(numberAtTile); // pega o color index pair 
    wbkgd(game_environment->gameBoard[index], COLOR_PAIR(colorPairIndex)); // pinta o background 
										
    wattron(game_environment->gameBoard[index], COLOR_PAIR(BACKGROUND_COLOR)); 
    box(game_environment->gameBoard[index], 0, 0);  // desenha a box default envolta da window
    wattroff(game_environment->gameBoard[index], COLOR_PAIR(BACKGROUND_COLOR));
    
    wattron(game_environment->gameBoard[index], A_BOLD); 
    mvwaddstr(game_environment->gameBoard[index], SQUARE_HEIGHT/ 2, SQUARE_WIDTH / 2 - charLength , to_char_buffer); /* print buffer to window */
    wattroff(game_environment->gameBoard[index], A_BOLD); 

    wrefresh(game_environment->gameBoard[index]); // refresh box
  }
  free(to_char_buffer);
  
}


void drawUiScreen(GAME_ENV *game_environment) {
  WINDOW *ui_window;
  char *user_actual_score = calloc(sizeof(char) * 10, sizeof(char));
  char *user_high_score = calloc(sizeof(char) * 10, sizeof(char));
  char *round_count = calloc(sizeof(char) * 10, sizeof(char));

  char *score_string = calloc(sizeof(char) * 30, sizeof(char));
  char *high_score_string = calloc(sizeof(char) * 30, sizeof(char));
  char *round_string = calloc(sizeof(char) * 30, sizeof(char));
  
  ui_window = game_environment->gameBoard[game_environment->width * game_environment->height];
  wbkgd(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  wattron(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  box(ui_window, 0, 0);
  wattroff(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  mvwaddstr(ui_window, 1, SQUARE_WIDTH * 3/2 - 4, "2048 em C");
  
  strcpy(score_string, "Score: ");
  strcpy(high_score_string, "High Score: ");
  strcpy(round_string, "Round: ");
  
  sprintf(user_actual_score, "%d", game_environment->actualScore);
  sprintf(user_high_score, "%d", game_environment->highScore);
  sprintf(round_count, "%d", game_environment->rounds);

  strcat(score_string, user_actual_score);
  strcat(high_score_string, user_high_score);
  strcat(round_string, round_count);
  
  mvwaddstr(ui_window, 2, 1, score_string);
  mvwaddstr(ui_window, 3, 1, high_score_string);
  mvwaddstr(ui_window, 4, 1, round_string);

  mvwaddstr(ui_window, 6, 1, "Aperte 'q' para terminar o jogo");
  
  wrefresh(ui_window);

  free(user_high_score);
  free(user_actual_score);
  free(round_count);
  
  free(score_string);
  free(high_score_string);
  free(round_string);
}

void showEndGameScreen(GAME_ENV *game_environment) {
  int index, color;
  WINDOW * board_window = malloc(sizeof(void*));
  WINDOW * ui_window = malloc(sizeof(void*));
  color = game_environment->gameStatus == 1 ? VITORIA_COLOR : DERROTA_COLOR;
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
  ui_window = game_environment->gameBoard[game_environment->width * game_environment->height];
  werase(ui_window);
  mvwaddstr(ui_window, 6, 1, "Aperte 'q' para fechar o jogo");
  mvwaddstr(ui_window, 7, 1, "ou 'r' para reiniciar o jogo");
  wattron(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  box(ui_window, 0, 0);
  wattroff(ui_window, COLOR_PAIR(BACKGROUND_COLOR));
  wrefresh(ui_window);
}


void blitToScreen(GAME_ENV *game_environment) {
  drawBoard(game_environment);
  drawUiScreen(game_environment);
}

void initColorPairs(void) { /* a paleta de cores para o jogo */
  init_pair(1, 59, 188); /* ordem: id, foreground, background */
  init_pair(2, 59, 187); /* cores do 4 */
  init_pair(3, 15, 180); /* cores do 8 */
  init_pair(4, 15, 173); /* cores do 16 */
  init_pair(5, 15, 174); /* cores do 32 */
  init_pair(6, 15, 166); /* cores do 64 */
  init_pair(7, 15, 186); /* cores do 128 */
  init_pair(8, 15, 185); /* cores do 256 */
  init_pair(9, 15, 184); /* cores do 512 */
  init_pair(10, 15, 221); /* cores do 1024 */
  init_pair(11, 15, 220); /* cores do 2048 */
  init_pair(BACKGROUND_COLOR, 0, 102); 
  init_pair(TILE_VAZIA_COLOR, 0, 102);
  init_pair(DERROTA_COLOR, 0, COLOR_RED);
  init_pair(VITORIA_COLOR, 0, COLOR_GREEN);
}

