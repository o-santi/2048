/* Implementação do jogo 2048 em C para Computação 1 (CC)
   Feito por :
   Leonardo Santiago DRE 120036072
   Miguel Uchoa      DRE 120036412
   Caio Monteiro     DRE 120036373
*/

#define SQUARE_WIDTH 11
#define SQUARE_HEIGHT 11
#define BOARD_WIDTH 4
#define BOARD_HEIGHT 4


#include <ncurses/curses.h>
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
      box(window, 0, 0);  //desenha a box default envolta da window
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
    if (numberAtTile > 0) { /* only print to screen if it's not 0 */
      itoa(numberAtTile, to_char_buffer, 10); /* print it to buffer*/
      mvwaddstr(game_environment->gameBoard[index], SQUARE_WIDTH / 2, SQUARE_WIDTH / 2,to_char_buffer); /* print buffer to window */
    }
  }
  refresh(); /* update screen */
  free(to_char_buffer);
}

int processUserMove(int userMove) {
  /* Processa o input do player e devolve a direcao escolhida baseada na letra
     TODO: pensar em valores mais meaningful pra esses returns
   */
  switch (userMove) {
  case 'D':
  case 'd':
  case KEY_RIGHT:
    return 1;
  case 'w':
  case 'W':
  case KEY_UP:
    return 2;
  case 's':
  case 'S':
  case KEY_DOWN:
    return 3;
  case 'a':
  case 'A':
  case KEY_LEFT:
    return 4;
  default:
    return -1;

  }
}

int moveBoard(char direction);

int getArrayLength(int * array){
  return sizeof(*array) / sizeof(array[0]);
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
  newTile = dois_ou_quatro > 0.9 ? 2 : 4; // 90% de chance de ser 2 e 10% de ser 4 (probabilidades advindas da internet)
  randomSquarePosition = temp_buffer[randomSquareIndex]; // pegamos a posição que está nesse Tile
  game_environment->gamePositions[randomSquarePosition] = newTile; // criamos um novo tile nessa posição
  free(temp_buffer);
  return 0;
  }

void startGameEnvironment(GAME_ENV * game_environment) {
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
  createBoard(game_environment); // cria todos as windows e salva no vetor gamePositions
  createRandomSquare(game_environment);
  createRandomSquare(game_environment); //chamamos 2 vezes para criar 2 quadrados iniciais aleatorios
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
    if (direction != 1) {
      moveBoard(userMove);
      blitToScreen(game_environment);
    }
  } while (game_environment->gameStatus);
}


int main(void) {
  GAME_ENV* game_environment = malloc(sizeof(GAME_ENV));
  startGameEnvironment(game_environment);
  runGameLoop(game_environment); 
  finishGame(game_environment);
  
  return 0;
}
