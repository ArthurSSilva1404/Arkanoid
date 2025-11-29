#include "Graficos.h"
#include "Constantes.h"
#include "Cores.h"
#include "Jogo.h"
#include "Itens.h"
#include "Utils.h"
#include <cmath>
#include <algorithm>

// Definição das cores do jogo
Color COR_FUNDO = {20, 20, 40, 255};
Color COR_TEXTO = {255, 255, 255, 255};
Color COR_TITULO = {255, 255, 0, 255};
Color COR_BLOCO_FORTE = {255, 0, 0, 255};
Color COR_BLOCO_MEDIO = {255, 165, 0, 255};
Color COR_BLOCO_FRACO = {255, 255, 0, 255};
Color COR_BOLA = {255, 255, 0, 255};
Color COR_BARRA = {255, 255, 255, 255};
Color COR_ITEM_VIDA = {0, 255, 0, 255};
Color COR_ITEM_AUMENTAR = {0, 0, 255, 255};
Color COR_ITEM_DIMINUIR = {128, 0, 128, 255};
Color COR_ITEM_PONTOS = {255, 215, 0, 255};
Color COR_ITEM_INVERSOR = {255, 192, 203, 255};

void inicializarCores() {
    // Cores já inicializadas acima
}

void desenharTelaCarregamento() {
    ClearBackground(COR_FUNDO);
    DrawText("CARREGANDO...", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 - 30, 40, COR_TITULO);
}

void desenharFundo() {
    ClearBackground(COR_FUNDO);
    
    // Desenhar padrão de fundo (opcional)
    for (int i = 0; i < LARGURA_TELA; i += 50) {
        DrawLine(i, 0, i, ALTURA_TELA, Color{40, 40, 60, 100});
    }
    for (int i = 0; i < ALTURA_TELA; i += 50) {
        DrawLine(0, i, LARGURA_TELA, i, Color{40, 40, 60, 100});
    }
}

void desenharBorda() {
    DrawRectangleLines(0, 0, LARGURA_TELA, ALTURA_TELA, COR_TEXTO);
}

void desenharFundoTematico(const Fase *fase, float animacao) {
    Color base = fase ? fase->corFundo : COR_FUNDO;
    Color detalhe = fase ? fase->corDetalhe : COR_TEXTO;

    ClearBackground(base);

    float ondas = animacao * 0.35f;
    for (int y = 0; y < ALTURA_TELA + 200; y += 80) {
        float deslocamento = sinf(ondas + y * 0.02f) * 50.0f;
        DrawEllipse(LARGURA_TELA / 2 + deslocamento, y - 100, 420, 120, Color{detalhe.r, detalhe.g, detalhe.b, 32});
    }

    for (int i = 0; i < LARGURA_TELA; i += 60) {
        Color linha = detalhe;
        linha.a = 40;
        DrawLine(i, 0, i, ALTURA_TELA, linha);
    }
}

void desenharEscudoVisual(Rectangle area, float intensidade) {
    intensidade = std::clamp(intensidade, 0.0f, 1.0f);
    Color corBase = Color{110, 255, 255, (unsigned char)(120 + intensidade * 100)};
    DrawRectangleGradientH((int)area.x, (int)area.y, (int)area.width, (int)area.height,
                           Fade(corBase, 0.3f), Fade(corBase, 0.8f));
    DrawRectangleLines((int)area.x, (int)area.y, (int)area.width, (int)area.height,
                       Color{200, 255, 255, 200});
}

