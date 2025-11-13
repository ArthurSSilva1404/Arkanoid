#include "raylib.h"

#include <cmath>
#include <iostream>

#include "Jogo.h"

  const int screenWidth = 800;
  const int screenHeight = 600;

void iniciarJogo(EstadoJogo &jogo){ 

  jogo.bola.posicao.x = 480;
  jogo.bola.posicao.y = 520;
  jogo.bola.radium = 8;
  jogo.bola.prendeu = true;
  jogo.bola.velocidadex = 200;
  jogo.bola.velocidadey = -250;

/*

  jogo.jogador.retangulo.posicao.x = 410;
  jogo.jogador.retangulo.posicao.y = 560;
  jogo.jogador.retangulo.largura = 160;
  jogo.jogador.retangulo.altura = 16;
  jogo.jogador.velocidade = 700;

  jogo.fase.blocos

*/

  jogo.niveis = 3;
  jogo.blocosRestantes = 0;
  jogo.vidas_jogador = 3;

}

void movimentarBola(){ 

}

void trocarNivel(){ 

}

int main() {
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("It works!", 20, 20, 20, BLACK);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
