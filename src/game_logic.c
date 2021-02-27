/* Implementação do jogo 2048 em C para Computação 1 (CC)
   Feito por :
   Leonardo Santiago DRE 120036072
   Miguel Uchoa      DRE 120036412
   Caio Monteiro     DRE 120036373
*/

/* Como estamos virando o tabuleiro 90 graus para mover,
   esses valores se referem a quantidade de rotações necessárias
   para depois também poder mover para a esquerda.
*/
#define RIGHT 2
#define UP 3
#define DOWN 1
#define LEFT 0
#define EXIT -100

/* Definindo as dimensões de cada bloco e do tabuleiro 
   para ser usado na visualização por ncurses.
*/
#define SQUARE_WIDTH 11
#define SQUARE_HEIGHT 7
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4

#ifndef __STRING_H
#include <string.h>
#endif

#ifndef __STDLIB_H__
#include <stdlib.h>
#endif

#include <stdio.h>
#include <time.h>

/* Estrutura que irá segurar todas as variaveis do jogo.
   Criamos um ponteiro para ela e passamos o ponteiro pelas funcoes.

   A biblioteca ncurses já é importada quando incluímos game_env.h
*/
#ifndef GAME_ENV_H
#include "../include/game_env.h"
#endif

/* Biblioteca que possui todos os códigos e funções para tornar possível
   a movimentação e utilização do jogo (funções para mover tabuleiro,
   ver se o movimento inputado muda o tabuleiro, checar se o jogador perdeu, etc.)
*/
#ifndef GAME_LOGIC_H
#include "../include/game_logic.h"
#endif

/* Biblioteca que possui todos os códigos e funções para printar na tela o
   tabuleiro, os blocos, a pontuação, o high score, o número de rodadas, etc. 
   usando ncurses. 
*/
#ifndef GAME_VISUALS_H
#include "../include/game_visuals.h"
#endif

int main(void) {
  GAME_ENV* game_environment = malloc(sizeof(GAME_ENV));
  startGameEnvironment(game_environment);
  runGameLoop(game_environment); 
  return 0;
}

int getArrayLength(int * array){
  return sizeof(*array) / sizeof(array[0]);
}

/* Função que é chamada quando o jogador quer sair do jogo. */
void terminarPrograma(GAME_ENV *game_environment) {
  registrarHighScore(game_environment);
  nocbreak(); /* Desfaz o cbreak() */
  keypad(stdscr, FALSE); /* Desativa o input por teclado númerico. */
  echo(); /* Ativa o echo do terminal. */
  curs_set(1); /* Deixa o cursor visível novamente. */
  standend(); /* Desativa todos os atributos ligados ao ncurses. */
  endwin(); /* Fecha todas as windows do ncurses. */
}

/* Cria uma WINDOW do ncurses para cada
   bloco do tabuleiro e armazena no vetor
   gameBoard.
*/
void createBoard(GAME_ENV *game_environment) {
  
  int coluna, linha;
  WINDOW * window; 
  for (coluna = 0; coluna < game_environment->height; coluna++) {
    for (linha = 0; linha < game_environment->width; linha++) {
      /* cria uma window na posição (coluna, linha). */
      window = newwin(
          SQUARE_HEIGHT, SQUARE_WIDTH, coluna * SQUARE_HEIGHT,
          linha * SQUARE_WIDTH); 
      /* Salva a window na estrutura do game_env */
      game_environment->gameBoard[coluna * game_environment->height + linha] =
          window; 
    }
  }
  /* criamos a window que mostra o nome do jogo, a pontuação, o high score e as rodadas. */
  game_environment
      ->gameBoard[game_environment->height * game_environment->width] =
      newwin(SQUARE_HEIGHT * game_environment->height, SQUARE_WIDTH * 3, 0,
             game_environment->width * SQUARE_WIDTH);
  
}

