#include "Jogo.h"

#include <cstring>
#include <cstdlib>
#include <ctime>

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
    estado->tempoParaProximoMeteoro = 0.0f;

    inicializarEstatisticas(estado);
    limparItens(estado);
    iniciarBarra(&estado->barra);
    iniciarBola(&estado->bola);
    estado->bola.velocidadex *= estado->multiplicadorVelocidade;
    estado->bola.velocidadey *= estado->multiplicadorVelocidade;
    inicializarSistemaParticulas(&estado->sistemaParticulas);
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