void desenharHUD(const EstadoJogo *estado) {
    DrawText(TextFormat("Pontuacao: %d", estado->jogador.pontuacao), 24, 18, 24, obterCorTexto());
    DrawText(TextFormat("Tempo: %02d:%02d", estado->jogador.tempo / 60, estado->jogador.tempo % 60), 24, 48, 24, obterCorTexto());
    DrawText(TextFormat("Vidas: %d", estado->jogador.vidas_jogador), 24, 78, 24, obterCorTexto());
    DrawText(TextFormat("Itens: %d", estado->jogador.itens_coletados), 24, 108, 24, obterCorTexto());
    
    // Mostrar último item coletado
    if (estado->tempoMostrarItem > 0.0f) {
        const char *nomeItem = "";
        Color corItem = WHITE;
        switch (estado->ultimoItemColetado) {
            case ITEM_VIDA:
                nomeItem = "Vida Extra!";
                corItem = GREEN;
                break;
            case ITEM_AUMENTAR_BARRA:
                nomeItem = "Barra Maior!";
                corItem = BLUE;
                break;
            case ITEM_DIMINUIR_BARRA:
                nomeItem = "Barra Menor!";
                corItem = PURPLE;
                break;
            case ITEM_PONTOS_EXTRAS:
                nomeItem = "Pontos Extras!";
                corItem = GOLD;
                break;
            case ITEM_MULTIPLICADOR:
                nomeItem = "Multiplicador x2!";
                corItem = ORANGE;
                break;
            case ITEM_INVERSOR:
                nomeItem = "Inversor Ativo!";
                corItem = RED;
                break;
        }
        DrawText(nomeItem, 24, 138, 22, corItem);
    }
    
    // Mostrar multiplicador ativo
    if (estado->multiplicadorPontos > 1.0f) {
        DrawText(TextFormat("Multiplicador x%.0f (%ds)", estado->multiplicadorPontos, estado->tempoMultiplicador / 60), 
                 24, 168, 20, ORANGE);
    }
    
    // Mostrar inversor ativo
    if (estado->inversorAtivo) {
        DrawText(TextFormat("INVERSOR ATIVO! (%ds)", estado->tempoInversor / 60), 24, 198, 20, RED);
    }

    DrawText(TextFormat("Fase %d - %s", estado->fase_atual, estado->fase.nome),
             LARGURA_TELA / 2 - 160, 20, 26, obterCorTitulo());
    DrawText(TextFormat("Dificuldade: %s", obterNomeDificuldade(estado->dificuldade)),
             LARGURA_TELA - 280, 18, 24, obterCorTexto());
    DrawText(TextFormat("Blocos: %d", estado->estatisticas.blocosQuebrados),
             LARGURA_TELA - 280, 48, 24, obterCorTexto());
}

void desenharTelaGameOver(const EstadoJogo *estado, bool vitoria) {
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Color{0, 0, 0, 190});
    DrawText(vitoria ? "Missao Cumprida!" : "Game Over", LARGURA_TELA / 2 - 180, 120, 40, vitoria ? GREEN : RED);

    DrawText(TextFormat("Pontuacao final: %d", estado->jogador.pontuacao), LARGURA_TELA / 2 - 200, 220, 28, obterCorTexto());
    DrawText(TextFormat("Tempo total: %02d:%02d", estado->jogador.tempo / 60, estado->jogador.tempo % 60),
             LARGURA_TELA / 2 - 200, 260, 24, obterCorTexto());
    DrawText(TextFormat("Blocos quebrados: %d", estado->estatisticas.blocosQuebrados),
             LARGURA_TELA / 2 - 200, 300, 24, obterCorTexto());
    DrawText(TextFormat("Itens coletados: %d", estado->estatisticas.itensColetados),
             LARGURA_TELA / 2 - 200, 340, 24, obterCorTexto());

    DrawText("ENTER - voltar ao menu", LARGURA_TELA / 2 - 150, ALTURA_TELA - 140, 22, LIGHTGRAY);
    DrawText("R - jogar novamente", LARGURA_TELA / 2 - 150, ALTURA_TELA - 100, 22, LIGHTGRAY);
}

