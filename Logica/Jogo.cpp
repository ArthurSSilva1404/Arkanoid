#include "Jogo.h"

#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "Barra.h"
#include "Bola.h"
#include "Constantes.h"
#include "Fases.h"
#include "Graficos.h"
#include "Itens.h"
#include "Jogador.h"
#include "Utils.h"

static void limparItens(EstadoJogo *estado) {
    estado->qtd_itens = 0;
    for (int i = 0; i < MAX_ITENS; i++) {
        estado->itens[i].esta_Ativo = false;
    }
}

static void inicializarEstatisticas(EstadoJogo *estado) {
    estado->estatisticas.blocosQuebrados = 0;
    estado->estatisticas.itensColetados = 0;
    estado->estatisticas.bonusPontuacao = 0;
    estado->estatisticas.vidasGastas = 0;
    estado->estatisticas.fasesConcluidas = 0;
    estado->estatisticas.pontuacaoInvertida = false;
}

static void registrarDataAtual(char *destino, size_t tamanho) {
    time_t agora = time(nullptr);
    tm *info = localtime(&agora);
    if (info != nullptr) {
        strftime(destino, tamanho, "%d/%m/%Y", info);
    } else if (tamanho > 0) {
        strncpy(destino, "--/--/----", tamanho - 1);
        destino[tamanho - 1] = '\0';
    }
}

float obterMultiplicadorDificuldade(int dificuldade) {
    switch (dificuldade) {
        case DIFICULDADE_MEDIO:
            return FATOR_DIFICULDADE_MEDIO;
        case DIFICULDADE_DIFICIL:
            return FATOR_DIFICULDADE_DIFICIL;
        default:
            return FATOR_DIFICULDADE_FACIL;
    }
}

const char *obterNomeDificuldade(int dificuldade) {
    switch (dificuldade) {
        case DIFICULDADE_MEDIO:
            return "Medio";
        case DIFICULDADE_DIFICIL:
            return "Dificil";
        default:
            return "Facil";
    }
}

void prepararEstadoJogo(EstadoJogo *estado, const char *nome, int dificuldade) {
    if (estado->fase.blocos != nullptr) {
        free(estado->fase.blocos);
        estado->fase.blocos = nullptr;
    }

    memset(&estado->jogador, 0, sizeof(Jogador));
    strncpy(estado->jogador.nome, nome, MAX_LETRAS - 1);
    estado->jogador.nome[MAX_LETRAS - 1] = '\0';
    estado->jogador.pontuacao = 0;
    estado->jogador.tempo = 0;
    estado->jogador.vidas_jogador = VIDAS_INICIAIS;
    estado->jogador.dificuldadeSelecionada = dificuldade;
    estado->jogador.itens_coletados = 0;
    estado->jogador.blocos_quebrados = 0;
    estado->jogador.dataRegistro[0] = '\0';

    estado->dificuldade = dificuldade;
    estado->fase_atual = 1;
    estado->qtd_itens = 0;
    estado->tempoFase = 0.0f;
    estado->aguardandoLancamento = true;
    estado->jogoConcluido = false;
    estado->multiplicadorVelocidade = obterMultiplicadorDificuldade(dificuldade);
    estado->escudoAtivo = false;
    estado->tempoEscudoRestante = 0.0f;
    estado->escudoArea = {0.0f, ALTURA_TELA - 18.0f, (float)LARGURA_TELA, 12.0f};
    estado->multiplicadorPontos = 1.0f;
    estado->tempoMultiplicador = 0;
    estado->ultimoItemColetado = 0;
    estado->tempoMostrarItem = 0.0f;
    estado->inversorAtivo = false;
    estado->tempoInversor = 0;

    inicializarEstatisticas(estado);
    limparItens(estado);
    iniciarBarra(&estado->barra);
    iniciarBola(&estado->bola);
    estado->bola.velocidadex *= estado->multiplicadorVelocidade;
    estado->bola.velocidadey *= estado->multiplicadorVelocidade;
    inicializarSistemaParticulas(&estado->sistemaParticulas);
    inicializarAudio(&estado->audio);
    tocarMusicaFundo(&estado->audio);
    iniciarFase(&estado->fase, estado->fase_atual);
}

void reiniciarElementosDaFase(EstadoJogo *estado) {
    iniciarBarra(&estado->barra);
    iniciarBola(&estado->bola);
    estado->bola.velocidadex *= estado->multiplicadorVelocidade;
    estado->bola.velocidadey *= estado->multiplicadorVelocidade;
    estado->aguardandoLancamento = true;
    limparItens(estado);
    estado->escudoAtivo = false;
    estado->tempoEscudoRestante = 0.0f;
}

