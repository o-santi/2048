# 2048
Trabalho de computação 1 (CC)

O principal objetivo do jogador é mover os blocos com números potências de 2 para formar o número 2048, podendo fazer movimentos para a esquerda, direita, cima e baixo. O jogo começa com dois blocos, cada um com 90% de probabilidade de ser o número 2 e 10% de ser o número 4. Quando dois blocos de mesmo número se juntam, formam um novo bloco, que agora tem número igual à soma dos dois blocos anteriores. A cada rodada, um novo bloco é adicionado aleatoriamente no tabuleiro, com as mesmas probabilidades para ser um número 2 ou número 4. Se o tabuleiro estiver totalmente populado por blocos e não houver possibilidade de juntar dois blocos, o jogador perde. A pontuação do jogo é calculada somando todos os blocos juntados pelo jogador.

Esse código implementa o jogo "2048" em C usando *ncurses* para uma melhor interface de usuário.

Uma *estrutura* guarda diversas variáveis, como o vetor que representa o tabuleiro, a pontuação, a quantidade de rodadas, etc.
Quando se quiser alterar essas variáveis, apenas passamos o ponteiro correspondente.

Para começar o jogo, criamos um tabuleiro vazio e adicionamos dois blocos aleatórios com as probabilidades para seus valores ditas acima. Inicializamos a interface de jogo usando a biblioteca *ncurses* e mostramos no terminal.

Para movimentar os blocos, usamos o seguinte truque: Se o movimento desejado pelo jogador é para a esquerda, movemos normalmente todos os blocos simultaneamente para a esquerda, e se o movimento desejado pelo jogador é para cima, direita ou baixo, rotacionamos o tabuleiro 90° no sentido  horário 3, 2 ou 1 vez, respectivamente, movimentamos todos os blocos para a esquerda, e rotacionamos o tabuleiro 1, 2 ou 3 vezes, respectivamente, para retornar o tabuleiro à orientação original. Em todo o movimento realizado pelo jogador, checamos se o movimento altera a posição de pelo menos um bloco do tabuleiro. Se sim, processamos o movimento. adicionamos um bloco aleatório e o jogo continua. Se não, checamos se o jogador perdeu ou não (mesmo que o tabuleiro esteja cheio, ainda pode haver a possibilidade de juntar blocos).

Fazemos isso porque a implementação das funções que rotacionam o tabuleiro e que movem todos os blocos para a esquerda é relativamente fácil, ao invés de fazer uma função para cada movimento posível do jogo.

Na tela de *GAME OVER* ou na tela de quando o jogador ganha o jogo, pode-se apertar a tecla 'q' para sair do jogo ou 'r' para começar outro jogo. Se estiver no meio de um jogo, pode-se apertar a tecla 'q' para desistir ddo jogo atual e sair, e se quiser começar outro jogo logo em seguida, pode-se apertar a tecla 'r'.








