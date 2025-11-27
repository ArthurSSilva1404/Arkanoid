#include <iostream>

#include "Estruturas.h"

#ifndef ITENS
#define ITENS

struct EstadoJogo;

void iniciarItem(ItensEspeciais *item, int tipo, float posx, float posy);

void desenharItem(ItensEspeciais *item);

void atualizarItem(ItensEspeciais *item);

void efeitoItem(ItensEspeciais *item, EstadoJogo *estado);

#endif
