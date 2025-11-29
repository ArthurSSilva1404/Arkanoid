#ifndef SONS_H
#define SONS_H

#include "raylib.h"

struct SistemaAudio {
    Music musicaFundo;
    Sound somQuebrarBloco;
    Sound somColisaoBolaBarra;
    Sound somColetouItem;
    Sound somVitoria;
    Sound somDerrota;
    bool audioCarregado;
};

void inicializarAudio(SistemaAudio *audio);
void tocarMusicaFundo(SistemaAudio *audio);
void pararMusicaFundo(SistemaAudio *audio);
void atualizarMusicaFundo(SistemaAudio *audio);
void tocarSomQuebrarBloco(SistemaAudio *audio);
void tocarSomColisaoBolaBarra(SistemaAudio *audio);
void tocarSomColetouItem(SistemaAudio *audio);
void tocarSomVitoria(SistemaAudio *audio);
void tocarSomDerrota(SistemaAudio *audio);
void descarregarAudio(SistemaAudio *audio);

#endif
