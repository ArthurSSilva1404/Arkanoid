#include "sons.h"

static GerenciadorSons gerenciador_global = {true, 1.0f};

void inicializarSons(GerenciadorSons *gerenciador) {
    gerenciador->som_ativado = true;
    gerenciador->volume = 1.0f;
    gerenciador_global = *gerenciador;
}

void tocarSomColisao() {
    if (gerenciador_global.som_ativado) {
        // Implementar som de colisão
        // PlaySound(som_colisao);
    }
}

void tocarSomBlocoDestruido() {
    if (gerenciador_global.som_ativado) {
        // Implementar som de bloco destruído
        // PlaySound(som_bloco);
    }
}

void tocarSomItemColetado() {
    if (gerenciador_global.som_ativado) {
        // Implementar som de item coletado
        // PlaySound(som_item);
    }
}

void tocarSomGameOver() {
    if (gerenciador_global.som_ativado) {
        // Implementar som de game over
        // PlaySound(som_game_over);
    }
}

void tocarSomVitoria() {
    if (gerenciador_global.som_ativado) {
        // Implementar som de vitória
        // PlaySound(som_vitoria);
    }
}

void definirVolume(float volume) {
    if (volume >= 0.0f && volume <= 1.0f) {
        gerenciador_global.volume = volume;
        // SetMasterVolume(volume);
    }
}

void alternarSom() {
    gerenciador_global.som_ativado = !gerenciador_global.som_ativado;
}

