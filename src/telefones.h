#ifndef TELEFONES_H
#define TELEFONES_H

typedef struct telefones
{
    char *numero;

} Telefones;

/**
 * @brief Adiciona um novo numero de telefone para um contato existente
 * 
 */
void telefones_insere(void *contato, char *numero);

#endif