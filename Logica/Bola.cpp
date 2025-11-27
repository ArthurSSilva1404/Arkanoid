#include "Estruturas.h"
#include "Bola.h"
#include "Constantes.h"
#include "Cores.h"
#include "Barra.h"
#include "raylib.h"
#include <cmath>

void iniciarBola(Bola *bola) {
    bola->posicao.x = LARGURA_TELA / 2.0f;
    bola->posicao.y = ALTURA_TELA - 60;
    bola->radium = RAIO_BOLA;
    bola->velocidadex = 0.0f;
    bola->velocidadey = -VELOCIDADE_BOLA_BASE;
    bola->velocidadeBase = VELOCIDADE_BOLA_BASE;
    bola->prendeu = true;
}

void alinharBolaNaBarra(Bola *bola, const Barra *barra) {
    float centroBarra = barra->retangulo.posicao.x + barra->retangulo.largura / 2.0f;
    bola->posicao.x = centroBarra;
    bola->posicao.y = barra->retangulo.posicao.y - bola->radium - 4.0f;
}

void soltarBola(Bola *bola, float fatorVelocidade) {
    bola->prendeu = false;
    bola->velocidadey = -bola->velocidadeBase * fatorVelocidade;
    bola->velocidadex = bola->velocidadeBase * fatorVelocidade * 0.35f;
}

void atualizarBola(Bola *bola, float dt) {
    if (!bola->prendeu) {
        bola->posicao.x += bola->velocidadex * dt;
        bola->posicao.y += bola->velocidadey * dt;

        // Colisão com as paredes laterais
        if (bola->posicao.x - bola->radium <= 0 || bola->posicao.x + bola->radium >= LARGURA_TELA) {
            bola->velocidadex *= -1;
            if (bola->posicao.x - bola->radium <= 0) {
                bola->posicao.x = bola->radium;
            } else {
                bola->posicao.x = LARGURA_TELA - bola->radium;
            }
        }

        // Colisão com a parede superior
        if (bola->posicao.y - bola->radium <= 0) {
            bola->velocidadey *= -1;
            bola->posicao.y = bola->radium;
        }
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
