#include "Estruturas.h"

#ifndef BOLA
#define BOLA

struct Barra;

void iniciarBola(Bola *bola);

void atualizarBola(Bola *bola, float dt);

void atualizarTamanho(Bola *bola, int tamanhoAd);

void desenharBola(Bola *bola);

void alinharBolaNaBarra(Bola *bola, const Barra *barra);

void soltarBola(Bola *bola, float fatorVelocidade);

#endif
