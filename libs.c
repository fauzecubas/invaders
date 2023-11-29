#include <stdio.h>
#include <stdlib.h>
#include "libs.h"

lista_tiro* criarListaDeTiros(){
	lista_tiro *lista = (lista_tiro*) malloc (sizeof(lista_tiro));
	if (lista == NULL) return NULL;
	lista->primeiro = NULL;
	lista->ultimo  = NULL;
	return lista;
}

lista_inimigo* criarListaDeInimigos() {
	lista_inimigo *lista = (lista_inimigo*) malloc (sizeof(lista_inimigo));
	if (lista == NULL) return NULL;
	lista->primeiro = NULL;
	lista->ultimo  = NULL;
	return lista;
}

tiro* removeTiro(tiro* atual, tiro* anterior, lista_tiro *lista){
    //	Remove os tiros da lista
    // é o primeiro da lista
    if (anterior == NULL) {
        lista->primeiro = atual->prox;
        if (atual) {
            free(atual);
            return NULL;
        }
    } 
    // é o último da lista
    else if (atual->prox == NULL) {
        if (anterior != NULL) {
            lista->ultimo = anterior;
            lista->ultimo->prox = NULL;
        }
        if (atual) {
            free(atual);
            return NULL;
        }
    }
    // é um elemento do meio
    else {
        anterior->prox = atual->prox;
        if (atual) {
            free(atual);
            return NULL;
        }
    }
    
    return atual;
}

void limpaTiros(lista_tiro *lista){
	tiro *atual = (tiro*) lista->primeiro;
	tiro *anterior = NULL;
	while (atual != NULL)
		atual = removeTiro(atual, anterior, lista);
}

tiro* adicionarTiroInimigo(mapa *mapa, lista_tiro *lista, inimigo *atirador) {
    //  Adiciona um novo tiro à lista. // verificar regras
    tiro *atual = malloc(sizeof(tiro));
	tiro *anterior = malloc(sizeof(tiro));

	if (!lista->primeiro) {
		lista->primeiro = malloc(sizeof(tiro));
		lista->ultimo = malloc(sizeof(tiro));

		lista->primeiro->x = atirador->x + 1;
		lista->primeiro->y = atirador->y;
		lista->primeiro->trajetoria = 1;
		lista->primeiro->prox = NULL;

		lista->ultimo->x = atirador->x + 1;
		lista->ultimo->y = atirador->y;
        lista->ultimo->trajetoria = 1;
        lista->ultimo->prox = NULL;

        mapa->entidades[atirador->x+1][atirador->y].objeto = (tiro*)atirador;
        mapa->entidades[atirador->x+1][atirador->y].tipo = TIRO;

		return lista->primeiro;
	}

	atual = lista->primeiro;
	anterior = NULL;

	while (atual != NULL) {
		if (atual->prox == NULL) {
			break;
		}

		anterior = atual;
		atual = atual->prox;

		if (atual->y >= mapa->maxY) break;
	}

	tiro* prox = malloc(sizeof(tiro));

	prox->x = atirador->x + 1;
	prox->y = atirador->y;
    prox->trajetoria = 1;
    prox->prox = NULL;

	atual->prox = prox;
	lista->ultimo = prox;

    mapa->entidades[atirador->x+1][atirador->y].objeto = (tiro*)atirador;
    mapa->entidades[atirador->x+1][atirador->y].tipo = TIRO;

	return prox;
}

inimigo* adicionarInimigo(mapa *mapa, int x, int y, lista_inimigo* lista) {
    //  Adiciona um novo tiro à lista. // verificar regras
    inimigo *atual = malloc(sizeof(inimigo));
	inimigo *anterior = malloc(sizeof(inimigo));

    // int nivel = rand() % 3 + 1;
    int nivel = 1;

	if (!lista->primeiro) {
		lista->primeiro = malloc(sizeof(inimigo));
		lista->ultimo = malloc(sizeof(inimigo));

		lista->primeiro->x = x;
		lista->primeiro->y = y;
		lista->primeiro->nivel = nivel;
		lista->primeiro->prox = NULL;

		lista->ultimo->x = x;
		lista->ultimo->y = y;
        lista->ultimo->nivel = nivel;
        lista->ultimo->prox = NULL;

        mapa->entidades[x][y].tipo = INIMIGO;

		return lista->primeiro;
	}

	atual = lista->primeiro;
	anterior = NULL;

	while (atual != NULL) {
		if (atual->prox == NULL) {
			break;
		}

		anterior = atual;
		atual = atual->prox;
	}

	inimigo* prox = malloc(sizeof(inimigo));

	prox->x = x;
	prox->y = y;
    prox->nivel = nivel;
    prox->prox = NULL;

	atual->prox = prox;
	lista->ultimo = prox;

    mapa->entidades[x][y].tipo = INIMIGO;

	return prox;
}

