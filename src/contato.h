// João Victor Otto
//
// Contatos são armazenados em structs
// nas quais cada campo armazena uma informação
//
// É possivel criar, excluir e editar contatos

#ifndef CONTATO_H
#define CONTATO_H

#include <stdint.h>

typedef enum Campos
{
    NOME,
    TELEFONES,
    SOBRENOME,
    CARGO,
    EMAIL,
    EMPRESA,
    OBSERVACOES
} Campos;

typedef struct Contato
{
    const char *nome;
    const char *telefones;
    const char *sobrenome;
    const char *cargo;
    const char *email;
    const char *empresa;
    const char *observacoes;
} Contato;

// Cria um novo contato
// Deve ser inicializado com nome e sobrenome, os demais campos são opcionais
Contato *CriaContato(const char *nome, const char *telefone,
                     uint8_t quantOpcionais, const Campos camposOpcionais[],
                     ...);

// Limpa a memoria alocada pelo contato
void ExcluiContato(Contato *contato);

// Possibilita editar as informações de um contato
Contato *EditaContato(Contato *contato, uint8_t quant_campos,
                      const Campos campos[], ...);
#endif
