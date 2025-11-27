#include "Menu.h"
#include "Constantes.h"
#include "Cores.h"
#include "Graficos.h"
#include "raylib.h"

int menuIniciar() {
    static int opcaoSelecionada = MENU_OP_INICIAR;

    desenharFundo();
    desenharBorda();

    DrawText("Projeto M3 - Arkanoid", LARGURA_TELA / 2 - 220, 80, 40, obterCorTitulo());
    DrawText("Escolha uma opcao para continuar", LARGURA_TELA / 2 - 220, 140, 20, obterCorTexto());

    const char *opcoes[] = {
        "Iniciar Jogo",
        "Dificuldades",
        "Ranking",
        "Sair"
    };

    for (int i = 0; i < 4; i++) {
        Color cor = (i == opcaoSelecionada) ? YELLOW : obterCorTexto();
        int tamanho = (i == opcaoSelecionada) ? 28 : 24;
        DrawText(opcoes[i], LARGURA_TELA / 2 - 120, 220 + i * 60, tamanho, cor);
    }

    DrawText("Use W/S ou as setas para navegar | ENTER para confirmar", LARGURA_TELA / 2 - 280, ALTURA_TELA - 80, 18, LIGHTGRAY);

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        opcaoSelecionada = (opcaoSelecionada + 1) % 4;
    } else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        opcaoSelecionada = (opcaoSelecionada - 1 + 4) % 4;
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        return opcaoSelecionada;
    }

    return -1;
}
