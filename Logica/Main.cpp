#include "raylib.h"
#include "Estruturas.h"
#include "Constantes.h"
#include "Bola.h"
#include "Barra.h"
#include "Bloco.h"
#include "Fases.h"
#include "Itens.h"
#include "Jogador.h"
#include "Menu.h"
#include "Ranking.h"
#include "Utils.h"
#include "Estado.h"
#include "Graficos.h"
#include "Cores.h"
#include "Sons.h"
#include "Jogo.h"

#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>

static bool telaCadastroNome(char *nome, int tamanhoMaximo, bool reiniciar) {
    static int posicao = 0;
    if (reiniciar) {
        nome[0] = '\0';
        posicao = 0;
    }

    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 126 && posicao < tamanhoMaximo - 1) {
            nome[posicao] = (char)key;
            posicao++;
            nome[posicao] = '\0';
        }
        key = GetCharPressed();
    }

    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE)) && posicao > 0) {
        posicao--;
        nome[posicao] = '\0';
    }

    ClearBackground(obterCorFundo());
    desenharBorda();
    DrawText("Digite seu nome galactico", LARGURA_TELA / 2 - 220, 120, 34, obterCorTitulo());
    DrawRectangle(LARGURA_TELA / 2 - 220, 220, 440, 60, Color{0, 0, 0, 120});
    DrawRectangleLines(LARGURA_TELA / 2 - 220, 220, 440, 60, obterCorTitulo());
    DrawText(TextFormat("%s_", nome), LARGURA_TELA / 2 - 200, 235, 32, obterCorTexto());
    DrawText("ENTER - confirmar | ESC - voltar", LARGURA_TELA / 2 - 200, ALTURA_TELA - 120, 22, LIGHTGRAY);

    if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && posicao > 0) {
        return true;
    }

    return false;
}

static void refletirNaBarra(EstadoJogo *estado) {
    float centroBarra = estado->barra.retangulo.posicao.x + estado->barra.retangulo.largura / 2.0f;
    float distancia = (estado->bola.posicao.x - centroBarra) / (estado->barra.retangulo.largura / 2.0f);
    estado->bola.velocidadex = estado->bola.velocidadeBase * distancia * estado->multiplicadorVelocidade * 1.2f;
    estado->bola.velocidadey = -fabsf(estado->bola.velocidadey);
    estado->bola.posicao.y = estado->barra.retangulo.posicao.y - estado->bola.radium - 1.0f;
    atualizarPontuacao(&estado->jogador, 5);
}

static ItensEspeciais *alocarItem(EstadoJogo *estado) {
    for (int i = 0; i < MAX_ITENS; i++) {
        if (!estado->itens[i].esta_Ativo) {
            if (i >= estado->qtd_itens) {
                estado->qtd_itens = i + 1;
            }
            return &estado->itens[i];
        }
    }
    return nullptr;
}

static float gerarIntervaloMeteoro() {
    int minimo = (int)(METEORO_INTERVALO_MIN * 100.0f);
    int maximo = (int)(METEORO_INTERVALO_MAX * 100.0f);
    return (float)GetRandomValue(minimo, maximo) / 100.0f;
}

static void prepararMeteoros(EstadoJogo *estado) {
    for (int i = 0; i < MAX_METEOROS; i++) {
        estado->meteoros[i].ativo = false;
    }
    estado->tempoParaProximoMeteoro = gerarIntervaloMeteoro();
}

static void dispararMeteoro(EstadoJogo *estado) {
    for (int i = 0; i < MAX_METEOROS; i++) {
        if (!estado->meteoros[i].ativo) {
            estado->meteoros[i].ativo = true;
            estado->meteoros[i].posicao = { (float)GetRandomValue(-50, LARGURA_TELA + 50), -30.0f };
            float velocidade = (float)GetRandomValue(180, 320);
            estado->meteoros[i].velocidade = { -velocidade * 0.4f, velocidade };
            estado->meteoros[i].tamanho = (float)GetRandomValue(6, 14);
            estado->meteoros[i].cor = Color{255, (unsigned char)GetRandomValue(180, 230), 120, 180};
            break;
        }
    }
}

