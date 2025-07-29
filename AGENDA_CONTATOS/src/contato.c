#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "contato.h"

// Definição das constantes de cor
const SDL_Color COR_VERMELHA = {230, 40, 50, 255};
const SDL_Color COR_ESCURA = {30, 30, 30, 255};
const SDL_Color COR_BRANCA = {255, 255, 255, 255};
const SDL_Color COR_CINZA = {180, 180, 180, 255};
const SDL_Color COR_DESTAQUE = {60, 60, 80, 255};

SDL_Texture* carregarTextura(SDL_Renderer *renderizador, const char* caminho) {
    if (caminho == NULL || caminho[0] == '\0') {
        return NULL;
    }
    SDL_Texture* novaTextura = IMG_LoadTexture(renderizador, caminho);
    if (novaTextura == NULL) {
        printf("Falha ao carregar a imagem %s! Erro do SDL_image: %s\n", caminho, IMG_GetError());
    }
    return novaTextura;
}

void renderizarTexto(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int x, int y) {
    if (!fonte || texto == NULL || texto[0] == '\0') {
        return;
    }
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

void renderizarTextoCentralizado(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int w, int y) {
    if (!fonte || texto == NULL || texto[0] == '\0') {
        return;
    }

    SDL_Surface *superficie = TTF_RenderUTF8_Blended(fonte, texto, cor);
    if (!superficie) return;
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderizador, superficie);
    if (!textura) {
        SDL_FreeSurface(superficie);
        return;
    }
    int x = (w - superficie->w) / 2; 
    SDL_Rect retangulo = {x, y, superficie->w, superficie->h};
    SDL_RenderCopy(renderizador, textura, NULL, &retangulo);
    SDL_FreeSurface(superficie);
    SDL_DestroyTexture(textura);
}

void renderizarContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, Contato *contato, int y_offset, bool selecionado, UI_Icones *icones, int x, int w, int h) {
    float item_h = h * 0.125; 
    SDL_Rect retangulo_fundo = {x, y_offset, w, item_h}; 

    if (selecionado) {
        SDL_SetRenderDrawColor(renderizador, COR_DESTAQUE.r, COR_DESTAQUE.g, COR_DESTAQUE.b, COR_DESTAQUE.a);
    } else {
        SDL_SetRenderDrawColor(renderizador, 40, 40, 40, 255);
    }
    SDL_RenderFillRect(renderizador, &retangulo_fundo);

    // --- POSICIONAMENTO DA FOTO E TEXTO (CÓDIGO ATUALIZADO) ---
    float perfil_size = item_h * 0.625;
    float perfil_y = y_offset + (item_h - perfil_size) / 2;
    float perfil_x = x + (w * 0.055); 

    if (contato->texturaPerfil) {
        SDL_Rect retangulo_perfil = {perfil_x, perfil_y, perfil_size, perfil_size};
        SDL_RenderCopy(renderizador, contato->texturaPerfil, NULL, &retangulo_perfil);
    }
    
    float espaco_foto_texto = item_h * 0.2;
    float texto_x = perfil_x + perfil_size + espaco_foto_texto;
    renderizarTexto(renderizador, fonte_nome, contato->nome, COR_BRANCA, texto_x, y_offset + item_h * 0.2);
    renderizarTexto(renderizador, fonte_num, contato->telefone, COR_CINZA, texto_x, y_offset + item_h * 0.55);

    // --- POSICIONAMENTO DOS ÍCONES DA DIREITA ---
    float icone_y_centro = y_offset + item_h / 2;
    float icone_tamanho = item_h * 0.35; 
    float espaco_entre_icones = item_h * 0.25;

    float raio_ponto_opcoes = item_h * 0.04;
    float x_opcoes = x + w * 0.95 - raio_ponto_opcoes; 
    renderizarIconeOpcoes(renderizador, x_opcoes, icone_y_centro, raio_ponto_opcoes, item_h * 0.1);
    
    float x_telefone = x_opcoes - raio_ponto_opcoes - espaco_entre_icones - icone_tamanho;
    if (icones->telefone) {
        SDL_Rect rect_telefone = {x_telefone, icone_y_centro - icone_tamanho / 2, icone_tamanho, icone_tamanho};
        SDL_RenderCopy(renderizador, icones->telefone, NULL, &rect_telefone);
    }

    float x_whatsapp = x_telefone - espaco_entre_icones - icone_tamanho;
    if (icones->whatsapp) {
        SDL_Rect rect_whatsapp = {x_whatsapp, icone_y_centro - icone_tamanho / 2, icone_tamanho, icone_tamanho};
        SDL_RenderCopy(renderizador, icones->whatsapp, NULL, &rect_whatsapp);
    }

    if (!selecionado) {
        SDL_SetRenderDrawColor(renderizador, 60, 60, 60, 255);
        SDL_RenderDrawLine(renderizador, x, y_offset + item_h -1, x + w, y_offset + item_h -1);
    }
}

void renderizarDetalheContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, TTF_Font* fonte_cabecalho, Contato *contato, UI_Icones *icones, int w, int h) {
    SDL_SetRenderDrawColor(renderizador, COR_ESCURA.r, COR_ESCURA.g, COR_ESCURA.b, COR_ESCURA.a);
    SDL_RenderClear(renderizador);

    float cabecalho_h = h * 0.0875;
    SDL_SetRenderDrawColor(renderizador, COR_VERMELHA.r, COR_VERMELHA.g, COR_VERMELHA.b, COR_VERMELHA.a);
    SDL_Rect retangulo_cabecalho = {0, 0, w, cabecalho_h};
    SDL_RenderFillRect(renderizador, &retangulo_cabecalho);
    renderizarTextoCentralizado(renderizador, fonte_cabecalho, "Detalhes do Contato", COR_BRANCA, w, cabecalho_h * 0.32);

    float tam_perfil = fmin(w * 0.4, h * 0.4); 
    float y_pos_perfil = cabecalho_h + h * 0.05;
    
    if (contato->texturaPerfil) {
        SDL_Rect retangulo_perfil_grande = {(w - tam_perfil) / 2, y_pos_perfil, tam_perfil, tam_perfil};
        SDL_RenderCopy(renderizador, contato->texturaPerfil, NULL, &retangulo_perfil_grande);
    }

    float y_texto_inicio = y_pos_perfil + tam_perfil + h * 0.04;
    renderizarTextoCentralizado(renderizador, fonte_nome, contato->nome, COR_BRANCA, w, y_texto_inicio);
    renderizarTextoCentralizado(renderizador, fonte_num, contato->telefone, COR_CINZA, w, y_texto_inicio + h * 0.05);
    renderizarTextoCentralizado(renderizador, fonte_num, contato->email, COR_CINZA, w, y_texto_inicio + h * 0.1);
 
    float y_icones = y_texto_inicio + h * 0.18;
    float tamanho_icone = fmin(w*0.1, h*0.1);
    float espaco_total_icones = 5 * tamanho_icone;
    float espaco_entre_icones = (w - espaco_total_icones) / 6;

    int x_atual = espaco_entre_icones;

    if (icones->sms) { SDL_Rect r = {x_atual, y_icones, tamanho_icone, tamanho_icone}; SDL_RenderCopy(renderizador, icones->sms, NULL, &r); }
    x_atual += tamanho_icone + espaco_entre_icones;
    if (icones->whatsapp) { SDL_Rect r = {x_atual, y_icones, tamanho_icone, tamanho_icone}; SDL_RenderCopy(renderizador, icones->whatsapp, NULL, &r); }
    x_atual += tamanho_icone + espaco_entre_icones;
    if (icones->telefone) { SDL_Rect r = {x_atual, y_icones, tamanho_icone, tamanho_icone}; SDL_RenderCopy(renderizador, icones->telefone, NULL, &r); }
    x_atual += tamanho_icone + espaco_entre_icones;
    if (icones->facebook) { SDL_Rect r = {x_atual, y_icones, tamanho_icone, tamanho_icone}; SDL_RenderCopy(renderizador, icones->facebook, NULL, &r); }
    x_atual += tamanho_icone + espaco_entre_icones;
    if (icones->gmail) { SDL_Rect r = {x_atual, y_icones, tamanho_icone, tamanho_icone}; SDL_RenderCopy(renderizador, icones->gmail, NULL, &r); }
    
    renderizarTextoCentralizado(renderizador, fonte_num, "Pressione ESC para voltar", COR_CINZA, w, h * 0.96);
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

void renderizarFormulario(SDL_Renderer* r, TTF_Font* f_cabecalho, TTF_Font* f_form, FormularioContato* form, bool cursorVisivel, const char* msgErro, int w, int h) {
    SDL_SetRenderDrawColor(r, COR_ESCURA.r, COR_ESCURA.g, COR_ESCURA.b, COR_ESCURA.a);
    SDL_RenderClear(r);
    
    float cabecalho_h = h * 0.0875;
    SDL_SetRenderDrawColor(r, COR_VERMELHA.r, COR_VERMELHA.g, COR_VERMELHA.b, COR_VERMELHA.a);
    SDL_Rect retangulo_cabecalho = {0, 0, w, cabecalho_h};
    SDL_RenderFillRect(r, &retangulo_cabecalho);
    renderizarTextoCentralizado(r, f_cabecalho, "Novo Contato", COR_BRANCA, w, cabecalho_h * 0.32);
    renderizarTextoCentralizado(r, f_form, "Pressione ESC para cancelar", COR_CINZA, w, h * 0.95);

    const char* nomesGrupos[] = {"Nenhum", "Amigos", "Família", "Trabalho"};
    float y_pos = h * 0.15;
    float y_step = h * 0.08;
    int label_x = w * 0.1;
    int input_x = w * 0.35;
    int text_w;

    renderizarTexto(r, f_form, "Nome:", form->campoAtivo == CAMPO_NOME ? COR_BRANCA : COR_CINZA, label_x, y_pos);
    renderizarTexto(r, f_form, form->nome, COR_BRANCA, input_x, y_pos);
    if (form->campoAtivo == CAMPO_NOME && cursorVisivel) { TTF_SizeText(f_form, form->nome, &text_w, NULL); renderizarTexto(r, f_form, "|", COR_BRANCA, input_x + text_w, y_pos); }
    y_pos += y_step;
    renderizarTexto(r, f_form, "Telefone:", form->campoAtivo == CAMPO_TELEFONE ? COR_BRANCA : COR_CINZA, label_x, y_pos);
    renderizarTexto(r, f_form, form->telefone, COR_BRANCA, input_x, y_pos);
     if (form->campoAtivo == CAMPO_TELEFONE && cursorVisivel) { TTF_SizeText(f_form, form->telefone, &text_w, NULL); renderizarTexto(r, f_form, "|", COR_BRANCA, input_x + text_w, y_pos); }
    y_pos += y_step;
    renderizarTexto(r, f_form, "Email:", form->campoAtivo == CAMPO_EMAIL ? COR_BRANCA : COR_CINZA, label_x, y_pos);
    renderizarTexto(r, f_form, form->email, COR_BRANCA, input_x, y_pos);
     if (form->campoAtivo == CAMPO_EMAIL && cursorVisivel) { TTF_SizeText(f_form, form->email, &text_w, NULL); renderizarTexto(r, f_form, "|", COR_BRANCA, input_x + text_w, y_pos); }
    y_pos += y_step;
    renderizarTexto(r, f_form, "Grupo:", form->campoAtivo == CAMPO_GRUPO ? COR_BRANCA : COR_CINZA, label_x, y_pos);
    char texto_grupo[50];
    sprintf(texto_grupo, "< %s >", nomesGrupos[form->grupo]);
    renderizarTexto(r, f_form, texto_grupo, COR_BRANCA, input_x, y_pos);
    y_pos += y_step * 1.5;

    SDL_Color corBotao = (form->campoAtivo == CAMPO_SALVAR) ? COR_DESTAQUE : COR_VERMELHA;
    SDL_SetRenderDrawColor(r, corBotao.r, corBotao.g, corBotao.b, corBotao.a);
    
    float btn_w = w * 0.5;
    float btn_h = h * 0.08;
    SDL_Rect rect_botao = {(w - btn_w) / 2, y_pos, btn_w, btn_h};
    SDL_RenderFillRect(r, &rect_botao);
    renderizarTextoCentralizado(r, f_form, "Salvar Contato", COR_BRANCA, w, y_pos + btn_h * 0.25);
    if (strlen(msgErro) > 0) {
        renderizarTextoCentralizado(r, f_form, msgErro, COR_VERMELHA, w, y_pos + btn_h + h*0.01);
    }
}


void renderizarDialogoConfirmacao(SDL_Renderer* r, TTF_Font* f_cabecalho, TTF_Font* f_texto, const char* nomeContato, int w, int h) {
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
    SDL_Rect fundo = {0, 0, w, h};
    SDL_RenderFillRect(r, &fundo);
    int box_w = fmin(w * 0.9, 400); 
    int box_h = h * 0.25;
    SDL_Rect caixa = {(w - box_w) / 2, (h - box_h) / 2, box_w, box_h};
    SDL_SetRenderDrawColor(r, COR_ESCURA.r, COR_ESCURA.g, COR_ESCURA.b, 255);
    SDL_RenderFillRect(r, &caixa);
    SDL_SetRenderDrawColor(r, COR_BRANCA.r, COR_BRANCA.g, COR_BRANCA.b, 255);
    SDL_RenderDrawRect(r, &caixa);
    renderizarTextoCentralizado(r, f_cabecalho, "Excluir Contato", COR_BRANCA, w, caixa.y + box_h * 0.15);
    char texto_pergunta[150];
    sprintf(texto_pergunta, "Deseja excluir %s?", nomeContato);
    renderizarTextoCentralizado(r, f_texto, texto_pergunta, COR_CINZA, w, caixa.y + box_h * 0.4);
    renderizarTextoCentralizado(r, f_texto, "Confirmar (S/N)", COR_VERMELHA, w, caixa.y + box_h * 0.7);
}