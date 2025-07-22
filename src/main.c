#define SDL_MAIN_HANDLED // Para usar sua própria função main em vez da do SDL

#include "listalib.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const SDL_Color COR_VERMELHA = {230, 40, 50, 255}; // Cor vermelha para o cabeçalho
const SDL_Color COR_ESCURA = {30, 30, 30, 255}; // Cor escura para o fundo
const SDL_Color COR_BRANCA = {255, 255, 255, 255}; // Cor branca para o texto
const SDL_Color COR_CINZA = {180, 180, 180, 255}; // Cor cinza para o texto secundário
const SDL_Color COR_DESTAQUE = {60, 60, 80, 255}; // Cor de destaque para o contato selecionado

// Função para renderizar texto na tela
void renderizarTexto(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int x, int y) {
    SDL_Surface *superficie = TTF_RenderUTF8_Blended(fonte, texto, cor); // Renderiza o texto em uma superfície
    // Verifica se a superfície foi criada corretamente
    if (!superficie) {
        printf("Falha ao renderizar texto: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderizador, superficie); // Cria uma textura a partir da superfície
    // Verifica se a textura foi criada corretamente
    if (!textura) {
        printf("Falha ao criar textura do texto: %s\n", SDL_GetError());
        SDL_FreeSurface(superficie); // Libera a superfície se a textura falhar
        return;
    }
    SDL_Rect retangulo = {x, y, superficie->w, superficie->h}; // Define o retângulo onde o texto será renderizado
    SDL_RenderCopy(renderizador, textura, NULL, &retangulo); // Renderiza a textura na tela
    SDL_FreeSurface(superficie); // Libera a superfície após o uso
    SDL_DestroyTexture(textura); // Libera a textura após o uso
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO); // Inicializa o SDL
    TTF_Init(); // Inicializa o SDL_ttf para renderização de texto
    SDL_Window *janela = SDL_CreateWindow("Agenda Telefonica Circular",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 640, SDL_WINDOW_RESIZABLE); // Cria uma janela
    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Cria um renderizador  
    TTF_Font *fonte_cabecalho = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", 20); // Fonte para o cabeçalho
    TTF_Font *fonte_nomeContato = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", 18); // Fonte para os nomes dos contatos
    TTF_Font *fonte_numeroContato = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", 14); // Fonte para os telefones dos contatos
    if (!fonte_cabecalho || !fonte_nomeContato || !fonte_numeroContato) {
        printf("Erro ao carregar fonte(s): %s\n", TTF_GetError());
        return 1;
    }
    bool executando = true; // Variável para controlar o loop principal de execução do código
    SDL_Event evento; // Variável para armazenar eventos
    while (executando) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) // Evento de parada do código
                executando = false;
        }
        SDL_SetRenderDrawColor(renderizador, COR_ESCURA.r, COR_ESCURA.g, COR_ESCURA.b, COR_ESCURA.a); // Define a cor de fundo
        SDL_RenderClear(renderizador); // Limpa o renderizador com a cor de fundo
        SDL_SetRenderDrawColor(renderizador, COR_VERMELHA.r, COR_VERMELHA.g, COR_VERMELHA.b, COR_VERMELHA.a); // Define a cor do cabeçalho
        SDL_Rect retangulo_cabecalho = {0, 0, 360, 56}; // Retângulo do cabeçalho
        SDL_RenderFillRect(renderizador, &retangulo_cabecalho); // Preenche o retângulo do cabeçalho
        renderizarTexto(renderizador, fonte_cabecalho, "Agenda de Contatos", COR_BRANCA, 15, 18);
        renderizarTexto(renderizador, fonte_numeroContato, "TODOS", COR_BRANCA, 20, 60);
        renderizarTexto(renderizador, fonte_numeroContato, "AMIGOS", COR_CINZA, 90, 60);
        renderizarTexto(renderizador, fonte_numeroContato, "FAMILIA", COR_CINZA, 160, 60);
        renderizarTexto(renderizador, fonte_numeroContato, "TRABALHO", COR_CINZA, 230, 60);
        SDL_RenderPresent(renderizador); // Apresenta o renderizador na tela
        SDL_Delay(16);
    }
    // Libera as fontes
    TTF_CloseFont(fonte_cabecalho); 
    TTF_CloseFont(fonte_nomeContato); 
    TTF_CloseFont(fonte_numeroContato); 
    SDL_DestroyRenderer(renderizador); // Destrói o renderizador
    SDL_DestroyWindow(janela); // Destrói a janela
    TTF_Quit(); // Finaliza o TTF
    SDL_Quit(); // Finaliza o SDL
    return 0;
}
