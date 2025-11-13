#include <iostream>

#include "Estruturas.h"

#ifndef ESTADO_DO_JOGO
#define ESTADO_DO_JOGO

#define MAX_BLOCOS 100

struct EstadoJogo {

    Bola bola;
    Jogador jogador;
    Bloco bloco[MAX_BLOCOS];
    int ranking;
    int niveis;
    int blocosRestantes;
    int vidas_jogador;

};

#endif
