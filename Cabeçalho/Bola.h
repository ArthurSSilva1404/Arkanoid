#include <iostream>

#include "Estruturas.h"

#ifndef BOLA
#define BOLA

void iniciarBola(Bola *bola);

void atualizarBola(Bola *bola, int dt);

void atualizarTamanho(Bola *bola, int tamanhoAd);

void desenharBola(Bola *bola);

#endif
