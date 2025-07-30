#ifndef _LISTALIB_H
#define _LISTALIB_H

#include <stddef.h>
#include <stdbool.h>

typedef unsigned char byte;
typedef struct {
    void *anterior;
    void *proximo;
    byte *dados;
} ElementoLista;

typedef struct {
    bool criada;
    bool duplamenteEncadeada;
    bool circular;
    unsigned int numElementos;
    size_t tamanhoElemento;
    ElementoLista *primeiro;
    ElementoLista *ultimo;
    ElementoLista *atual;
} Lista;

// Função para criar uma lista
void Lista_Criar(Lista *lista, size_t tamanhoElemento, bool duplamenteEncadeada, bool circular);

// Função para destruir uma lista
void Lista_Destruir(Lista *lista);

// Função para obter o primeiro elemento da lista
ElementoLista *Lista_ObterPrimeiro(Lista *lista);

// Função para obter o último elemento da lista
ElementoLista *Lista_ObterUltimo(Lista *lista);

// Função para obter o elemento atual da lista
ElementoLista *Lista_ObterAtual(Lista *lista);

// Função para obter o tamanho total da lista em bytes
size_t Lista_ObterTamanhoTotal(Lista *lista);

// Função para exibir o tamanho total da lista
void Lista_ExibirTamanhoTotal(Lista *lista);

// Função para ir para o primeiro elemento da lista
void Lista_IrParaPrimeiro(Lista *lista);

// Função para ir para o último elemento da lista
void Lista_IrParaUltimo(Lista *lista);

// Função para percorrer a lista diretamente
void Lista_PercorrerDiretamente(Lista *lista, unsigned int numVezesParaPercorrer);

// Função para percorrer a lista inversamente
void Lista_PercorrerInversamente(Lista *lista, unsigned int numVezesParaPercorrer);

// Função para inserir um elemento na lista
void Lista_DumpParametros(Lista *lista);

// Função para avançar para o próximo elemento da lista
void Lista_Avancar(Lista *lista);

// Função para retroceder para o elemento anterior da lista
void Lista_Retroceder(Lista *lista);

// Função para inserir um elemento na lista
void Lista_Inserir(Lista *lista, void *dados);

// Função para remover o último elemento da lista
void Lista_RemoverUltimo(Lista *lista);

// Função para remover um elemento específico da lista
void Lista_RemoverElemento(Lista *lista, ElementoLista *elementoARemover);

// Função para esvaziar a lista
void Lista_Esvaziar(Lista *lista);

// Função para obter o endereço do elemento
void ElementoLista_Dump(ElementoLista *elemento, size_t tamanhoDados);

// Função para obter os dados do elemento
byte *ElementoLista_ObterDados(ElementoLista *elemento);

// Função para obter o elemento anterior
void *ElementoLista_ObterAnterior(ElementoLista *elemento);

// Função para obter o próximo elemento
void *ElementoLista_ObterProximo(ElementoLista *elemento);

// Função para atribuir dados ao elemento
void ElementoLista_AtribuirDados(ElementoLista *elemento, byte *dados);

// Função para atribuir o elemento anterior
void ElementoLista_AtribuirAnterior(ElementoLista *elemento, ElementoLista *anterior);

// Função para atribuir o próximo elemento
void ElementoLista_AtribuirProximo(ElementoLista *elemento, ElementoLista *proximo);

#endif // _LISTALIB_H