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

void Lista_Criar(Lista *lista, size_t tamanhoElemento, bool duplamenteEncadeada, bool circular);
void Lista_Destruir(Lista *lista);
ElementoLista *Lista_ObterPrimeiro(Lista *lista);
ElementoLista *Lista_ObterUltimo(Lista *lista);
ElementoLista *Lista_ObterAtual(Lista *lista);
size_t Lista_ObterTamanhoTotal(Lista *lista);
void Lista_ExibirTamanhoTotal(Lista *lista);
void Lista_IrParaPrimeiro(Lista *lista);
void Lista_IrParaUltimo(Lista *lista);
void Lista_PercorrerDiretamente(Lista *lista, unsigned int numVezesParaPercorrer);
void Lista_PercorrerInversamente(Lista *lista, unsigned int numVezesParaPercorrer);
void Lista_DumpParametros(Lista *lista);
void Lista_Avancar(Lista *lista);
void Lista_Retroceder(Lista *lista);
void Lista_Inserir(Lista *lista, void *dados);
void Lista_RemoverUltimo(Lista *lista);
void Lista_RemoverElemento(Lista *lista, ElementoLista *elementoARemover); // Nova função
void Lista_Esvaziar(Lista *lista);

void ElementoLista_Dump(ElementoLista *elemento, size_t tamanhoDados);
byte *ElementoLista_ObterDados(ElementoLista *elemento);
void *ElementoLista_ObterAnterior(ElementoLista *elemento);
void *ElementoLista_ObterProximo(ElementoLista *elemento);
void ElementoLista_AtribuirDados(ElementoLista *elemento, byte *dados);
void ElementoLista_AtribuirAnterior(ElementoLista *elemento, ElementoLista *anterior);
void ElementoLista_AtribuirProximo(ElementoLista *elemento, ElementoLista *proximo);

#endif // _LISTALIB_H