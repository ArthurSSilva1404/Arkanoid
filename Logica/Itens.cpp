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
        case ITEM_INVERSOR:
            item->cor = obterCorItemInversor();
            break;
        case ITEM_ESCUDO:
            item->cor = obterCorItemEscudo();
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
            case ITEM_INVERSOR:
                label = "!";
                break;
            case ITEM_ESCUDO:
                label = "S";
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

    switch (item->tipo) {
        case ITEM_VIDA:
            jogador->pontuacao += 250;
            estatisticas->bonusPontuacao += 250;
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
            break;
        case ITEM_INVERSOR:
            jogador->pontuacao = -jogador->pontuacao;
            estatisticas->pontuacaoInvertida = !estatisticas->pontuacaoInvertida;
            break;
        case ITEM_ESCUDO:
            estado->escudoAtivo = true;
            estado->tempoEscudoRestante = TEMPO_ESCUDO;
            break;
    }

    item->esta_Ativo = false;
}

