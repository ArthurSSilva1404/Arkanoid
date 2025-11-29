#include "raylib.h"
#include "Estruturas.h"

#ifndef GRAFICOS_H
#define GRAFICOS_H

struct EstadoJogo;

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

void desenharFundoTematico(const Fase *fase, float animacao);

void desenharEscudoVisual(Rectangle area, float intensidade);

void desenharHUD(const EstadoJogo *estado);

void desenharTelaGameOver(const EstadoJogo *estado, bool vitoria);

void desenharTelaDificuldade(int selecionada);

void inicializarSistemaParticulas(SistemaParticulas *sistema);

void gerarExplosao(SistemaParticulas *sistema, Vector2 origem, Color corBase);

void atualizarParticulas(SistemaParticulas *sistema, float dt);

void desenharParticulas(const SistemaParticulas *sistema);

// Funções auxiliares
bool capturarNomeMenu(char *nome, int tamanhoMaximo);
void desenharItensAtivos(const EstadoJogo *estado);

#endif
