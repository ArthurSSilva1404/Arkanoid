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
    SistemaParticulas sistemaParticulas;
    EstatisticasPartida estatisticas;
    Jogador jogador;
    int dificuldade;
    int qtd_itens;
    int fase_atual;
    bool aguardandoLancamento;
    bool jogoConcluido;
    float tempoFase;
    float multiplicadorVelocidade;
    bool escudoAtivo;
    float tempoEscudoRestante;
    Rectangle escudoArea;
    Meteoro meteoros[MAX_METEOROS];
    float tempoParaProximoMeteoro;
};

void prepararEstadoJogo(EstadoJogo *estado, const char *nome, int dificuldade);

void reiniciarElementosDaFase(EstadoJogo *estado);

float obterMultiplicadorDificuldade(int dificuldade);

const char *obterNomeDificuldade(int dificuldade);

void consolidarPontuacaoFinal(EstadoJogo *estado);

#endif
