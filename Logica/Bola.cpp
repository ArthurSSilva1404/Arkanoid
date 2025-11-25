#include "Estruturas.h"
#include "Bola.h"
#include "Constantes.h"
#include "cores.h"
#include "raylib.h"
#include <cmath>

void iniciarBola(Bola *bola) {
    bola->posicao.x = LARGURA_TELA / 2.0f;
    bola->posicao.y = ALTURA_TELA - 50;
    bola->radium = RAIO_BOLA;
    bola->velocidadex = 200.0f;
    bola->velocidadey = -250.0f;
    bola->prendeu = true;
}

void atualizarBola(Bola *bola, int dt) {
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

