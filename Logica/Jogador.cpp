#include "../Cabeçalho/Jogador.h"
#include "../Cabeçalho/Estruturas.h"
#include <iostream>

void atualizarPontuacao(Jogador *jogador, int pontuacao) {
    if (jogador != nullptr) {
        jogador->pontuacao += pontuacao;
    }
}

void atualizarTempo(Jogador *jogador) {
    if (jogador != nullptr) {
        jogador->tempo++;
    }
}

void atualizarVidas(Jogador *jogador) {
    if (jogador != nullptr) {
        jogador->vidas_jogador--;
    }
}
