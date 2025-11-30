
# Objetivos do Projeto
- Desenvolver um jogo **Arkanoid 2D** em **C++** utilizando a biblioteca **Raylib**.  
- Aplicar os principais conteúdos da disciplina: ponteiros, alocação dinâmica, structs, funções, modularização com `.h` e `.cpp`, manipulação de arquivos, recursividade e templates.  
- Criar um projeto organizado, modular, funcional e visualmente estÃ¡vel.  
- Implementar fases progressivas, nÃ­veis de dificuldade, itens especiais e sistema de pontuação com ranking salvo em arquivo.

# Bibliotecas Utilizadas
- **raylib**  
- **algorithm**  
- **cmath**  
- **ctime**  
- **cstring**  
- **cstdlib**  
- **cstdio**

# Controles do Jogo
| Açãoo | Tecla |
|------|-------|
| **Mover para a esquerda** | ** <-** ou **A** |
| **Mover para a direita** | ** -> ** ou **D** |
| **Lançar a bola / iniciar rodada** | **Espaaço** |
| **Pausar o jogo** | **P** |
| **Voltar ao menu** | **Esc** |

# Regras do Jogo

## Objetivo Geral  
Rebater a bola usando a barra (paddle) para destruir todos os blocos de cada fase, avançar progressivamente e obter a maior pontuaçãoo possível.

## Blocos  
- Distribuição de cores varia por fase.  
- Blocos podem exigir de **1 a 3 acertos** para quebrar.  
- Alguns blocos soltam **itens especiais**.

# Itens Especiais
| Símbolo | Item | Efeito |
|---------|------|--------|
| **+** | Vida extra | Adiciona uma vida ao jogador |
| **>** | Aumentar barra | Aumenta o tamanho da barra |
| **<** | Diminuir barra | Diminui o tamanho da barra |
| **P** | Pontos extras | Concede pontos bÃ´nus |

## Dificuldades  
- **Fácil**  
- **Médio** (+0.25 — na velocidade)  
- **Difícil** (+0.5 — na velocidade)

## PontuaÃ§Ã£o  
Pontuada com base em:
- Blocos destruídos  
- Itens coletados  
- Tempo de jogo  
- Dificuldade selecionada  

O ranking é salvo em arquivo incluindo **nome, pontuação, data e tempo**.

## Informações na Tela  
- Pontuação  
- Tempo  
- Vidas  
- Itens ativos  