#ifndef GERENCIADOR_APP_H
#define GERENCIADOR_APP_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "listalib.h"
#include "contato.h" 
#include "app_types.h"

// Constante global para o nome do arquivo de dados
extern const char* NOME_ARQUIVO_CONTATOS;

// Funções de filtro e navegação na lista
bool contatoCorrespondeFiltro(Contato* c, CategoriaFiltro filtro);
void avancarParaProximoVisivel(Lista *lista, CategoriaFiltro filtro);
void retrocederParaAnteriorVisivel(Lista *lista, CategoriaFiltro filtro);
void irParaPrimeiroVisivel(Lista *lista, CategoriaFiltro filtro);

// Função para resetar o formulário de contato
void resetarFormulario(FormularioContato* form);

// Função para salvar um novo contato a partir do formulário
bool salvarNovoContato(Lista* contatos, FormularioContato* form, SDL_Renderer* r, char* msgErro, TTF_Font *font, int h);

// Funções de persistência de dados (carregar/salvar)
void salvarContatos(Lista* lista, const char* nomeArquivo);
void carregarContatos(Lista* lista, SDL_Renderer* renderer, const char* nomeArquivo, TTF_Font** font);

// Função para calcular o deslocamento Y de um elemento
float calcularYOffsetParaElemento(Lista* lista, ElementoLista* alvo, CategoriaFiltro filtro, float y_base, float item_h);


#endif // GERENCIADOR_APP_H