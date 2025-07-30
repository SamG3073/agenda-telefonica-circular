#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "contato.h"
#include <ctype.h>

// Definições de cores
const SDL_Color COR_FUNDO = {18, 18, 24, 255};
const SDL_Color COR_CARTAO = {30, 30, 40, 255};
const SDL_Color COR_SOMBRA = {5, 5, 5, 100};
const SDL_Color COR_INPUT_FUNDO = {25, 25, 35, 255}; // <-- NOVA COR AQUI
const SDL_Color COR_GRADIENTE_INICIO = {40, 50, 100, 255};
const SDL_Color COR_GRADIENTE_FIM = {70, 80, 180, 255};
const SDL_Color COR_DESTAQUE_BORDA = {90, 100, 200, 255};
const SDL_Color COR_TEXTO_PRIMARIO = {240, 240, 240, 255};
const SDL_Color COR_TEXTO_SECUNDARIO = {160, 160, 170, 255};
const SDL_Color COR_ERRO = {230, 80, 90, 255};

void SDL_ConvertHSVtoRGB(float h, float s, float v, Uint8 *r, Uint8 *g, Uint8 *b) {
    if (s == 0.0f) {
        *r = *g = *b = v * 255;
        return;
    }
    int i = (int)(h * 6.0f);
    float f = (h * 6.0f) - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));
    i %= 6;
    switch (i) {
        case 0: *r = v * 255; *g = t * 255; *b = p * 255; break;
        case 1: *r = q * 255; *g = v * 255; *b = p * 255; break;
        case 2: *r = p * 255; *g = v * 255; *b = t * 255; break;
        case 3: *r = p * 255; *g = q * 255; *b = v * 255; break;
        case 4: *r = t * 255; *g = p * 255; *b = v * 255; break;
        case 5: *r = v * 255; *g = p * 255; *b = q * 255; break;
    }
}

