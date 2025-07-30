#ifndef APP_TYPES_H
#define APP_TYPES_H

// Enum para os grupos de contatos
typedef enum {
    GRUPO_NENHUM,
    GRUPO_AMIGOS,
    GRUPO_FAMILIA,
    GRUPO_TRABALHO
} GrupoContato;

// Enum para gerenciar os estados da aplicação
typedef enum {
    ESTADO_LISTA,
    ESTADO_DETALHE,
    ESTADO_ADICIONAR,
    ESTADO_CONFIRMAR_DELETE
} EstadoAplicacao;

typedef struct {
    bool animando;
    float y_atual;
    float y_alvo;
    Uint32 tempo_inicio;
} EstadoAnimacao;

// Enum para as categorias de filtro da UI
typedef enum {
    FILTRO_TODOS, FILTRO_AMIGOS, FILTRO_FAMILIA, FILTRO_TRABALHO
} CategoriaFiltro;

// Enum para os campos do formulário de adição
typedef enum {
    CAMPO_NOME, CAMPO_TELEFONE, CAMPO_EMAIL, CAMPO_GRUPO, CAMPO_SALVAR, NUM_CAMPOS_FORM
} CampoFormulario;

// Estrutura para o formulário de adição de contato
typedef struct {
    char nome[100];
    char telefone[20];
    char email[100];
    GrupoContato grupo;
    int campoAtivo;
} FormularioContato;

#endif // APP_TYPES_H