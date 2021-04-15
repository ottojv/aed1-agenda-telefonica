/**
 * @author Joao Victor Otto
 *
 * @brief A agenda é uma implementação de listas duplamente encadeadas que
 * armazenam os contatos
 *
 * A agenda possui funcionalidades basicas de listas:
 *      - Inicializar uma nova agenda
 *      - Inserção (Ordenada) / Remoção
 *      - Buscar elementos (por nome/sobrenome ou telefone)
 *
 * Alem disso sera possível importar arquivos .csv seguindo o padrão Google e
 * exportar para o mesmo formato
 */

#ifndef AGENDA_H
#define AGENDA_H

#include <stdint.h>

#include "contato.h"

typedef struct agenda_no {
  Contato *contato;
  Contato *prox;
  Contato *ant;
} Agenda_No;

typedef struct agenda {
  Agenda_No *contatos;
  uint16_t quant_contatos;
} Agenda;

/**
 * Aloca memoria e inicializa uma nova agenda
 */
Agenda *AgendaCria();

/**
 * Insere um novo contato na agenda, mantendo uma ordenação alfabetica pelo nome
 * A ordenação ira priorizar o primeiro nome, mas em caso de nomes iguais ira
 * utilizar o sobrenome para ordenacao
 */
void AgendaInsere(Agenda *agenda, Contato *novo);

/**
 * Remove o contato da agenda
 */
void AgendaRemove(Agenda *agenda, Contato *contato);

/**
 * Procura por um contato utilizando o nome como chave de busca
 */
Contato *AgendaProcuraNome(Agenda *agenda, const char *nome);

/**
 * Procura por um contato utilizando um numero de telefone como chave de busca
 */
Contato *AgendaProcuraTelefone(Agenda *agenda, const char *telefone);

/**
 * Importa um arquivo.csv seguindo o padrão Google
 */
void AgendaImporta(const char *arquivo);

/**
 * Exporta a agenda atual para um arquivo.csv
 */
void AgendaExporta(const char *arquivo);

/**
 * Libera a memoria alocada por cada contato e pela agenda
 */
void AgendaExclui();
#endif
