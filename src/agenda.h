/**
 * @file lista.h
 * @author Joao Victor Otto
 * @brief Funcoes das agendas (listas de contatos)
 * 
 */
#ifndef AGENDA_H
#define AGENDA_H

#include "contato.h"
#include <stdio.h>

/**
 * @brief Lista circular de contatos
 * 
 */
typedef struct agenda
{
    Contato *contato;
    Contato *prox;
    Contato *ant;
} Agenda;

/**
 * @brief Aloca e inicializa uma nova lista de contatos
 * 
 * @return Agenda* endereço do inicio da agenda
 */
Agenda *agenda_init();

/**
 * @brief Exporta os dados salvos na agenda para um arquivo permanente em
 * formato .csv seguindo o padrão Google
 * 
 */
void agenda_exporta();

/**
 * @brief Importa os dados de uma agenda .csv para que possam ser editadas pelo programa
 * 
 */
void agenda_importa(FILE agendacsv);

/**
 * @brief Insere um novo contato na agenda, mantendo uma ordenação alfabetica pelo nome
 * A ordenação ira priorizar o primeiro nome, mas em caso de nomes iguais ira
 * utilizar o sobrenome para ordenacao
 * 
 * @param agenda a agenda na qual o contato sera inserido
 * @param contato o contato que sera inserido
 */
void agenda_insere(Agenda *agenda, Contato *contato);

/**
 * @brief 
 * 
 * @param agenda 
 * @param contato 
 */
void agenda_remove(Agenda *agenda, Contato *contato);

/**
 * @brief Busca por algum contato com o nome dado
 * @param agenda a agenda que sera utilizada para buscar o contato
 * @param nome o nome que sera procurado na agenda
 */
Contato *agenda_procura_nome(Agenda *agenda, const char *nome);

/**
 * @brief Busca por algum contato com o numero de telefone dado
 * 
 * @param agenda a agenda que sera utilizada para buscar o contato
 * @param telefone o numero de telefone que sera procurado
 */
Contato *agenda_procura_telefone(Agenda *agenda, const char *telefone);

#endif