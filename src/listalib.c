#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listalib.h"

void Lista_Criar(Lista *lista, size_t tamanhoElemento, bool duplamenteEncadeada, bool circular) {
    if(lista->criada) {
        printf("Lista já existe.\n");
        return;
    }
    lista->criada = true;
    lista->duplamenteEncadeada = duplamenteEncadeada;
    lista->circular = circular;
    lista->numElementos = 0;
    lista->tamanhoElemento = tamanhoElemento;
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    lista->atual = NULL;
    printf("Lista criada.\n");
}

void Lista_Destruir(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return;
    }
    Lista_Esvaziar(lista);
    memset(lista, 0, sizeof(Lista));
    printf("Lista destruída.\n");
}

ElementoLista *Lista_ObterPrimeiro(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return NULL;
    }
    return lista->primeiro;
}

ElementoLista *Lista_ObterUltimo(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return NULL;
    }
    return lista->ultimo;
}

ElementoLista *Lista_ObterAtual(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return NULL;
    }
    return lista->atual;
}

size_t Lista_ObterTamanhoTotal(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return 0;
    }
    return sizeof(Lista) + lista->numElementos*(sizeof(ElementoLista) + lista->tamanhoElemento);
}

void Lista_ExibirTamanhoTotal(Lista *lista) {
    printf("Tamanho total da lista em bytes\n");
    printf("(estruturas da lista e dos elementos +\n");
    printf("áreas de dados de cada um) : %4lu\n\n", Lista_ObterTamanhoTotal(lista));
}

void Lista_IrParaPrimeiro(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return;
    }
    lista->atual = lista->primeiro;
}

void Lista_IrParaUltimo(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return;
    }
    lista->atual = lista->ultimo;
}

void Lista_PercorrerDiretamente(Lista *lista, unsigned int numVezesParaPercorrer) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return;
    }
    if(!lista->numElementos) {
        printf("Lista vazia.\n\n");
        return;
    }

    int dadosInteiro;
    printf("Percorrendo a lista do início ao fim, %u vezes...\n", numVezesParaPercorrer);
    
    Lista_IrParaPrimeiro(lista);
    for(unsigned int i = 1; i <= numVezesParaPercorrer; i++) {
        memcpy(&dadosInteiro, ElementoLista_ObterDados(lista->atual), sizeof dadosInteiro);
        printf("Lista[%d] : %d\n", i, dadosInteiro);
        Lista_Avancar(lista);
    }
    printf("\n");
    printf("----------------------------------------------------\n\n");
}

void Lista_PercorrerInversamente(Lista *lista, unsigned int numVezesParaPercorrer) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return;
    }
    if(!lista->duplamenteEncadeada) {
        printf("Lista não é duplamente encadeada. Impossível percorrer inversamente.\n\n");
        return;
    }
    if(!lista->numElementos) {
        printf("Lista vazia.\n\n");
        return;
    }

    int dadosInteiro;
    printf("Percorrendo a lista do fim ao início, %u vezes...\n", numVezesParaPercorrer);

    Lista_IrParaUltimo(lista);
    for(unsigned int i = 1; i <= numVezesParaPercorrer; i++) {
        memcpy(&dadosInteiro, ElementoLista_ObterDados(lista->atual), sizeof dadosInteiro);
        printf("Lista[%d] : %d\n", i, dadosInteiro);
        Lista_Retroceder(lista);
    }
    printf("\n");
    printf("----------------------------------------------------\n\n");
}

void Lista_Avancar(Lista *lista) {
    if(!lista || !lista->criada || !lista->atual) {
        return;
    }
    lista->atual = lista->atual->proximo;
}

void Lista_Retroceder(Lista *lista) {
    if(!lista || !lista->criada || !lista->atual || !lista->duplamenteEncadeada) {
        return;
    }
    lista->atual = lista->atual->anterior;
}

void Lista_DumpParametros(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return;
    }
    printf("\nInformações sobre a lista:\n");
    printf("Duplamente encadeada           : %s\n", lista->duplamenteEncadeada? "sim" : "não");
    printf("Circular                       : %s\n", lista->circular? "sim" : "não");
    printf("Número de elementos            : %d\n", lista->numElementos);
    printf("Tamanho do elemento em bytes   : %lu\n", lista->tamanhoElemento);
    printf("End. do primeiro elemento      : 0x%016lx\n", (long unsigned int)lista->primeiro);
    printf("End. do último elemento        : 0x%016lx\n", (long unsigned int)lista->ultimo);
    printf("End. do elemento atual         : 0x%016lx\n\n", (long unsigned int)lista->atual);

    if(lista->numElementos) {
        Lista_IrParaPrimeiro(lista);
        for(unsigned int i = 1; i <= lista->numElementos; ++i) {
            printf("Detalhes do elemento %d:\n", i);
            ElementoLista_Dump(lista->atual, lista->tamanhoElemento);
            Lista_Avancar(lista);
        }
    }

    Lista_ExibirTamanhoTotal(lista);
    printf("----------------------------------------------------\n\n");
}

