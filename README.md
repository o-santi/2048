
# Tabela de Conteúdos

1.  [Objetivos](#orgd9c7fad)
2.  [Premissas](#org193defe)
    1.  [Funcionamento do jogo](#org2067556)
    2.  [Especificações do usuário](#org554c980)
3.  [Regras](#orgb9b511e)
    1.  [Comandos do jogo](#orgd448981)
    2.  [Compilação](#orgd6b75f1)
4.  [Saída](#orgf5ad89e)
5.  [Ferramentas Utilizadas](#orgd8df0e8)

<a id="orgd9c7fad"></a>

# Objetivos

Implementar o jogo 2048 em C para o terminal com uma interface com cores (utilizando ncurses) e com um high score que persiste mesmo após o término da execução do programa.


<a id="org193defe"></a>

# Premissas


<a id="org2067556"></a>

## Funcionamento do jogo

O jogo terá sempre, no máximo, um tabuleiro rodando por vez. Quando o jogo é terminado, não terá nenhum tabuleiro ativo.

As informações do tabuleiro serão guardadas em uma estrutura `GAME_ENV` e as funções e métodos irão operar sobre a estrutura geral (para evitar o uso de variáveis de escopo global)


<a id="org554c980"></a>

## Especificações do usuário

Como utilizamos `ncurses` para a parte visual, assumimos que o usuário utilize um sistema `unix`, seja ele uma distribuição `linux` que suporte a biblioteca ou `MacOS` (testado em ambos sistemas operacionais). Além disso, assumimos que o usuário possui um terminal que suporte, no mínimo, 256 cores, caso contrário toda a parte do ncurses que utiliza cores não irá funcionar; tal premissa é razoável dado que a maior parte dos terminais modernos possuem suporte para 256 cores. Por fim, para compilar o jogo localmente, assumimos também que o jogador possui a biblioteca `ncurses` instalada.


<a id="orgb9b511e"></a>

# Regras


<a id="orgd448981"></a>

## Comandos do jogo

A movimentação do tabuleiro poderá ser feita com as teclas WASD (maiúscula ou minúscula) bem como com as setas do keypad.
Para terminar o jogo atual, o usuário pode apertar a letra 'q' (maiúscula ou minúscula) em qualquer momento enquanto o jogo estiver ativo, e então será levado para a tela de "Fim de jogo".

Na tela de "Fim de jogo", o usuário pode apertar a letra 'q' (maiúscula ou minúscula) novamente para finalizar a execução do programa (e o highscore será salvo automaticamente) ou apertar a letra 'r' (maiúscula ou minúscula) para iniciar um novo jogo.
Suporte para finalizar a execução precocemente (através do input `Ctrl-C`) **não** foi adicionado. Caso o usuário deseje fechar o jogo, deve apertar 'q' duas vezes.


<a id="orgd6b75f1"></a>

## Compilação

A makefile utilizada facilita o processo de compilação. Para compilar, o usuário apenas precisa rodar o comando `make build` após clonar o repositório. Para deletar os arquivos criadas, deve-se usar o comando `make clean`.


<a id="orgf5ad89e"></a>

# Saída

-   Ao executar o programa, será primeiramente exibido na tela o tabuleiro `4x4` inicial junto da interface de usuário à direita. Cada casa que tiver uma peça ativa será colorida com a cor da peça, e as que estão vazias serão coloridas com a cor padrão do tabuleiro.
-   Ao movimentar o tabuleiro, caso o movimento mude o estado atual das peças, as informações de `score`, `highscore` e `rounds` serão devidamente atualizadas e o novo tabuleiro será exibido na tela; caso não mude o estado atual, nada será feito.
-   Ao apertar "q", a tela ficará vermelha, sinalizando ao usuário que o jogo foi terminado. Nesse momento, também será uma saída o arquivo `high_score.txt`, que será criado para armazenar o atual highscore (caso seja maior que o antigo).
    -   Ao apertar "r" na tela de fim de jogo, o background voltará às cores normais, e a interface de usuário retornará à tela padrão com o score e rounds reiniciados, e o highscore devidamente definido, para indicar que um novo jogo foi criado.
    -   Ao apertar "q" novamente na tela de fim de jogo, o programa salva o highscore novamente e finaliza a execução
-   Ao atingir uma casa com 2048, o background ficará verde, indicando ao usuário que o jogo foi ganho; o highscore será salvo e os mesmos comandos da tela de fim de jogo("r" para reiniciar e "q" para sair) serão aceitos.


<a id="orgd8df0e8"></a>

# Ferramentas Utilizadas

-   Ponteiros e alocação dinâmica de memória foram constantemente utilizados, bem como o uso de vetores uni e bidimensionais, para armazenar as diversas informações do tabuleiro.
-   Multiarquivos e compilação com arquivos header.
-   Entrada e saída de arquivos utilizando as funções `fscanf` e `fprintf` da biblioteca `stdio`.
-   Estruturas e o acesso de variáveis nelas através de ponteiros.
-   Escopo de variáveis e funções.
-   Uso amplo de funções e variáveis.