static void atualizarMeteoros(EstadoJogo *estado, float dt) {
    estado->tempoParaProximoMeteoro -= dt;
    if (estado->tempoParaProximoMeteoro <= 0.0f) {
        dispararMeteoro(estado);
        estado->tempoParaProximoMeteoro = gerarIntervaloMeteoro();
    }

    for (int i = 0; i < MAX_METEOROS; i++) {
        if (!estado->meteoros[i].ativo) continue;
        estado->meteoros[i].posicao.x += estado->meteoros[i].velocidade.x * dt;
        estado->meteoros[i].posicao.y += estado->meteoros[i].velocidade.y * dt;
        if (estado->meteoros[i].posicao.y > ALTURA_TELA + 40) {
            estado->meteoros[i].ativo = false;
        }
    }
}

static void processarBlocos(EstadoJogo *estado) {
    int total_blocos = estado->fase.linha * estado->fase.coluna;
    for (int i = 0; i < total_blocos; i++) {
        Bloco *bloco = &estado->fase.blocos[i];
        if (!bloco->ativo) {
            continue;
        }

        if (verificarColisaoCirculoRetangulo(estado->bola.posicao, estado->bola.radium, bloco->retangulo)) {
            float centroX = bloco->retangulo.posicao.x + bloco->retangulo.largura / 2.0f;
            float centroY = bloco->retangulo.posicao.y + bloco->retangulo.altura / 2.0f;
            float diferencaX = estado->bola.posicao.x - centroX;
            float diferencaY = estado->bola.posicao.y - centroY;

            if (fabsf(diferencaX) > fabsf(diferencaY)) {
                estado->bola.velocidadex *= -1;
            } else {
                estado->bola.velocidadey *= -1;
            }

            estado->bola.posicao.x += estado->bola.velocidadex * GetFrameTime();
            estado->bola.posicao.y += estado->bola.velocidadey * GetFrameTime();

            if (bloco->indestrutivel) {
                gerarExplosao(&estado->sistemaParticulas,
                              Vector2{centroX, centroY},
                              bloco->cor);
                continue;
            }

            bloco->TotalDeVida--;

            if (bloco->TotalDeVida <= 0) {
                bloco->ativo = false;
                estado->estatisticas.blocosQuebrados++;
                estado->jogador.blocos_quebrados++;

                int pontos = (int)(bloco->pontosBase * estado->multiplicadorVelocidade);
                atualizarPontuacao(&estado->jogador, pontos);
                gerarExplosao(&estado->sistemaParticulas,
                              Vector2{centroX, centroY},
                              bloco->cor);

                if (bloco->possuiItem) {
                    ItensEspeciais *novoItem = alocarItem(estado);
                    if (novoItem != nullptr) {
                        int tipo = gerarAleatorio(ITEM_VIDA, ITEM_ESCUDO);
                        if (tipo == ITEM_ESCUDO && estado->escudoAtivo) {
                            tipo = ITEM_PONTOS_EXTRAS;
                        }
                        iniciarItem(novoItem, tipo, centroX, centroY);
                    }
                }
            }
            break;
        }
    }
}

static void atualizarItens(EstadoJogo *estado) {
    for (int i = 0; i < estado->qtd_itens; i++) {
        ItensEspeciais *item = &estado->itens[i];
        if (!item->esta_Ativo) {
            continue;
        }

        atualizarItem(item);
        if (verificarColisaoCirculoRetangulo(item->posicao_do_item, item->tamanho / 2.0f, estado->barra.retangulo)) {
            efeitoItem(item, estado);
        }
    }
}

static void desenharItens(const EstadoJogo *estado) {
    for (int i = 0; i < estado->qtd_itens; i++) {
        if (estado->itens[i].esta_Ativo) {
            desenharItem((ItensEspeciais *)&estado->itens[i]);
        }
    }
}

static void atualizarPartida(EstadoJogo *estado, float dt) {
    if (estado->aguardandoLancamento) {
        alinharBolaNaBarra(&estado->bola, &estado->barra);
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) {
            estado->aguardandoLancamento = false;
            soltarBola(&estado->bola, estado->multiplicadorVelocidade);
        }
    } else {
        atualizarBola(&estado->bola, dt);
    }

    atualizarBarra(&estado->barra, dt);

    if (verificarColisaoCirculoRetangulo(estado->bola.posicao, estado->bola.radium, estado->barra.retangulo) &&
        estado->bola.velocidadey > 0) {
        refletirNaBarra(estado);
    }

    processarBlocos(estado);
    atualizarItens(estado);
    atualizarParticulas(&estado->sistemaParticulas, dt);
    atualizarMeteoros(estado, dt);

    if (estado->escudoAtivo) {
        estado->tempoEscudoRestante -= dt;
        if (estado->tempoEscudoRestante <= 0.0f) {
            estado->escudoAtivo = false;
            estado->tempoEscudoRestante = 0.0f;
        }
    }
}

