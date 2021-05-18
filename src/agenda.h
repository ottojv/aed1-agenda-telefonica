// Aluno: João Victor Otto
// Matricula: 202000180
//
// A agenda armazena os contatos em listas circulares duplamente encadeadas
// e mantém um registro das chamadas realizadas, que são armazenadas em uma
// estrutura similar a filas porem de maneira invertida (inserção no inicio e
// remoção do fim);
// A inserção de contatos é feita mantendo a ordenação alfabetica dos nomes;
// A inserção no historico é feita sempre no inicio e quando este esta cheio, o
// ultimo elemento (mais antigo) é removido para gerar espaço;
// As funções nesta biblioteca são:
// criar agenda, adicionar e remover contatos na agenda, procurar contato por
// nome, ligar para contato (inserir entrada no historico), limpar agenda e
// exportar agenda

#ifndef AGENDA_H
#define AGENDA_H

#define MAXAGENDA UINT16_MAX
#define MAXTELEFONES 5 // Valor baixo para demonstar funcionalidade de fila
#define MAXCONTATOS UINT16_MAX

#include <stdint.h>

typedef struct contato Contato;

typedef struct historico Historico;
struct historico {
    // Uma entrada no historico pode ser um contato
    // ou um numero de telefone avulso
    union entrada {
        Contato *contato;
        const char *numero;
    } entrada;
    int conhecido;
    Historico *proximo;
    Historico *anterior;
};

typedef struct agenda {
    Contato *contatos;
    Historico *historico;
    unsigned int ncontatos;
    unsigned int maxcontatos;
    unsigned int nhistorico;
    unsigned int maxhistorico;
} Agenda;

// Aloca memoria e inicializa uma nova agenda
Agenda *cria_agenda();

// Cria e insere um novo contato na agenda
// a inserção é feita mantendo a ordenação alfabetica dos nomes
void add_contato(Agenda *agenda, Contato *novo);

// Remove um contato da agenda
Contato *rm_contato(Agenda *agenda, Contato *contato);

// Procura por um contato utilizando o nome como chave de busca
Contato *procura_nome(Agenda *agenda, const char *nome, const char *sobrenome);

// Importa um arquivo.csv seguindo o padrão Google
// Não importa corretamente se algum contato tiver campos com tags de tipo
// efinida Ex.: Telefone - residencial / trabalho / fax / etc
// Pode ser chamada passando NULL para agenda, nesse caso uma nova agenda é
// criada
Agenda *importar_agenda(Agenda *agenda, const char *arquivo);

// Exporta a agenda atual para um arquivo.csv
void exportar_agenda(Agenda *agenda, const char *arquivo);

// Libera a memoria alocada pela agenda, incluindo contatos e historico
void exclui_agenda(Agenda *agenda);

// Realiza uma ligação para um contato, adicionando uma referencia no historico
// Caso o historico esteja cheio a referencia mais antiga é removida
void ligar(Agenda *agenda, const char *numero);
#endif