void Lista_Inserir(Lista *lista, void *dados) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return;
    }

    ElementoLista *elemento; byte *elemento_dados;
    if(!(elemento = malloc(sizeof(ElementoLista)))) {
        printf("Erro ao alocar memória para o novo elemento. Saindo...\n");
        exit(EXIT_FAILURE);
    }
    if(!(elemento_dados = malloc(lista->tamanhoElemento))) {
        printf("Erro ao alocar memória para os dados. Saindo...\n");
        free(elemento);
        exit(EXIT_FAILURE);
    }

    ElementoLista_AtribuirDados(elemento, elemento_dados);
    memcpy(elemento_dados, dados, lista->tamanhoElemento);

    if(!(lista->numElementos)) {
        if (lista->circular) {
            ElementoLista_AtribuirAnterior(elemento, elemento);
            ElementoLista_AtribuirProximo(elemento, elemento);
        } else {
            ElementoLista_AtribuirAnterior(elemento, NULL);
            ElementoLista_AtribuirProximo(elemento, NULL);
        }
        lista->primeiro = elemento;
        lista->ultimo = elemento;
    }
    else {
        if(lista->duplamenteEncadeada && lista->circular) ElementoLista_AtribuirAnterior(lista->primeiro, elemento);
        ElementoLista_AtribuirProximo(lista->ultimo, elemento);
        ElementoLista_AtribuirAnterior(elemento, lista->duplamenteEncadeada? lista->ultimo : NULL);
        ElementoLista_AtribuirProximo(elemento, lista->circular? lista->primeiro : NULL);
        lista->ultimo = elemento;
    }
    
    lista->atual = elemento;
    lista->numElementos++;
    // printf("Elemento inserido.\n");
}

void Lista_RemoverUltimo(Lista *lista) {
    if(!lista->criada) {
        printf("Lista não existe.\n");
        return;
    }

    if(lista->numElementos) {
        ElementoLista *antigoUltimo = lista->ultimo;
        
        if(lista->numElementos > 1) {
            lista->ultimo = antigoUltimo->anterior;
            lista->ultimo->proximo = lista->circular ? lista->primeiro : NULL;
            if(lista->circular) {
                lista->primeiro->anterior = lista->ultimo;
            }
        }
        else {
            lista->primeiro = NULL;
            lista->ultimo = NULL;
            lista->atual = NULL;
        }

        free(ElementoLista_ObterDados(antigoUltimo));
        free(antigoUltimo);
        lista->numElementos--;
        printf("Último elemento removido.\n");
    }
    else printf("Lista vazia.\n");
}


void Lista_RemoverElemento(Lista *lista, ElementoLista *elementoARemover) {
    if (!lista || !elementoARemover || lista->numElementos == 0) {
        return;
    }

    if (lista->numElementos == 1) {
        lista->primeiro = NULL;
        lista->ultimo = NULL;
        lista->atual = NULL;
    } 
    else {
        ElementoLista *anterior = elementoARemover->anterior;
        ElementoLista *proximo = elementoARemover->proximo;
        
        anterior->proximo = proximo;
        proximo->anterior = anterior;

        if (lista->primeiro == elementoARemover) {
            lista->primeiro = proximo;
        }
        if (lista->ultimo == elementoARemover) {
            lista->ultimo = anterior;
        }
    }
    
    free(elementoARemover->dados);
    free(elementoARemover);
    lista->numElementos--;
    // printf("Elemento removido.\n");
}


void Lista_Esvaziar(Lista *lista) {
    if(!lista || !lista->criada) {
        return;
    }
    if(!lista->numElementos) {
        return;
    }

    ElementoLista* atual = lista->primeiro;
    ElementoLista* proximo;
    do {
        proximo = atual->proximo;
        free(ElementoLista_ObterDados(atual));
        free(atual);
        atual = proximo;
    } while(atual != lista->primeiro);

    lista->numElementos = 0;
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    lista->atual = NULL;
    printf("Lista esvaziada.\n");
}

void ElementoLista_Dump(ElementoLista *elemento, size_t tamanhoDados) {
    byte *p = elemento->dados;
    printf("Endereço                       : 0x%016lx\n", (long unsigned int)elemento);
    printf("Endereço do bloco de dados     : 0x%016lx\n", (long unsigned int)elemento->dados);
    printf("Endereço do elemento anterior  : 0x%016lx\n", (long unsigned int)elemento->anterior);
    printf("Endereço do próximo elemento   : 0x%016lx\n", (long unsigned int)elemento->proximo);

    for(size_t i = 0; i < tamanhoDados; ++i) {
        printf("0x%016lx | %02x | %03d | %c\n", (long unsigned int)p, *p, *p, *p);
        ++p;
    }
    printf("\n");
}

byte *ElementoLista_ObterDados(ElementoLista *elemento) {
    return elemento ? elemento->dados : NULL;
}

void *ElementoLista_ObterAnterior(ElementoLista *elemento) {
    return elemento ? elemento->anterior : NULL;
}

void *ElementoLista_ObterProximo(ElementoLista *elemento) {
    return elemento ? elemento->proximo : NULL;
}

void ElementoLista_AtribuirDados(ElementoLista *elemento, byte *dados) {
    if(elemento) elemento->dados = dados;
}

void ElementoLista_AtribuirAnterior(ElementoLista *elemento, ElementoLista *anterior) {
    if(elemento) elemento->anterior = anterior;
}

void ElementoLista_AtribuirProximo(ElementoLista *elemento, ElementoLista *proximo) {
    if(elemento) elemento->proximo = proximo;
}