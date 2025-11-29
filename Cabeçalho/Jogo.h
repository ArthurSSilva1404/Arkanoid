#include "raylib.h"

#include <iostream>

#include "Estruturas.h"
#include "Sons.h"

#ifndef ESTADO_DO_JOGO
#define ESTADO_DO_JOGO

#define MAX_ITENS 100

struct EstadoJogo {

    Bola bola;
    Barra barra;
    Fase fase;
    ItensEspeciais itens[MAX_ITENS];
    SistemaParticulas sistemaParticulas;
    SistemaAudio audio;
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
    float multiplicadorPontos;
    int tempoMultiplicador;
    int ultimoItemColetado;
    float tempoMostrarItem;
    bool inversorAtivo;
    int tempoInversor;
};

void prepararEstadoJogo(EstadoJogo *estado, const char *nome, int dificuldade);

void reiniciarElementosDaFase(EstadoJogo *estado);

float obterMultiplicadorDificuldade(int dificuldade);

const char *obterNomeDificuldade(int dificuldade);

void consolidarPontuacaoFinal(EstadoJogo *estado);

// Funções de controle do jogo
void atualizarJogo(EstadoJogo *estado, float dt);
void processarColisoes(EstadoJogo *estado);
void verificarCondicaoVida(EstadoJogo *estado, bool *perdeuJogo);
void avancarParaProximaFase(EstadoJogo *estado);
void resetarVida(EstadoJogo *estado);

#endif
