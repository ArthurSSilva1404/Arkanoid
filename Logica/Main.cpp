#include "raylib.h"
#include "Estruturas.h"
#include "Constantes.h"
#include "Menu.h"
#include "Ranking.h"
#include "Estado.h"
#include "Graficos.h"
#include "Jogo.h"
#include "Fases.h"
#include "Barra.h"
#include "Bola.h"
#include "Cores.h"

#include <cstring>
#include <cstdlib>
#include <ctime>

int main() {
    srand((unsigned int)time(NULL));
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Arkanoid 2D - Projeto M3");
    SetTargetFPS(60);
    SetExitKey(0);  // Desabilita ESC como tecla de saída padrão

    EstadoJogo jogo = {};
    jogo.fase.blocos = nullptr;
    Ranking ranking = {};
    iniciarRanking(&ranking, (char *)ARQUIVO_RANKING);

    int estadoAtual = ESTADO_MENU;
    int nivelDificuldade = DIFICULDADE_FACIL;
    float tempoTotal = 0.0f;
    char nomeJogador[MAX_LETRAS] = "Jogador";

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (estadoAtual == ESTADO_JOGANDO) {
            tempoTotal += deltaTime;
            jogo.jogador.tempo = (int)tempoTotal;
        }

        BeginDrawing();

        if (estadoAtual == ESTADO_MENU) {
            desenharFundo();
            desenharBorda();
            int opcao = menuIniciar();
            if (opcao == MENU_OP_INICIAR) {
                nivelDificuldade = DIFICULDADE_FACIL;
                estadoAtual = ESTADO_CADASTRO;
            } else if (opcao == MENU_OP_DIFICULDADE) {
                estadoAtual = ESTADO_DIFICULDADE;
            } else if (opcao == MENU_OP_RANKING) {
                estadoAtual = ESTADO_RANKING;
            } else if (opcao == MENU_OP_SAIR) {
                estadoAtual = ESTADO_SAIR;
            }
        }
        else if (estadoAtual == ESTADO_CADASTRO) {
            if (capturarNomeMenu(nomeJogador, MAX_LETRAS)) {
                estadoAtual = ESTADO_DIFICULDADE;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = ESTADO_MENU;
            }
        }
        else if (estadoAtual == ESTADO_DIFICULDADE) {
            desenharTelaDificuldade(nivelDificuldade);
            
            // Navegação por setas ou W/S
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                nivelDificuldade--;
                if (nivelDificuldade < DIFICULDADE_FACIL) {
                    nivelDificuldade = DIFICULDADE_DIFICIL;
                }
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                nivelDificuldade++;
                if (nivelDificuldade > DIFICULDADE_DIFICIL) {
                    nivelDificuldade = DIFICULDADE_FACIL;
                }
            }
            
            // Atalhos numéricos
            if (IsKeyPressed(KEY_ONE)) nivelDificuldade = DIFICULDADE_FACIL;
            if (IsKeyPressed(KEY_TWO)) nivelDificuldade = DIFICULDADE_MEDIO;
            if (IsKeyPressed(KEY_THREE)) nivelDificuldade = DIFICULDADE_DIFICIL;
            
            if (IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = ESTADO_MENU;
            } else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                prepararEstadoJogo(&jogo, nomeJogador, nivelDificuldade);
                tempoTotal = 0.0f;
                estadoAtual = ESTADO_JOGANDO;
            }
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            desenharFundoTematico(&jogo.fase, tempoTotal);
            atualizarJogo(&jogo, deltaTime);

            bool perdeuJogo = false;
            verificarCondicaoVida(&jogo, &perdeuJogo);
            
            if (perdeuJogo) {
                tocarSomDerrota(&jogo.audio);
                consolidarPontuacaoFinal(&jogo);
                atualizarRanking(&ranking, &jogo.jogador);
                jogo.jogoConcluido = true;
                estadoAtual = ESTADO_GAME_OVER;
            }

            if (terminarFase(&jogo.fase)) {
                if (jogo.fase_atual >= MAX_FASES) {
                    tocarSomVitoria(&jogo.audio);
                    jogo.fase_atual = MAX_FASES + 1;
                    consolidarPontuacaoFinal(&jogo);
                    atualizarRanking(&ranking, &jogo.jogador);
                    jogo.jogoConcluido = true;
                    estadoAtual = ESTADO_GAME_OVER;
                } else {
                    avancarParaProximaFase(&jogo);
                }
            }

            if (IsKeyPressed(KEY_P)) {
                estadoAtual = ESTADO_PAUSA;
            }

            desenharFase(&jogo.fase);
            desenharParticulas(&jogo.sistemaParticulas);
            desenharBarra(&jogo.barra);
            desenharBola(&jogo.bola);
            desenharItensAtivos(&jogo);
            desenharHUD(&jogo);
        }
        else if (estadoAtual == ESTADO_PAUSA) {
            desenharFundoTematico(&jogo.fase, tempoTotal);
            desenharFase(&jogo.fase);
            desenharBarra(&jogo.barra);
            desenharBola(&jogo.bola);
            desenharItensAtivos(&jogo);
            DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Color{0, 0, 0, 180});
            DrawText("Jogo Pausado", LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 40, 36, obterCorTitulo());
            DrawText("P - Retomar | ESC - Menu", LARGURA_TELA / 2 - 200, ALTURA_TELA / 2 + 10, 24, obterCorTexto());
            if (IsKeyPressed(KEY_P)) {
                estadoAtual = ESTADO_JOGANDO;
            } else if (IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = ESTADO_MENU;
            }
        }
        else if (estadoAtual == ESTADO_GAME_OVER) {
            desenharFundoTematico(&jogo.fase, tempoTotal);
            desenharFase(&jogo.fase);
            desenharTelaGameOver(&jogo, jogo.fase_atual > MAX_FASES);
            if (IsKeyPressed(KEY_ENTER)) {
                estadoAtual = ESTADO_MENU;
            } else if (IsKeyPressed(KEY_R)) {
                prepararEstadoJogo(&jogo, nomeJogador, nivelDificuldade);
                tempoTotal = 0.0f;
                estadoAtual = ESTADO_JOGANDO;
            }
        }
        else if (estadoAtual == ESTADO_RANKING) {
            desenharRanking(&ranking);
            if (IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = ESTADO_MENU;
            }
        }
        else if (estadoAtual == ESTADO_SAIR) {
            CloseWindow();
            return 0;
        }

        EndDrawing();
    }

    if (jogo.fase.blocos != nullptr) {
        free(jogo.fase.blocos);
    }
    CloseWindow();
    return 0;
}
