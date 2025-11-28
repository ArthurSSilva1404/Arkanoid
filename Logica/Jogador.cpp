#include "Estruturas.h"
#include "Jogador.h"
#include "Constantes.h"

void atualizarPontuacao(Jogador *jogador, int pontuacao) {
    jogador->pontuacao += pontuacao;
    if (jogador->pontuacao < 0) {
        jogador->pontuacao = 0;
    }
}

void atualizarTempo(Jogador *jogador) {
    jogador->tempo++;
}

void atualizarVidas(Jogador *jogador) {
    jogador->vidas_jogador--;
}
