#ifndef CONTATO_H
#define CONTATO_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

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
} Contato;

// Estrutura para armazenar os ícones da interface do usuário
typedef struct {
    SDL_Texture *telefone;
    SDL_Texture *whatsapp;
    SDL_Texture *gmail;
    SDL_Texture *sms;
    SDL_Texture *facebook;
} UI_Icones;

// Enum para gerenciar os estados da aplicação
typedef enum {
    ESTADO_LISTA,
    ESTADO_DETALHE
} EstadoAplicacao;

// |________________________|
// | Declaração das Funções |
// |------------------------|


// Função para renderizar um contato na lista
void renderizarContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, Contato *contato, int y_offset, bool selecionado, UI_Icones *icones);

// Função para renderizar a tela de detalhes do contato
void renderizarDetalheContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, TTF_Font *fonte_cabecalho, Contato *contato, UI_Icones *icones);

// Função para carregar uma textura a partir de um caminho da imagem
SDL_Texture* carregarTextura(SDL_Renderer *renderizador, const char* caminho);

// Função para renderizar texto na tela
void renderizarTexto(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int x, int y);

// Função para renderizar texto centralizado na tela
void renderizarTextoCentralizado(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int y);

// Função para renderizar um círculo preenchido
void renderizarCirculoPreenchido(SDL_Renderer *renderizador, int centroX, int centroY, int raio);

// Função para renderizar um ícone de opções
void renderizarIconeOpcoes(SDL_Renderer *renderizador, int x, int y, int raioPonto, int espacamento);

#endif // CONTATO_H