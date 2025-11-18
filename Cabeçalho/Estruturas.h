#include <iostream>

#include "Constantes.h"

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
};

struct Bola {
    Posicao posicao;
    float radium;
    float velocidadex;
    float velocidadey;
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
    
};

struct ItensEspeciais {
    Posicao posicao_do_item;
    int tipo;
    float velocidadey;
    bool esta_Ativo;
};

struct Jogador {
    char nome[MAX_LETRAS];
    int pontuacao;
    int tempo;
    int vidas_jogador;

};

struct Ranking {
    Jogador jogador[MAX_JOGADORES];

}

#endif
