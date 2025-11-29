# 🎮 Arkanoid 2D - Projeto M3

## 📋 Objetivos do Projeto

Desenvolver um jogo **Arkanoid 2D** completo em **C++** utilizando a biblioteca **Raylib**, aplicando de forma integrada todos os conceitos fundamentais da disciplina de Algoritmos e Programação II:

- ✅ **Alocação dinâmica de memória**
- ✅ **Ponteiros**
- ✅ **Funções e sobrecargas**
- ✅ **Structs**
- ✅ **Passagem de parâmetros** (valor e referência)
- ✅ **Módulos com .h e .cpp**
- ✅ **Manipulação de arquivos**
- ✅ **Recursividade**
- ✅ **Templates de função**
- ✅ **Escopo de variáveis**

O projeto é **modular**, **funcional** e **visualmente estável**, implementando fases progressivas, níveis de dificuldade, itens especiais e sistema de pontuação com ranking salvo em arquivo.

---

## 📚 Bibliotecas Utilizadas

- **raylib** - Biblioteca gráfica principal
- **algorithm** - Algoritmos STL
- **cmath** - Funções matemáticas
- **ctime** - Manipulação de tempo
- **cstring** - Manipulação de strings
- **cstdlib** - Funções padrão
- **cstdio** - Entrada/saída

---

## 🎮 Controles do Jogo

| Ação | Tecla |
|------|-------|
| Mover para a esquerda | **←** ou **A** |
| Mover para a direita | **→** ou **D** |
| Lançar a bola | **Espaço** ou **↑** |
| Pausar o jogo | **P** |
| Voltar ao menu | **ESC** |
| Selecionar dificuldade | **1**, **2**, **3** |
| Reiniciar (Game Over) | **R** |
| Confirmar | **ENTER** |

---

## 📖 Regras do Jogo

### 🎯 Objetivo Geral
Rebater a bola usando a **barra (paddle)** para destruir todos os blocos de cada fase, avançar progressivamente pelas **3 fases** e obter a **maior pontuação possível**.

### 🧱 Blocos
- **Distribuição e cores** variam por fase
- Blocos podem exigir de **1 a 3 acertos** para quebrar:
  - **Cristal**: 1 vida
  - **Cerâmica**: 2 vidas
  - **Obsidiana**: 3 vidas
  - **Metal**: 2 vidas
  - **Titânio**: 3 vidas
- Blocos podem soltar **itens especiais** ao serem destruídos

### 🎁 Itens Especiais (6 tipos)

| Símbolo | Item | Efeito | Duração |
|---------|------|--------|---------|
| **+** | Vida Extra | Adiciona 1 vida + 250 pontos | Permanente |
| **>** | Aumentar Barra | Aumenta o tamanho da barra (+24px) | Permanente |
| **<** | Diminuir Barra | Diminui o tamanho da barra (-24px) | Permanente |
| **P** | Pontos Extras | Concede +500 pontos bônus | Instantâneo |
| **x2** | Multiplicador | Dobra os pontos dos blocos | 10 segundos |
| **!** | Inversor | Inverte pontos dos blocos (negativos) | 5 segundos |

### 🎚️ Níveis de Dificuldade

1. **Fácil** - Velocidade base (1.0x)
2. **Médio** - Velocidade aumentada (+25%)
3. **Difícil** - Velocidade aumentada (+50%)

### 📊 Sistema de Pontuação

A pontuação é calculada com base em:
- **Blocos destruídos** × 120
- **Itens coletados** × 80
- **Bônus de tempo** (máximo 1800 pontos)
- **Vidas restantes** × 250
- **Multiplicador de dificuldade**
- **Multiplicador de item** (x2 se ativo)
- **Inversor** (pontos negativos se ativo)

**Fórmula:**
```
pontos_bloco = pontosBase × dificuldade × multiplicador
if (inversor_ativo) pontos_bloco = -pontos_bloco
```

### 🏆 Ranking
- Salvo automaticamente em `ranking.txt`
- Exibe **top 10 jogadores**
- Ordenado por **maior pontuação**
- Inclui: **nome**, **pontuação**, **data** e **tempo**
- Salva mesmo em **derrota**

---

## 📺 Informações na Tela (HUD)

Durante o jogo, são exibidas:
- **Pontuação** atual
- **Tempo** de jogo (MM:SS)
- **Vidas** restantes
- **Itens** coletados
- **Último item** coletado (2 segundos)
- **Multiplicador ativo** (com contador)
- **Inversor ativo** (com contador)
- **Fase** atual e nome
- **Dificuldade** selecionada
- **Blocos** quebrados

