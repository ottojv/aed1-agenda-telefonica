// Aluno: João Victor Otto
// Matricula: 202000180
//
// Os numeros de telefones são armazenados como nós de uma lista
// permitindo que os contatos tenham multiplos numeros de telefone
// e abstraindo a implementação das funções de inserção, remoção e busca
#ifndef TELEFONES_H
#define TELEFONES_H

#include <stdint.h>

#define TELSIZ 15

typedef struct contato Contato;

typedef struct telefones Telefones;
struct telefones {
    const char *numero;
    Telefones *proximo;
};

// Aloca memoria e inicializa um nó de uma lista telefonica
Telefones *cria_tel(const char *numero);

// Insere um novo numero para um contato existente
void add_tel(Contato *contato, const char *numero);

// Remove um numero de telefone de um contato
void rm_tel(Contato *contato, const char *numero);

// Busca por um contato com um determinado numero de telefone
// Se o contato for encontrado retorna um ponteiro para o mesmo
// Caso nenhum contato seja encontrado retorna NULL
Contato *busca_tel(const char *numero);

// Libera a memoria alocada por uma lista de telefones
void exclui_telefones(Telefones *lista);

// Retorna uma string com todos os telefones separados por " ::: "
char *string_telefones(Contato* contato);

#endif
