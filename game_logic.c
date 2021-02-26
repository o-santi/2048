/* Implementação do jogo 2048 em C para Computação 1 (CC)
   Feito por :
   Leonardo Santiago DRE 120036072
   Miguel Uchoa      DRE 120036412
   Caio Monteiro     DRE 120036373
*/

/* BOARD CONSTANTS*/


/* SYSTEM CONSTANTS*/
/* como estamos virando o tabuleiro 90 graus pra mover,
   esses valores se referem a quantidade de rotacoes necessarias
   pra depois também poder mover pra esquerda
*/

#define RIGHT 2
#define UP 3
#define DOWN 1
#define LEFT 0
#define EXIT -100

#define SQUARE_WIDTH 11
#define SQUARE_HEIGHT 7
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4

#ifdef _WIN32
#include <ncurses/ncurses.h>
#elif defined(unix) || defined(__unix__) || defined(__unix) || __APPLE__
#include <ncurses.h>
#endif

#ifndef __STRING_H
#include <string.h>
#endif

#ifndef __STDLIB_H__
#include <stdlib.h>
#endif

#include <stdio.h>
#include <time.h>
#include "game_visuals.h"

#ifndef GAME_ENV_H
#include "game_env.h"
#endif

#ifndef GAME_LOGIC_H
#include "game_logic.h"
#endif

#ifndef GAME_VISUALS_H
#include "game_visuals.h"
#endif

/* Estrutura que irá segurar todas as variaveis do jogo
   Criamos um ponteiro para ela e
   passamos o ponteiro pelas funcoes
*/

int getArrayLength(int * array){
  return sizeof(*array) / sizeof(array[0]);
}

void terminarPrograma(GAME_ENV *game_environment) {
  registrarHighScore(game_environment);
  nocbreak();
  keypad(stdscr, FALSE);
  echo();
  curs_set(1);
  standend();
  endwin();
}
void createBoard(GAME_ENV *game_environment) {
  /* Cria uma WINDOW do ncurses para cada
     quadrado do board e armazena no vetor
     gameBoard
  */
  int coluna, linha;
  WINDOW * window; 
  for (coluna = 0; coluna < game_environment->height; coluna++) {
    for (linha = 0; linha < game_environment->width; linha++) {
      window = newwin(
          SQUARE_HEIGHT, SQUARE_WIDTH, coluna * SQUARE_HEIGHT,
          linha * SQUARE_WIDTH); // cria uma window na posição (coluna, linha)
      game_environment->gameBoard[coluna * game_environment->height + linha] =
          window; // Salva a window na estrutura do game env
    }
  }
  game_environment
      ->gameBoard[game_environment->height * game_environment->width] =
      newwin(SQUARE_HEIGHT * game_environment->height, SQUARE_WIDTH * 3, 0,
             game_environment->width * SQUARE_WIDTH);
  // criamos a tela a direita de UI
}


int processUserMove(int userMove) {
  /* Processa o input do player e devolve a direcao escolhida baseada na letra
   */
  switch (userMove) {
  case 'D':
  case 'd':
  case KEY_RIGHT:
    return RIGHT;
  case 'w':
  case 'W':
  case KEY_UP:
    return UP;
  case 's':
  case 'S':
  case KEY_DOWN:
    return DOWN;
  case 'a':
  case 'A':
  case KEY_LEFT:
    return LEFT;
  case 'q':
  case 'Q':
    return EXIT;
  default:
    return -1;
  }
}

void transporMatrix(int **matrix) {
  /* Inverte as colunas com as linhas de uma matrix
     aceita o ponteiro do vetor e ja modifica-a diretamente
   */ 
  int coluna, linha, j;
  int temp_array[BOARD_WIDTH * BOARD_HEIGHT];
  j = 0;
  for (coluna = 0; coluna < BOARD_HEIGHT; coluna++) {
    for (linha = 0; linha < BOARD_WIDTH; linha++) {
      temp_array[j++] = (*matrix)[linha*BOARD_HEIGHT + coluna];
    }
  }
  for (j = 0; j < BOARD_WIDTH * BOARD_HEIGHT; j++) {
    (*matrix)[j] = temp_array[j];
  }
}

void inverterMatrix(int **matrix) {
  int coluna, linha, temp;
  for (coluna = 0; coluna < BOARD_HEIGHT; coluna++) {
    for (linha = 0; linha < BOARD_WIDTH / 2 ; linha++) {
      temp = (*matrix)[coluna*BOARD_HEIGHT + linha]; // salva o valor de matrix[coluna][linha]
      (*matrix)[coluna*BOARD_HEIGHT + linha] = (*matrix)[coluna*BOARD_HEIGHT + BOARD_WIDTH - linha - 1];
      (*matrix)[coluna*BOARD_HEIGHT + BOARD_WIDTH - linha - 1] = temp;
    }
  }
}

void rotacionarMatrix90Graus(int **matrix) {
  transporMatrix(matrix);
  inverterMatrix(matrix);
}

