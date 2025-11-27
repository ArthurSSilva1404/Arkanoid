#include <iostream>

#include "Estruturas.h"

#ifndef FASES
#define FASES

void iniciarFase(Fase *fase, int numeroFase);

void desenharFase(Fase *fase);

void gerarItem(Fase *fase, ItensEspeciais item[], int maxItens);

bool terminarFase(Fase *fase);

#endif
