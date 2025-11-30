
# Objetivos do Projeto (Resumo)
- Desenvolver um jogo **Arkanoid 2D** em **C++** utilizando a biblioteca **Raylib**.  
- Aplicar os principais conteÃºdos da disciplina: ponteiros, alocaÃ§Ã£o dinÃ¢mica, structs, funÃ§Ãµes, modularizaÃ§Ã£o com `.h` e `.cpp`, manipulaÃ§Ã£o de arquivos, recursividade e templates.  
- Criar um projeto organizado, modular, funcional e visualmente estÃ¡vel.  
- Implementar fases progressivas, nÃ­veis de dificuldade, itens especiais e sistema de pontuaÃ§Ã£o com ranking salvo em arquivo.

# Bibliotecas Utilizadas
- **raylib**  
- **algorithm**  
- **cmath**  
- **ctime**  
- **cstring**  
- **cstdlib**  
- **cstdio**

# Controles do Jogo
| AÃ§Ã£o | Tecla |
|------|-------|
| **Mover para a esquerda** | **â†** ou **A** |
| **Mover para a direita** | **â†’** ou **D** |
| **LanÃ§ar a bola / iniciar rodada** | **EspaÃ§o** |
| **Pausar o jogo** | **P** |
| **Voltar ao menu** | **Esc** |

# Regras do Jogo

## Objetivo Geral  
Rebater a bola usando a barra (paddle) para destruir todos os blocos de cada fase, avanÃ§ar progressivamente e obter a maior pontuaÃ§Ã£o possÃ­vel.

## Blocos  
- DistribuiÃ§Ã£o e cores variam por fase.  
- Blocos podem exigir de **1 a 3 acertos** para quebrar.  
- Alguns blocos soltam **itens especiais**.

# Itens Especiais
| SÃ­mbolo | Item | Efeito |
|---------|------|--------|
| **+** | Vida extra | Adiciona uma vida ao jogador |
| **>** | Aumentar barra | Aumenta o tamanho da barra |
| **<** | Diminuir barra | Diminui o tamanho da barra |
| **P** | Pontos extras | Concede pontos bÃ´nus |
| **!** | Inversor | Inverte temporariamente a direÃ§Ã£o do movimento da barra |
| **S** | Escudo | Bloqueia a perda de vida uma vez |

## Dificuldades  
- **FÃ¡cil**  
- **MÃ©dio** (+0.25Ã— na velocidade)  
- **DifÃ­cil** (+0.5Ã— na velocidade)

## PontuaÃ§Ã£o  
Pontuada com base em:
- Blocos destruÃ­dos  
- Itens coletados  
- Tempo de jogo  
- Dificuldade selecionada  

O ranking Ã© salvo em arquivo incluindo **nome, pontuaÃ§Ã£o, data e tempo**.

## InformaÃ§Ãµes na Tela  
- PontuaÃ§Ã£o  
- Tempo  
- Vidas  
- Itens ativos  