void moverTabuleiroParaEsquerda(int ** matrix, GAME_ENV *game_environment) {
  /* move todas as tiles para a esquerda 
     ideia para a função foi tirada de:
     https://flothesof.github.io/2048-game.html

     TODO: ajeitar essa bagunça
   */
  int i, j, anterior, coluna, linha, quadrado;
  int *nova_linha = malloc(sizeof(int) * game_environment->width);
  for (coluna = 0; coluna < game_environment->height; coluna++) {
    memset(nova_linha, 0, sizeof(int) * game_environment->width);
    anterior = 0;
    j = 0;
    for (linha = 0; linha < game_environment->width; linha++) {
      quadrado = (*matrix)[coluna*game_environment->height + linha]; // salva o valor de matrix[coluna][linha]
      if (quadrado > 0) {
        if (anterior == 0) {
	  anterior = quadrado;
        }
	else {
          if (anterior == quadrado) {
	    nova_linha[j] = 2 * quadrado; /* adiciona na nova linha 2x o  quadrado */
            if (matrix == &game_environment->gamePositions) {
              game_environment->actualScore += 2 * quadrado;
              if (game_environment->actualScore > game_environment->highScore) {
                game_environment->highScore = game_environment->actualScore;
              }
            }
            j++;
	    anterior = 0;
          }
	  else {
	    nova_linha[j] = anterior;
	    j++;
	    anterior = quadrado;
          }
        }
      }
    }
    if (anterior > 0) {
      nova_linha[j] = anterior;
    }
    for (i = 0; i < game_environment->width; i++) {
      (*matrix)[coluna*game_environment->height + i] = nova_linha[i];
    }
  }
  free(nova_linha);
}

void executarMovimento(int **matrix, int direcao, GAME_ENV *game_environment) {
  /* rotaciona o tabuleiro pra direção correta, depois
     movimenta o tabuleiro pra esquerda e depois rotaciona
     pra direcao correta novamente

     TODO: ajeitar esta merda
   */
  int index;
  for (index = 0; index < direcao; index++) {
    rotacionarMatrix90Graus(matrix);
  }
  
  moverTabuleiroParaEsquerda(matrix, game_environment);
  for (index = 0; index < 4 - direcao; index++) {
    rotacionarMatrix90Graus(matrix);
  }
  if (matrix == &game_environment->gamePositions) {
    game_environment->rounds++;
  }
}

void registrarHighScore(GAME_ENV *game_environment){
  FILE *fhs; /* Arquivo que guarda o high score. */
  char stringHighScore[11]; /* O ultimo high score em string. O tamanho maximo de uma int eh 10. */
  int ultimoHighScore;
  fhs = fopen("high_score.txt", "r");
  /* if (fhs == NULL){
    fhs = fopen("high_score.txt", "w");
    fprintf(fhs, "%d", game_environment->highScore);
  }else { */
    fscanf(fhs, "%s", stringHighScore);
    ultimoHighScore = atoi(stringHighScore); /* Converte o high score de string para int. */
    if (game_environment->highScore > ultimoHighScore){
      freopen("high_score.txt", "w", fhs); /* Reabre o arquivo, deletando seu conteudo. */
      fprintf(fhs, "%d", game_environment->highScore);
    }
  fclose(fhs);
}

void pegaHighScore(GAME_ENV *game_environment){
  FILE *fhs;
  char stringHighScore[11];
  fhs = fopen("high_score.txt", "r");
  if (fhs == NULL){
    fhs = fopen("high_score.txt", "w");
    fprintf(fhs, "0");
  }else {
    fscanf(fhs, "%s", stringHighScore);
    game_environment->highScore = atoi(stringHighScore);
  }
  fclose(fhs);
}


int createRandomSquare(GAME_ENV * game_environment){
  /* Escolhe um quadrado vazio do tabuleiro e adiciona
     uma tile nele
   */
  int qntdQuadradosVazios, randomSquareIndex, randomSquarePosition, index, newTile;
  int * temp_buffer = malloc(sizeof(int) * game_environment->width * game_environment->height);
  float dois_ou_quatro;
  qntdQuadradosVazios = 0;
  for (index=0; index < game_environment->height * game_environment->width; index ++){
    if (game_environment->gamePositions[index] == 0){
      temp_buffer[qntdQuadradosVazios] = index;
      qntdQuadradosVazios++;
    }  
}
  if (qntdQuadradosVazios == 0){
    return -1;
  }
  randomSquareIndex = (int) rand() / (RAND_MAX + 1.0) * qntdQuadradosVazios; // escolhemos um numero de [0, qntdquadradosvazios-1]
  dois_ou_quatro = rand() / (RAND_MAX + 1.0);
  newTile = dois_ou_quatro > 0.9 ? 4 : 2; // 90% de chance de ser 2 e 10% de ser 4 (probabilidades advindas da internet)
  randomSquarePosition = temp_buffer[randomSquareIndex]; // pegamos a posição que está nesse Tile
  game_environment->gamePositions[randomSquarePosition] = newTile; // criamos um novo tile nessa posição
  free(temp_buffer);
  return 0;
}


