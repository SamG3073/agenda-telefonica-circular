#define SDL_MAIN_HANDLED

#include <stdbool.h>
#include <string.h>
#include <math.h> 
#include "gerenciador_app.h" // Principal cabeçalho da lógica do app
#include "listalib.h"
#include "contato.h" 

int main(int argc, char *argv[]) {
    // Inicialização do SDL, Janela, Fontes, Ícones
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *janela = SDL_CreateWindow("Agenda de Contatos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 640, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    TTF_Font *fonte_cabecalho = NULL;
    TTF_Font *fonte_nomeContato = NULL;
    TTF_Font *fonte_numeroContato = NULL;

    UI_Icones icones_ui;
    icones_ui.telefone = carregarTextura(renderizador, "assets/icons/telefone.svg");
    icones_ui.whatsapp = carregarTextura(renderizador, "assets/icons/whatsapp.svg");
    icones_ui.sms = carregarTextura(renderizador, "assets/icons/sms.svg");
    icones_ui.facebook = carregarTextura(renderizador, "assets/icons/facebook.svg");
    icones_ui.gmail = carregarTextura(renderizador, "assets/icons/gmail.svg");
    
    Lista contatos = {0};
    Lista_Criar(&contatos, sizeof(Contato), true, true);
    carregarContatos(&contatos, renderizador, NOME_ARQUIVO_CONTATOS);
    Lista_IrParaPrimeiro(&contatos);
    
    EstadoAplicacao estadoAtual = ESTADO_LISTA;
    Contato *contatoAtual = NULL; 
    ElementoLista *elementoParaDeletar = NULL;
    const char* nomesFiltros[] = {"TODOS", "AMIGOS", "FAMILIA", "TRABALHO"};
    const int numFiltros = 4;
    CategoriaFiltro filtroSelecionado = FILTRO_TODOS;
    FormularioContato formulario = {0};
    resetarFormulario(&formulario);
    Uint32 tempoCursor = 0;
    bool cursorVisivel = true;
    char mensagemErroFormulario[100] = "";
    Uint32 tempoExibicaoErro = 0;
    const int DURACAO_MENSAGEM_ERRO_MS = 2500;
    bool executando = true;
    SDL_Event evento;

    int last_w = 0, last_h = 0;
    
    while (executando) {
        int w, h;
        SDL_GetWindowSize(janela, &w, &h);

        // Lógica para recarregar fontes com tamanho proporcional.
        if (w != last_w || h != last_h) {
            if (fonte_cabecalho) TTF_CloseFont(fonte_cabecalho);
            if (fonte_nomeContato) TTF_CloseFont(fonte_nomeContato);
            if (fonte_numeroContato) TTF_CloseFont(fonte_numeroContato);

            int tam_cabecalho = fmax(h / 32, 12);
            int tam_nome = fmax(h / 36, 10);
            int tam_num = fmax(h / 45, 8);

            fonte_cabecalho = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", tam_cabecalho);
            fonte_nomeContato = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", tam_nome);
            fonte_numeroContato = TTF_OpenFont("assets/fonts/DejaVu_Sans/DejaVuSans.ttf", tam_num);

            last_w = w;
            last_h = h;
        }

        if (strlen(mensagemErroFormulario) > 0) {
            if (SDL_GetTicks() - tempoExibicaoErro > DURACAO_MENSAGEM_ERRO_MS) {
                strcpy(mensagemErroFormulario, "");
            }
        }
        
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                executando = false;
            }
            if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_F11) {
                Uint32 flags = SDL_GetWindowFlags(janela);
                SDL_SetWindowFullscreen(janela, flags & SDL_WINDOW_FULLSCREEN_DESKTOP ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            
            switch (estadoAtual) {
                 case ESTADO_LISTA:
                    if (evento.type == SDL_KEYDOWN) {
                        switch (evento.key.keysym.sym) {
                            case SDLK_DOWN: if (contatos.atual) avancarParaProximoVisivel(&contatos, filtroSelecionado); break;
                            case SDLK_UP: if (contatos.atual) retrocederParaAnteriorVisivel(&contatos, filtroSelecionado); break;
                            case SDLK_RIGHT:
                                filtroSelecionado = (filtroSelecionado + 1) % numFiltros;
                                irParaPrimeiroVisivel(&contatos, filtroSelecionado);
                                break;
                            case SDLK_LEFT:
                                filtroSelecionado = (filtroSelecionado - 1 + numFiltros) % numFiltros;
                                irParaPrimeiroVisivel(&contatos, filtroSelecionado);
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
                            case SDLK_DOWN:
                            case SDLK_TAB:
                                formulario.campoAtivo = (formulario.campoAtivo + 1) % NUM_CAMPOS_FORM;
                                break;
                            case SDLK_UP:
                                formulario.campoAtivo = (formulario.campoAtivo - 1 + NUM_CAMPOS_FORM) % NUM_CAMPOS_FORM;
                                break;
                            case SDLK_RETURN: case SDLK_KP_ENTER:
                                if (formulario.campoAtivo == CAMPO_SALVAR) {
                                    if (salvarNovoContato(&contatos, &formulario, renderizador, mensagemErroFormulario)) {
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
                                    
                                    ElementoLista* proximo = elementoParaDeletar->proximo;
                                    if (contatos.numElementos == 1) {
                                        proximo = NULL;
                                    }

                                    Lista_RemoverElemento(&contatos, elementoParaDeletar);
                                    contatos.atual = proximo;
                                    if(contatos.atual == NULL && contatos.numElementos > 0) {
                                        contatos.atual = contatos.primeiro;
                                    }
                                    elementoParaDeletar = NULL;
                                    estadoAtual = ESTADO_LISTA;
                                }
                                break;

                            case SDLK_n:
                            case SDLK_ESCAPE:
                                elementoParaDeletar = NULL;
                                estadoAtual = ESTADO_LISTA;
                                break;
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderizador, COR_ESCURA.r, COR_ESCURA.g, COR_ESCURA.b, COR_ESCURA.a);
        SDL_RenderClear(renderizador);

        switch (estadoAtual) {
            case ESTADO_LISTA:
            case ESTADO_CONFIRMAR_DELETE:
                {
                    float cabecalho_h = h * 0.0875;
                    SDL_SetRenderDrawColor(renderizador, COR_VERMELHA.r, COR_VERMELHA.g, COR_VERMELHA.b, COR_VERMELHA.a);
                    SDL_Rect retangulo_cabecalho = {0, 0, w, cabecalho_h};
                    SDL_RenderFillRect(renderizador, &retangulo_cabecalho);
                    renderizarTextoCentralizado(renderizador, fonte_cabecalho, "Agenda de Contatos", COR_BRANCA, w, cabecalho_h * 0.32);

                    renderizarTextoCentralizado(renderizador, fonte_numeroContato, "INSERT: Adicionar | DELETE: Deletar", COR_CINZA, w, h * 0.95);
                    
                    int total_cat_width = 0, text_w_temp;
                    for (int i = 0; i < numFiltros; i++) {
                        TTF_SizeText(fonte_numeroContato, nomesFiltros[i], &text_w_temp, NULL);
                        total_cat_width += text_w_temp;
                    }
                    total_cat_width += (w * 0.07) * (numFiltros - 1); 
                    int x_offset_cat = (w - total_cat_width) / 2;

                    for (int i = 0; i < numFiltros; i++) {
                        SDL_Color cor = (i == filtroSelecionado) ? COR_BRANCA : COR_CINZA;
                        renderizarTexto(renderizador, fonte_numeroContato, nomesFiltros[i], cor, x_offset_cat, h * 0.125);
                        TTF_SizeText(fonte_numeroContato, nomesFiltros[i], &text_w_temp, NULL);
                        x_offset_cat += text_w_temp + (w * 0.07);
                    }

                    float y_offsetAtual = h * 0.1875;
                    float item_h = h * 0.125;
                    
                    if (contatos.primeiro) {
                        ElementoLista *contato_desenho = contatos.primeiro;
                        for (unsigned int i = 0; i < contatos.numElementos; ++i) {
                            if (contatoCorrespondeFiltro((Contato*)contato_desenho->dados, filtroSelecionado)) {
                                bool selecionado = (contato_desenho == contatos.atual);
                                renderizarContato(renderizador, fonte_nomeContato, fonte_numeroContato, (Contato *)contato_desenho->dados, y_offsetAtual, selecionado, &icones_ui, 0, w, h);
                                y_offsetAtual += item_h;
                            }
                            contato_desenho = contato_desenho->proximo;
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

    // Salvamento e Liberação de Memória
    salvarContatos(&contatos, NOME_ARQUIVO_CONTATOS);

    if (contatos.primeiro) {
        ElementoLista *elementoAtual = contatos.primeiro;
        for (unsigned int i = 0; i < contatos.numElementos; i++) {
            Contato* c_atual = (Contato*)elementoAtual->dados;
            if (c_atual->texturaPerfil) SDL_DestroyTexture(c_atual->texturaPerfil);
            elementoAtual = elementoAtual->proximo;
        }
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
    
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}