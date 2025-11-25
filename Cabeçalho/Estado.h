#include "Estruturas.h"

#ifndef ESTADO_H
#define ESTADO_H

// Estados do jogo
#define ESTADO_MENU 0
#define ESTADO_DIFICULDADE 1
#define ESTADO_JOGANDO 2
#define ESTADO_GAME_OVER 3
#define ESTADO_RANKING 4
#define ESTADO_SAIR 5
#define ESTADO_PAUSA 6

// Estrutura para gerenciar o estado global do jogo
struct GerenciadorEstado {
    int estado_atual;
    int estado_anterior;
    bool em_pausa;
    float tempo_total;
    int blocos_quebrados_total;
};

void inicializarEstado(GerenciadorEstado *gerenciador);

void mudarEstado(GerenciadorEstado *gerenciador, int novo_estado);

void pausarJogo(GerenciadorEstado *gerenciador);

void retomarJogo(GerenciadorEstado *gerenciador);

void resetarEstado(GerenciadorEstado *gerenciador);

#endif
