#include <iostream>

#include "Constantes.h"
#include "raylib.h"

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

struct Posicao {
    float x;
    float y;
};

struct Retangulo2D {
    Posicao posicao;
    int largura;
    int altura;
};

struct Bloco {
    Retangulo2D retangulo;
    bool ativo;
    int TotalDeVida;
    Color cor;
    int pontosBase;
    bool possuiItem;
    bool indestrutivel;
    int tipo;
};

struct Bola {
    Posicao posicao;
    float radium;
    float velocidadex;
    float velocidadey;
    float velocidadeBase;
    bool prendeu;
};

struct Barra {
    Retangulo2D retangulo;
    float velocidadex;
};

struct Fase {
    Bloco *blocos;
    int linha;
    int coluna;
    int indiceMapa;
    char nome[32];
    Color corFundo;
    Color corDetalhe;
};

struct ItensEspeciais {
    Posicao posicao_do_item;
    int tipo;
    float velocidadey;
    bool esta_Ativo;
    float tamanho;
    Color cor;
};

struct EstatisticasPartida {
    int blocosQuebrados;
    int itensColetados;
    int bonusPontuacao;
    int vidasGastas;
    int fasesConcluidas;
    bool pontuacaoInvertida;
};

struct Particula {
    Vector2 posicao;
    Vector2 velocidade;
    float tempoVida;
    float tempoMaximo;
    Color cor;
    bool ativa;
};

struct SistemaParticulas {
    Particula particulas[MAX_PARTICULAS];
};

struct Jogador {
    char nome[MAX_LETRAS];
    int pontuacao;
    int tempo;
    int vidas_jogador;
    char dataRegistro[20];
    int dificuldadeSelecionada;
    int itens_coletados;
    int blocos_quebrados;
};

struct Meteoro {
    Vector2 posicao;
    Vector2 velocidade;
    float tamanho;
    Color cor;
    bool ativo;
};

struct Ranking {
    Jogador jogador[MAX_JOGADORES];
    int total_jogadores;
};

#endif
