#ifndef __LIBS__
#define __LIBS__

#include <stdio.h>
#include <stdlib.h>

#define LARGURA_TELA 600
#define ALTURA_TELA 600

#define MAX_X 20
#define MAX_Y 19

#define TAMANHO_BLOCO 31
#define FPS 12
typedef enum {
    MENU,
    JOGO,
    FIM,
    GAME_OVER,
    VENCEU
} EstadoJogo;

extern EstadoJogo estado;

extern int pontuacao;

// inimigo ------------------------
typedef struct inimigo inimigo;
struct inimigo {
    int x;
    int y;
    int nivel;
    inimigo *prox;
};

typedef struct lista_inimigo {
    inimigo *primeiro;
    inimigo *ultimo;
} lista_inimigo;
// inimigo ------------------------

// mapa ---------------------------
typedef struct {
	void *objeto;
	enum {INIMIGO = 1, OBSTACULO, NAVE, TIRO} tipo;
    int enemy;
} entidade;

typedef struct {
	entidade **entidades;
	int maxX;
	int maxY;
} mapa;
// mapa ---------------------------

// nave ---------------------------
typedef struct nave {
    int x;
    int y;
    int vida;
} nave;
// nave ---------------------------

// tiro ---------------------------
typedef struct tiro tiro;
struct tiro {
    int x;
    int y;
    int trajetoria;
    tiro *prox;
};

typedef struct lista_tiro {
    tiro *primeiro;
    tiro *ultimo;
} lista_tiro;
// tiro ---------------------------

// TIRO ---------------------------
lista_tiro* criarListaDeTiros(void);

tiro* removeTiro(tiro* atual, tiro* anterior, lista_tiro *lista);

void limpaTiros(lista_tiro *lista);

tiro* adicionarTiroInimigo(mapa *mapa, lista_tiro *lista, inimigo *atirador);

lista_tiro *atualizarListaTiros(mapa *mapa, lista_tiro *listaTiro, lista_inimigo *listaInimigo, nave *nave);

void *inimigoAtira(mapa *mapa, lista_tiro *listaTiro, nave *nave, lista_inimigo *listaInimigo);

void imprimirTiros(lista_tiro *lista);

int permiteNaveAtirar(lista_tiro *lista, nave *nave);
// TIRO ---------------------------

// INIMIGO ------------------------
lista_inimigo* criarListaDeInimigos();

inimigo *adicionarInimigo(mapa *mapa, int x, int y, lista_inimigo *lista);

inimigo* removerInimigoAux(inimigo* atual, inimigo* anterior, lista_inimigo *lista);

int removerInimigo(lista_inimigo *lista, int x, int y);

void imprimirInimigos(lista_inimigo *lista);
// INIMIGO ------------------------

// MAPA ------------------------
mapa* gerarMapa(int maxX, int maxY);

void imprimirMapa(mapa *m);

mapa *atualizarMapa(mapa *mapa, lista_tiro *listaTiros, nave *nave, lista_inimigo *listaInimigos);

void destruirMapa(mapa *mapa);
// MAPA ------------------------

// NAVE ------------------------
nave *adicionarNave(mapa *mapa, int x, int y);

tiro* adicionarTiroNave(mapa *mapa, lista_tiro *lista, nave *atirador);

int removerNave(mapa *mapa, int x, int y);

void moverNave(nave *n, char direcao);
// NAVE ------------------------

#endif