inimigo* removerInimigoAux(inimigo* atual, inimigo* anterior, lista_inimigo *lista) {
    if (atual->nivel == 1) pontuacao = pontuacao + 10;
    else if (atual->nivel == 2) pontuacao = pontuacao + 20;
    else if (atual->nivel == 3) pontuacao = pontuacao + 40;

    // é o primeiro da lista
    if (anterior == NULL) {
        lista->primeiro = atual->prox;
        if (atual) {
            free(atual);
            return NULL;
        }
    } 
    // é o último da lista
    else if (atual->prox == NULL) {
        lista->ultimo = anterior;
        lista->ultimo->prox = NULL;
        if (atual) {
            free(atual);

            return NULL;
        }
    }
    // é um elemento do meio
    else {
        anterior->prox = atual->prox;
        if (atual) {
            free(atual);
            return NULL;
        }
    }
    
    return atual;
}

int contarInimigos(lista_inimigo *lista) {
	//IMPLEMENTAR!
	inimigo *atual = malloc(sizeof(inimigo));
    inimigo *anterior = malloc(sizeof(inimigo));

    int n = 0;

    if (lista->primeiro) {
        atual = lista->primeiro;
        anterior = NULL;
        n++;

        while (atual != NULL) {
            anterior = atual;
            atual = atual->prox;
            n++;
        }
    }

    return n;
}

int removerInimigo(lista_inimigo *lista, int x, int y) {
	//IMPLEMENTAR!
	inimigo *atual = malloc(sizeof(inimigo));
    inimigo *anterior = malloc(sizeof(inimigo));

    if (lista->primeiro) {
        atual = lista->primeiro;
        anterior = NULL;

        while (atual != NULL) {
            if (atual->x == x && atual->y == y) {
                break;
            }

            anterior = atual;
            atual = atual->prox;
        }
        

        removerInimigoAux(atual, anterior, lista);
    }

    if (contarInimigos(lista) == 0) estado = VENCEU;

    return 1;
}

mapa* gerarMapa(int maxX, int maxY) {
    mapa *novoMapa = malloc(sizeof(mapa)); // Aloca memória para a estrutura do mapa
    if (novoMapa == NULL) {
        // Tratar erro de alocação de memória
        return NULL;
    }
    
    novoMapa->maxX = maxX; // Define o valor de maxX no mapa
    novoMapa->maxY = maxY; // Define o valor de maxY no mapa
    
    // Aloca memória para o array de ponteiros para entidade (linhas do mapa)
    novoMapa->entidades = malloc(maxX * sizeof(entidade*));
    if (novoMapa->entidades == NULL) {
        free(novoMapa); // Libera a memória alocada anteriormente para a estrutura do mapa
        return NULL;
    }
    
    // Aloca memória para cada linha do mapa (array de entidade)
    for (int i = 0; i < maxX; ++i) {
        novoMapa->entidades[i] = malloc(maxY * sizeof(entidade));
        if (novoMapa->entidades[i] == NULL) { // Trata erro de alocação
            for (int j = 0; j < i; ++j) {
                free(novoMapa->entidades[j]);
            }
            free(novoMapa->entidades);
            free(novoMapa);
            return NULL;
        }
    }
    
    return novoMapa; // Retorna o ponteiro para o novo mapa alocado
} 

void destruirMapa(mapa* mapa) {
	for (int i = 0; i < mapa->maxY; ++i) {
        free(mapa->entidades[i]);
    }
    free(mapa->entidades);
    free(mapa);
}

void imprimirMapa(mapa *m) {
    for (int x = 0; x < m->maxX; x++) {
        for (int y = 0; y < m->maxY; y++) {
            switch (m->entidades[x][y].tipo) {
                case INIMIGO:
                    printf("I ");
                    break;
                case OBSTACULO:
                    printf("O ");
                    break;
                case NAVE:
                    printf("N ");
                    break;
                case TIRO:
                    printf("T ");
                    break;
                default:
                    printf(". "); // Espaço em branco para células vazias
            }
        }
        printf("\n");
    }

    for (int i = 0; i < m->maxY * 2 - 1; i++) {
        printf("_");
    }
    printf("\n");

    return;
}

