#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "libs.h"

EstadoJogo estado;
int pontuacao;

void desenharMapa(mapa *m, ALLEGRO_BITMAP *imagemNave, ALLEGRO_BITMAP *imagemInimigo, nave *nave, ALLEGRO_FONT *font) {
    char textoVida[20]; // Tamanho deve ser suficiente para armazenar o valor convertido
    char textoPontuacao[40]; // Tamanho deve ser suficiente para armazenar o valor convertido

    sprintf(textoVida, "VIDA: %d", nave->vida);
    sprintf(textoPontuacao, "PONTUAÇÃO: %d", pontuacao);

    float larguraTextoVida = al_get_text_width(font, textoVida);
    float larguraTextoPontuacao = al_get_text_width(font, textoPontuacao);

    // Coordenadas x e y para o canto superior direito
    float x = LARGURA_TELA - larguraTextoVida;
    float y = 0;

    al_draw_text(font, al_map_rgb(255, 255, 255), x-10, y+1, 0, textoVida);
    al_draw_text(font, al_map_rgb(255, 255, 255), 1, 2, 0, textoPontuacao);

    for (int i = 0; i < m->maxX; i++) {
        for (int j = 0; j < m->maxY; j++) {
            int x = i * TAMANHO_BLOCO;
            int y = j * TAMANHO_BLOCO;

            switch (m->entidades[i][j].tipo) {
                case INIMIGO:
                    al_draw_scaled_bitmap(imagemInimigo, 10, 10, al_get_bitmap_width(imagemInimigo), al_get_bitmap_height(imagemInimigo),
                                          y-3, x-3, TAMANHO_BLOCO, TAMANHO_BLOCO, 0);
                    // al_draw_filled_rectangle(y+2.5, x+2.5, y + TAMANHO_BLOCO - 5, x + TAMANHO_BLOCO - 5, al_map_rgb(255, 0, 0)); // Quadrado vermelho para inimigo
                    break;
                case NAVE:
                    al_draw_scaled_bitmap(imagemNave, 0-2, 0, al_get_bitmap_width(imagemNave), al_get_bitmap_height(imagemNave),
                                          y, x, TAMANHO_BLOCO, TAMANHO_BLOCO, 0);
                    // al_draw_filled_triangle((y + TAMANHO_BLOCO / 2.0) - 2, x, y - 2,  x + TAMANHO_BLOCO,  y + TAMANHO_BLOCO, x + TAMANHO_BLOCO, al_map_rgb(0, 0, 255)); // Triângulo azul para a nave
                    break;
                case TIRO:
                    if (m->entidades[i][j].enemy) {
                        al_draw_filled_rectangle(y + 15, x + 2, y + TAMANHO_BLOCO - 10, x + TAMANHO_BLOCO - 16, al_map_rgb(255, 0, 0)); // Quadrado branco para tiro
                    } else {
                        al_draw_filled_rectangle(y + 15, x + 2, y + TAMANHO_BLOCO - 10, x + TAMANHO_BLOCO - 16, al_map_rgb(255, 255, 255)); // Quadrado branco para tiro
                    }

                    break;
                default:
                    break;
            }
        }
    }
}

void executarEvento(mapa *mapa, lista_tiro *listaTiros, nave *nave, lista_inimigo *listaInimigos){
    // Sua lógica de jogo aqui
    atualizarListaTiros(mapa, listaTiros, listaInimigos, nave);
    
    int num = rand() % 10 + 1;
    // 20% das vezes terá tiro
    if (num < 3) {
        inimigoAtira(mapa, listaTiros, nave, listaInimigos);
    }

    atualizarMapa(mapa, listaTiros, nave, listaInimigos);

}