/* Processa o input do jogador e devolve a direção escolhida baseada na letra. */  
int processUserMove(int userMove) {
  
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

/* Função que inverte as colunas com as linhas de uma matriz.
   Aceita o ponteiro do vetor a ser transposto e já modifica-a diretamente.
*/ 
void transporMatriz(int **matriz) {
  
  int coluna, linha, j;
  int temp_array[BOARD_WIDTH * BOARD_HEIGHT];
  j = 0;
  for (coluna = 0; coluna < BOARD_HEIGHT; coluna++) {
    for (linha = 0; linha < BOARD_WIDTH; linha++) {
      temp_array[j++] = (*matriz)[linha*BOARD_HEIGHT + coluna];
    }
  }
  for (j = 0; j < BOARD_WIDTH * BOARD_HEIGHT; j++) {
    (*matriz)[j] = temp_array[j];
  }
}

/* Função que, a cada linha, troca os valores da primera posição com a última, da segunda posição com a 
   antepenúltima, e assim por diante, essencialmente trocando os valores da matriz baseando-se num eixo
   vertical imaginário posicionado no meio da matriz.
*/
void inverterMatriz(int **matriz) {
  int coluna, linha, temp;
  for (coluna = 0; coluna < BOARD_HEIGHT; coluna++) {
    for (linha = 0; linha < BOARD_WIDTH / 2 ; linha++) {
      /* salva o valor de matriz[coluna][linha] */
      temp = (*matriz)[coluna*BOARD_HEIGHT + linha]; 
      (*matriz)[coluna*BOARD_HEIGHT + linha] = (*matriz)[coluna*BOARD_HEIGHT + BOARD_WIDTH - linha - 1];
      (*matriz)[coluna*BOARD_HEIGHT + BOARD_WIDTH - linha - 1] = temp;
    }
  }
}

/* Ao combinar as funções transporMatriz e inverterMatriz, o efeito produzido é que a matriz foi
   rotacionada 90 graus para a direita.*/
void rotacionarMatrix90Graus(int **matriz) {
  transporMatriz(matriz);
  inverterMatriz(matriz);
}

/* move todas os blocos com valores para a esquerda. 
   ideia para a função foi tirada de:
   https://flothesof.github.io/2048-game.html

   TODO: ajeitar
*/
void moverTabuleiroParaEsquerda(int ** matriz, GAME_ENV *game_environment) {
  
  int i, j, anterior, coluna, linha, quadrado;
  int *nova_linha = malloc(sizeof(int) * game_environment->width);
  for (coluna = 0; coluna < game_environment->height; coluna++) {
    memset(nova_linha, 0, sizeof(int) * game_environment->width);
    anterior = 0;
    j = 0;
    for (linha = 0; linha < game_environment->width; linha++) {
      /* salva o valor de matriz[coluna][linha] */
      quadrado = (*matriz)[coluna*game_environment->height + linha]; 
      if (quadrado > 0) {
        if (anterior == 0) {
	  anterior = quadrado;
        }
	else {
          if (anterior == quadrado) {
	    nova_linha[j] = 2 * quadrado; /* adiciona na nova linha 2x o  quadrado */
            if (matriz == &game_environment->gamePositions) {
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
      (*matriz)[coluna*game_environment->height + i] = nova_linha[i];
    }
  }
  free(nova_linha);
}

/* Rotaciona o tabuleiro para a orientação correta, depois
   movimenta o tabuleiro para a esquerda e depois rotaciona
   para a orientação original.    
*/
void executarMovimento(int **matriz, int direcao, GAME_ENV *game_environment) {
  
  int index;
  for (index = 0; index < direcao; index++) {
    rotacionarMatrix90Graus(matriz);
  }
  
  moverTabuleiroParaEsquerda(matriz, game_environment);
  for (index = 0; index < 4 - direcao; index++) {
    rotacionarMatrix90Graus(matriz);
  }
  if (matriz == &game_environment->gamePositions) {
    game_environment->rounds++;
  }
}

/* Função que vai registrar o high score do jogador num arquivo,
   para que fique registrado e com fácil acesso. 
*/
void registrarHighScore(GAME_ENV *game_environment){
  FILE *fhs; /* Arquivo que guarda o high score. */
  char stringHighScore[11]; /* O último high score em string. O tamanho máximo de uma int é 10. */
  int ultimoHighScore;
  /* Podemos assumir que fhs existe pois a função pegaHighScore é chamada antes e esta já cria a pasta, caso não exista. */
  fhs = fopen("high_score.txt", "r");
    fscanf(fhs, "%s", stringHighScore);
    ultimoHighScore = atoi(stringHighScore); /* Converte o high score de string para int. */
    if (game_environment->highScore > ultimoHighScore){
      freopen("high_score.txt", "w", fhs); /* Reabre o arquivo, deletando seu conteúdo, já qque o jogador fez sua maior pontuação até agora. */
      fprintf(fhs, "%d", game_environment->highScore);
    }
  fclose(fhs);
}

/* Função que lê o high score do jogador do arquivo high_score.txt 
   e salva ele no game_env para que possa ser visualizado pelo jogador
   a todo momento quando joga.  
*/
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

/* Função que cria um valor (com 90% de chance de ser 2 e 10% de ser 4)
   e o coloca aleatoriamente em um bloco vazio qualquer do tabuleiro.
*/
int createRandomSquare(GAME_ENV * game_environment){
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
  randomSquareIndex = (int) rand() / (RAND_MAX + 1.0) * qntdQuadradosVazios; /* Escolhemos um numero de [0, qntdquadradosvazios-1] */
  dois_ou_quatro = rand() / (RAND_MAX + 1.0);
  newTile = dois_ou_quatro > 0.9 ? 4 : 2; /* 90% de chance de ser 2 e 10% de ser 4 (probabilidades advindas da internet) */
  randomSquarePosition = temp_buffer[randomSquareIndex]; /* Pegamos uma posição aleatória de um bloco do tabuleiro que esteja vazio. */
  game_environment->gamePositions[randomSquarePosition] = newTile; /* criamos um novo valor com as probablidades supracitadas nessa posição */
  free(temp_buffer);
  return 0;
}

/* Função que testa se o movimento inputado muda o tabuleiro ou não.
   O teste é feito fazendo esse movimento num tabuleiro temporário idêntico
   e comparando se esse tabuleiro temporário ainda é igual ao tabuleiro
   original.
*/
int testIfMoveChangedBoard(GAME_ENV *game_environment, int direcao) {
  int index;
  int *temporary_board = calloc(sizeof(int) * game_environment->width * game_environment->height, sizeof(int));
  memcpy(temporary_board, game_environment->gamePositions, sizeof(int) * game_environment->width * game_environment->height);
  executarMovimento(&temporary_board, direcao, game_environment);
  for (index = 0; index < game_environment->width * game_environment->height; index++) {
    if (game_environment->gamePositions[index] != temporary_board[index]) {
      free(temporary_board);
      return 1; /* O movimento mudou pelo menos um bloco do tabuleiro. */
    }
  }
  free(temporary_board);
  return 0; /* O movimento não mudou o tabuleiro. */
}

/* Função que basicamente começa o jogo e mostra o tabuleiro inicial, com dois blocos
   de valor 2 ou 4 (90% de chance de ser 2 e 10% de ser 4).
   
   Também inicia a visualização do tabuleiro através do ncurses, com o high score do jogador. 
*/
void startGameEnvironment(GAME_ENV *game_environment) {
  initscr(); /* Inicializa a tela ncurses. */
  if (has_colors() != TRUE) {
    printf("Seu terminal nao suporta 256 cores. Troque para um terminal que suporte 256 cores ou mais para utilizar o programa.");
    exit(-1); /* O usuário não possui terminal com suporte para 256 cores ou mais. */
  }
  start_color(); /* Inicia as cores a serem usadas no ncurses. */
  use_default_colors(); /* Usamos as cores padrão do terminal. */
  initColorPairs(); /* Inicializa os pares de cores para o ncurses (cor de fundo e cor da fonte). */
  keypad(stdscr, TRUE); /* Para pegar inputs do keypad (setinhas). */
  cbreak(); /* Input do usuário passa a ser lido no momento que escreve algo. */
  noecho(); /* Desativa a visualização dos inputs do usuário no terminal. */
  curs_set(0); /* Deixa o cursor do terminal invisível. */
  refresh(); /* Atualiza a tela. */
  srand(time(NULL)); /* Inicializamos a semente para os números randômicos, baseado no tempo. */
  game_environment->width = BOARD_WIDTH;
  game_environment->height = BOARD_HEIGHT;
  game_environment->gamePositions = calloc(sizeof(int) * game_environment->height * game_environment->width, sizeof(int));
  game_environment->gameBoard = malloc(sizeof(void*) * (game_environment->height * game_environment->width + 1)); /* Inicializamos o vetor que segurará as windows. */
  game_environment->gameStatus = 0;
  /* gameStatus = 0  -> o jogo está rodando.
     gameStatus = 1  -> o jogo foi ganho.
     gameStatus = -1 -> o jogo foi perdido.
  */
  game_environment->actualScore = 0; /* pontuação inicia com 0. */
  game_environment->rounds = 0;
  pegaHighScore(game_environment);
  createBoard(game_environment); /* cria todos as windows e as salva no vetor gameBoard. */
  createRandomSquare(game_environment);
  createRandomSquare(game_environment); /* chamamos 2 vezes para criar 2 blocos iniciais aleatórios. */
  blitToScreen(game_environment);
}

/* Função que é chamada quando o jogador aperta 'q' no meio de um jogo. A tela de fim de jogo será mostrada e
   se o jogador apertar 'q' novamente, encerra-se o programa, e se apertar 'r', outro jogo é iniciado.
*/
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
  case 'q': /* o usuário escolheu sair do jogo. */
    terminarPrograma(game_environment);
    break;
  case 'R':
  case 'r': /* o usuário escolheu jogar novamente. */
    registrarHighScore(game_environment);
    free(game_environment->gamePositions);
    free(game_environment->gameBoard);
    startGameEnvironment(game_environment);
    runGameLoop(game_environment);
  }
}

/* Função que é chamada toda rodada para ver se o jogador perdeu. O jogador perde quando não há
   nenhm movimento possível para nenhuma das quatro direções, ou seja, o tabuleiro não poe ser 
   mudado.
*/
int testIfGameIsLost(GAME_ENV *game_environment) {
  int direcao, changed;
  for (direcao = 0; direcao < 4; direcao++) {
    /* Testamos as quatro possíveis direções para jogar. */
    changed = testIfMoveChangedBoard(game_environment, direcao);
    if (changed == 1) {
      return 0; /* O jogo deve continuar, pois ainda há movimentos possíveis. */
    }
  }
  return -1; /* O jogo acabou. */
}

/* Função que é chamada toda rodada para ver se o jogador ganhou. O jogador ganha se houver
   um bloco com valor 2048.
*/
int testIfGameIsWon(GAME_ENV *game_environment) {
  int tile;
  for (tile = 0; tile < game_environment->width * game_environment->height;
       tile++) {
    if (game_environment->gamePositions[tile] == 2048) {
      return 1; /* O jogo foi ganho. */
    }
  }
  return 0; /* O jogo deve continuar. */
}

/* Inicia o laço o jogo, que continua até que o jogador perca, ganhe ou pare o jogo com 'q'. */
void runGameLoop(GAME_ENV * game_environment) {
  int direcao, userMove, movimentoMudouTabuleiro;
  do {
    userMove = getch();
    direcao = processUserMove(userMove); /* Retorna -1 se a direção não for válida. */
    if (direcao == EXIT) {
      break;
    }
    if (direcao != -1) {
      movimentoMudouTabuleiro = testIfMoveChangedBoard(game_environment, direcao);
      if (movimentoMudouTabuleiro == 1) {
        executarMovimento(&game_environment->gamePositions, direcao, game_environment);                
        createRandomSquare(game_environment);
        blitToScreen(game_environment);
        game_environment->gameStatus = testIfGameIsLost(game_environment);
	      game_environment->gameStatus += testIfGameIsWon(game_environment);
      }
    }
  } while (game_environment->gameStatus == 0);
  manageFimDeJogo(game_environment);
}