void desenharTelaDificuldade(int selecionada) {
    ClearBackground(obterCorFundo());
    desenharBorda();
    DrawText("Selecione a dificuldade", LARGURA_TELA / 2 - 240, 80, 36, obterCorTitulo());

    const char *labels[] = {
        "1 - Facil (velocidade base)",
        "2 - Medio (+25% rapidez)",
        "3 - Dificil (+50% rapidez)"
    };

    for (int i = 0; i < 3; i++) {
        int dificuldade = i + 1;
        Color cor = (selecionada == dificuldade) ? YELLOW : obterCorTexto();
        int tamanho = (selecionada == dificuldade) ? 30 : 26;
        
        // Desenhar indicador visual
        if (selecionada == dificuldade) {
            DrawText(">", LARGURA_TELA / 2 - 260, 180 + i * 60, tamanho, YELLOW);
        }
        
        DrawText(labels[i], LARGURA_TELA / 2 - 220, 180 + i * 60, tamanho, cor);
    }

    DrawText("Use W/S ou SETAS para navegar", LARGURA_TELA / 2 - 180, ALTURA_TELA - 150, 20, LIGHTGRAY);
    DrawText("ENTER - confirmar | ESC - voltar", LARGURA_TELA / 2 - 200, ALTURA_TELA - 120, 22, LIGHTGRAY);
}
void inicializarSistemaParticulas(SistemaParticulas *sistema) {
    for (int i = 0; i < MAX_PARTICULAS; i++) {
        sistema->particulas[i].ativa = false;
        sistema->particulas[i].tempoVida = 0.0f;
    }
}

static unsigned char limitarCor(int valor) {
    if (valor < 0) return 0;
    if (valor > 255) return 255;
    return (unsigned char)valor;
}

void gerarExplosao(SistemaParticulas *sistema, Vector2 origem, Color corBase) {
    for (int i = 0; i < MAX_PARTICULAS; i++) {
        if (!sistema->particulas[i].ativa) {
            sistema->particulas[i].ativa = true;
            sistema->particulas[i].posicao = origem;
            float angulo = GetRandomValue(0, 360) * DEG2RAD;
            float velocidade = (float)GetRandomValue(40, 140);
            sistema->particulas[i].velocidade = {cosf(angulo) * velocidade, sinf(angulo) * velocidade};
            sistema->particulas[i].tempoVida = 0.0f;
            sistema->particulas[i].tempoMaximo = TEMPO_PARTICULA;
            sistema->particulas[i].cor = Color{limitarCor(corBase.r + GetRandomValue(-30, 30)),
                                              limitarCor(corBase.g + GetRandomValue(-30, 30)),
                                              limitarCor(corBase.b + GetRandomValue(-30, 30)),
                                              255};
        }
    }
}

void atualizarParticulas(SistemaParticulas *sistema, float dt) {
    for (int i = 0; i < MAX_PARTICULAS; i++) {
        if (!sistema->particulas[i].ativa) continue;
        sistema->particulas[i].tempoVida += dt;
        if (sistema->particulas[i].tempoVida >= sistema->particulas[i].tempoMaximo) {
            sistema->particulas[i].ativa = false;
            continue;
        }
        sistema->particulas[i].posicao.x += sistema->particulas[i].velocidade.x * dt;
        sistema->particulas[i].posicao.y += sistema->particulas[i].velocidade.y * dt;
        sistema->particulas[i].velocidade.y += 20.0f * dt;
    }
}

void desenharParticulas(const SistemaParticulas *sistema) {
    for (int i = 0; i < MAX_PARTICULAS; i++) {
        if (!sistema->particulas[i].ativa) continue;
        float progresso = sistema->particulas[i].tempoVida / sistema->particulas[i].tempoMaximo;
        unsigned char alpha = (unsigned char)(255 * (1.0f - progresso));
        Color cor = sistema->particulas[i].cor;
        cor.a = alpha;
        DrawCircleV(sistema->particulas[i].posicao, 2.0f, cor);
    }
}

bool capturarNomeMenu(char *nome, int tamanhoMaximo) {
    static int posicao = 0;
    static bool inicializado = false;
    
    if (!inicializado) {
        nome[0] = '\0';
        posicao = 0;
        inicializado = true;
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
        inicializado = false;
        return true;
    }

    return false;
}

void desenharItensAtivos(const EstadoJogo *estado) {
    for (int i = 0; i < estado->qtd_itens; i++) {
        if (estado->itens[i].esta_Ativo) {
            desenharItem((ItensEspeciais *)&estado->itens[i]);
        }
    }
}
