#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <string.h>
#include "listalib.h"
#include "contato.h" // Inclui todas as nossas definições e declarações

int main(int argc, char *argv[]) {
    // Inicialização do SDL, TTF e IMG
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    // Criação da janela e do renderizador
    SDL_Window *janela = SDL_CreateWindow("Agenda de Contatos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 640, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    // Criação das fontes
    TTF_Font *fonte_cabecalho = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", 20);
    TTF_Font *fonte_nomeContato = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", 18);
    TTF_Font *fonte_numeroContato = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", 14);

    // Criação dos ícones da interface do contato
    UI_Icones icones_ui;
    icones_ui.telefone = carregarTextura(renderizador, "assets/icons/telefone.svg");
    icones_ui.whatsapp = carregarTextura(renderizador, "assets/icons/whatsapp.svg");
    icones_ui.sms = carregarTextura(renderizador, "assets/icons/sms.svg");
    icones_ui.facebook = carregarTextura(renderizador, "assets/icons/facebook.svg");
    icones_ui.gmail = carregarTextura(renderizador, "assets/icons/gmail.svg");

    // Criação da lista de contatos
    Lista contatos = {0};
    Lista_Criar(&contatos, sizeof(Contato), true, true);

    // Inserção de contatos na lista de contatos
    Contato c;
    strcpy(c.nome, "Fabrício");
    strcpy(c.telefone, "+55 11 98354-8736");
    strcpy(c.email, "fabricio18372@gmail.com");
    strcpy(c.caminhoImagem, "assets/images/fotoPerfil.png");
    c.texturaPerfil = carregarTextura(renderizador, c.caminhoImagem);
    Lista_Inserir(&contatos, &c);

    strcpy(c.nome, "Gabriel");
    strcpy(c.telefone, "+55 11 98612-4059");
    strcpy(c.email, "gabriel193729@gmail.com");
    strcpy(c.caminhoImagem, "assets/images/fotoPerfil.png");
    c.texturaPerfil = carregarTextura(renderizador, c.caminhoImagem);
    Lista_Inserir(&contatos, &c);

    strcpy(c.nome, "Davi");
    strcpy(c.telefone, "+55 11 98543-1220");
    strcpy(c.email, "davi9271@gmail.com");
    strcpy(c.caminhoImagem, "assets/images/fotoPerfil.png");
    c.texturaPerfil = carregarTextura(renderizador, c.caminhoImagem);
    Lista_Inserir(&contatos, &c);

    strcpy(c.nome, "Matheus");
    strcpy(c.telefone, "+55 85 98127-2963");
    strcpy(c.email, "matheus432664@gmail.com");
    strcpy(c.caminhoImagem, "assets/images/fotoPerfil.png");
    c.texturaPerfil = carregarTextura(renderizador, c.caminhoImagem);
    Lista_Inserir(&contatos, &c);

    strcpy(c.nome, "Fernando");
    strcpy(c.telefone, "+55 85 98772-1178");
    strcpy(c.email, "fernando2983218@gmail.com");
    strcpy(c.caminhoImagem, "assets/images/fotoPerfil.png");
    c.texturaPerfil = carregarTextura(renderizador, c.caminhoImagem);
    Lista_Inserir(&contatos, &c);

    Lista_IrParaPrimeiro(&contatos);
    
    EstadoAplicacao estadoAtual = ESTADO_LISTA; // Estado inicial da aplicação
    Contato *contatoAtual = NULL; 

    bool executando = true; // Variável de controle do loop principal
    SDL_Event evento; // Variável para armazenar eventos do SDL
    // Loop principal da aplicação
    while (executando) {
        while (SDL_PollEvent(&evento)) {
            // Verifica se o usuário fechou a janela ou pressionou a tecla ESC
            if (evento.type == SDL_QUIT) {
                executando = false;
            }
            // Verifica se uma tecla foi pressionada
            if (evento.type == SDL_KEYDOWN) {
                switch (estadoAtual) {
                    // Estado da lista de contatos
                    case ESTADO_LISTA:
                        switch (evento.key.keysym.sym) {
                            case SDLK_RIGHT: 
                                Lista_Avancar(&contatos); 
                                break;
                            case SDLK_LEFT: 
                                Lista_Retroceder(&contatos); 
                                break;
                            case SDLK_RETURN: 
                            case SDLK_KP_ENTER:
                                if (contatos.atual != NULL) {
                                    contatoAtual = (Contato *)contatos.atual->dados;
                                    estadoAtual = ESTADO_DETALHE;
                                }
                                break;
                            case SDLK_ESCAPE: executando = false; break;
                        }
                        break;
                    // Estado de detalhes do contato
                    case ESTADO_DETALHE:
                        switch (evento.key.keysym.sym) {
                            case SDLK_ESCAPE: 
                            case SDLK_BACKSPACE:
                                estadoAtual = ESTADO_LISTA;
                                contatoAtual = NULL; 
                                break;
                        }
                        break;
                }
            }
        }

        switch (estadoAtual) {
            // Renderiza a lista de contatos
            case ESTADO_LISTA:
                SDL_SetRenderDrawColor(renderizador, COR_ESCURA.r, COR_ESCURA.g, COR_ESCURA.b, COR_ESCURA.a);
                SDL_RenderClear(renderizador);

                SDL_SetRenderDrawColor(renderizador, COR_VERMELHA.r, COR_VERMELHA.g, COR_VERMELHA.b, COR_VERMELHA.a);
                SDL_Rect retangulo_cabecalho = {0, 0, 360, 56};
                SDL_RenderFillRect(renderizador, &retangulo_cabecalho);
                renderizarTexto(renderizador, fonte_cabecalho, "Agenda de Contatos", COR_BRANCA, 15, 18);

                renderizarTexto(renderizador, fonte_numeroContato, "TODOS", COR_BRANCA, 20, 80);
                renderizarTexto(renderizador, fonte_numeroContato, "AMIGOS", COR_CINZA, 90, 80);
                renderizarTexto(renderizador, fonte_numeroContato, "FAMILIA", COR_CINZA, 160, 80);
                renderizarTexto(renderizador, fonte_numeroContato, "TRABALHO", COR_CINZA, 230, 80);

                int y_offsetAtual = 120;
                ElementoLista *contato_desenho = contatos.primeiro;

                for (unsigned int i = 0; i < contatos.numElementos; ++i) {
                    bool selecionado = (contato_desenho == contatos.atual);
                    renderizarContato(renderizador, fonte_nomeContato, fonte_numeroContato, (Contato *)contato_desenho->dados, y_offsetAtual, selecionado, &icones_ui);
                    y_offsetAtual += 80;
                    contato_desenho = contato_desenho->proximo;
                }
                break; 
            
            // Renderiza os detalhes do contato selecionado
            case ESTADO_DETALHE:
                if (contatoAtual != NULL) {
                    renderizarDetalheContato(renderizador, fonte_nomeContato, fonte_numeroContato, fonte_cabecalho, contatoAtual, &icones_ui);
                }
                break;
        }

        SDL_RenderPresent(renderizador);
        SDL_Delay(16);
    }

    // --- Finalização e Limpeza ---
    if (contatos.primeiro) {
        ElementoLista *elementoAtual = contatos.primeiro;
        for (unsigned int i = 0; i < contatos.numElementos; i++) {
            Contato* c_atual = (Contato*)elementoAtual->dados;
            if (c_atual->texturaPerfil) {
                SDL_DestroyTexture(c_atual->texturaPerfil);
            }
            elementoAtual = elementoAtual->proximo;
        }
    }

    Lista_Destruir(&contatos);

    SDL_DestroyTexture(icones_ui.telefone);
    SDL_DestroyTexture(icones_ui.whatsapp);
    SDL_DestroyTexture(icones_ui.sms);
    SDL_DestroyTexture(icones_ui.facebook);
    SDL_DestroyTexture(icones_ui.gmail);

    TTF_CloseFont(fonte_cabecalho);
    TTF_CloseFont(fonte_nomeContato);
    TTF_CloseFont(fonte_numeroContato);

    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}