static void perderVida(EstadoJogo *estado) {
    atualizarVidas(&estado->jogador);
    estado->estatisticas.vidasGastas++;
    estado->aguardandoLancamento = true;
    estado->bola.prendeu = true;
    estado->bola.velocidadex = 0.0f;
    estado->bola.velocidadey = -estado->bola.velocidadeBase * estado->multiplicadorVelocidade;
    alinharBolaNaBarra(&estado->bola, &estado->barra);
    estado->escudoAtivo = false;
    estado->tempoEscudoRestante = 0.0f;
}

static void avancarFase(EstadoJogo *estado) {
    estado->fase_atual++;
    estado->estatisticas.fasesConcluidas++;
    iniciarFase(&estado->fase, estado->fase_atual);
    reiniciarElementosDaFase(estado);
    prepararMeteoros(estado);
}

static void finalizarRodada(EstadoJogo *estado, Ranking *ranking) {
    consolidarPontuacaoFinal(estado);
    atualizarRanking(ranking, &estado->jogador);
    estado->jogoConcluido = true;
}

int main() {
    srand((unsigned int)time(NULL));
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Arkanoid 2D - Projeto M3");
    SetTargetFPS(60);

    EstadoJogo estado = {};
    estado.fase.blocos = nullptr;
    Ranking ranking = {};
    iniciarRanking(&ranking, (char *)ARQUIVO_RANKING);

    int estadoPrincipal = ESTADO_MENU;
    int dificuldadeSelecionada = DIFICULDADE_FACIL;
    float tempoDecorrido = 0.0f;
    bool precisaLimparNome = true;
    char nomeDigitado[MAX_LETRAS] = "Jogador";

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (estadoPrincipal == ESTADO_JOGANDO) {
            tempoDecorrido += dt;
            estado.jogador.tempo = (int)tempoDecorrido;
        }

        BeginDrawing();

        switch (estadoPrincipal) {
            case ESTADO_MENU: {
                desenharFundo();
                desenharBorda();
                int opcao = menuIniciar();
                if (opcao == MENU_OP_INICIAR) {
                    dificuldadeSelecionada = DIFICULDADE_FACIL;
                    estadoPrincipal = ESTADO_CADASTRO;
                    precisaLimparNome = true;
                } else if (opcao == MENU_OP_DIFICULDADE) {
                    estadoPrincipal = ESTADO_DIFICULDADE;
                } else if (opcao == MENU_OP_RANKING) {
                    estadoPrincipal = ESTADO_RANKING;
                } else if (opcao == MENU_OP_SAIR) {
                    estadoPrincipal = ESTADO_SAIR;
                }
                break;
            }
            case ESTADO_CADASTRO: {
                bool confirmado = telaCadastroNome(nomeDigitado, MAX_LETRAS, precisaLimparNome);
                precisaLimparNome = false;
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoPrincipal = ESTADO_MENU;
                } else if (confirmado) {
                    estadoPrincipal = ESTADO_DIFICULDADE;
                }
                break;
            }
            case ESTADO_DIFICULDADE: {
                desenharTelaDificuldade(dificuldadeSelecionada);
                if (IsKeyPressed(KEY_ONE)) dificuldadeSelecionada = DIFICULDADE_FACIL;
                if (IsKeyPressed(KEY_TWO)) dificuldadeSelecionada = DIFICULDADE_MEDIO;
                if (IsKeyPressed(KEY_THREE)) dificuldadeSelecionada = DIFICULDADE_DIFICIL;
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoPrincipal = ESTADO_MENU;
                } else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                    prepararEstadoJogo(&estado, nomeDigitado, dificuldadeSelecionada);
                    prepararMeteoros(&estado);
                    tempoDecorrido = 0.0f;
                    estadoPrincipal = ESTADO_JOGANDO;
                }
                break;
            }
            case ESTADO_JOGANDO: {
                desenharFundoTematico(&estado.fase, tempoDecorrido);
                atualizarPartida(&estado, dt);

                if (estado.bola.posicao.y - estado.bola.radium > ALTURA_TELA) {
                    if (estado.escudoAtivo) {
                        estado.escudoAtivo = false;
                        estado.tempoEscudoRestante = 0.0f;
                        estado.bola.velocidadey = -fabsf(estado.bola.velocidadeBase * estado.multiplicadorVelocidade);
                        estado.bola.posicao.y = estado.escudoArea.y - estado.bola.radium - 2.0f;
                        gerarExplosao(&estado.sistemaParticulas,
                                      Vector2{estado.bola.posicao.x, estado.escudoArea.y},
                                      obterCorItemEscudo());
                    } else if (estado.jogador.vidas_jogador > 1) {
                        perderVida(&estado);
                    } else {
                        atualizarVidas(&estado.jogador);
                        estado.estatisticas.vidasGastas++;
                        finalizarRodada(&estado, &ranking);
                        estadoPrincipal = ESTADO_GAME_OVER;
                    }
                }

                if (terminarFase(&estado.fase)) {
                    if (estado.fase_atual >= MAX_FASES) {
                        estado.fase_atual = MAX_FASES + 1;
                        finalizarRodada(&estado, &ranking);
                        estadoPrincipal = ESTADO_GAME_OVER;
                    } else {
                        avancarFase(&estado);
                    }
                }

                if (IsKeyPressed(KEY_P)) {
                    estadoPrincipal = ESTADO_PAUSA;
                }

                desenharMeteoros(estado.meteoros, MAX_METEOROS);
                desenharFase(&estado.fase);
                desenharParticulas(&estado.sistemaParticulas);
                desenharBarra(&estado.barra);
                desenharBola(&estado.bola);
                desenharItens(&estado);
                if (estado.escudoAtivo) {
                    desenharEscudoVisual(estado.escudoArea, estado.tempoEscudoRestante / TEMPO_ESCUDO);
                }
                desenharHUD(&estado);
                break;
            }
            case ESTADO_PAUSA: {
                desenharFundoTematico(&estado.fase, tempoDecorrido);
                desenharMeteoros(estado.meteoros, MAX_METEOROS);
                desenharFase(&estado.fase);
                desenharBarra(&estado.barra);
                desenharBola(&estado.bola);
                desenharItens(&estado);
                if (estado.escudoAtivo) {
                    desenharEscudoVisual(estado.escudoArea, estado.tempoEscudoRestante / TEMPO_ESCUDO);
                }
                DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Color{0, 0, 0, 180});
                DrawText("Jogo Pausado", LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 40, 36, obterCorTitulo());
                DrawText("P - Retomar | ESC - Menu", LARGURA_TELA / 2 - 200, ALTURA_TELA / 2 + 10, 24, obterCorTexto());
                if (IsKeyPressed(KEY_P)) {
                    estadoPrincipal = ESTADO_JOGANDO;
                } else if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoPrincipal = ESTADO_MENU;
                }
                break;
            }
            case ESTADO_GAME_OVER: {
                desenharFundoTematico(&estado.fase, tempoDecorrido);
                desenharMeteoros(estado.meteoros, MAX_METEOROS);
                desenharFase(&estado.fase);
                desenharTelaGameOver(&estado, estado.fase_atual > MAX_FASES);
                if (IsKeyPressed(KEY_ENTER)) {
                    estadoPrincipal = ESTADO_MENU;
                    precisaLimparNome = true;
                } else if (IsKeyPressed(KEY_R)) {
                    prepararEstadoJogo(&estado, nomeDigitado, dificuldadeSelecionada);
                    prepararMeteoros(&estado);
                    tempoDecorrido = 0.0f;
                    estadoPrincipal = ESTADO_JOGANDO;
                }
                break;
            }
            case ESTADO_RANKING: {
                desenharRanking(&ranking);
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoPrincipal = ESTADO_MENU;
                }
                break;
            }
            case ESTADO_SAIR: {
                CloseWindow();
                return 0;
            }
        }

        EndDrawing();
    }

    if (estado.fase.blocos != nullptr) {
        free(estado.fase.blocos);
    }
    CloseWindow();
    return 0;
}
