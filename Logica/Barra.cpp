#include "Estruturas.h"
#include "Barra.h"

void iniciarBarra(Barra *barra) {

    barra->retangulo.posicao.x = 350;
    barra->retangulo.posicao.y = 550;
    barra->retangulo.largura = 100;
    barra->retangulo.altura = 20;
    barra->velocidadex = 0;

}

void atualizarTamanho(Barra *barra, int tamanhoAd, bool adicionar)
{
    
    if (adicionar) {

        barra->retangulo.largura += tamanhoAd;

    } else {

        barra->retangulo.largura -= tamanhoAd;

    } //adicionar verificação se a barra está maior que a tela

}

void desenharBarra(Barra *barra) {

    DrawRectangle((int)barra->retangulo.posicao.x, (int)barra->retangulo.posicao.y,
                  (int)barra->retangulo.largura, (int)barra->retangulo.altura, WHITE);

    DrawRectangleLines ((int)barra->retangulo.posicao.x, (int)barra->retangulo.posicao.y,
                  (int)barra->retangulo.largura, (int)barra->retangulo.altura, BLACK);
}
