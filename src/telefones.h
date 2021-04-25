// Aluno: João Victor Otto
// Matricula: 202000180
//
// Os numeros de telefones são armazenados como nós de uma lista
// permitindo que os contatos tenham multiplos numeros de telefone
// e abstraindo a implementação das funções de inserção, remoção e busca
#ifndef TELEFONES_H
#define TELEFONES_H

#define MAXTEL (UINT8_MAX)

#include <stdint.h>

typedef struct contato Contato;

typedef struct notelefone NoTelefone;
struct notelefone {
    char *numero;
    NoTelefone *prox;
    NoTelefone *ant;
};

typedef struct telefones Telefones;
struct telefones {
    NoTelefone *inicio;
    uint8_t ntel;
    const uint8_t nmax;
};

// Aloca memoria e inicializa uma nova lista de telefones
Telefones *inicia_tel();

// Insere um novo numero para um contato existente
void add_tel(Contato *contato, char *numero);

// Remove um numero de telefone de um contato
void rm_tel(Contato *contato, const char *numero);

// Retorna true se a lista de telefones estiver cheia e false caso contrario
uint8_t lista_cheia(Telefones *lista);

// Busca por um contato com um determinado numero de telefone
// Se o contato for encontrado retorna um ponteiro para o mesmo
// Caso nenhum contato seja encontrado retorna NULL
Contato *busca_tel(const char *numero);

// Libera a memoria alocada por uma listqa de telefones
void exclui_telefones(Telefones *lista);

#endif
