#include "Ranking.h"
#include "Constantes.h"
#include "Cores.h"
#include "Jogo.h"
#include "raylib.h"
#include <cstdio>
#include <cstring>

void iniciarRanking(Ranking *ranking, char *arquivo) {
    ranking->total_jogadores = 0;
    
    // Inicializar todos os jogadores com valores padrão
    for (int i = 0; i < MAX_JOGADORES; i++) {
        strcpy(ranking->jogador[i].nome, "---");
        ranking->jogador[i].pontuacao = 0;
        ranking->jogador[i].tempo = 0;
        ranking->jogador[i].vidas_jogador = 0;
        strcpy(ranking->jogador[i].dataRegistro, "--/--");
        ranking->jogador[i].dificuldadeSelecionada = DIFICULDADE_FACIL;
    }

    carregarRanking(ranking);
}

void atualizarRanking(Ranking *ranking, Jogador *jogador) {
    // Encontrar a posição correta para inserir o novo jogador
    int posicao = ranking->total_jogadores;

    // Procurar a posição baseada na pontuação
    for (int i = 0; i < ranking->total_jogadores; i++) {
        if (jogador->pontuacao > ranking->jogador[i].pontuacao) {
            posicao = i;
            break;
        }
    }

    // Se o ranking está cheio e a pontuação é menor que a última, não adicionar
    if (ranking->total_jogadores >= MAX_JOGADORES && posicao == ranking->total_jogadores) {
        return;
    }

    // Deslocar os jogadores para baixo
    if (ranking->total_jogadores < MAX_JOGADORES) {
        ranking->total_jogadores++;
    }

    for (int i = ranking->total_jogadores - 1; i > posicao; i--) {
        ranking->jogador[i] = ranking->jogador[i - 1];
    }

    // Inserir o novo jogador
    ranking->jogador[posicao] = *jogador;

    salvarRanking(ranking);
}

void desenharRanking(Ranking *ranking) {
    ClearBackground(obterCorFundo());

    DrawText("Ranking Galatico", LARGURA_TELA / 2 - 180, 40, 40, obterCorTitulo());

    DrawText("Pos", 60, 120, 20, obterCorTexto());
    DrawText("Nome", 110, 120, 20, obterCorTexto());
    DrawText("Pts", 290, 120, 20, obterCorTexto());
    DrawText("Tempo", 380, 120, 20, obterCorTexto());
    DrawText("Dif", 500, 120, 20, obterCorTexto());
    DrawText("Data", 580, 120, 20, obterCorTexto());

    for (int i = 0; i < ranking->total_jogadores && i < MAX_JOGADORES; i++) {
        int linhaY = 160 + i * 36;
        DrawText(TextFormat("%d", i + 1), 60, linhaY, 20, obterCorTexto());
        DrawText(ranking->jogador[i].nome, 110, linhaY, 20, obterCorTexto());
        DrawText(TextFormat("%d", ranking->jogador[i].pontuacao), 290, linhaY, 20, obterCorTexto());
        DrawText(TextFormat("%ds", ranking->jogador[i].tempo), 380, linhaY, 20, obterCorTexto());
        DrawText(obterNomeDificuldade(ranking->jogador[i].dificuldadeSelecionada), 500, linhaY, 20, obterCorTexto());
        DrawText(ranking->jogador[i].dataRegistro, 580, linhaY, 20, obterCorTexto());
    }

    DrawText("ESC - voltar ao menu", LARGURA_TELA / 2 - 120, ALTURA_TELA - 60, 20, LIGHTGRAY);
}

void salvarRanking(Ranking *ranking) {
    FILE *arquivo = fopen(ARQUIVO_RANKING, "w");
    
    if (arquivo == NULL) {
        return;
    }

    fprintf(arquivo, "%d\n", ranking->total_jogadores);

    for (int i = 0; i < ranking->total_jogadores; i++) {
        fprintf(arquivo, "%s;%d;%d;%d;%s\n", 
                ranking->jogador[i].nome,
                ranking->jogador[i].pontuacao,
                ranking->jogador[i].tempo,
                ranking->jogador[i].dificuldadeSelecionada,
                ranking->jogador[i].dataRegistro);
    }

    fclose(arquivo);
}

void carregarRanking(Ranking *ranking) {
    FILE *arquivo = fopen(ARQUIVO_RANKING, "r");
    
    if (arquivo == NULL) {
        ranking->total_jogadores = 0;
        return;
    }

    fscanf(arquivo, "%d\n", &ranking->total_jogadores);
    if (ranking->total_jogadores > MAX_JOGADORES) {
        ranking->total_jogadores = MAX_JOGADORES;
    }

    for (int i = 0; i < ranking->total_jogadores; i++) {
        fscanf(arquivo, "%49[^;];%d;%d;%d;%19[^\n]\n",
               ranking->jogador[i].nome,
               &ranking->jogador[i].pontuacao,
               &ranking->jogador[i].tempo,
               &ranking->jogador[i].dificuldadeSelecionada,
               ranking->jogador[i].dataRegistro);
    }

    fclose(arquivo);
}