mapa *atualizarMapa(mapa *mapa, lista_tiro *listaTiros, nave *nave, lista_inimigo *listaInimigos) {
    for (int x = 0; x < mapa->maxX; x++) {
        for (int y = 0; y < mapa->maxY; y++) {
            if (mapa->entidades[x][y].tipo == 4) {
                mapa->entidades[x][y].tipo = 0;
                mapa->entidades[x][y].objeto = NULL;
            } else if (mapa->entidades[x][y].tipo == 3) {
                mapa->entidades[x][y].tipo = 0;
                mapa->entidades[x][y].objeto = NULL;
            } else if (mapa->entidades[x][y].tipo == 1) {
                mapa->entidades[x][y].tipo = 0;
                mapa->entidades[x][y].objeto = NULL;
            }
        }
    }
    
    adicionarNave(mapa, nave->x, nave->y);
    
    tiro *tiroAtual = malloc(sizeof(tiro));
    tiro *tiroAnterior = malloc(sizeof(tiro));

    if (listaTiros->primeiro) {
        tiroAtual = listaTiros->primeiro;
        tiroAnterior = NULL;

        while (tiroAtual != NULL) {
            if (tiroAtual->x <= 0 || tiroAtual->x >= MAX_X) {
                removeTiro(tiroAtual, tiroAnterior, listaTiros);
                break;
            }

            imprimirTiros(listaTiros);
            printf("%d %d\n", tiroAtual->x, tiroAtual->y);
            mapa->entidades[tiroAtual->x][tiroAtual->y].tipo = 4;
            if (tiroAtual->trajetoria == 1)  mapa->entidades[tiroAtual->x][tiroAtual->y].enemy = 1;
            else mapa->entidades[tiroAtual->x][tiroAtual->y].enemy = 0;

            if (tiroAtual->prox == NULL) {
                break;
            }

            tiroAnterior = tiroAtual;
            tiroAtual = tiroAtual->prox;
        }
    }

    inimigo *inimigoAtual = malloc(sizeof(inimigo));
    inimigo *inimigoAnterior = malloc(sizeof(inimigo));

    if (listaInimigos->primeiro) {
        inimigoAtual = listaInimigos->primeiro;
        inimigoAnterior = NULL;

        while (inimigoAtual != NULL) {
            mapa->entidades[inimigoAtual->x][inimigoAtual->y].tipo = 1;

            if (inimigoAtual->prox == NULL) {
                break;
            }

            inimigoAnterior = inimigoAtual;
            inimigoAtual = inimigoAtual->prox;
        }
    }

    return mapa;
}

nave *adicionarNave(mapa *mapa, int x, int y) {
    mapa->entidades[x][y].tipo = 3;

    nave *navePtr = malloc(sizeof(nave));
    navePtr->x = x;
    navePtr->y = y;
    navePtr->vida = 3;

    mapa->entidades[x][y].objeto = (nave *)navePtr;

    return navePtr;
}

tiro* adicionarTiroNave(mapa *mapa, lista_tiro *lista, nave *atirador){
    //  Adiciona um novo tiro à lista. // verificar regras
    tiro *atual = malloc(sizeof(tiro));
	tiro *anterior = malloc(sizeof(tiro));

	if (!lista->primeiro) {
		lista->primeiro = malloc(sizeof(tiro));
		lista->ultimo = malloc(sizeof(tiro));

		lista->primeiro->x = atirador->x - 1;
		lista->primeiro->y = atirador->y;
		lista->primeiro->trajetoria = -1;
		lista->primeiro->prox = NULL;

		lista->ultimo->x = atirador->x - 1;
		lista->ultimo->y = atirador->y;
        lista->ultimo->trajetoria = -1;
        lista->ultimo->prox = NULL;

        // mapa->entidades[atirador->x-1][atirador->y].objeto = (nave*)atirador;
        mapa->entidades[atirador->x-1][atirador->y].tipo = TIRO;

		return lista->primeiro;
	}

	atual = lista->primeiro;
	anterior = NULL;

	while (atual != NULL) {
		if (atual->prox == NULL) {
			break;
		}

		anterior = atual;
		atual = atual->prox;

		if (atual->x <= 0 || atual->x > MAX_X) {
            removeTiro(atual, anterior, lista);
            break;
        }
	}

	tiro* prox = malloc(sizeof(tiro));

	prox->x = atirador->x - 1;
	prox->y = atirador->y;
    prox->trajetoria = -1;
    prox->prox = NULL;

	atual->prox = prox;
	lista->ultimo = prox;

    // mapa->entidades[atirador->x-1][atirador->y].objeto = (nave*)atirador;
    mapa->entidades[atirador->x-1][atirador->y].tipo = TIRO;

	return prox;
}

int removerNave(mapa *mapa, int x, int y) {

}

void moverNave(nave *n, char direcao) {
    if (direcao == 'd') {
        if (n->y < MAX_Y - 1) n->y = n->y + 1;
    } else if (direcao == 'e') {
        if (n->y > 0) n->y = n->y - 1;
    }
}

