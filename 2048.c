/* Implementação do jogo 2048 em C para Computação 1 (CC)
   Feito por :
   Leonardo Santiago DRE 120036072
   Miguel Uchoa      DRE 120036412
   Caio Monteiro     DRE 120036373
*/


/* BOARD CONSTANTS*/
#define SQUARE_WIDTH 11
#define SQUARE_HEIGHT 7
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4


/* SYSTEM CONSTANTS*/
#define RIGHT 2
#define UP 3
#define DOWN 1
#define LEFT 0
/* como estamos virando o tabuleiro 90 graus pra mover,
   esses valores se referem a quantidade de rotacoes necessarias
   pra depois também poder mover pra esquerda
*/


#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


/* Estrutura que irá segurar todas as variaveis do jogo
   Criamos um ponteiro para ela e
   passamos o ponteiro pelas funcoes
*/
typedef struct{
  int actualScore;
  int highScore;
  int gameStatus;
  int rounds;
  int * gamePositions;
  WINDOW ** gameBoard;
} GAME_ENV;


int getArrayLength(int * array){
  return sizeof(*array) / sizeof(array[0]);
}

void finishGame(GAME_ENV * game_environment);

void createBoard(GAME_ENV *game_environment) {
  /* Cria uma WINDOW do ncurses para cada
     quadrado do board e armazena no vetor
     gameBoard
  */
  int coluna, linha;
  WINDOW * window; 
  for (coluna = 0; coluna < BOARD_HEIGHT; coluna++) {
    for (linha = 0; linha < BOARD_WIDTH; linha++) {
      window = newwin(SQUARE_HEIGHT, SQUARE_WIDTH, coluna * SQUARE_HEIGHT, linha * SQUARE_WIDTH); // cria uma window na posição (coluna, linha)
      game_environment->gameBoard[coluna*BOARD_HEIGHT + linha] = window; // Salva a window na estrutura do game env
    }
  }
}