void renderizarCirculoPreenchido(SDL_Renderer *r, int cx, int cy, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)sqrt((double)(radius * radius - dy * dy));
        SDL_RenderDrawLine(r, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void renderizarRetanguloArredondado(SDL_Renderer *renderer, int x, int y, int w, int h, int radius, SDL_Color color) {
    if (radius > w / 2) radius = w / 2;
    if (radius > h / 2) radius = h / 2;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect rect_central_h = {x + radius, y, w - 2 * radius, h};
    SDL_Rect rect_central_v = {x, y + radius, w, h - 2 * radius};
    SDL_RenderFillRect(renderer, &rect_central_h);
    SDL_RenderFillRect(renderer, &rect_central_v);
    
    renderizarCirculoPreenchido(renderer, x + radius, y + radius, radius, color);
    renderizarCirculoPreenchido(renderer, x + w - radius -1, y + radius, radius, color);
    renderizarCirculoPreenchido(renderer, x + radius, y + h - radius -1, radius, color);
    renderizarCirculoPreenchido(renderer, x + w - radius -1, y + h - radius -1, radius, color);
}

SDL_Texture* carregarTextura(SDL_Renderer *renderizador, const char* caminho) {
    if (caminho == NULL || caminho[0] == '\0') return NULL;
    SDL_Texture* novaTextura = IMG_LoadTexture(renderizador, caminho);
    if (novaTextura == NULL) printf("Falha ao carregar a imagem %s! Erro: %s\n", caminho, IMG_GetError());
    return novaTextura;
}

SDL_Texture* criarTexturaInicial(SDL_Renderer *r, TTF_Font *f, const char* nome, float tamanho) {
    if (!f || !nome || nome[0] == '\0') return NULL;

    SDL_Surface* superficie = SDL_CreateRGBSurfaceWithFormat(0, tamanho, tamanho, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_SetSurfaceBlendMode(superficie, SDL_BLENDMODE_BLEND);

    int hash = 0;
    for(int i=0; nome[i] != '\0'; i++) hash = (hash * 31 + nome[i]) % 360;
    SDL_Color corFundo;
    SDL_Color corTexto = COR_TEXTO_PRIMARIO;
    SDL_ConvertHSVtoRGB(hash / 360.0, 0.5, 0.6, &corFundo.r, &corFundo.g, &corFundo.b);
    
    Uint32 corFundo32 = SDL_MapRGBA(superficie->format, corFundo.r, corFundo.g, corFundo.b, 255);
    int centro = tamanho / 2;
    for (int y = 0; y < tamanho; y++) {
        for (int x = 0; x < tamanho; x++) {
            if (pow(x - centro, 2) + pow(y - centro, 2) <= pow(centro, 2)) {
                *((Uint32*)superficie->pixels + y * superficie->w + x) = corFundo32;
            }
        }
    }

    char inicial[2] = { toupper(nome[0]), '\0' };
    SDL_Surface *textoSuperficie = TTF_RenderUTF8_Blended(f, inicial, corTexto);
    SDL_Rect destRect = {
        (tamanho - textoSuperficie->w) / 2,
        (tamanho - textoSuperficie->h) / 2,
        textoSuperficie->w,
        textoSuperficie->h
    };

    SDL_BlitSurface(textoSuperficie, NULL, superficie, &destRect);
    SDL_FreeSurface(textoSuperficie);

    SDL_Texture *textura = SDL_CreateTextureFromSurface(r, superficie);
    SDL_FreeSurface(superficie);
    return textura;
}

void renderizarContato(SDL_Renderer *r, TTF_Font *f_nome, TTF_Font *f_num, Contato *contato, int y_offset, bool selecionado, UI_Icones *icones, int x, int w, int h, float y_animado) {
    float item_h = h * 0.125;
    int padding = w * 0.04;
    int card_x = x + padding;
    int card_w = w - (2 * padding);
    int card_h = item_h * 0.9;
    int radius = 12;

    if (selecionado) {
        SDL_Rect rect_selecao = {card_x - 3, y_animado + (item_h - card_h)/2 - 3, card_w + 6, card_h + 6};
        renderizarRetanguloArredondado(r, rect_selecao.x, rect_selecao.y, rect_selecao.w, rect_selecao.h, radius + 2, COR_DESTAQUE_BORDA);
    }
    
    renderizarRetanguloArredondado(r, card_x, y_offset + (item_h - card_h)/2 + 4, card_w, card_h, radius, COR_SOMBRA);
    renderizarRetanguloArredondado(r, card_x, y_offset + (item_h - card_h)/2, card_w, card_h, radius, COR_CARTAO);

    float perfil_size = item_h * 0.625;
    float perfil_y = y_offset + (item_h - perfil_size) / 2;
    float perfil_x = card_x + padding;
    if (contato->texturaPerfil) {
        SDL_Rect r_perfil = {perfil_x, perfil_y, perfil_size, perfil_size};
        SDL_RenderCopy(r, contato->texturaPerfil, NULL, &r_perfil);
    }
    
    float texto_x = perfil_x + perfil_size + padding;
    renderizarTexto(r, f_nome, contato->nome, COR_TEXTO_PRIMARIO, texto_x, y_offset + item_h * 0.25);
    renderizarTexto(r, f_num, contato->telefone, COR_TEXTO_SECUNDARIO, texto_x, y_offset + item_h * 0.55);

    if (selecionado) {
        float icone_tamanho = item_h * 0.35;
        float icone_y = y_offset + (item_h - icone_tamanho) / 2;
        float espaco_icones = item_h * 0.2;
        float x_icone = card_x + card_w - padding - icone_tamanho;

        if (icones->telefone) {
            SDL_Rect r_tel = {x_icone, icone_y, icone_tamanho, icone_tamanho};
            SDL_RenderCopy(r, icones->telefone, NULL, &r_tel);
        }
        x_icone -= (icone_tamanho + espaco_icones);
        if (icones->whatsapp) {
            SDL_Rect r_wpp = {x_icone, icone_y, icone_tamanho, icone_tamanho};
            SDL_RenderCopy(r, icones->whatsapp, NULL, &r_wpp);
        }
    }
}

void renderizarCabecalhoGradiente(SDL_Renderer* r, const char* titulo, TTF_Font* f_cabecalho, int w, int h) {
    float cabecalho_h = h * 0.1;

    for(int i = 0; i < cabecalho_h; i++) {
        float p = (float)i / cabecalho_h;
        Uint8 red = COR_GRADIENTE_INICIO.r + p * (COR_GRADIENTE_FIM.r - COR_GRADIENTE_INICIO.r);
        Uint8 green = COR_GRADIENTE_INICIO.g + p * (COR_GRADIENTE_FIM.g - COR_GRADIENTE_INICIO.g);
        Uint8 blue = COR_GRADIENTE_INICIO.b + p * (COR_GRADIENTE_FIM.b - COR_GRADIENTE_INICIO.b);
        SDL_SetRenderDrawColor(r, red, green, blue, 255);
        SDL_RenderDrawLine(r, 0, i, w, i);
    }
    renderizarTextoCentralizado(r, f_cabecalho, titulo, COR_TEXTO_PRIMARIO, w, cabecalho_h * 0.35);
}

void renderizarFiltros(SDL_Renderer* r, TTF_Font* f_filtros, const char** nomes, int num_filtros, int selecionado, int w, int h) {
    float y_pos = h * 0.125;
    int total_width = 0, text_w;
    int padding = w * 0.07;
    for (int i = 0; i < num_filtros; i++) {
        TTF_SizeText(f_filtros, nomes[i], &text_w, NULL);
        total_width += text_w;
    }
    total_width += padding * (num_filtros - 1);
    int x_offset = (w - total_width) / 2;

    for (int i = 0; i < num_filtros; i++) {
        SDL_Color cor = (i == selecionado) ? COR_TEXTO_PRIMARIO : COR_TEXTO_SECUNDARIO;
        renderizarTexto(r, f_filtros, nomes[i], cor, x_offset, y_pos);
        if(i == selecionado) {
            TTF_SizeText(f_filtros, nomes[i], &text_w, NULL);
            SDL_SetRenderDrawColor(r, COR_DESTAQUE_BORDA.r, COR_DESTAQUE_BORDA.g, COR_DESTAQUE_BORDA.b, 255);
            SDL_Rect line = {x_offset, y_pos + h * 0.03, text_w, 2};
            SDL_RenderFillRect(r, &line);
        }
        TTF_SizeText(f_filtros, nomes[i], &text_w, NULL);
        x_offset += text_w + padding;
    }
}

void renderizarTexto(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int x, int y) {
    if (!fonte || texto == NULL || texto[0] == '\0') {
        return;
    }
    SDL_Surface *superficie = TTF_RenderUTF8_Blended(fonte, texto, cor);
    if (!superficie) return;
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderizador, superficie);
    SDL_Rect retangulo = {x, y, superficie->w, superficie->h};
    SDL_RenderCopy(renderizador, textura, NULL, &retangulo);
    SDL_FreeSurface(superficie);
    SDL_DestroyTexture(textura);
}

void renderizarTextoCentralizado(SDL_Renderer *renderizador, TTF_Font *fonte, const char *texto, SDL_Color cor, int w, int y) {
    if (!fonte || texto == NULL || texto[0] == '\0') return;
    int text_w;
    TTF_SizeText(fonte, texto, &text_w, NULL);
    renderizarTexto(renderizador, fonte, texto, cor, (w - text_w) / 2, y);
}

void renderizarDetalheContato(SDL_Renderer *renderizador, TTF_Font *fonte_nome, TTF_Font *fonte_num, TTF_Font* fonte_cabecalho, Contato *contato, UI_Icones *icones, int w, int h) {
    SDL_SetRenderDrawColor(renderizador, COR_FUNDO.r, COR_FUNDO.g, COR_FUNDO.b, COR_FUNDO.a);
    SDL_RenderClear(renderizador);

    renderizarCabecalhoGradiente(renderizador, "Detalhes do Contato", fonte_cabecalho, w, h);

    float cabecalho_h = h * 0.1;
    float tam_perfil = fmin(w * 0.4, h * 0.25); 
    float y_pos_perfil = cabecalho_h + h * 0.05;
    
    if (contato->texturaPerfil) {
        SDL_Rect retangulo_perfil_grande = {(w - tam_perfil) / 2, y_pos_perfil, tam_perfil, tam_perfil};
        SDL_RenderCopy(renderizador, contato->texturaPerfil, NULL, &retangulo_perfil_grande);
    }

    float y_texto_inicio = y_pos_perfil + tam_perfil + h * 0.04;
    renderizarTextoCentralizado(renderizador, fonte_nome, contato->nome, COR_TEXTO_PRIMARIO, w, y_texto_inicio);
    renderizarTextoCentralizado(renderizador, fonte_num, contato->telefone, COR_TEXTO_SECUNDARIO, w, y_texto_inicio + h * 0.05);
    renderizarTextoCentralizado(renderizador, fonte_num, contato->email, COR_TEXTO_SECUNDARIO, w, y_texto_inicio + h * 0.1);
 
    float y_icones = y_texto_inicio + h * 0.18;
    float tamanho_icone = fmin(w*0.1, h*0.07);
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
    
    renderizarTextoCentralizado(renderizador, fonte_num, "Pressione ESC para voltar", COR_TEXTO_SECUNDARIO, w, h * 0.96);
}

void renderizarFormulario(SDL_Renderer* r, TTF_Font* f_cabecalho, TTF_Font* f_form, FormularioContato* form, bool cursorVisivel, const char* msgErro, int w, int h) {
    SDL_SetRenderDrawColor(r, COR_FUNDO.r, COR_FUNDO.g, COR_FUNDO.b, COR_FUNDO.a);
    SDL_RenderClear(r);
    
    renderizarCabecalhoGradiente(r, "Novo Contato", f_cabecalho, w, h);
    renderizarTextoCentralizado(r, f_form, "Pressione ESC para cancelar", COR_TEXTO_SECUNDARIO, w, h * 0.95);

    const char* nomesGrupos[] = {"Nenhum", "Amigos", "Família", "Trabalho"};
    float y_pos = h * 0.20;
    float y_step = h * 0.12;
    int margin_x = w * 0.1;
    int input_w = w * 0.8;
    int input_h = TTF_FontHeight(f_form) + 16;
    int radius = 8;
    int text_padding_x = 10;
    int text_w;

    renderizarTexto(r, f_form, "Nome", COR_TEXTO_SECUNDARIO, margin_x, y_pos - y_step * 0.3);
    SDL_Color corBordaNome = (form->campoAtivo == CAMPO_NOME) ? COR_DESTAQUE_BORDA : COR_TEXTO_SECUNDARIO;
    renderizarRetanguloArredondado(r, margin_x - 2, y_pos - 2, input_w + 4, input_h + 4, radius, corBordaNome); // Borda
    renderizarRetanguloArredondado(r, margin_x, y_pos, input_w, input_h, radius, COR_INPUT_FUNDO); // Fundo
    renderizarTexto(r, f_form, form->nome, COR_TEXTO_PRIMARIO, margin_x + text_padding_x, y_pos + 8);
    if (form->campoAtivo == CAMPO_NOME && cursorVisivel) { 
        TTF_SizeText(f_form, form->nome, &text_w, NULL); 
        renderizarTexto(r, f_form, "|", COR_TEXTO_PRIMARIO, margin_x + text_padding_x + text_w, y_pos + 8); 
    }
    y_pos += y_step;

    renderizarTexto(r, f_form, "Telefone", COR_TEXTO_SECUNDARIO, margin_x, y_pos - y_step * 0.3);
    SDL_Color corBordaTelefone = (form->campoAtivo == CAMPO_TELEFONE) ? COR_DESTAQUE_BORDA : COR_TEXTO_SECUNDARIO;
    renderizarRetanguloArredondado(r, margin_x - 2, y_pos - 2, input_w + 4, input_h + 4, radius, corBordaTelefone);
    renderizarRetanguloArredondado(r, margin_x, y_pos, input_w, input_h, radius, COR_INPUT_FUNDO);
    renderizarTexto(r, f_form, form->telefone, COR_TEXTO_PRIMARIO, margin_x + text_padding_x, y_pos + 8);
    if (form->campoAtivo == CAMPO_TELEFONE && cursorVisivel) { 
        TTF_SizeText(f_form, form->telefone, &text_w, NULL); 
        renderizarTexto(r, f_form, "|", COR_TEXTO_PRIMARIO, margin_x + text_padding_x + text_w, y_pos + 8); 
    }
    y_pos += y_step;

    renderizarTexto(r, f_form, "Email", COR_TEXTO_SECUNDARIO, margin_x, y_pos - y_step * 0.3);
    SDL_Color corBordaEmail = (form->campoAtivo == CAMPO_EMAIL) ? COR_DESTAQUE_BORDA : COR_TEXTO_SECUNDARIO;
    renderizarRetanguloArredondado(r, margin_x - 2, y_pos - 2, input_w + 4, input_h + 4, radius, corBordaEmail);
    renderizarRetanguloArredondado(r, margin_x, y_pos, input_w, input_h, radius, COR_INPUT_FUNDO);
    renderizarTexto(r, f_form, form->email, COR_TEXTO_PRIMARIO, margin_x + text_padding_x, y_pos + 8);
    if (form->campoAtivo == CAMPO_EMAIL && cursorVisivel) { 
        TTF_SizeText(f_form, form->email, &text_w, NULL); 
        renderizarTexto(r, f_form, "|", COR_TEXTO_PRIMARIO, margin_x + text_padding_x + text_w, y_pos + 8);
    }
    y_pos += y_step;

    renderizarTexto(r, f_form, "Grupo", COR_TEXTO_SECUNDARIO, margin_x, y_pos - y_step * 0.3);
    SDL_Color corBordaGrupo = (form->campoAtivo == CAMPO_GRUPO) ? COR_DESTAQUE_BORDA : COR_TEXTO_SECUNDARIO;
    renderizarRetanguloArredondado(r, margin_x - 2, y_pos - 2, input_w + 4, input_h + 4, radius, corBordaGrupo);
    renderizarRetanguloArredondado(r, margin_x, y_pos, input_w, input_h, radius, COR_INPUT_FUNDO);
    char texto_grupo[50];
    sprintf(texto_grupo, "<   %s   >", nomesGrupos[form->grupo]);
    renderizarTextoCentralizado(r, f_form, texto_grupo, COR_TEXTO_PRIMARIO, w, y_pos + 8);
    y_pos += y_step * 1.5;

    SDL_Color corBotao = (form->campoAtivo == CAMPO_SALVAR) ? COR_DESTAQUE_BORDA : COR_GRADIENTE_FIM;
    float btn_w = w * 0.8;
    float btn_h = h * 0.08;
    renderizarRetanguloArredondado(r, (w - btn_w) / 2, y_pos, btn_w, btn_h, 10, corBotao);
    renderizarTextoCentralizado(r, f_form, "Salvar Contato", COR_TEXTO_PRIMARIO, w, y_pos + (btn_h - TTF_FontHeight(f_form))/2);
    
    if (strlen(msgErro) > 0) {
        renderizarTextoCentralizado(r, f_form, msgErro, COR_ERRO, w, y_pos + btn_h + 10);
    }
}

void renderizarDialogoConfirmacao(SDL_Renderer* r, TTF_Font* f_cabecalho, TTF_Font* f_texto, const char* nomeContato, int w, int h) {
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
    SDL_Rect fundo = {0, 0, w, h};
    SDL_RenderFillRect(r, &fundo);
    
    int box_w = fmin(w * 0.9, 400); 
    int box_h = h * 0.25;
    int box_x = (w - box_w) / 2;
    int box_y = (h - box_h) / 2;

    renderizarRetanguloArredondado(r, box_x, box_y, box_w, box_h, 15, COR_CARTAO);
    
    SDL_SetRenderDrawColor(r, COR_DESTAQUE_BORDA.r, COR_DESTAQUE_BORDA.g, COR_DESTAQUE_BORDA.b, 255);
    SDL_Rect line = {box_x, box_y, box_w, 4};
    SDL_RenderFillRect(r, &line);

    renderizarTextoCentralizado(r, f_cabecalho, "Excluir Contato", COR_TEXTO_PRIMARIO, w, box_y + box_h * 0.15);
    char texto_pergunta[150];
    sprintf(texto_pergunta, "Deseja excluir %s?", nomeContato);
    renderizarTextoCentralizado(r, f_texto, texto_pergunta, COR_TEXTO_SECUNDARIO, w, box_y + box_h * 0.4);
    renderizarTextoCentralizado(r, f_texto, "Confirmar (S/N)", COR_ERRO, w, box_y + box_h * 0.7);
}