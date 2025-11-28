
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