---

## 🎨 Fases

O jogo possui **3 fases progressivas**, cada uma com:
- **Mapa único** de blocos
- **Tema visual** diferente
- **Cores** e **fundos** exclusivos
- **Dificuldade crescente**

### Fases Disponíveis:
1. **Nebulosa Coral** - Tema azul/coral
2. **Caverna Prismática** - Tema roxo/rosa
3. **Templo Aurora** - Tema laranja/dourado

---

## 🔧 Compilação e Execução

### Requisitos:
- **g++** com suporte a C++17
- **Raylib** instalado
- **OpenGL**

### Comandos:
```bash
cd Arkanoid
make clean
make
./arkanoid_m3
```

---

## 🏗️ Estrutura do Projeto

```
Arkanoid/
├── Cabeçalho/          # Arquivos .h (16 arquivos)
│   ├── Barra.h
│   ├── Bloco.h
│   ├── Bola.h
│   ├── Constantes.h
│   ├── Cores.h
│   ├── Estado.h
│   ├── Estruturas.h
│   ├── Fases.h
│   ├── Graficos.h
│   ├── Itens.h
│   ├── Jogador.h
│   ├── Jogo.h
│   ├── Menu.h
│   ├── Ranking.h
│   ├── Sons.h
│   └── Utils.h
├── Logica/             # Arquivos .cpp (15 arquivos)
│   ├── Barra.cpp
│   ├── Bloco.cpp
│   ├── Bola.cpp
│   ├── Cores.cpp
│   ├── Estado.cpp
│   ├── Fases.cpp
│   ├── Graficos.cpp
│   ├── Itens.cpp
│   ├── Jogador.cpp
│   ├── Jogo.cpp
│   ├── Main.cpp
│   ├── Menu.cpp
│   ├── Ranking.cpp
│   ├── Sons.cpp
│   └── Util.cpp
├── Makefile
└── Readme.md
```

---

## 💡 Conceitos Aplicados

### 1. **Alocação Dinâmica**
```cpp
fase->blocos = (Bloco *)malloc(total_blocos * sizeof(Bloco));
free(estado->fase.blocos);
```

### 2. **Ponteiros**
```cpp
void processarColisoes(EstadoJogo *estado);
Bloco *bloco = &estado->fase.blocos[i];
```

### 3. **Templates**
```cpp
template<typename T>
T maximo(T a, T b);

template<typename T>
void trocar(T *a, T *b);
```

### 4. **Recursividade**
```cpp
int somarRecursivo(int n);
int contarBlocosRecursivo(Bloco *blocos, int inicio, int fim);
void ordenarPontosRecursivo(int *pontos, int inicio, int fim);
```

### 5. **Manipulação de Arquivos**
```cpp
FILE *arquivo = fopen(ARQUIVO_RANKING, "r");
fprintf(arquivo, "%s %d %d %s\n", ...);
```

### 6. **Structs**
```cpp
struct Bloco, Bola, Barra, Fase, EstadoJogo, Jogador, etc.
```

---

## 🎯 Funcionalidades Extras

- ✨ **Sistema de partículas** - Explosões visuais ao quebrar blocos
- 🎨 **Temas por fase** - Cores e fundos únicos
- 📊 **HUD completo** - Feedback visual de todos os efeitos
- ⚡ **Física precisa** - Normalização de velocidade e colisões
- 🎮 **6 tipos de itens** - Todos funcionais com feedback
- 🔄 **Efeitos temporários** - Multiplicador e inversor com contadores

---

## 📝 Status do Projeto

```
✅ Todos os requisitos do PROJETO.MD implementados
✅ Todos os conceitos da disciplina aplicados
✅ Código modular e organizado
✅ Jogo funcional e jogável
✅ Física corrigida e fluida
✅ 6 tipos de itens funcionando
✅ Recursividade e templates implementados
✅ Sistema de ranking funcional
✅ Compilação sem erros
✅ Pronto para apresentação
```

---

## 👥 Desenvolvido para

**Disciplina:** Algoritmos e Programação II  
**Instituição:** UNIVALI  
**Linguagem:** C++17  
**Biblioteca:** Raylib  

---

## 📄 Licença

Projeto acadêmico desenvolvido para fins educacionais.