void blitToScreen(GAME_ENV *game_environment) {
  int index, numberAtTile;
  char *to_char_buffer = malloc(sizeof * to_char_buffer * 10); /* colocamos 10 chars so pra ter certeza, apesar de nao conhecer ninguem que chegaria em 10 digitos no 2048*/
  for (index = 0; index < BOARD_HEIGHT * BOARD_WIDTH; index++) {
    wclear(game_environment->gameBoard[index]); /* erase the tile in window*/
    numberAtTile = game_environment->gamePositions[index]; /* number at the current tile*/
    //if (numberAtTile > 0) { /* only print to buffer if it's not 0 */
    sprintf(to_char_buffer, "%d", numberAtTile); /* print it to buffer*/
    mvwaddstr(game_environment->gameBoard[index], SQUARE_WIDTH / 2, SQUARE_WIDTH / 2,to_char_buffer); /* print buffer to window */
    box(game_environment->gameBoard[index], 0, 0);  // desenha a box default envolta da window
    wrefresh(game_environment->gameBoard[index]); // refresh box
    //}
  }
  free(to_char_buffer);
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

void moverTabuleiroParaEsquerda(int **matrix) {
  /* move todas as tiles para a esquerda 
     ideia para a função foi tirada de:
     https://flothesof.github.io/2048-game.html

     TODO: ajeitar essa bagunça
   */
  int i, j, anterior, coluna, linha, quadrado;
  for (coluna = 0; coluna < BOARD_HEIGHT; coluna++) {
    int *nova_linha = malloc(sizeof(int) * BOARD_WIDTH);
    memset(nova_linha, 0, sizeof(int) * BOARD_WIDTH);
    anterior = 0;
    j = 0;
    for (linha = 0; linha < BOARD_WIDTH; linha++) {
      quadrado = (*matrix)[coluna*BOARD_HEIGHT + linha]; // salva o valor de matrix[coluna][linha]
      if (quadrado > 0) {
        if (anterior == 0) {
	  anterior = quadrado;
        }
	else {
          if (anterior == quadrado) {
	    nova_linha[j] = 2 * quadrado; /* adiciona na nova linha 2x o  quadrado */
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
    for (i = 0; i < BOARD_WIDTH; i++) {
      (*matrix)[coluna*BOARD_HEIGHT + i] = nova_linha[i];
    }
    free(nova_linha);
  }
}

void executarMovimento(int direcao, GAME_ENV *game_environment) {
  /* rotaciona o tabuleiro pra direção correta, depois
     movimenta o tabuleiro pra esquerda e depois rotaciona
     pra direcao correta novamente

     TODO: ajeitar esta merda
   */
  int index;
  for (index = 0; index < direcao; index++) {
    rotacionarMatrix90Graus(&game_environment->gamePositions);
  }
  moverTabuleiroParaEsquerda(&game_environment->gamePositions);
  for (index = 0; index < 4- direcao; index++) {
    rotacionarMatrix90Graus(&game_environment->gamePositions);
  }
}


int createRandomSquare(GAME_ENV * game_environment){
  /* Escolhe um quadrado vazio do tabuleiro e adiciona
     uma tile nele
   */
  int qntdQuadradosVazios, randomSquareIndex, randomSquarePosition, index, newTile;
  int * temp_buffer = malloc(sizeof(int) * BOARD_WIDTH * BOARD_HEIGHT);
  float dois_ou_quatro;
  qntdQuadradosVazios = 0;
  for (index=0; index < BOARD_HEIGHT * BOARD_WIDTH; index ++){
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
  newTile = dois_ou_quatro > 0.9 ? 4 :
    2; // 90% de chance de ser 2 e 10% de ser 4 (probabilidades advindas da
       // internet)
    randomSquarePosition =
        temp_buffer[randomSquareIndex]; // pegamos a posição que está nesse Tile
    game_environment->gamePositions[randomSquarePosition] =
        newTile; // criamos um novo tile nessa posição
    free(temp_buffer);
    return 0;
  }

void startGameEnvironment(GAME_ENV * game_environment) {
  initscr(); // inicia a curses screen
  keypad(stdscr, TRUE); // pega o input do keypad
  cbreak(); // desativa line breaking
  noecho(); // desativa printing to screen
  refresh();
  srand(time(NULL)); // inicializamos a seed para os numeros randomicos, baseado no tempo
  game_environment->gamePositions = malloc(sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);
  game_environment->gameBoard = malloc(sizeof(WINDOW) * BOARD_HEIGHT * BOARD_WIDTH); // inicializamos o array que segurará as windows
  game_environment->gameStatus = 1; // gamestatus será valido
  game_environment->actualScore = 0; // score inicia com 0
  game_environment->rounds = 0;
  createBoard(game_environment); // cria todos as windows e salva no vetor gamePositions
  createRandomSquare(game_environment);
  createRandomSquare(game_environment);
  createRandomSquare(game_environment);
  createRandomSquare(game_environment); //chamamos 2 vezes para criar 2 quadrados iniciais aleatorios
  blitToScreen(game_environment);
}

void runGameLoop(GAME_ENV * game_environment) {
  // Inicia o game loop e mantem enquanto
  // o jogo nao acabou
  int direction;
  int userMove;
  do {
    userMove = getch();
    direction= processUserMove(userMove);
    // retorna a direcao escolhida,
    // se for -1, o movimento nao é valido
    // TODO: piscar a tela de vermelho quando for invalido
    if (direction != -1) {
      executarMovimento(direction, game_environment);
      createRandomSquare(game_environment);
      blitToScreen(game_environment);
    }
  } while (game_environment->gameStatus);
}


int main(void) {
  GAME_ENV* game_environment = malloc(sizeof(GAME_ENV));
  startGameEnvironment(game_environment);
  runGameLoop(game_environment); 
  //finishGame(game_environment);
  
  return 0;
}
