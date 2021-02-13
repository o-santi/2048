// Implementação do jogo 2048 em C para Computação 1 (CC)
// Feito por :
// Leonardo Santiago DRE 120036072
// Miguel Uchoa      DRE
// Caio Monteiro     DRE 120036373


#define SQUARE_WIDTH 11
#define SQUARE_HEIGHT 11
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4


#include <ncurses/curses.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


// Estrutura que irá segurar todas as variaveis do jogo
// Criamos um ponteiro para ela e
// passamos o ponteiro pelas funcoes
typedef struct{
  int actualScore;
  int highScore;
  int gameStatus;
  int rounds;
  int * gamePositions;
  WINDOW ** gameBoard;
} GAME_ENV;


void finishGame(GAME_ENV * game_environment);

void createBoardFromGamePosition(GAME_ENV *game_environment) {
  /* Cria uma WINDOW do ncurses para cada
     quadrado do board e armazena no vetor
     gameBoard
  */
  int coluna, linha;
  WINDOW * window; 
  for (coluna = 0; coluna < BOARD_HEIGHT; coluna++) {
    for (linha = 0; linha < BOARD_WIDTH; linha++) {
      window = newwin(SQUARE_HEIGHT, SQUARE_WIDTH, coluna * SQUARE_HEIGHT, linha * SQUARE_WIDTH); // cria uma window na posição (coluna, linha)
      box(window, 0, 0);  //desenha a box default envolta da window
      game_environment->gameBoard[coluna*BOARD_HEIGHT + linha] = window; // Salva a window na estrutura do game env
    }
  }
}
void updateBoard(GAME_ENV * game_environment);

void blitToScreen(GAME_ENV * game_environment);

int processUserMove(char userMove, GAME_ENV * game_environment);

int getArrayLength(int * array){
  return sizeof(*array) / sizeof(array[0]);
}

int createRandomSquare(GAME_ENV * game_environment){
  /* Escolhe um quadrado vazio do tabuleiro e adiciona
     uma tile nele
   */
  int qntdQuadradosVazios, randomSquareIndex, randomSquarePosition, index;
  int * temp_buffer;
  qntdQuadradosVazios = 0;
  temp_buffer = malloc(sizeof(int) * BOARD_WIDTH * BOARD_HEIGHT);
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
  randomSquarePosition = temp_buffer[randomSquareIndex]; // pegamos a posição que está nesse index
  game_environment->gamePositions[randomSquarePosition] = 2; //criamos um novo tile nessa posição
  free(temp_buffer);
  return 0;
}

void startGameEnvironment(GAME_ENV* game_environment) {
  initscr(); // inicia a curses screen
  keypad(stdscr, TRUE); // pega o input do keypad
  cbreak(); // desativa line breaking
  noecho(); // desativa printing to screen
  srand(time(NULL)); // inicializamos a seed para os numeros randomicos, baseado no tempo
  game_environment->gamePositions = malloc(sizeof(int) * BOARD_HEIGHT * BOARD_WIDTH);
  game_environment->gameBoard = malloc(sizeof(WINDOW) * BOARD_HEIGHT * BOARD_WIDTH); // inicializamos o array que segurará as windows
  game_environment->gameStatus = 1; // gamestatus será valido
  game_environment->actualScore = 0; // score inicia com 0
  game_environment->rounds = 0;
  createBoardFromGamePosition(game_environment); // cria todos as windows e salva no vetor gamePositions
  createRandomSquare(game_environment);
  createRandomSquare(game_environment); //chamamos 2 vezes para criar 2 quadrados iniciais aleatorios
  
  
}

void runGameLoop(GAME_ENV * game_environment) {
  // Inicia o game loop e mantem enquanto
  // o jogo nao acabou
  int is_valid;
  int userMove;
  //gameStatus representa o estado atual do jogo
  //quando = 1 -> significa que o jogo deve continuar
  //quando = 0 -> significia que o jogador perdeu
  //quando != 1 && !=0 -> significa que houve algum erro
  do {
    userMove = getch();
    is_valid = processUserMove(userMove, game_environment);
    if (is_valid==1) {
      updateBoard(game_environment);
      blitToScreen(game_environment);
    }
  } while (game_environment->gameStatus);
}


int main(void) {
  GAME_ENV * game_environment;
  startGameEnvironment(game_environment);
  runGameLoop(game_environment);
  finishGame(game_environment);
  
  return 0;
}