int permiteInimigoAtirar(inimigo *inimigo, lista_tiro *listaTiro) {
    int permite = 1;
    tiro *atual = listaTiro->primeiro;

    while (atual != NULL) {
        // inimigo fraco ou intermediario
        if (inimigo->nivel == 1 || inimigo->nivel == 2) {
            if (atual->y == inimigo->y) {
                permite = 0;
                break;
            }
        }
        // inimigo forte
        else if (inimigo->nivel == 3) {
            if (atual->y == inimigo->y && atual->trajetoria == -1) {
                permite = 0;
                break;
            }
        }

        atual = atual->prox;
    }

    return permite;
}

void *inimigoAtira(mapa *mapa, lista_tiro *listaTiro, nave *nave, lista_inimigo *listaInimigo) {
    for (int range = 0; range < MAX_Y; range++) {
        inimigo *atual = malloc(sizeof(inimigo));
        inimigo *anterior = malloc(sizeof(inimigo));

        if (listaInimigo->primeiro) {
            atual = listaInimigo->primeiro;
            anterior = NULL;

            while (atual != NULL) {
                if (abs(atual->y - nave->y) == range) {
                    if (permiteInimigoAtirar(atual, listaTiro)) {
                        adicionarTiroInimigo(mapa, listaTiro, atual);
                        return NULL;
                    };
                }

                anterior = atual;
                atual = atual->prox;
            }
        }
    }
}

lista_tiro *atualizarListaTiros(mapa *mapa, lista_tiro *listaTiro, lista_inimigo *listaInimigo, nave *nave) {
    tiro *atual = malloc(sizeof(tiro));
    tiro *anterior = malloc(sizeof(tiro));

    if (listaTiro->primeiro) {
        atual = listaTiro->primeiro;
        anterior = NULL;

        while (atual != NULL) {

            if (atual->prox == NULL) {
                break;
            }

            atual->x = atual->x + atual->trajetoria;

            if (atual->x <= 0 || atual->x >= MAX_X ) {
                removeTiro(atual, anterior, listaTiro);
            } else if (mapa->entidades[atual->x][atual->y].tipo == 1) {
                // atingiu inimigo
                removerInimigo(listaInimigo, atual->x, atual->y);
                removeTiro(atual, anterior, listaTiro);
            } else if (mapa->entidades[atual->x][atual->y].tipo == 3) {
                // atingiu nave
                nave->vida = nave->vida - 1;
                removeTiro(atual, anterior, listaTiro);
                if (nave->vida <= 0) {
                    estado = GAME_OVER;
                }
            }

            anterior = atual;
            atual = atual->prox;
        }

        atual->x = atual->x + atual->trajetoria;
        if (atual->x <= 0 || atual->x >= MAX_X) {
            removeTiro(atual, anterior, listaTiro);
        } else if (mapa->entidades[atual->x][atual->y].tipo == 1) {
            // atingiu inimigo
            removerInimigo(listaInimigo, atual->x, atual->y);
            removeTiro(atual, anterior, listaTiro);
        } else if (mapa->entidades[atual->x][atual->y].tipo == 3) {
            nave->vida = nave->vida - 1;
            removeTiro(atual, anterior, listaTiro);
            if (nave->vida <= 0) {
                estado = GAME_OVER;
            }
        }


        return listaTiro;
    }

    return listaTiro;
}

void imprimirInimigos(lista_inimigo *lista) {
    printf("------ IMPRIME LISTA INI ------\n");
    inimigo *atual = malloc(sizeof(inimigo));
    inimigo *anterior = malloc(sizeof(inimigo));

    if (lista->primeiro) {
        atual = lista->primeiro;
        anterior = NULL;

        while (atual != NULL) {
            printf("inimigo: %d, %d\n", atual->x, atual->y);
            if (atual->prox == NULL) {
                break;
            }
            anterior = atual;
            atual = atual->prox;
        }
    }
    printf("------ IMPRIME LISTA FIM ------\n");
}

void imprimirTiros(lista_tiro *lista) {
    printf("------ IMPRIME LISTA INI ------\n");
    tiro *atual = malloc(sizeof(tiro));
    tiro *anterior = malloc(sizeof(tiro));

    if (lista->primeiro) {
        atual = lista->primeiro;
        anterior = NULL;

        while (atual != NULL) {
            printf("tiro: %d, %d\n", atual->x, atual->y);
            if (atual->prox == NULL) {
                break;
            }
            anterior = atual;
            atual = atual->prox;
        }
    }
    printf("------ IMPRIME LISTA FIM ------\n");
}

int permiteNaveAtirar(lista_tiro *lista, nave *nave) {
    int permite = 1;
    tiro *atual = lista->primeiro;

    while (atual != NULL) {
        if (atual->y == nave->y) {
            permite = 0;
            break;
        }
        atual = atual->prox;
    }

    return permite;
}