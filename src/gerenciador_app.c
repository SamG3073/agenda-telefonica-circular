#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gerenciador_app.h"

const char* NOME_ARQUIVO_CONTATOS = "contatos.dat";
const int DURACAO_ANIMACAO_MS = 180;

// Função de easing para animações (movida de main.c)
static float easeOutQuad(float t) {
    return t * (2 - t);
}

// Calcula o deslocamento Y de um elemento na lista filtrada (movido de main.c)
float calcularYOffsetParaElemento(Lista* lista, ElementoLista* alvo, CategoriaFiltro filtro, float y_base, float item_h) {
    if (!lista->primeiro || !alvo) return y_base;
    float y_offset = y_base;
    ElementoLista* atual = lista->primeiro;
    do {
        if (atual == alvo) return y_offset;
        if (contatoCorrespondeFiltro((Contato*)atual->dados, filtro)) y_offset += item_h;
        atual = atual->proximo;
    } while (atual != lista->primeiro);
    return y_base; 
}


void salvarContatos(Lista* lista, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo para salvar");
        return;
    }

    if (lista && lista->primeiro) {
        ElementoLista* atual = lista->primeiro;
        do {
            Contato* c = (Contato*)atual->dados;
            fprintf(arquivo, "nome:%s\n", c->nome);
            fprintf(arquivo, "telefone:%s\n", c->telefone);
            fprintf(arquivo, "email:%s\n", c->email);
            fprintf(arquivo, "grupo:%d\n", c->grupo);
            fprintf(arquivo, "imagem:%s\n", c->caminhoImagem);
            fprintf(arquivo, "---\n");
            atual = atual->proximo;
        } while (atual != lista->primeiro);
    }
    
    fclose(arquivo);
}

void carregarContatos(Lista* lista, SDL_Renderer* renderer, const char* nomeArquivo, TTF_Font** font) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Arquivo de contatos nao encontrado. Comecando com lista vazia.\n");
        return;
    }
    
    srand(time(NULL));

    char linha[256];
    Contato c_temp = {0};

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;

        if (strncmp(linha, "nome:", 5) == 0) strcpy(c_temp.nome, linha + 5);
        else if (strncmp(linha, "telefone:", 9) == 0) strcpy(c_temp.telefone, linha + 9);
        else if (strncmp(linha, "email:", 6) == 0) strcpy(c_temp.email, linha + 6);
        else if (strncmp(linha, "grupo:", 6) == 0) c_temp.grupo = (GrupoContato)atoi(linha + 6);
        else if (strncmp(linha, "imagem:", 7) == 0) strcpy(c_temp.caminhoImagem, linha + 7);
        else if (strcmp(linha, "---") == 0) {
            
            if (c_temp.caminhoImagem[0] == '\0') {
                strcpy(c_temp.caminhoImagem, "src/assets/images/fotoPerfil.png");
            }
            
            c_temp.texturaPerfil = carregarTextura(renderer, c_temp.caminhoImagem);
            c_temp.usaIniciais = false;

            if (c_temp.texturaPerfil == NULL) {
                int h_padrao = 640;
                float item_h = h_padrao * 0.125;
                c_temp.texturaPerfil = criarTexturaInicial(renderer, *font, c_temp.nome, item_h * 0.625);
                c_temp.usaIniciais = true;
            }

            Lista_Inserir(lista, &c_temp);
            memset(&c_temp, 0, sizeof(Contato));
        }
    }
    fclose(arquivo);
    printf("Contatos carregados.\n");
}


void resetarFormulario(FormularioContato* form) {
    strcpy(form->nome, "");
    strcpy(form->telefone, "");
    strcpy(form->email, "");
    form->grupo = GRUPO_NENHUM;
    form->campoAtivo = CAMPO_NOME;
}

bool salvarNovoContato(Lista* contatos, FormularioContato* form, SDL_Renderer* r, char* msgErro, TTF_Font *font, int h) {
    if (strlen(form->nome) == 0 || strlen(form->telefone) == 0 || strlen(form->email) == 0) {
        strcpy(msgErro, "Preencha todos os campos!");
        return false;
    }

    Contato c;
    strcpy(c.nome, form->nome);
    strcpy(c.telefone, form->telefone);
    strcpy(c.email, form->email);
    c.grupo = form->grupo;
    
    strcpy(c.caminhoImagem, "src/assets/images/fotoPerfil.png");
    
    c.texturaPerfil = carregarTextura(r, c.caminhoImagem);
    c.usaIniciais = false;

    if (c.texturaPerfil == NULL) {
        float item_h = h * 0.125;
        c.texturaPerfil = criarTexturaInicial(r, font, c.nome, item_h * 0.625);
        c.usaIniciais = true;
    }

    Lista_Inserir(contatos, &c);
    Lista_IrParaUltimo(contatos);
    strcpy(msgErro, "");
    return true;
}


bool contatoCorrespondeFiltro(Contato* c, CategoriaFiltro filtro) {
    if (!c) return false;
    switch (filtro) {
        case FILTRO_TODOS: return true;
        case FILTRO_AMIGOS: return c->grupo == GRUPO_AMIGOS;
        case FILTRO_FAMILIA: return c->grupo == GRUPO_FAMILIA;
        case FILTRO_TRABALHO: return c->grupo == GRUPO_TRABALHO;
        default: return false;
    }
}

void avancarParaProximoVisivel(Lista *lista, CategoriaFiltro filtro) {
    if (!lista->atual || lista->numElementos <= 1) return;
    ElementoLista *inicio = lista->atual;
    do {
        Lista_Avancar(lista);
        if (contatoCorrespondeFiltro((Contato*)lista->atual->dados, filtro)) return;
    } while (lista->atual != inicio);
}

void retrocederParaAnteriorVisivel(Lista *lista, CategoriaFiltro filtro) {
    if (!lista->atual || lista->numElementos <= 1) return;
    ElementoLista *inicio = lista->atual;
    do {
        Lista_Retroceder(lista);
        if (contatoCorrespondeFiltro((Contato*)lista->atual->dados, filtro)) return;
    } while (lista->atual != inicio);
}

void irParaPrimeiroVisivel(Lista *lista, CategoriaFiltro filtro) {
    if (!lista->primeiro) { 
        lista->atual = NULL; 
        return; 
    }
    Lista_IrParaPrimeiro(lista);
    if (contatoCorrespondeFiltro((Contato*)lista->atual->dados, filtro)) {
        return;
    }
    ElementoLista *inicio = lista->primeiro;
    do {
        Lista_Avancar(lista);
        if (contatoCorrespondeFiltro((Contato*)lista->atual->dados, filtro)) return;
    } while(lista->atual != inicio);
    
    lista->atual = NULL;
}
