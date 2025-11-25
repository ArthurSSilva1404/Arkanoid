#include "estado.h"

void inicializarEstado(GerenciadorEstado *gerenciador) {
    gerenciador->estado_atual = ESTADO_MENU;
    gerenciador->estado_anterior = ESTADO_MENU;
    gerenciador->em_pausa = false;
    gerenciador->tempo_total = 0.0f;
    gerenciador->blocos_quebrados_total = 0;
}

void mudarEstado(GerenciadorEstado *gerenciador, int novo_estado) {
    gerenciador->estado_anterior = gerenciador->estado_atual;
    gerenciador->estado_atual = novo_estado;
}

void pausarJogo(GerenciadorEstado *gerenciador) {
    if (gerenciador->estado_atual == ESTADO_JOGANDO) {
        gerenciador->em_pausa = true;
        gerenciador->estado_anterior = gerenciador->estado_atual;
        gerenciador->estado_atual = ESTADO_PAUSA;
    }
}

void retomarJogo(GerenciadorEstado *gerenciador) {
    if (gerenciador->estado_atual == ESTADO_PAUSA) {
        gerenciador->em_pausa = false;
        gerenciador->estado_atual = gerenciador->estado_anterior;
    }
}

void resetarEstado(GerenciadorEstado *gerenciador) {
    gerenciador->estado_atual = ESTADO_MENU;
    gerenciador->estado_anterior = ESTADO_MENU;
    gerenciador->em_pausa = false;
    gerenciador->tempo_total = 0.0f;
    gerenciador->blocos_quebrados_total = 0;
}

