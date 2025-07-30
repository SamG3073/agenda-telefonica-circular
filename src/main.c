#define SDL_MAIN_HANDLED

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "gerenciador_app.h" 
#include "listalib.h"
#include "contato.h"

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    if (TTF_Init() == -1) {
        printf("ERRO CRÍTICO: Falha ao inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *janela = SDL_CreateWindow("Agenda de Contatos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 640, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font *fonte_cabecalho = NULL, *fonte_nomeContato = NULL, *fonte_numeroContato = NULL, *fonte_iniciais = NULL;
    UI_Icones icones_ui;
    icones_ui.telefone = carregarTextura(renderizador, "assets/icons/telefone.svg");
    icones_ui.whatsapp = carregarTextura(renderizador, "assets/icons/whatsapp.svg");
    icones_ui.sms = carregarTextura(renderizador, "assets/icons/sms.svg");
    icones_ui.facebook = carregarTextura(renderizador, "assets/icons/facebook.svg");
    icones_ui.gmail = carregarTextura(renderizador, "assets/icons/gmail.svg");

    Lista contatos = {0};
    Lista_Criar(&contatos, sizeof(Contato), true, true);
    carregarContatos(&contatos, renderizador, NOME_ARQUIVO_CONTATOS, &fonte_iniciais);
    irParaPrimeiroVisivel(&contatos, FILTRO_TODOS);

    EstadoAplicacao estadoAtual = ESTADO_LISTA;
    Contato *contatoAtual = NULL;
    ElementoLista *elementoParaDeletar = NULL;
    CategoriaFiltro filtroSelecionado = FILTRO_TODOS;
    FormularioContato formulario = {0};
    bool executando = true;
    SDL_Event evento;
    
    int scroll_offset = 0;

    const char* nomesFiltros[] = {"TODOS", "AMIGOS", "FAMILIA", "TRABALHO"};
    int numFiltros = 4;
    resetarFormulario(&formulario);
    Uint32 tempoCursor = 0;
    bool cursorVisivel = true;
    char mensagemErroFormulario[100] = "";
    Uint32 tempoExibicaoErro = 0;
    const int DURACAO_MENSAGEM_ERRO_MS = 2500;
    int last_w = 0, last_h = 0;
    EstadoAnimacao animacao = { .animando = false };

    while (executando) {
        int w, h;
        SDL_GetWindowSize(janela, &w, &h);
        float item_h = h * 0.125;
        float y_base_lista = h * 0.1;
        float altura_lista_visivel = h - y_base_lista - (h * 0.08); 
        int max_itens_visiveis = floor(altura_lista_visivel / item_h);

        if (w != last_w || h != last_h) {
             if (fonte_cabecalho) TTF_CloseFont(fonte_cabecalho);
            if (fonte_nomeContato) TTF_CloseFont(fonte_nomeContato);
            if (fonte_numeroContato) TTF_CloseFont(fonte_numeroContato);
            if (fonte_iniciais) TTF_CloseFont(fonte_iniciais);

            int tam_cabecalho = fmax(h / 32, 12);
            int tam_nome = fmax(h / 36, 10);
            int tam_num = fmax(h / 45, 8);
            int tam_iniciais = fmax(item_h * 0.3, 14);

            const char* caminho_fonte_bold = "assets/fonts/DejaVu_Sans/DejaVuSans-Bold.ttf";
            const char* caminho_fonte_regular = "assets/fonts/DejaVu_Sans/DejaVuSans.ttf";

            fonte_cabecalho = TTF_OpenFont(caminho_fonte_bold, tam_cabecalho);
            if (!fonte_cabecalho) printf("ERRO: Falha ao carregar fonte '%s': %s\n", caminho_fonte_bold, TTF_GetError());

            fonte_nomeContato = TTF_OpenFont(caminho_fonte_regular, tam_nome);
             if (!fonte_nomeContato) printf("ERRO: Falha ao carregar fonte '%s': %s\n", caminho_fonte_regular, TTF_GetError());

            fonte_numeroContato = TTF_OpenFont(caminho_fonte_regular, tam_num);
            if (!fonte_numeroContato) printf("ERRO: Falha ao carregar fonte '%s': %s\n", caminho_fonte_regular, TTF_GetError());

            fonte_iniciais = TTF_OpenFont(caminho_fonte_bold, tam_iniciais);
            if (!fonte_iniciais) printf("ERRO: Falha ao carregar fonte '%s': %s\n", caminho_fonte_bold, TTF_GetError());
            
            if(contatos.numElementos > 0) {
                 ElementoLista *elem = contatos.primeiro;
                 do {
                    Contato *c = (Contato*)elem->dados;
                    if(c->usaIniciais) {
                        if(c->texturaPerfil) SDL_DestroyTexture(c->texturaPerfil);
                        c->texturaPerfil = criarTexturaInicial(renderizador, fonte_iniciais, c->nome, item_h * 0.625);
                    }
                    elem = elem->proximo;
                 } while(elem != contatos.primeiro);
            }

            last_w = w;
            last_h = h;
        }
        
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) executando = false;
            
            if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_F11) {
                Uint32 flags = SDL_GetWindowFlags(janela);
                SDL_SetWindowFullscreen(janela, flags & SDL_WINDOW_FULLSCREEN_DESKTOP ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
            }

            switch (estadoAtual) {
                 case ESTADO_LISTA:
                    if (evento.type == SDL_KEYDOWN) {
                        if (contatos.atual && (evento.key.keysym.sym == SDLK_UP || evento.key.keysym.sym == SDLK_DOWN)) {
                            int indice_atual_visivel = 0;
                            int total_visiveis = 0;
                            ElementoLista* iter = contatos.primeiro;
                            bool encontrado = false;
                            do {
                                if (contatoCorrespondeFiltro((Contato*)iter->dados, filtroSelecionado)) {
                                    if(iter == contatos.atual) {
                                        indice_atual_visivel = total_visiveis;
                                        encontrado = true;
                                    }
                                    total_visiveis++;
                                }
                                iter = iter->proximo;
                            } while(iter != contatos.primeiro);
                            
                             if(evento.key.keysym.sym == SDLK_DOWN) {
                                avancarParaProximoVisivel(&contatos, filtroSelecionado);
                                indice_atual_visivel = (indice_atual_visivel + 1) % total_visiveis;
                            } else {
                                retrocederParaAnteriorVisivel(&contatos, filtroSelecionado);
                                indice_atual_visivel = (indice_atual_visivel - 1 + total_visiveis) % total_visiveis;
                            }

                            if (indice_atual_visivel < scroll_offset) {
                                scroll_offset = indice_atual_visivel; 
                            } else if (indice_atual_visivel >= scroll_offset + max_itens_visiveis) {
                                scroll_offset = indice_atual_visivel - max_itens_visiveis + 1; 
                            }
                            animacao.animando = false; 
                        }
                        else { 
                             switch (evento.key.keysym.sym) {
                                case SDLK_RIGHT:
                                    filtroSelecionado = (filtroSelecionado + 1) % numFiltros;
                                    irParaPrimeiroVisivel(&contatos, filtroSelecionado);
                                    scroll_offset = 0; 
                                    break;
                                case SDLK_LEFT:
                                    filtroSelecionado = (filtroSelecionado - 1 + numFiltros) % numFiltros;
                                    irParaPrimeiroVisivel(&contatos, filtroSelecionado);
                                    scroll_offset = 0;
                                    break;
                                case SDLK_INSERT: 
                                    resetarFormulario(&formulario);
                                    estadoAtual = ESTADO_ADICIONAR;
                                    SDL_StartTextInput();
                                    break;
                                case SDLK_DELETE:
                                    if (contatos.atual) {
                                        elementoParaDeletar = contatos.atual;
                                        estadoAtual = ESTADO_CONFIRMAR_DELETE;
                                    }
                                    break;
                                case SDLK_RETURN: case SDLK_KP_ENTER:
                                    if (contatos.atual != NULL) {
                                        contatoAtual = (Contato *)contatos.atual->dados;
                                        estadoAtual = ESTADO_DETALHE;
                                    }
                                    break;
                                case SDLK_ESCAPE: executando = false; break;
                            }
                        }
                    }
                    break;
                case ESTADO_DETALHE:
                    if (evento.type == SDL_KEYDOWN && (evento.key.keysym.sym == SDLK_ESCAPE || evento.key.keysym.sym == SDLK_BACKSPACE)) {
                        estadoAtual = ESTADO_LISTA;
                        contatoAtual = NULL;
                    }
                    break;
                case ESTADO_ADICIONAR:
                     if (evento.type == SDL_KEYDOWN) {
                        switch(evento.key.keysym.sym) {
                            case SDLK_ESCAPE:
                                estadoAtual = ESTADO_LISTA;
                                SDL_StopTextInput();
                                break;
                            case SDLK_DOWN: case SDLK_TAB:
                                formulario.campoAtivo = (formulario.campoAtivo + 1) % NUM_CAMPOS_FORM;
                                break;
                            case SDLK_UP:
                                formulario.campoAtivo = (formulario.campoAtivo - 1 + NUM_CAMPOS_FORM) % NUM_CAMPOS_FORM;
                                break;
                            case SDLK_RETURN: case SDLK_KP_ENTER:
                                if (formulario.campoAtivo == CAMPO_SALVAR) {
                                    if (salvarNovoContato(&contatos, &formulario, renderizador, mensagemErroFormulario, fonte_iniciais, h)) {
                                        irParaPrimeiroVisivel(&contatos, filtroSelecionado);
                                        estadoAtual = ESTADO_LISTA;
                                        SDL_StopTextInput();
                                    } else {
                                        tempoExibicaoErro = SDL_GetTicks();
                                    }
                                } else { 
                                     formulario.campoAtivo = (formulario.campoAtivo + 1) % NUM_CAMPOS_FORM;
                                }
                                break;
                            case SDLK_BACKSPACE:
                                if (formulario.campoAtivo == CAMPO_NOME && strlen(formulario.nome) > 0) formulario.nome[strlen(formulario.nome) - 1] = '\0';
                                if (formulario.campoAtivo == CAMPO_TELEFONE && strlen(formulario.telefone) > 0) formulario.telefone[strlen(formulario.telefone) - 1] = '\0';
                                if (formulario.campoAtivo == CAMPO_EMAIL && strlen(formulario.email) > 0) formulario.email[strlen(formulario.email) - 1] = '\0';
                                break;
                            case SDLK_LEFT:
                                if (formulario.campoAtivo == CAMPO_GRUPO) formulario.grupo = (formulario.grupo - 1 + 4) % 4;
                                break;
                             case SDLK_RIGHT:
                                if (formulario.campoAtivo == CAMPO_GRUPO) formulario.grupo = (formulario.grupo + 1) % 4;
                                break;
                        }
                    } else if (evento.type == SDL_TEXTINPUT) {
                        if (formulario.campoAtivo == CAMPO_NOME && strlen(formulario.nome) < sizeof(formulario.nome) -1) strcat(formulario.nome, evento.text.text);
                        if (formulario.campoAtivo == CAMPO_TELEFONE && strlen(formulario.telefone) < sizeof(formulario.telefone) -1) strcat(formulario.telefone, evento.text.text);
                        if (formulario.campoAtivo == CAMPO_EMAIL && strlen(formulario.email) < sizeof(formulario.email) -1) strcat(formulario.email, evento.text.text);
                    }
                    break;
                case ESTADO_CONFIRMAR_DELETE:
                     if (evento.type == SDL_KEYDOWN) {
                        switch(evento.key.keysym.sym) {
                            case SDLK_s: 
                                {
                                    Contato* contato = (Contato*)elementoParaDeletar->dados;
                                    SDL_DestroyTexture(contato->texturaPerfil);
                                    ElementoLista* proximoVisivel = elementoParaDeletar->proximo;
                                    Lista_RemoverElemento(&contatos, elementoParaDeletar);
                                    if (contatos.numElementos > 0) {
                                        contatos.atual = proximoVisivel;
                                        if (!contatos.atual || !contatoCorrespondeFiltro((Contato*)contatos.atual->dados, filtroSelecionado)) {
                                            irParaPrimeiroVisivel(&contatos, filtroSelecionado);
                                            scroll_offset = 0;
                                        }
                                    } else {
                                        contatos.atual = NULL;
                                    }
                                    elementoParaDeletar = NULL;
                                    estadoAtual = ESTADO_LISTA;
                                    animacao.animando = false;
                                }
                                break;
                            case SDLK_n: case SDLK_ESCAPE:
                                elementoParaDeletar = NULL;
                                estadoAtual = ESTADO_LISTA;
                                break;
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderizador, COR_FUNDO.r, COR_FUNDO.g, COR_FUNDO.b, COR_FUNDO.a);
        SDL_RenderClear(renderizador);

        switch (estadoAtual) {
            case ESTADO_LISTA:
            case ESTADO_CONFIRMAR_DELETE:
                {
                    renderizarCabecalhoGradiente(renderizador, "Agenda de Contatos", fonte_cabecalho, w, h);
                    renderizarFiltros(renderizador, fonte_numeroContato, nomesFiltros, numFiltros, filtroSelecionado, w, h);
                    
                    if (fonte_numeroContato) { 
                        const char* insert_hint = "INSERT: Adicionar Contato";
                        const char* delete_hint = "DEL: Deletar Contato";
                        
                        float y_hint1 = h * 0.93;
                        renderizarTextoCentralizado(renderizador, fonte_numeroContato, insert_hint, COR_TEXTO_SECUNDARIO, w, y_hint1);

                        int font_height = TTF_FontHeight(fonte_numeroContato);
                        float y_hint2 = y_hint1 + font_height + 2;
                        renderizarTextoCentralizado(renderizador, fonte_numeroContato, delete_hint, COR_TEXTO_SECUNDARIO, w, y_hint2);
                    }

                    if (contatos.primeiro) {
                        int total_visiveis = 0;
                        int indice_selecionado = -1;
                        ElementoLista* iter = contatos.primeiro;

                        do {
                            if (contatoCorrespondeFiltro((Contato*)iter->dados, filtroSelecionado)) {
                                if (iter == contatos.atual) {
                                    indice_selecionado = total_visiveis;
                                }
                                total_visiveis++;
                            }
                            iter = iter->proximo;
                        } while (iter != contatos.primeiro);
                        
                        float y_desenho_base = y_base_lista + h * 0.08;
                        float y_offset_desenho = y_desenho_base;
                        int itens_desenhados = 0;
                        int indice_visivel_atual = 0;
                        iter = contatos.primeiro;
                        do {
                            if (contatoCorrespondeFiltro((Contato*)iter->dados, filtroSelecionado)) {
                                if (indice_visivel_atual >= scroll_offset && itens_desenhados < max_itens_visiveis) {
                                    bool selecionado = (iter == contatos.atual);
                                    float y_realce = y_desenho_base + (indice_selecionado - scroll_offset) * item_h;
                                    renderizarContato(renderizador, fonte_nomeContato, fonte_numeroContato, (Contato *)iter->dados, y_offset_desenho, selecionado, &icones_ui, 0, w, h, y_realce);
                                    y_offset_desenho += item_h;
                                    itens_desenhados++;
                                }
                                indice_visivel_atual++;
                            }
                            iter = iter->proximo;
                        } while (iter != contatos.primeiro && contatos.numElementos > 1);

                        if (total_visiveis > max_itens_visiveis) {
                            float altura_total_lista = altura_lista_visivel + h*0.04;
                            float altura_scrollbar = fmax(20.0, altura_total_lista * ((float)max_itens_visiveis / total_visiveis));
                            float y_scrollbar = y_desenho_base + (altura_total_lista - altura_scrollbar) * ((float)scroll_offset / (total_visiveis - max_itens_visiveis));
                            
                            SDL_Rect scrollbar_fundo = {w - 8, y_desenho_base, 4, altura_total_lista};
                            SDL_SetRenderDrawColor(renderizador, 40, 40, 50, 255);
                            SDL_RenderFillRect(renderizador, &scrollbar_fundo);

                            SDL_Rect scrollbar_thumb = {w - 8, y_scrollbar, 4, altura_scrollbar};
                            SDL_SetRenderDrawColor(renderizador, 80, 80, 100, 255);
                            SDL_RenderFillRect(renderizador, &scrollbar_thumb);
                        }
                    }
                    
                    if (estadoAtual == ESTADO_CONFIRMAR_DELETE && elementoParaDeletar) {
                        renderizarDialogoConfirmacao(renderizador, fonte_cabecalho, fonte_nomeContato, ((Contato*)elementoParaDeletar->dados)->nome, w, h);
                    }
                }
                break;
             case ESTADO_DETALHE:
                if (contatoAtual != NULL) {
                    renderizarDetalheContato(renderizador, fonte_nomeContato, fonte_numeroContato, fonte_cabecalho, contatoAtual, &icones_ui, w, h);
                }
                break;
            case ESTADO_ADICIONAR:
                if (SDL_GetTicks() - tempoCursor > 500) {
                    cursorVisivel = !cursorVisivel;
                    tempoCursor = SDL_GetTicks();
                }
                renderizarFormulario(renderizador, fonte_cabecalho, fonte_nomeContato, &formulario, cursorVisivel, mensagemErroFormulario, w, h);
                break;
        }

        SDL_RenderPresent(renderizador);
        SDL_Delay(16);
    }

    salvarContatos(&contatos, NOME_ARQUIVO_CONTATOS);

    if (contatos.primeiro) {
        ElementoLista *elementoAtual = contatos.primeiro;
         do {
            Contato* c_atual = (Contato*)elementoAtual->dados;
            if (c_atual->texturaPerfil) SDL_DestroyTexture(c_atual->texturaPerfil);
            elementoAtual = elementoAtual->proximo;
        } while(elementoAtual != contatos.primeiro);
    }
    Lista_Destruir(&contatos);
    SDL_DestroyTexture(icones_ui.telefone);
    SDL_DestroyTexture(icones_ui.whatsapp);
    SDL_DestroyTexture(icones_ui.sms);
    SDL_DestroyTexture(icones_ui.facebook);
    SDL_DestroyTexture(icones_ui.gmail);

    if (fonte_cabecalho) TTF_CloseFont(fonte_cabecalho);
    if (fonte_nomeContato) TTF_CloseFont(fonte_nomeContato);
    if (fonte_numeroContato) TTF_CloseFont(fonte_numeroContato);
    if (fonte_iniciais) TTF_CloseFont(fonte_iniciais);


    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}