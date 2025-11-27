#include "Estruturas.h"
#include "Bloco.h"
#include "Constantes.h"
#include "Cores.h"
#include "raylib.h"
#include <algorithm>

void iniciarBloco(Bloco *bloco, float posx, float posy, int vida) {
    bloco->retangulo.posicao.x = posx;
    bloco->retangulo.posicao.y = posy;
    bloco->retangulo.largura = TAMANHO_BLOCO_LARGURA;
    bloco->retangulo.altura = TAMANHO_BLOCO_ALTURA;
    bloco->ativo = true;
    bloco->TotalDeVida = vida;
    bloco->cor = obterCorBlocoFraco();
    bloco->pontosBase = 80;
    bloco->possuiItem = false;
}

void desenharBloco(Bloco *bloco) {
    if (bloco->ativo) {
        Color cor = bloco->cor;
        if (bloco->TotalDeVida == 2) {
            cor = Color{(unsigned char)std::min(255, cor.r + 20),
                        (unsigned char)std::min(255, cor.g + 20),
                        (unsigned char)std::min(255, cor.b + 20),
                        cor.a};
        } else if (bloco->TotalDeVida == 1) {
            cor = Color{(unsigned char)std::min(255, cor.r + 40),
                        (unsigned char)std::min(255, cor.g + 40),
                        (unsigned char)std::min(255, cor.b + 40),
                        cor.a};
        }

        DrawRectangle((int)bloco->retangulo.posicao.x, (int)bloco->retangulo.posicao.y,
                      bloco->retangulo.largura, bloco->retangulo.altura, cor);
        DrawRectangleLines((int)bloco->retangulo.posicao.x, (int)bloco->retangulo.posicao.y,
                           bloco->retangulo.largura, bloco->retangulo.altura, BLACK);
    }
}
