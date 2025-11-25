#include "Estruturas.h"
#include "Constantes.h"
#include "raylib.h"
#include <cmath>

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
int calcularPontuacao(int tempo, int dificuldade, int blocos_quebrados) {
    int pontos_base = blocos_quebrados * 100;
    float multiplicador_dificuldade = 1.0f;

    if (dificuldade == 1) {
        multiplicador_dificuldade = 1.0f;
    } else if (dificuldade == 2) {
        multiplicador_dificuldade = 1.25f;
    } else if (dificuldade == 3) {
        multiplicador_dificuldade = 1.5f;
    }

    // Bônus por tempo (quanto menos tempo, mais pontos)
    int bonus_tempo = (tempo > 0) ? (1000 / tempo) : 0;

    return (int)(pontos_base * multiplicador_dificuldade) + bonus_tempo;
}

// Função para gerar número aleatório entre min e max
int gerarAleatorio(int min, int max) {
    return min + (rand() % (max - min + 1));
}
