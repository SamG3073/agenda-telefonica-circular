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

typedef struct {
    char nome[100]; // Nome do contato
    char telefone[20]; // Telefone do contato
    char email[100]; // Email do contato
    SDL_Color corPerfil;
} Contato;

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

// Função para renderizar um item de contato na lista
void renderizarContato(SDL_Renderer *renderizador, TTF_Font *fontName, TTF_Font *fontPhone, Contato *contato, int y_offset, bool selecionado) {
    SDL_Rect retangulo_fundo = {0, y_offset, 360, 80}; // Define o retângulo de fundo do item
    SDL_SetRenderDrawColor(renderizador, 40, 40, 40, 255); // Cor de fundo para o item não destacado
    SDL_RenderFillRect(renderizador, &retangulo_fundo); // Preenche o retângulo de fundo do item
    // Renderiza o avatar do contato
    SDL_SetRenderDrawColor(renderizador, contato->corPerfil.r, contato->corPerfil.g, contato->corPerfil.b, contato->corPerfil.a);
    SDL_Rect retangulo_avatar = {20, y_offset + 15, 50, 50}; // Define o retângulo do avatar
    SDL_RenderFillRect(renderizador, &retangulo_avatar); // Preenche o retângulo do avatar
    // Renderiza o nome do contato
    renderizarTexto(renderizador, fontName, contato->nome, COR_BRANCA, 90, y_offset + 10);
    // Renderiza o telefone do contato
    renderizarTexto(renderizador, fontPhone, contato->telefone, COR_CINZA, 90, y_offset + 40);
    // Renderiza a linha divisória
    SDL_SetRenderDrawColor(renderizador, 60, 60, 60, 255); // Cor da linha divisória
    SDL_RenderDrawLine(renderizador, 0, y_offset + 79, 360, y_offset + 79); // Desenha a linha divisória
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

    Lista contatos = {0};
    Lista_Criar(&contatos, sizeof(Contato), true, true);

    // Adiciona contatos à lista
    Contato c;
    strcpy(c.nome, "Richard");
    strcpy(c.telefone, "+55 85 98628-3167");
    c.corPerfil = (SDL_Color){0, 150, 136, 255};
    Lista_Inserir(&contatos, &c);

    strcpy(c.nome, "Samuel");
    strcpy(c.telefone, "+55 85 98527-5399");
    c.corPerfil = (SDL_Color){255, 152, 0, 255}; 
    Lista_Inserir(&contatos, &c);

    strcpy(c.nome, "Luis André");
    strcpy(c.telefone, "+55 85 99766-8518");
    c.corPerfil = (SDL_Color){233, 30, 99, 255};
    Lista_Inserir(&contatos, &c);

    Lista_IrParaPrimeiro(&contatos);

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
        int y_offsetAtual = 120; // Offset vertical inicial para o primeiro contato
        ElementoLista *contato_desenho = contatos.primeiro; // Ponto de partida para desenhar os contatos
        for (int i = 0; i < contatos.numElementos; ++i) {
            if (contato_desenho) {
                bool selecionado = (contato_desenho == contatos.atual); // Verifica se o contato atual é o selecionado
                renderizarContato(renderizador, fonte_nomeContato, fonte_numeroContato, (Contato *)contato_desenho->dados, y_offsetAtual, selecionado);
                y_offsetAtual += 80; // Incrementa o offset vertical para o próximo contato
                contato_desenho = contato_desenho->proximo; // Avança para o próximo contato
                // Se a lista for circular, volta ao início se chegar ao final
                if (contatos.circular && contato_desenho == contatos.primeiro && i < contatos.numElementos - 1) {
                    break;
                }
            } else {
                break;
            }
        }
        SDL_RenderPresent(renderizador); // Apresenta o renderizador na tela
        SDL_Delay(16);
    }
    Lista_Destruir(&contatos); // Libera a memória alocada para a lista de contatos
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
