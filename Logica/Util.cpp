#include "Estruturas.h"
#include "Constantes.h"
#include "raylib.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

// Função para verificar colisão entre círculo (bola) e retângulo (barra/bloco)
bool verificarColisaoCirculoRetangulo(Posicao circulo, float raio, Retangulo2D retangulo) {
    float closest_x = circulo.x;
    float closest_y = circulo.y;

    // Encontrar o ponto mais próximo do retângulo
    if (circulo.x < retangulo.posicao.x) {
        closest_x = retangulo.posicao.x;
    } else if (circulo.x > retangulo.posicao.x + retangulo.largura) {
        closest_x = retangulo.posicao.x + retangulo.largura;
    }

    if (circulo.y < retangulo.posicao.y) {
        closest_y = retangulo.posicao.y;
    } else if (circulo.y > retangulo.posicao.y + retangulo.altura) {
        closest_y = retangulo.posicao.y + retangulo.altura;
    }

    // Calcular distância
    float distancia_x = circulo.x - closest_x;
    float distancia_y = circulo.y - closest_y;
    float distancia = sqrt(distancia_x * distancia_x + distancia_y * distancia_y);

    return distancia < raio;
}

// Função para calcular a pontuação baseada em tempo, dificuldade e blocos quebrados
int calcularPontuacao(int tempo, int dificuldade, int blocos_quebrados, int itens_coletados, int vidas_restantes) {
    int pontos_base = blocos_quebrados * 120;
    int bonus_itens = itens_coletados * 80;
    int bonus_tempo = (tempo > 0) ? std::max(0, 1800 - tempo * 4) : 1800;
    int bonus_vidas = vidas_restantes * 250;

    float multiplicador_dificuldade = 1.0f;
    if (dificuldade == DIFICULDADE_MEDIO) {
        multiplicador_dificuldade = FATOR_DIFICULDADE_MEDIO;
    } else if (dificuldade == DIFICULDADE_DIFICIL) {
        multiplicador_dificuldade = FATOR_DIFICULDADE_DIFICIL;
    }

    float total = (pontos_base + bonus_itens + bonus_tempo + bonus_vidas);
    total *= multiplicador_dificuldade;
    return (int)total;
}

// Função para gerar número aleatório entre min e max
int gerarAleatorio(int min, int max) {
    return min + (rand() % (max - min + 1));
}

// ============================================
// TEMPLATES DE FUNÇÃO (Requisito do Projeto)
// ============================================

// Template para encontrar o máximo entre dois valores
template<typename T>
T maximo(T a, T b) {
    return (a > b) ? a : b;
}

// Template para encontrar o mínimo entre dois valores
template<typename T>
T minimo(T a, T b) {
    return (a < b) ? a : b;
}

// Template para trocar dois valores
template<typename T>
void trocar(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

// Instanciações explícitas dos templates
template int maximo<int>(int, int);
template float maximo<float>(float, float);
template int minimo<int>(int, int);
template float minimo<float>(float, float);
template void trocar<int>(int*, int*);
template void trocar<float>(float*, float*);

// ============================================
// FUNÇÕES RECURSIVAS (Requisito do Projeto)
// ============================================

// Função recursiva para somar números de 1 até n
int somarRecursivo(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + somarRecursivo(n - 1);
}

// Função recursiva para calcular pontos baseado em blocos
int calcularPontosRecursivo(int blocos, int multiplicador) {
    if (blocos <= 0) {
        return 0;
    }
    return (10 * multiplicador) + calcularPontosRecursivo(blocos - 1, multiplicador);
}

// Função recursiva para contar blocos ativos
int contarBlocosRecursivo(Bloco *blocos, int inicio, int fim) {
    if (inicio > fim) {
        return 0;
    }
    int count = blocos[inicio].ativo ? 1 : 0;
    return count + contarBlocosRecursivo(blocos, inicio + 1, fim);
}

// Função recursiva auxiliar para quicksort (ordenação de pontuação)
static int particionar(int *array, int inicio, int fim) {
    int pivo = array[fim];
    int i = inicio - 1;
    
    for (int j = inicio; j < fim; j++) {
        if (array[j] >= pivo) {  // Ordem decrescente
            i++;
            trocar(&array[i], &array[j]);
        }
    }
    trocar(&array[i + 1], &array[fim]);
    return i + 1;
}

// Função recursiva para ordenar pontuações (QuickSort)
void ordenarPontosRecursivo(int *pontos, int inicio, int fim) {
    if (inicio >= fim) {
        return;
    }
    
    int pivoPos = particionar(pontos, inicio, fim);
    ordenarPontosRecursivo(pontos, inicio, pivoPos - 1);
    ordenarPontosRecursivo(pontos, pivoPos + 1, fim);
}
