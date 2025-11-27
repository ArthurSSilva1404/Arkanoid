#include "Estruturas.h"
#include "Barra.h"
#include "Cores.h"
#include "Constantes.h"
#include "raylib.h"

void iniciarBarra(Barra *barra) {

    barra->retangulo.posicao.x = (LARGURA_TELA / 2.0f) - 70.0f;
    barra->retangulo.posicao.y = ALTURA_TELA - 80.0f;
    barra->retangulo.largura = 140;
    barra->retangulo.altura = 20;
    barra->velocidadex = 0.0f;

}
void atualizarBarra(Barra *barra, float dt) {
    
    if (IsKeyDown(KEY_LEFT) || IsKeyDown (KEY_A)) {
        barra->velocidadex = -VELOCIDADE_BARRA;
    } else if (IsKeyDown(KEY_RIGHT) || IsKeyDown (KEY_D)) {
        barra->velocidadex = VELOCIDADE_BARRA;
    } else {
        barra->velocidadex = 0.0f;
    }

    barra->retangulo.posicao.x += barra->velocidadex * dt;

    
    if (barra->retangulo.posicao.x < 0) {
        barra->retangulo.posicao.x = 0;
    }
    if (barra->retangulo.posicao.x + barra->retangulo.largura > LARGURA_TELA) {

        barra->retangulo.posicao.x = LARGURA_TELA - barra->retangulo.largura;

    } 
}

void atualizarTamanho(Barra *barra, int tamanhoAd, bool adicionar)
{
    if (adicionar) {
        barra->retangulo.largura += tamanhoAd;
    } else {
        barra->retangulo.largura -= tamanhoAd;
    }

    if (barra->retangulo.largura < 60) {
        barra->retangulo.largura = 60;
    } else if (barra->retangulo.largura > 220) {
        barra->retangulo.largura = 220;
    }
}

void desenharBarra(Barra *barra) {

    DrawRectangle((int)barra->retangulo.posicao.x, (int)barra->retangulo.posicao.y,
                  (int)barra->retangulo.largura, (int)barra->retangulo.altura, obterCorBarra());

    DrawRectangleLines ((int)barra->retangulo.posicao.x, (int)barra->retangulo.posicao.y,
                  (int)barra->retangulo.largura, (int)barra->retangulo.altura, BLACK);
}