void consolidarPontuacaoFinal(EstadoJogo *estado) {
    int bonus = calcularPontuacao(
        estado->jogador.tempo,
        estado->dificuldade,
        estado->estatisticas.blocosQuebrados,
        estado->estatisticas.itensColetados,
        estado->jogador.vidas_jogador);

    int total = estado->jogador.pontuacao + estado->estatisticas.bonusPontuacao + bonus;
    if (estado->estatisticas.pontuacaoInvertida) {
        total = -total;
    }

    if (total < 0) {
        total = 0;
    }

    estado->jogador.pontuacao = total;
    estado->jogador.itens_coletados = estado->estatisticas.itensColetados;
    estado->jogador.blocos_quebrados = estado->estatisticas.blocosQuebrados;
    registrarDataAtual(estado->jogador.dataRegistro, sizeof(estado->jogador.dataRegistro));
}

static ItensEspeciais *buscarItemDisponivel(EstadoJogo *estado) {
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

static void aplicarReflexaoNaBarra(EstadoJogo *estado) {
    // Corrigir posição da bola
    estado->bola.posicao.y = estado->barra.retangulo.posicao.y - estado->bola.radium;
    
    // Calcular posição relativa (0.0 = esquerda, 1.0 = direita)
    float posRelativa = (estado->bola.posicao.x - estado->barra.retangulo.posicao.x) / 
                        estado->barra.retangulo.largura;
    
    // Calcular ângulo de reflexão
    float angulo;
    if (fabsf(posRelativa - 0.5f) < 0.05f) {
        angulo = 0.0f;
    } else {
        angulo = -75.0f + posRelativa * 150.0f;
    }
    
    float anguloRad = angulo * 3.14159265358979323846f / 180.0f;
    float velocidade = sqrtf(estado->bola.velocidadex * estado->bola.velocidadex + 
                             estado->bola.velocidadey * estado->bola.velocidadey);
    
    estado->bola.velocidadex = velocidade * sinf(anguloRad);
    estado->bola.velocidadey = -velocidade * cosf(anguloRad);
    
    if (fabsf(estado->bola.velocidadey) < 50.0f) {
        estado->bola.velocidadey = -50.0f;
    }
    
    atualizarPontuacao(&estado->jogador, 5);
}

static void processarColisaoBlocos(EstadoJogo *estado) {
    int totalBlocos = estado->fase.linha * estado->fase.coluna;
    
    for (int i = 0; i < totalBlocos; i++) {
        Bloco *bloco = &estado->fase.blocos[i];
        if (!bloco->ativo) continue;

        if (verificarColisaoCirculoRetangulo(estado->bola.posicao, estado->bola.radium, bloco->retangulo)) {
            float centroX = bloco->retangulo.posicao.x + bloco->retangulo.largura / 2.0f;
            float centroY = bloco->retangulo.posicao.y + bloco->retangulo.altura / 2.0f;
            
            float dx = estado->bola.posicao.x - centroX;
            float dy = estado->bola.posicao.y - centroY;

            float metadeLargura = bloco->retangulo.largura / 2.0f;
            float metadeAltura = bloco->retangulo.altura / 2.0f;

            float sobreposicaoX = metadeLargura + estado->bola.radium - fabsf(dx);
            float sobreposicaoY = metadeAltura + estado->bola.radium - fabsf(dy);

            float velocidadeAntes = sqrtf(estado->bola.velocidadex * estado->bola.velocidadex +
                                          estado->bola.velocidadey * estado->bola.velocidadey);

            if (sobreposicaoX < sobreposicaoY) {
                if (dx > 0) {
                    estado->bola.posicao.x = bloco->retangulo.posicao.x + bloco->retangulo.largura + estado->bola.radium;
                } else {
                    estado->bola.posicao.x = bloco->retangulo.posicao.x - estado->bola.radium;
                }
                estado->bola.velocidadex = -estado->bola.velocidadex;
            } else {
                if (dy > 0) {
                    estado->bola.posicao.y = bloco->retangulo.posicao.y + bloco->retangulo.altura + estado->bola.radium;
                } else {
                    estado->bola.posicao.y = bloco->retangulo.posicao.y - estado->bola.radium;
                }
                estado->bola.velocidadey = -estado->bola.velocidadey;
                
                if (fabsf(estado->bola.velocidadey) < 50.0f) {
                    estado->bola.velocidadey = (estado->bola.velocidadey > 0) ? 50.0f : -50.0f;
                }
            }

            float velocidadeDepois = sqrtf(estado->bola.velocidadex * estado->bola.velocidadex +
                                           estado->bola.velocidadey * estado->bola.velocidadey);
            if (velocidadeAntes > 0.001f && velocidadeDepois > 0.001f) {
                estado->bola.velocidadex *= velocidadeAntes / velocidadeDepois;
                estado->bola.velocidadey *= velocidadeAntes / velocidadeDepois;
            }

            bloco->TotalDeVida--;

            if (bloco->TotalDeVida <= 0) {
                bloco->ativo = false;
                estado->estatisticas.blocosQuebrados++;
                estado->jogador.blocos_quebrados++;

                // Calcular pontos do bloco
                int pontos = (int)(bloco->pontosBase * estado->multiplicadorVelocidade);
                pontos = (int)(pontos * estado->multiplicadorPontos);
                
                // Aplicar inversor se ativo
                if (estado->inversorAtivo) {
                    pontos = -pontos;
                }
                
                atualizarPontuacao(&estado->jogador, pontos);
                tocarSomQuebrarBloco(&estado->audio);
                gerarExplosao(&estado->sistemaParticulas, Vector2{centroX, centroY}, bloco->cor);

                if (bloco->possuiItem) {
                    ItensEspeciais *novoItem = buscarItemDisponivel(estado);
                    if (novoItem != nullptr) {
                        int tipo = gerarAleatorio(ITEM_VIDA, ITEM_INVERSOR);
                        iniciarItem(novoItem, tipo, centroX, centroY);
                    }
                }
            }
            break;
        }
    }
}

static void processarItens(EstadoJogo *estado) {
    for (int i = 0; i < estado->qtd_itens; i++) {
        ItensEspeciais *item = &estado->itens[i];
        if (!item->esta_Ativo) continue;

        atualizarItem(item);
        if (verificarColisaoCirculoRetangulo(item->posicao_do_item, item->tamanho / 2.0f, 
                                             estado->barra.retangulo)) {
            efeitoItem(item, estado);
        }
    }
}

void processarColisoes(EstadoJogo *estado) {
    if (verificarColisaoCirculoRetangulo(estado->bola.posicao, estado->bola.radium, 
                                         estado->barra.retangulo) &&
        estado->bola.velocidadey > 0) {
        tocarSomColisaoBolaBarra(&estado->audio);
        aplicarReflexaoNaBarra(estado);
    }

    processarColisaoBlocos(estado);
    processarItens(estado);
}

void atualizarJogo(EstadoJogo *estado, float dt) {
    atualizarMusicaFundo(&estado->audio);
    
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
    processarColisoes(estado);
    atualizarParticulas(&estado->sistemaParticulas, dt);
    
    // Atualizar tempo do multiplicador
    if (estado->tempoMultiplicador > 0) {
        estado->tempoMultiplicador--;
        if (estado->tempoMultiplicador <= 0) {
            estado->multiplicadorPontos = 1.0f;
        }
    }
    
    // Atualizar tempo do inversor
    if (estado->tempoInversor > 0) {
        estado->tempoInversor--;
        if (estado->tempoInversor <= 0) {
            estado->inversorAtivo = false;
        }
    }
    
    // Atualizar tempo de mostrar item
    if (estado->tempoMostrarItem > 0.0f) {
        estado->tempoMostrarItem -= dt;
        if (estado->tempoMostrarItem < 0.0f) {
            estado->tempoMostrarItem = 0.0f;
        }
    }
}

void verificarCondicaoVida(EstadoJogo *estado, bool *perdeuJogo) {
    *perdeuJogo = false;
    
    if (estado->bola.posicao.y - estado->bola.radium > ALTURA_TELA) {
        if (estado->jogador.vidas_jogador > 1) {
            resetarVida(estado);
        } else {
            atualizarVidas(&estado->jogador);
            estado->estatisticas.vidasGastas++;
            *perdeuJogo = true;
        }
    }
}

void resetarVida(EstadoJogo *estado) {
    atualizarVidas(&estado->jogador);
    estado->estatisticas.vidasGastas++;
    estado->aguardandoLancamento = true;
    estado->bola.prendeu = true;
    estado->bola.velocidadex = 0.0f;
    estado->bola.velocidadey = -estado->bola.velocidadeBase * estado->multiplicadorVelocidade;
    alinharBolaNaBarra(&estado->bola, &estado->barra);
}

void avancarParaProximaFase(EstadoJogo *estado) {
    estado->fase_atual++;
    estado->estatisticas.fasesConcluidas++;
    iniciarFase(&estado->fase, estado->fase_atual);
    reiniciarElementosDaFase(estado);
}
