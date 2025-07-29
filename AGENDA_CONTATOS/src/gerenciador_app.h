#ifndef GERENCIADOR_APP_H
#define GERENCIADOR_APP_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "listalib.h"
#include "contato.h" // Precisa da definição de Contato
#include "app_types.h"

// Constante global para o nome do arquivo de dados
extern const char* NOME_ARQUIVO_CONTATOS;

// Funções de filtro e navegação na lista
bool contatoCorrespondeFiltro(Contato* c, CategoriaFiltro filtro);
void avancarParaProximoVisivel(Lista *lista, CategoriaFiltro filtro);
void retrocederParaAnteriorVisivel(Lista *lista, CategoriaFiltro filtro);
void irParaPrimeiroVisivel(Lista *lista, CategoriaFiltro filtro);

// Funções de gerenciamento do formulário
void resetarFormulario(FormularioContato* form);
bool salvarNovoContato(Lista* contatos, FormularioContato* form, SDL_Renderer* r, char* msgErro);

// Funções de persistência de dados (carregar/salvar)
void salvarContatos(Lista* lista, const char* nomeArquivo);
void carregarContatos(Lista* lista, SDL_Renderer* renderer, const char* nomeArquivo);

#endif // GERENCIADOR_APP_H