/**
 * @file contato.h
 * @author Joao Victor Otto
 * @brief Funcoes dos contatos 
 * 
 */

#ifndef CONTATO_H
#define CONTATO_H

typedef struct contato {
    char *nome;
    char *sobrenome;
    char *cargo;
    char *empresa;
    char *email;
    char *telefones;
    char *observacoes;
} Contato;

/**
 * @brief Aloca memoria e solicita informações da entrada padrão para criar um novo contato
 * 
 * @return struct Contato* ponteiro para o contato criado
 */
Contato *contato_init(char *nome, char *telefone, ...);

/**
 * @brief Libera a memoria alocada para um contato
 * 
 * @param cont o contato que sera excluido
 */
void contato_del(Contato *contato);

/**
 * @brief Edita as informações de um contato realocando a memoria se for preciso
 * 
 * @return struct Contato* ponteiro para o contato editado
 */
Contato *contato_edit();

/**
 * @brief Transforma as informações do contato em uma unica string
 * 
 * @return const char* o endereço de memoria da string que contem as informações do contato
 */
const char* contato_string();

#endif