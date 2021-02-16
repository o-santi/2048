# 2048
Trabalho de computação 1 (CC)

## Regras do jogo 2048

O principal objetivo do jogador é mover os blocos com números potências de 2 para formar o número 2048, podendo fazer movimentos para a esquerda, direita, cima e baixo. O jogo começa com dois blocos, cada um com 90% de probabilidade de ser o número 2 e 10% de ser o número 4. 

Quando dois blocos de mesmo número se juntam, formam um novo bloco, que agora tem número igual à soma dos dois blocos anteriores. A cada rodada, um novo bloco é adicionado aleatoriamente no tabuleiro, com as mesmas probabilidades para ser um número 2 ou número 4. 

Se o tabuleiro estiver totalmente populado por blocos e não houver possibilidade de juntar dois blocos, o jogador perde. A pontuação do jogo é calculada somando todos os blocos juntados pelo jogador.

## A implementação
Esse código implementa o jogo "2048" em C usando `ncurses` para uma melhor interface de usuário.

A estrutura `GAME_ENV` guarda diversas variáveis, como o vetor que representa o tabuleiro, a pontuação, a quantidade de rodadas, etc.
Quando se quiser alterar essas variáveis, apenas passamos o ponteiro correspondente.
```C
typedef struct{
  int actualScore;
  int highScore;
  int gameStatus;
  int rounds;
  int * gamePositions;
  WINDOW ** gameBoard;
} GAME_ENV;
```
Na função `main`, apenas temos duas funções:
```C
int main(void) {
  GAME_ENV* game_environment = malloc(sizeof(GAME_ENV));
  startGameEnvironment(game_environment);
  runGameLoop(game_environment); 
  return 0;
}
```
* `startGameEnvironment` inicia a `curses` screen, inicia o jogo, zerando o número de rodadas e pontuação do jogador, cria dois blocos em posições aleatórias e mostra o tabuleiro recém-criado no terminal.
* `runGameLoop` pega, para cada rodada, o movimento desejado pelo jogador, testa se o movimento muda a posição de pelo menos um bloco no tabuleiro, e se mudar, executamos o movimento. A função que executa o movimento do jogador é a `executarMovimento`:
  * ```C
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
    ```
    Usamos o seguinte truque: Se o movimento desejado pelo jogador é para a esquerda, movemos normalmente todos os blocos simultaneamente para a esquerda, e se o movimento           desejado pelo jogador é para cima, direita ou baixo, rotacionamos o tabuleiro 90° no sentido  horário 3, 2 ou 1 vez, respectivamente, movimentamos todos os blocos para a         esquerda, e rotacionamos o tabuleiro 1, 2 ou 3 vezes, respectivamente, para retornar o tabuleiro à orientação original. Em todo o movimento realizado pelo jogador, checamos     se o movimento altera a posição de pelo menos um bloco do tabuleiro. Se sim, processamos o movimento. adicionamos um bloco aleatório e o jogo continua. Se não, checamos se o     jogador perdeu ou não (mesmo que o tabuleiro esteja cheio, ainda pode haver a possibilidade de juntar blocos).


 

Fazemos isso porque a implementação das funções que rotacionam o tabuleiro e que movem todos os blocos para a esquerda é relativamente fácil, ao invés de fazer uma função para cada movimento posível do jogo.

Na tela de *GAME OVER* ou na tela de quando o jogador ganha o jogo, pode-se apertar a tecla 'q' para sair do jogo ou 'r' para começar outro jogo. Se estiver no meio de um jogo, pode-se apertar a tecla 'q' para desistir ddo jogo atual e sair, e se quiser começar outro jogo logo em seguida, pode-se apertar a tecla 'r'.








