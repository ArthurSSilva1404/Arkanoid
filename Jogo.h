#include <iostream>

#include "Estruturas.h"

#ifndef ESTADO_DO_JOGO
#define ESTADO_DO_JOGO

#define MAX_BLOCOS 100

struct EstadoJogo {

    Bloco bloco[MAX_BLOCOS];
    Bola bola;
    Jogador jogador;
    int ranking;
    int niveis;
    int blocosRestantes;
    int vidas_jogador;
    int tempo;

};

#endif
