// Aluno: João Victor Otto
// Matricula: 202000180
//
// Os numeros de telefone de cada contato são armazenados como uma lista
// Esta biblioteca implementa funções para:
// criar novo telefone, adicionar e remover telefone da lista, buscar contato
// por numero de telefone (usando uma tabela hash), limpar memoria alocada por
// um telefone e transformar a lista em uma string

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
void exclui_telefones(Contato *contato);

// Retorna uma string com todos os telefones separados por " ::: "
char *string_telefones(Contato *contato);

#endif