int main() {
    srand(time(NULL));

    estado = MENU;
    pontuacao = 0;

    mapa *mapa = gerarMapa(MAX_X, MAX_Y);
    lista_tiro *listaTiros = criarListaDeTiros();
    lista_inimigo *listaInimigos = criarListaDeInimigos();

    // cria layout inicial INICIO
    for (int x = 1; x < 3; x++) {
        for (int y = 1; y < MAX_Y - 1; y++) {
            adicionarInimigo(mapa, x, y, listaInimigos);
        }   
    }
    nave *nave = adicionarNave(mapa, MAX_Y - 1, 9);
    // cria layout inicial FIM

    // Inicialização do Allegro
    al_init();             // Faz a preparação de requisitos da biblioteca Allegro
    al_install_keyboard(); // Habilita a entrada via teclado (eventos de teclado), no programa
    al_init_primitives_addon();
    al_init_image_addon();

    // CARREGA IMAGENS
    ALLEGRO_BITMAP *imagemNave = al_load_bitmap("imagens/nave.png");
    ALLEGRO_BITMAP *imagemFundo = al_load_bitmap("imagens/fundo3.jpg");
    ALLEGRO_BITMAP *imagemInimigo = al_load_bitmap("imagens/inimigo.png");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);   // Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); // Cria a fila de eventos; todos os eventos (programação orientada a eventos)
    ALLEGRO_FONT *font = al_create_builtin_font();        // Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
    ALLEGRO_DISPLAY *display = al_create_display(LARGURA_TELA, ALTURA_TELA);  // Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)
    ALLEGRO_BITMAP *buffer = al_create_bitmap(720, 720);

    al_register_event_source(queue, al_get_keyboard_event_source());    // Indica que eventos de teclado serão inseridos na nossa fila de eventos
    al_register_event_source(queue, al_get_display_event_source(display)); // Indica que eventos de tela serão inseridos na nossa fila de eventos
    al_register_event_source(queue, al_get_timer_event_source(timer));  // Indica que eventos de relógio serão inseridos na nossa fila de eventos

    ALLEGRO_EVENT event;   // Variável que guarda um evento capturado, sua estrutura é definida em: https://www.allegro.cc/manual/5/ALLEGRO_EVENT
    al_start_timer(timer); // Função que inicializa o relógio do programa
    while (estado != FIM) {                           // Atualiza a telas
        al_wait_for_event(queue, &event); // Função que captura eventos da fila, inserindo os mesmos na variável de eventos
        al_clear_to_color(al_map_rgb(255, 255, 255)); // Limpa o buffer

        if (event.type == 30) {                                           // O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
            al_set_target_bitmap(buffer);
            al_draw_bitmap(imagemFundo, 0, 0, 0);    // Desenha a imagem no buffer

            if (estado == MENU) {
                // Desenha o menu
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "Pressione ESPAÇO para iniciar");
            } else if (estado == GAME_OVER) {
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER!");
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 12, ALLEGRO_ALIGN_CENTER, "Precione ESPAÇO para reiniciar");
            } else if (estado == VENCEU) {
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2, ALLEGRO_ALIGN_CENTER, "VOCÊ GANHOU!");
                al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA_TELA / 2, ALTURA_TELA / 2 + 12, ALLEGRO_ALIGN_CENTER, "Precione ESPAÇO para iniciar uma NOVA rodada");
            } else if (estado == JOGO) {
                // imprimirMapa(mapa);
                desenharMapa(mapa, imagemNave, imagemInimigo, nave, font);
                executarEvento(mapa, listaTiros, nave, listaInimigos);

                al_set_target_backbuffer(display);
                al_draw_bitmap(buffer, 0, 0, 0); // Desenha o buffer na tela principal
                al_flip_display();               // Atualiza a tela
            }

            al_set_target_backbuffer(display);
            al_draw_bitmap(buffer, 0, 0, 0); // Desenha o buffer na tela principal
            al_flip_display();               // Atualiza a tela
        }
        else if (event.type == 42) {
            break; // Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (estado == MENU || estado == GAME_OVER || estado == VENCEU) {
                mapa = gerarMapa(MAX_X, MAX_Y);
                listaTiros = criarListaDeTiros();
                listaInimigos = criarListaDeInimigos();

                for (int x = 1; x < 3; x++)
                {
                    for (int y = 1; y < MAX_Y - 1; y++)
                    {
                        adicionarInimigo(mapa, x, y, listaInimigos);
                    }
                }
                nave = adicionarNave(mapa, MAX_Y - 1, 9);

                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    estado = JOGO;  // Muda para o estado de jogo quando a tecla de espaço é pressionada no menu
                }
            } else if (estado == JOGO) {
                // TECLAS CLICADAS
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    if (permiteNaveAtirar(listaTiros, nave)) {
                        adicionarTiroNave(mapa, listaTiros, nave);
                    }
                } else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                    moverNave(nave, 'd');
                } else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT){
                    moverNave(nave, 'e');
                }
            }
        }
    }

    al_destroy_font(font);         // Destrutor da fonte padrão
    al_destroy_display(display);      // Destrutor da tela
    al_destroy_timer(timer);       // Destrutor do relógio
    al_destroy_event_queue(queue); // Destrutor da fila
    
    // DESTRUTORES DE IMAGEM
    al_destroy_bitmap(imagemNave);
    al_destroy_bitmap(imagemFundo);
    al_destroy_bitmap(imagemInimigo);

    // destruirMapa(mapa);
    // destruirListaDeTiros(listaTiros);

    return 0;
}
