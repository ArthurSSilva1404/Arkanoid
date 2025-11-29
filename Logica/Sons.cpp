#include "Sons.h"
#include <cstring>

void inicializarAudio(SistemaAudio *audio) {
    if (audio == nullptr) return;
    
    InitAudioDevice();
    
    memset(audio, 0, sizeof(SistemaAudio));
    
    // Tentar carregar música de fundo - se falhar, continua sem som
    audio->musicaFundo = LoadMusicStream("Sons/somMusicaDeFundo.wav");
    SetMusicVolume(audio->musicaFundo, 0.3f);
    audio->musicaFundo.looping = true;
    
    // Tentar carregar sons - se falharem, continua sem som
    audio->somQuebrarBloco = LoadSound("Sons/somQuebrarBloco.wav");
    SetSoundVolume(audio->somQuebrarBloco, 0.6f);
    
    audio->somColisaoBolaBarra = LoadSound("Sons/somColisaoBolaBarra.wav");
    SetSoundVolume(audio->somColisaoBolaBarra, 0.6f);
    
    audio->somColetouItem = LoadSound("Sons/somColetouItem.wav");
    SetSoundVolume(audio->somColetouItem, 0.8f);
    
    audio->somVitoria = LoadSound("Sons/somVitoria.wav");
    SetSoundVolume(audio->somVitoria, 0.7f);
    
    audio->somDerrota = LoadSound("Sons/somDerrota.wav");
    SetSoundVolume(audio->somDerrota, 0.7f);
    
    audio->audioCarregado = true;
}

void tocarMusicaFundo(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    PlayMusicStream(audio->musicaFundo);
}

void pararMusicaFundo(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    StopMusicStream(audio->musicaFundo);
}

void atualizarMusicaFundo(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    UpdateMusicStream(audio->musicaFundo);
}

void tocarSomQuebrarBloco(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    PlaySound(audio->somQuebrarBloco);
}

void tocarSomColisaoBolaBarra(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    PlaySound(audio->somColisaoBolaBarra);
}

void tocarSomColetouItem(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    PlaySound(audio->somColetouItem);
}

void tocarSomVitoria(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    PlaySound(audio->somVitoria);
}

void tocarSomDerrota(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    PlaySound(audio->somDerrota);
}

void descarregarAudio(SistemaAudio *audio) {
    if (audio == nullptr || !audio->audioCarregado) return;
    
    UnloadMusicStream(audio->musicaFundo);
    UnloadSound(audio->somQuebrarBloco);
    UnloadSound(audio->somColisaoBolaBarra);
    UnloadSound(audio->somColetouItem);
    UnloadSound(audio->somVitoria);
    UnloadSound(audio->somDerrota);
    
    CloseAudioDevice();
    audio->audioCarregado = false;
}
