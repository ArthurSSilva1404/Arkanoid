#include "Estruturas.h"
#include "Fases.h"
#include "Bloco.h"
#include "Constantes.h"
#include "Cores.h"
#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <algorithm>

static const int MAPAS[MAX_FASES][MAPA_LINHAS][MAPA_COLUNAS] = {
    {
        {0,1,1,1,1,1,1,1,1,0},
        {1,2,2,2,2,2,2,2,2,1},
        {1,2,3,3,4,4,3,3,2,1},
        {0,2,3,5,5,5,5,3,2,0},
        {0,1,2,3,5,5,3,2,1,0},
        {0,0,1,1,2,2,1,1,0,0},
    },
    {
        {0,4,4,4,4,4,4,4,4,0},
        {2,2,2,3,3,3,3,2,2,2},
        {2,3,3,5,5,5,5,3,3,2},
        {2,3,5,0,0,0,0,5,3,2},
        {1,2,3,3,5,5,3,3,2,1},
        {0,1,2,2,3,3,2,2,1,0},
    },
    {
        {4,0,4,0,4,0,4,0,4,0},
        {2,2,5,5,5,5,5,5,2,2},
        {1,2,3,3,3,3,3,3,2,1},
        {1,3,3,4,4,4,4,3,3,1},
        {0,2,3,5,5,5,5,3,2,0},
        {0,1,2,3,3,3,3,2,1,0},
    }
};

static const char *NOMES_FASES[MAX_FASES] = {
    "Nebulosa Coral",
    "Caverna Prismatica",
    "Templo Aurora"
};

static const Color FUNDOS_TEMA[MAX_FASES] = {
    {8, 9, 26, 255},
    {18, 6, 24, 255},
    {4, 14, 30, 255}
};

static const Color DETALHES_TEMA[MAX_FASES] = {
    {94, 200, 255, 255},
    {224, 120, 255, 255},
    {255, 197, 110, 255}
};

static Color paletaBlocos[] = {
    {255, 99, 71, 255},
    {245, 158, 66, 255},
    {110, 182, 255, 255},
    {160, 206, 78, 255},
    {227, 110, 255, 255}
};

static Color gerarCorPorTipo(int tipo) {
    Color base;
    switch (tipo) {
        case TIPO_BLOCO_CRISTAL:
            base = Color{200, 220, 255, 255};
            break;
        case TIPO_BLOCO_CERAMICA:
            base = Color{255, 180, 130, 255};
            break;
        case TIPO_BLOCO_OBSIDIANA:
            base = Color{120, 100, 180, 255};
            break;
        case TIPO_BLOCO_TITANIO:
            base = obterCorBlocoTitanio();
            break;
        default:
            base = paletaBlocos[GetRandomValue(0, 4)];
            break;
    }

    int variacao = GetRandomValue(-18, 18);
    base.r = (unsigned char)std::min(255, std::max(0, base.r + variacao));
    base.g = (unsigned char)std::min(255, std::max(0, base.g + variacao));
    base.b = (unsigned char)std::min(255, std::max(0, base.b + variacao));
    return base;
}

void iniciarFase(Fase *fase, int numeroFase) {
    if (fase->blocos != nullptr) {
        free(fase->blocos);
        fase->blocos = nullptr;
    }

    fase->linha = MAPA_LINHAS;
    fase->coluna = MAPA_COLUNAS;
    fase->indiceMapa = (numeroFase - 1) % MAX_FASES;
    strncpy(fase->nome, NOMES_FASES[fase->indiceMapa], sizeof(fase->nome));
    fase->nome[sizeof(fase->nome) - 1] = '\0';

    int total_blocos = fase->linha * fase->coluna;
    fase->blocos = (Bloco *)malloc(total_blocos * sizeof(Bloco));
    fase->corFundo = FUNDOS_TEMA[fase->indiceMapa];
    fase->corDetalhe = DETALHES_TEMA[fase->indiceMapa];

    float inicioX = (LARGURA_TELA - (MAPA_COLUNAS * (TAMANHO_BLOCO_LARGURA + 4))) / 2.0f;
    float inicioY = 100.0f;

    int indice = 0;
    for (int i = 0; i < fase->linha; i++) {
        for (int j = 0; j < fase->coluna; j++) {
            Bloco *bloco = &fase->blocos[indice++];
            int tipo = MAPAS[fase->indiceMapa][i][j];
            if (tipo <= 0) {
                bloco->ativo = false;
                bloco->TotalDeVida = 0;
                bloco->possuiItem = false;
                bloco->pontosBase = 0;
                bloco->retangulo = {{0, 0}, 0, 0};
                bloco->cor = obterCorBlocoFraco();
                bloco->indestrutivel = false;
                bloco->tipo = 0;
                continue;
            }

            float posx = inicioX + j * (TAMANHO_BLOCO_LARGURA + 4);
            float posy = inicioY + i * (TAMANHO_BLOCO_ALTURA + 6);

            int vida = 1;
            bloco->indestrutivel = false;
            bloco->tipo = tipo;

            switch (tipo) {
                case TIPO_BLOCO_CRISTAL:
                    vida = 1 + GetRandomValue(0, 1);
                    break;
                case TIPO_BLOCO_CERAMICA:
                    vida = 2;
                    break;
                case TIPO_BLOCO_OBSIDIANA:
                    vida = 3;
                    break;
                case TIPO_BLOCO_METAL:
                    vida = 1;
                    bloco->indestrutivel = true;
                    bloco->cor = obterCorBlocoMetal();
                    break;
                case TIPO_BLOCO_TITANIO:
                    vida = 5;
                    break;
                default:
                    vida = 1;
                    break;
            }

            iniciarBloco(bloco, posx, posy, vida);
            if (!bloco->indestrutivel) {
                bloco->cor = gerarCorPorTipo(tipo);
            }
            bloco->pontosBase = bloco->indestrutivel ? 0 : 90 * vida;
            bloco->possuiItem = (!bloco->indestrutivel) && (GetRandomValue(0, 100) < (18 + numeroFase * 4));
            if (!bloco->indestrutivel && GetRandomValue(0, 100) < 8) {
                bloco->ativo = false;
            }
        }
    }
}

void desenharFase(Fase *fase) {
    int total_blocos = fase->linha * fase->coluna;
    for (int i = 0; i < total_blocos; i++) {
        desenharBloco(&fase->blocos[i]);
    }
}

void gerarItem(Fase *fase, ItensEspeciais item[], int maxItens) {
    // Gerenciamento realizado em Itens.cpp
    (void)fase;
    (void)item;
    (void)maxItens;
}

bool terminarFase(Fase *fase) {
    int total_blocos = fase->linha * fase->coluna;

    for (int i = 0; i < total_blocos; i++) {
        if (fase->blocos[i].ativo) {
            return false;
        }
    }

    return true;
}
