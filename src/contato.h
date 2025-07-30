#ifndef CONTATO_H
#define CONTATO_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "app_types.h" 

// Paleta de cores
extern const SDL_Color COR_FUNDO;
extern const SDL_Color COR_CARTAO;
extern const SDL_Color COR_SOMBRA;
extern const SDL_Color COR_INPUT_FUNDO;
extern const SDL_Color COR_GRADIENTE_INICIO;
extern const SDL_Color COR_GRADIENTE_FIM;
extern const SDL_Color COR_DESTAQUE_BORDA;
extern const SDL_Color COR_TEXTO_PRIMARIO;
extern const SDL_Color COR_TEXTO_SECUNDARIO;
extern const SDL_Color COR_ERRO;

// Estrutura para armazenar os dados de um contato
typedef struct {
    char nome[100];
    char telefone[20];
    char email[100];
    char caminhoImagem[128];
    SDL_Texture *texturaPerfil;
    GrupoContato grupo;
    bool usaIniciais; 
} Contato;

// Estrutura para armazenar os ícones da interface do usuário
typedef struct {
    SDL_Texture *telefone;
    SDL_Texture *whatsapp;
    SDL_Texture *gmail;
    SDL_Texture *sms;
    SDL_Texture *facebook;
} UI_Icones;

// --- DECLARAÇÕES DE FUNÇÕES ---

// Função para converter HSV para RGB
void SDL_ConvertHSVtoRGB(float h, float s, float v, Uint8 *r, Uint8 *g, Uint8 *b);

// Função para renderizar um círculo preenchido
void renderizarCirculoPreenchido(SDL_Renderer *r, int cx, int cy, int radius, SDL_Color color);

// Função para renderizar um retângulo arredondado
void renderizarRetanguloArredondado(SDL_Renderer *renderer, int x, int y, int w, int h, int radius, SDL_Color color);

// Função para carregar uma textura a partir de um arquivo
SDL_Texture* carregarTextura(SDL_Renderer *renderizador, const char* caminho);

// Função para criar uma textura inicial a partir de um nome
SDL_Texture* criarTexturaInicial(SDL_Renderer *r, TTF_Font *f, const char* nome, float tamanho);

// Função para renderizar um contato na lista
void renderizarContato(SDL_Renderer *r, TTF_Font *f_nome, TTF_Font *f_num, Contato *contato, int y_offset, bool selecionado, UI_Icones *icones, int x, int w, int h, float y_animado);

// Função para renderizar o cabeçalho com gradiente
void renderizarCabecalhoGradiente(SDL_Renderer* r, const char* titulo, TTF_Font* f_cabecalho, int w, int h);

// Função para renderizar filtros
void renderizarFiltros(SDL_Renderer* r, TTF_Font* f_filtros, const char** nomes, int num_filtros, int selecionado, int w, int h);

// Função para renderizar texto
void renderizarTexto(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int x, int y);

// Função para renderizar texto centralizado
void renderizarTextoCentralizado(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int w, int y);

// Função para renderizar detalhes do contato
void renderizarDetalheContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, TTF_Font* fonte_cabecalho, Contato *contato, UI_Icones *icones, int w, int h);

// Função para renderizar o formulário de contato
void renderizarFormulario(SDL_Renderer* r, TTF_Font* f_cabecalho, TTF_Font* f_form, FormularioContato* form, bool cursorVisivel, const char* msgErro, int w, int h);

// Função para renderizar o diálogo de confirmação
void renderizarDialogoConfirmacao(SDL_Renderer* r, TTF_Font* f_cabecalho, TTF_Font* f_texto, const char* nomeContato, int w, int h);

#endif // CONTATO_H