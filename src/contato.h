// Aluno: João Victor Otto
// Matricula: 202000180
//
// A estrutura contato armazena as informações individuais dos contatos
// As funções implementadas são: criar, excluir e editar contatos

#ifndef CONTATO_H
#define CONTATO_H

#include <stdint.h>

typedef struct telefones Telefones;
typedef struct agenda Agenda;

typedef enum Campos {
    NOME,
    TELEFONES,
    SOBRENOME,
    CARGO,
    EMAIL,
    EMPRESA,
    OBSERVACOES
} Campos;

typedef struct contato {
    Telefones *telefones;
    char *nome;
    char *cargo;
    char *email;
    char *empresa;
    char *observacoes;
    char *sobrenome;
} Contato;

// Cria um novo contato
// Deve ser inicializado com nome e sobrenome
Contato *cria_contato(char *nome, char *telefone);

// Limpa a memoria alocada pelo contato
void exclui_contato(Contato *contato);

// Edita as informações de um contato
Contato *edita_contato(Contato *contato, char *novo, Campos campos);

// Realiza uma ligação para um contato adicionando o contato no historico
void liga_contato(Agenda *agenda, Contato *contato);

#endif
