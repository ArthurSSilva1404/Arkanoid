#include "raylib.h"

#include <iostream>

#include "Estruturas.h"

#ifndef ESTADO_DO_JOGO
#define ESTADO_DO_JOGO

#define MAX_ITENS 100

struct EstadoJogo {

    Bola bola;
    Barra barra;
    Fase fase;
    ItensEspeciais itens[MAX_ITENS];
    Jogador jogador;
    int dificuldade;
    int qtd_itens;
    int fase_atual;
   // Font fonte;

};

#endif
