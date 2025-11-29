#include "Estruturas.h"

#ifndef UTILS_H
#define UTILS_H

bool verificarColisaoCirculoRetangulo(Posicao circulo, float raio, Retangulo2D retangulo);

int calcularPontuacao(int tempo, int dificuldade, int blocos_quebrados, int itens_coletados, int vidas_restantes);

int gerarAleatorio(int min, int max);

// Templates de função
template<typename T>
T maximo(T a, T b);

template<typename T>
T minimo(T a, T b);

template<typename T>
void trocar(T *a, T *b);

// Funções recursivas
int somarRecursivo(int n);
int calcularPontosRecursivo(int blocos, int multiplicador);
int contarBlocosRecursivo(Bloco *blocos, int inicio, int fim);
void ordenarPontosRecursivo(int *pontos, int inicio, int fim);

#endif
