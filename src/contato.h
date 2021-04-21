// Aluno: João Victor Otto
// Matricula: 202000180
//
// A estrutura contato armazena as informações individuais dos contatos
// As funções implementadas são: criar, excluir e editar contatos

#ifndef CONTATO_H
#define CONTATO_H

#include <stdint.h>

typedef struct telefones Telefones;

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
    const char *nome;
    const char *cargo;
    const char *email;
    const char *empresa;
    const char *observacoes;
    const char *sobrenome;
} Contato;

// Cria um novo contato
// Deve ser inicializado com nome e sobrenome, os demais campos são opcionais
// @param nopcionais quantidade de campos opcionais que serão preenchidos
// @param campos_opcionais quais campos opcionais serão preenchidos
Contato *cria_contato(const char *nome, const char *telefone,
                      uint8_t nopcionais, const Campos campos_opcionais[], ...);

// Limpa a memoria alocada pelo contato
void exclui_contato(Contato *contato);

// Edita as informações de um contato
Contato *edita_contato(Contato *contato, uint8_t ncampos, const Campos campos[],
                       ...);

#endif
