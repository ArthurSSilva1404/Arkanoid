#ifndef RANKING_H
#define RANKING_H

#include "raylib.h"
#include <iostream>
#include "Estruturas.h"

void iniciarRanking(Ranking *ranking, char *arquivo);
void atualizarRanking(Ranking *ranking, Jogador *jogador);
void desenharRanking(Ranking *ranking);
void salvarRanking(Ranking *ranking);
void carregarRanking(Ranking *ranking);

#endif
