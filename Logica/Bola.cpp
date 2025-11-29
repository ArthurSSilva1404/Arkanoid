#include "Estruturas.h"
#include "Bola.h"
#include "Constantes.h"
#include "Cores.h"
#include "Barra.h"
#include "raylib.h"
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

// Função auxiliar para normalizar a velocidade da bola
static void normalizarVelocidade(Bola *bola, float velocidadeDesejada) {
    float velocidadeAtual = sqrtf(bola->velocidadex * bola->velocidadex + 
                                   bola->velocidadey * bola->velocidadey);
    if (velocidadeAtual > 0.001f) {
        bola->velocidadex *= velocidadeDesejada / velocidadeAtual;
        bola->velocidadey *= velocidadeDesejada / velocidadeAtual;
    }
}

void iniciarBola(Bola *bola) {
    bola->posicao.x = LARGURA_TELA / 2.0f;
    bola->posicao.y = ALTURA_TELA - 60;
    bola->radium = RAIO_BOLA;
    bola->velocidadex = 0.0f;
    bola->velocidadey = 0.0f;
    bola->velocidadeBase = VELOCIDADE_BOLA_BASE;
    bola->prendeu = true;
}

void alinharBolaNaBarra(Bola *bola, const Barra *barra) {
    float centroBarra = barra->retangulo.posicao.x + barra->retangulo.largura / 2.0f;
    bola->posicao.x = centroBarra;
    bola->posicao.y = barra->retangulo.posicao.y - bola->radium - 4.0f;
}

void soltarBola(Bola *bola, float fatorVelocidade) {
    if (bola->prendeu) {
        bola->prendeu = false;
        // Lançar com ângulo aleatório entre -60 e 60 graus
        float angulo = -60.0f + (float)(GetRandomValue(0, 120));
        float anguloRad = angulo * PI / 180.0f;
        float velocidade = bola->velocidadeBase * fatorVelocidade;
        bola->velocidadex = velocidade * sinf(anguloRad);
        bola->velocidadey = -velocidade * cosf(anguloRad);
    }
}

void atualizarBola(Bola *bola, float dt) {
    if (bola->prendeu) {
        return;
    }

    // Guardar velocidade atual para normalização
    float velocidadeAtual = sqrtf(bola->velocidadex * bola->velocidadex + 
                                   bola->velocidadey * bola->velocidadey);

    bola->posicao.x += bola->velocidadex * dt;
    bola->posicao.y += bola->velocidadey * dt;

    // Colisão com a parede esquerda
    if (bola->posicao.x - bola->radium < 0) {
        bola->posicao.x = bola->radium;
        bola->velocidadex = fabsf(bola->velocidadex);
        normalizarVelocidade(bola, velocidadeAtual);
    }
    
    // Colisão com a parede direita
    if (bola->posicao.x + bola->radium > LARGURA_TELA) {
        bola->posicao.x = LARGURA_TELA - bola->radium;
        bola->velocidadex = -fabsf(bola->velocidadex);
        normalizarVelocidade(bola, velocidadeAtual);
    }
    
    // Colisão com a parede superior
    if (bola->posicao.y - bola->radium < 0) {
        bola->posicao.y = bola->radium;
        bola->velocidadey = fabsf(bola->velocidadey);
        // Garantir velocidade mínima no eixo Y
        if (fabsf(bola->velocidadey) < 50.0f) {
            bola->velocidadey = 50.0f;
        }
        normalizarVelocidade(bola, velocidadeAtual);
    }
}

void atualizarTamanho(Bola *bola, int tamanhoAd) {
    bola->radium += tamanhoAd;
    if (bola->radium < 2) {
        bola->radium = 2;
    }
}

void desenharBola(Bola *bola) {
    DrawCircle((int)bola->posicao.x, (int)bola->posicao.y, bola->radium, obterCorBola());
    DrawCircleLines((int)bola->posicao.x, (int)bola->posicao.y, bola->radium, ORANGE);
}
