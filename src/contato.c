#include <stdio.h>
#include <math.h>
#include "contato.h"

// Definição das constantes de cor
const SDL_Color COR_VERMELHA = {230, 40, 50, 255};
const SDL_Color COR_ESCURA = {30, 30, 30, 255};
const SDL_Color COR_BRANCA = {255, 255, 255, 255};
const SDL_Color COR_CINZA = {180, 180, 180, 255};
const SDL_Color COR_DESTAQUE = {60, 60, 80, 255};

SDL_Texture* carregarTextura(SDL_Renderer *renderizador, const char* caminho) {
    SDL_Texture* novaTextura = IMG_LoadTexture(renderizador, caminho);
    if (novaTextura == NULL) {
        printf("Falha ao carregar a imagem %s! Erro do SDL_image: %s\n", caminho, IMG_GetError());
    }
    return novaTextura;
}

void renderizarTexto(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int x, int y) {
    SDL_Surface *superficie = TTF_RenderUTF8_Blended(fonte, texto, cor);
    if (!superficie) {
        printf("Falha ao renderizar texto: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderizador, superficie);
    if (!textura) {
        printf("Falha ao criar textura do texto: %s\n", SDL_GetError());
        SDL_FreeSurface(superficie);
        return;
    }
    SDL_Rect retangulo = {x, y, superficie->w, superficie->h};
    SDL_RenderCopy(renderizador, textura, NULL, &retangulo);
    SDL_FreeSurface(superficie);
    SDL_DestroyTexture(textura);
}

void renderizarTextoCentralizado(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int y) {
    SDL_Surface *superficie = TTF_RenderUTF8_Blended(fonte, texto, cor);
    if (!superficie) return;
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderizador, superficie);
    if (!textura) {
        SDL_FreeSurface(superficie);
        return;
    }
    int x = (360 - superficie->w) / 2;
    SDL_Rect retangulo = {x, y, superficie->w, superficie->h};
    SDL_RenderCopy(renderizador, textura, NULL, &retangulo);
    SDL_FreeSurface(superficie);
    SDL_DestroyTexture(textura);
}

void renderizarContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, Contato *contato, int y_offset, bool selecionado, UI_Icones *icones) {
    SDL_Rect retangulo_fundo = {0, y_offset, 360, 80};

    if (selecionado) {
        SDL_SetRenderDrawColor(renderizador, COR_DESTAQUE.r, COR_DESTAQUE.g, COR_DESTAQUE.b, COR_DESTAQUE.a);
    } else {
        SDL_SetRenderDrawColor(renderizador, 40, 40, 40, 255);
    }
    SDL_RenderFillRect(renderizador, &retangulo_fundo);

    if (contato->texturaPerfil) {
        SDL_Rect retangulo_perfil = {20, y_offset + 15, 50, 50};
        SDL_RenderCopy(renderizador, contato->texturaPerfil, NULL, &retangulo_perfil);
    }
    
    renderizarTexto(renderizador, fonte_nome, contato->nome, COR_BRANCA, 90, y_offset + 15);
    renderizarTexto(renderizador, fonte_num, contato->telefone, COR_CINZA, 90, y_offset + 45);

    int icone_y_centro = y_offset + 40;
    int icone_tamanho = 28;

    if (icones->whatsapp) {
        SDL_Rect rect_whatsapp = {250, icone_y_centro - icone_tamanho / 2, icone_tamanho, icone_tamanho};
        SDL_RenderCopy(renderizador, icones->whatsapp, NULL, &rect_whatsapp);
    }

    if (icones->telefone) {
        SDL_Rect rect_telefone = {290, icone_y_centro - icone_tamanho / 2, icone_tamanho, icone_tamanho};
        SDL_RenderCopy(renderizador, icones->telefone, NULL, &rect_telefone);
    }

    renderizarIconeOpcoes(renderizador, 335, icone_y_centro, 3, 8); 

    if (!selecionado) {
        SDL_SetRenderDrawColor(renderizador, 60, 60, 60, 255);
        SDL_RenderDrawLine(renderizador, 0, y_offset + 79, 360, y_offset + 79);
    }
}

void renderizarDetalheContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, TTF_Font* fonte_cabecalho, Contato *contato, UI_Icones *icones) {
    SDL_SetRenderDrawColor(renderizador, COR_ESCURA.r, COR_ESCURA.g, COR_ESCURA.b, COR_ESCURA.a);
    SDL_RenderClear(renderizador);

    SDL_SetRenderDrawColor(renderizador, COR_VERMELHA.r, COR_VERMELHA.g, COR_VERMELHA.b, COR_VERMELHA.a);
    SDL_Rect retangulo_cabecalho = {0, 0, 360, 56};
    SDL_RenderFillRect(renderizador, &retangulo_cabecalho);
    renderizarTexto(renderizador, fonte_cabecalho, "Detalhes do Contato", COR_BRANCA, 15, 18);

    if (contato->texturaPerfil) {
        SDL_Rect retangulo_perfil_grande = {105, 90, 150, 150};
        SDL_RenderCopy(renderizador, contato->texturaPerfil, NULL, &retangulo_perfil_grande);
    }

    renderizarTextoCentralizado(renderizador, fonte_nome, contato->nome, COR_BRANCA, 260);
    renderizarTextoCentralizado(renderizador, fonte_num, contato->telefone, COR_CINZA, 290);
    renderizarTextoCentralizado(renderizador, fonte_num, contato->email, COR_CINZA, 320);
 
    int y_icones = 380;
    int tamanho_icone = 48;
    int espaco_entre_icones = (360 - (5 * tamanho_icone)) / 6;
    int x_atual = espaco_entre_icones;

    if (icones->sms) {
        SDL_Rect rect_sms = {x_atual, y_icones, tamanho_icone, tamanho_icone};
        SDL_RenderCopy(renderizador, icones->sms, NULL, &rect_sms);
    }
    x_atual += tamanho_icone + espaco_entre_icones;

    if (icones->whatsapp) {
        SDL_Rect rect_whatsapp = {x_atual, y_icones, tamanho_icone, tamanho_icone};
        SDL_RenderCopy(renderizador, icones->whatsapp, NULL, &rect_whatsapp);
    }
    x_atual += tamanho_icone + espaco_entre_icones;

    if (icones->telefone) {
        SDL_Rect rect_telefone = {x_atual, y_icones, tamanho_icone, tamanho_icone};
        SDL_RenderCopy(renderizador, icones->telefone, NULL, &rect_telefone);
    }
    x_atual += tamanho_icone + espaco_entre_icones;

    if (icones->facebook) {
        SDL_Rect rect_facebook = {x_atual, y_icones, tamanho_icone, tamanho_icone};
        SDL_RenderCopy(renderizador, icones->facebook, NULL, &rect_facebook);
    }
    x_atual += tamanho_icone + espaco_entre_icones;

    if (icones->gmail) {
        SDL_Rect rect_gmail = {x_atual, y_icones, tamanho_icone, tamanho_icone};
        SDL_RenderCopy(renderizador, icones->gmail, NULL, &rect_gmail);
    }
}

void renderizarCirculoPreenchido(SDL_Renderer *renderizador, int centroX, int centroY, int raio) {
    for (int y = -raio; y <= raio; y++) {
        for (int x = -raio; x <= raio; x++) {
            if (pow(x, 2) + pow(y, 2) <= pow(raio, 2)) {
                SDL_RenderDrawPoint(renderizador, centroX + x, centroY + y);
            }
        }
    }
}

void renderizarIconeOpcoes(SDL_Renderer *renderizador, int x, int y, int raioPonto, int espacamento) {
    SDL_SetRenderDrawColor(renderizador, COR_CINZA.r, COR_CINZA.g, COR_CINZA.b, COR_CINZA.a);
    renderizarCirculoPreenchido(renderizador, x, y - espacamento, raioPonto);
    renderizarCirculoPreenchido(renderizador, x, y, raioPonto);
    renderizarCirculoPreenchido(renderizador, x, y + espacamento, raioPonto);
}