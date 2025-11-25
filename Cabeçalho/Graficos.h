#include "raylib.h"
#include "Estruturas.h"

#ifndef GRAFICOS_H
#define GRAFICOS_H

// Cores do jogo
extern Color COR_FUNDO;
extern Color COR_TEXTO;
extern Color COR_TITULO;
extern Color COR_BLOCO_FORTE;
extern Color COR_BLOCO_MEDIO;
extern Color COR_BLOCO_FRACO;
extern Color COR_BOLA;
extern Color COR_BARRA;
extern Color COR_ITEM_VIDA;
extern Color COR_ITEM_AUMENTAR;
extern Color COR_ITEM_DIMINUIR;
extern Color COR_ITEM_PONTOS;
extern Color COR_ITEM_INVERSOR;

void inicializarCores();

void desenharTelaCarregamento();

void desenharFundo();

void desenharBorda();

#endif
