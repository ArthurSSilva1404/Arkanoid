#include "Estruturas.h"

#ifndef UTILS_H
#define UTILS_H

bool verificarColisaoCirculoRetangulo(Posicao circulo, float raio, Retangulo2D retangulo);

int calcularPontuacao(int tempo, int dificuldade, int blocos_quebrados, int itens_coletados, int vidas_restantes);

int gerarAleatorio(int min, int max);

#endif
