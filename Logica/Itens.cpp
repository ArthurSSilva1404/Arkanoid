#include "Estruturas.h"
#include "Itens.h"
#include "Constantes.h"
#include "Cores.h"
#include "Jogo.h"
#include "raylib.h"

void iniciarItem(ItensEspeciais *item, int tipo, float posx, float posy) {
    item->posicao_do_item.x = posx;
    item->posicao_do_item.y = posy;
    item->tipo = tipo;
    item->velocidadey = 150.0f;
    item->esta_Ativo = true;
    item->tamanho = TAMANHO_ITEM;
    switch (tipo) {
        case ITEM_VIDA:
            item->cor = obterCorItemVida();
            break;
        case ITEM_AUMENTAR_BARRA:
            item->cor = obterCorItemAumentar();
            break;
        case ITEM_DIMINUIR_BARRA:
            item->cor = obterCorItemDiminuir();
            break;
        case ITEM_PONTOS_EXTRAS:
            item->cor = obterCorItemPontos();
            break;
        case ITEM_MULTIPLICADOR:
            item->cor = obterCorItemMultiplicador();
            break;
        case ITEM_INVERSOR:
            item->cor = obterCorItemInversor();
            break;
        default:
            item->cor = WHITE;
    }
}

void desenharItem(ItensEspeciais *item) {
    if (item->esta_Ativo) {
        DrawCircleV({item->posicao_do_item.x, item->posicao_do_item.y}, item->tamanho / 2.0f, Fade(item->cor, 0.8f));
        DrawCircleLines((int)item->posicao_do_item.x, (int)item->posicao_do_item.y, item->tamanho / 2.0f, BLACK);

        const char *label = "";
        switch (item->tipo) {
            case ITEM_VIDA:
                label = "+";
                break;
            case ITEM_AUMENTAR_BARRA:
                label = ">";
                break;
            case ITEM_DIMINUIR_BARRA:
                label = "<";
                break;
            case ITEM_PONTOS_EXTRAS:
                label = "P";
                break;
            case ITEM_MULTIPLICADOR:
                label = "x2";
                break;
            case ITEM_INVERSOR:
                label = "!";
                break;
        }
        DrawText(label, (int)item->posicao_do_item.x - 5, (int)item->posicao_do_item.y - 6, 16, BLACK);
    }
}

void atualizarItem(ItensEspeciais *item) {
    if (item->esta_Ativo) {
        item->posicao_do_item.y += item->velocidadey * GetFrameTime();

        // Desativar item se sair da tela
        if (item->posicao_do_item.y > ALTURA_TELA) {
            item->esta_Ativo = false;
        }
    }
}

void efeitoItem(ItensEspeciais *item, EstadoJogo *estado) {
    if (!item->esta_Ativo || estado == nullptr) return;

    Jogador *jogador = &estado->jogador;
    Barra *barra = &estado->barra;
    EstatisticasPartida *estatisticas = &estado->estatisticas;

    jogador->itens_coletados++;
    estatisticas->itensColetados++;
    tocarSomColetouItem(&estado->audio);

    switch (item->tipo) {
        case ITEM_VIDA:
            // Adicionar vida ao jogador
            jogador->vidas_jogador++;
            jogador->pontuacao += 250;
            estatisticas->bonusPontuacao += 250;
            estado->ultimoItemColetado = ITEM_VIDA;
            estado->tempoMostrarItem = 2.0f;
            break;
        case ITEM_AUMENTAR_BARRA:
            if (barra->retangulo.largura < 220) {
                barra->retangulo.largura += 24;
            }
            break;
        case ITEM_DIMINUIR_BARRA:
            if (barra->retangulo.largura > 70) {
                barra->retangulo.largura -= 24;
            }
            break;
        case ITEM_PONTOS_EXTRAS:
            jogador->pontuacao += 500;
            estatisticas->bonusPontuacao += 500;
            estado->ultimoItemColetado = ITEM_PONTOS_EXTRAS;
            estado->tempoMostrarItem = 2.0f;
            break;
        case ITEM_MULTIPLICADOR:
            estado->multiplicadorPontos = 2.0f;
            estado->tempoMultiplicador = 600;
            estado->ultimoItemColetado = ITEM_MULTIPLICADOR;
            estado->tempoMostrarItem = 2.0f;
            break;
        case ITEM_INVERSOR:
            // Ativar inversor por tempo limitado (5 segundos)
            estado->inversorAtivo = true;
            estado->tempoInversor = 300;  // 5 segundos a 60 FPS
            estado->ultimoItemColetado = ITEM_INVERSOR;
            estado->tempoMostrarItem = 2.0f;
            break;
    }

    item->esta_Ativo = false;
}