int testIfMoveChangedBoard(GAME_ENV *game_environment, int direcao) {
  /* executa o movimento num tabuleiro temporario e testa pra ver se
     o movimento muda alguma pecao de lugar */
  int index;
  int *temporary_board = calloc(sizeof(int) * game_environment->width * game_environment->height, sizeof(int));
  memcpy(temporary_board, game_environment->gamePositions, sizeof(int) * game_environment->width * game_environment->height);
  executarMovimento(&temporary_board, direcao, game_environment);
  for (index = 0; index < game_environment->width * game_environment->height; index++) {
    if (game_environment->gamePositions[index] != temporary_board[index]) {
      free(temporary_board);
      return 1; /* o movimento mudou pelo menos uma peca */
    }
  }
  free(temporary_board);
  return 0; /* o movimento nao mudou o tabuleiro */
}


void startGameEnvironment(GAME_ENV *game_environment) {
  initscr(); // inicia a curses screen
  if (has_colors() == TRUE) {
    start_color(); // use colors
    use_default_colors();
    initColorPairs();
  }
  keypad(stdscr, TRUE); // pega o input do keypad
  cbreak(); // desativa line breaking
  noecho(); // desativa printing to screen
  curs_set(0); // set cursor to be invisible
  refresh(); // refresh screen
  srand(time(NULL)); // inicializamos a seed para os numeros randomicos, baseado no tempo
  game_environment->width = BOARD_WIDTH;
  game_environment->height = BOARD_HEIGHT;
  game_environment->gamePositions = calloc(sizeof(int) * game_environment->height * game_environment->width, sizeof(int));
  game_environment->gameBoard = malloc(sizeof(void*) * (game_environment->height * game_environment->width + 1)); // inicializamos o array que segurará as windows
  game_environment->gameStatus = 0;
  /* gameStatus = 0  -> o jogo está rodando
     gameStatus = 1  -> o jogo foi ganho
     gameStatus = -1 -> o jogo foi perdido
  */
  game_environment->actualScore = 0; // score inicia com 0
  game_environment->rounds = 0;
  pegaHighScore(game_environment);
  createBoard(game_environment); // cria todos as windows e salva no vetor gamePositions
  createRandomSquare(game_environment);
  createRandomSquare(game_environment); //chamamos 2 vezes para criar 2 quadrados iniciais aleatorios
  blitToScreen(game_environment);
}



void manageFimDeJogo(GAME_ENV *game_environment) {
  int userChoice;
  showEndGameScreen(game_environment);
  while(1) {
    userChoice = getch();
    if ((userChoice == 'r') || (userChoice == 'R') ||  (userChoice == 'q') || (userChoice == 'Q')){
      break;
    }
  }
  switch (userChoice) {
  case 'Q':
  case 'q': /* o usuario escolheu sair do jogo */
    terminarPrograma(game_environment);
    break;
  case 'R':
  case 'r': /* o usuario escolheu jogar novamente */
    registrarHighScore(game_environment);
    free(game_environment->gamePositions);
    free(game_environment->gameBoard);
    startGameEnvironment(game_environment);
    runGameLoop(game_environment);
  }
}


int testIfGameIsLost(GAME_ENV *game_environment) {
  int direcao, changed;
  for (direcao = 0; direcao < 4; direcao++) {
    /* testamos as quatro possiveis direcoes pra jogar */
    changed = testIfMoveChangedBoard(game_environment, direcao);
    if (changed == 1) {
      return 0; /* o jogo deve continuar */
    }
  }
  return -1; /* o jogo acabou */
}

int testIfGameIsWon(GAME_ENV *game_environment) {
  int tile;
  for (tile = 0; tile < game_environment->width * game_environment->height;
       tile++) {
    if (game_environment->gamePositions[tile] == 2048) {
      return 1; /* o jogo foi ganho */
    }
  }
  return 0; /* o jogo deve continuar */
}



void runGameLoop(GAME_ENV * game_environment) {
  // Inicia o game loop e mantem enquanto
  // o jogo nao acabou
  int direcao, userMove, movimentoMudouTabuleiro;
  do {
    userMove = getch();
    direcao= processUserMove(userMove);
    // retorna a direcao escolhida,
    // se for -1, o movimento nao é valido
    // TODO: piscar a tela de vermelho quando for invalido
    if (direcao == EXIT) {
      break;
    }
    if (direcao != -1) {
      movimentoMudouTabuleiro = testIfMoveChangedBoard(game_environment, direcao);
      if (movimentoMudouTabuleiro == 1) {
        executarMovimento(&game_environment->gamePositions, direcao,
                          game_environment);
        createRandomSquare(game_environment);
        blitToScreen(game_environment);
        game_environment->gameStatus = testIfGameIsLost(game_environment);
	game_environment->gameStatus += testIfGameIsWon(game_environment);
	/* TODO: ajeitar isso, fazer com que só seja um se o jogo tiver sido ganho */
      }
    }
  } while (game_environment->gameStatus == 0);
  manageFimDeJogo(game_environment);
}


int main(void) {
  GAME_ENV* game_environment = malloc(sizeof(GAME_ENV));
  startGameEnvironment(game_environment);
  runGameLoop(game_environment); 
  return 0;
}
