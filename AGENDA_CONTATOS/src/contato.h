#ifndef CONTATO_H
#define CONTATO_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "app_types.h" // Inclui as definições de tipo compartilhadas

// Definição das cores globais
extern const SDL_Color COR_VERMELHA;
extern const SDL_Color COR_ESCURA;
extern const SDL_Color COR_BRANCA;
extern const SDL_Color COR_CINZA;
extern const SDL_Color COR_DESTAQUE;

// Estrutura para armazenar os dados de um contato
typedef struct {
    char nome[100];
    char telefone[20];
    char email[100];
    char caminhoImagem[128];
    SDL_Texture *texturaPerfil;
    GrupoContato grupo; // Este tipo agora vem de app_types.h
} Contato;

// Estrutura para armazenar os ícones da interface do usuário
typedef struct {
    SDL_Texture *telefone;
    SDL_Texture *whatsapp;
    SDL_Texture *gmail;
    SDL_Texture *sms;
    SDL_Texture *facebook;
} UI_Icones;

// Declaração das Funções

// Funções de renderização da UI
void renderizarContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, Contato *contato, int y_offset, bool selecionado, UI_Icones *icones, int x, int w, int h);
void renderizarDetalheContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, TTF_Font *fonte_cabecalho, Contato *contato, UI_Icones *icones, int w, int h);
void renderizarFormulario(SDL_Renderer* r, TTF_Font* f_cabecalho, TTF_Font* f_form, FormularioContato* form, bool cursorVisivel, const char* msgErro, int w, int h);
void renderizarDialogoConfirmacao(SDL_Renderer* r, TTF_Font* f_cabecalho, TTF_Font* f_texto, const char* nomeContato, int w, int h);

// Funções utilitárias de renderização
SDL_Texture* carregarTextura(SDL_Renderer *renderizador, const char* caminho);
void renderizarTexto(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int x, int y);
void renderizarTextoCentralizado(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int w, int y);
void renderizarCirculoPreenchido(SDL_Renderer *renderizador, int centroX, int centroY, int raio);
void renderizarIconeOpcoes(SDL_Renderer *renderizador, int x, int y, int raioPonto, int espacamento);

#endif // CONTATO_H