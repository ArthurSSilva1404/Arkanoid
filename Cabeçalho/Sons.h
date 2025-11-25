#include "raylib.h"

#ifndef SONS_H
#define SONS_H

// Estrutura para gerenciar sons
struct GerenciadorSons {
    bool som_ativado;
    float volume;
};

void inicializarSons(GerenciadorSons *gerenciador);

void tocarSomColisao();

void tocarSomBlocoDestruido();

void tocarSomItemColetado();

void tocarSomGameOver();

void tocarSomVitoria();

void definirVolume(float volume);

void alternarSom();

#endif

