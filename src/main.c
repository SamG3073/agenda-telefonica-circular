#define SDL_MAIN_HANDLED // Para usar sua própria função main em vez da do SDL

#include "listalib.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO); // Inicializa o SDL
    SDL_Window *window = SDL_CreateWindow("Agenda Telefonica Circular",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 640, SDL_WINDOW_RESIZABLE); // Cria uma janela
    bool executando = true; // Variável para controlar o loop principal de execução do código
    SDL_Event evento; // Variável para armazenar eventos
    while (executando) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) // Evento de parada do código
                executando = false;
            SDL_Delay(16);
        }
    }
    SDL_DestroyWindow(window); // Destrói a janela
    SDL_Quit(); // Finaliza o SDL
    return 0;
}
