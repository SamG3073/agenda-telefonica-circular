#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gerenciador_app.h"

// Constantes
const char* NOME_ARQUIVO_CONTATOS = "contatos.dat";

// Implementação das Funções

void salvarContatos(Lista* lista, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Nao foi possivel abrir o arquivo para salvar os contatos");
        return;
    }

    if (lista && lista->primeiro) {
        ElementoLista* atual = lista->primeiro;
        for (unsigned int i = 0; i < lista->numElementos; i++) {
            Contato* c = (Contato*)atual->dados;
            fprintf(arquivo, "nome:%s\n", c->nome);
            fprintf(arquivo, "telefone:%s\n", c->telefone);
            fprintf(arquivo, "email:%s\n", c->email);
            fprintf(arquivo, "grupo:%d\n", c->grupo);
            fprintf(arquivo, "---\n");
            atual = atual->proximo;
        }
    }
    
    fclose(arquivo);
    printf("Contatos salvos com sucesso.\n");
}

void carregarContatos(Lista* lista, SDL_Renderer* renderer, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Arquivo de contatos nao encontrado. Comecando com lista vazia.\n");
        return;
    }

    char linha[256];
    Contato c_temp = {0};
    bool lendo_contato = false;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;

        if (strncmp(linha, "nome:", 5) == 0) {
            strcpy(c_temp.nome, linha + 5);
            lendo_contato = true;
        } else if (strncmp(linha, "telefone:", 9) == 0) {
            strcpy(c_temp.telefone, linha + 9);
        } else if (strncmp(linha, "email:", 6) == 0) {
            strcpy(c_temp.email, linha + 6);
        } else if (strncmp(linha, "grupo:", 6) == 0) {
            c_temp.grupo = (GrupoContato)atoi(linha + 6);
        } else if (strcmp(linha, "---") == 0) {
            if (lendo_contato) {
                strcpy(c_temp.caminhoImagem, "assets/images/fotoPerfil.png");
                c_temp.texturaPerfil = carregarTextura(renderer, c_temp.caminhoImagem);
                Lista_Inserir(lista, &c_temp);
                memset(&c_temp, 0, sizeof(Contato));
                lendo_contato = false;
            }
        }
    }
    fclose(arquivo);
    printf("Contatos carregados com sucesso.\n");
}

void resetarFormulario(FormularioContato* form) {
    strcpy(form->nome, "");
    strcpy(form->telefone, "");
    strcpy(form->email, "");
    form->grupo = GRUPO_NENHUM;
    form->campoAtivo = CAMPO_NOME;
}

bool salvarNovoContato(Lista* contatos, FormularioContato* form, SDL_Renderer* r, char* msgErro) {
    if (strlen(form->nome) == 0 || strlen(form->telefone) == 0 || strlen(form->email) == 0) {
        strcpy(msgErro, "Preencha todos os campos!");
        return false;
    }

    Contato c;
    strcpy(c.nome, form->nome);
    strcpy(c.telefone, form->telefone);
    strcpy(c.email, form->email);
    c.grupo = form->grupo;
    strcpy(c.caminhoImagem, "assets/images/fotoPerfil.png");
    c.texturaPerfil = carregarTextura(r, c.caminhoImagem);

    Lista_Inserir(contatos, &c);
    Lista_IrParaUltimo(contatos);
    strcpy(msgErro, ""); // Limpa a mensagem de erro em caso de sucesso
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
    if (!lista->atual) return;
    ElementoLista *inicio = lista->atual;
    do {
        Lista_Avancar(lista);
        if (contatoCorrespondeFiltro((Contato*)lista->atual->dados, filtro)) return;
    } while (lista->atual != inicio);
}

void retrocederParaAnteriorVisivel(Lista *lista, CategoriaFiltro filtro) {
    if (!lista->atual) return;
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
    
    // Se nenhum item corresponde, o cursor não deve apontar para nada visível
    lista->atual = NULL;
}