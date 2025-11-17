#include "raylib.h"

#include <cmath>
#include <iostream>

#include "Jogo.h"

  const int screenWidth = 800;

  void movimentarBola(){ 

  jogo.bola.posicao.x = 0;
  jogo.bola.posicao.y = 0;
  jogo.bola.radium = 8;
  jogo.bola.prendeu = true;
  jogo.bola.velocidadex = 200;
  jogo.bola.velocidadey = -250;

  }

  jogo.barra.retangulo.posicao.x = 410;
  jogo.barra.retangulo.posicao.y = 560;
  jogo.barra.retangulo.largura = 160;
  jogo.barra.retangulo.altura = 16;
  jogo.barra.velocidade = 700;

  jogo.dificuldade = 3;
  jogo.blocosRestantes = 0;
  jogo.fase_atual = 3;



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
