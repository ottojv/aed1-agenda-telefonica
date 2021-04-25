#include "agenda.h"
#include "contato.h"
#include "telefones.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX (1024);

Agenda *cria_agenda()
{
    return &(Agenda){NULL, NULL, 0, UINT16_MAX};
}

void add_contato(Agenda *agenda, Contato *contato)
{
    // Agenda vazia
    if (agenda->inicio == NULL) {
        agenda->inicio = (NoAgenda *)malloc(sizeof(NoAgenda));
        agenda->inicio->contato = contato;
        agenda->inicio->prox = NULL;
        agenda->inicio->ant = NULL;
        agenda->ncont++;
        return;
    }
    // Agenda cheia
    if (agenda->ncont == agenda->nmax) {
        puts("Erro ao adicionar novo contato, agenda está cheia");
        exclui_contato(contato);
        return;
    }
    NoAgenda *pos = agenda->inicio;
    while (strcmp(pos->prox->contato->nome, contato->nome) < 0) {
        pos = pos->prox;
    }
    if (contato->sobrenome != NULL) {
        while (pos->prox->contato->sobrenome != NULL &&
               strcmp(pos->prox->contato->nome, contato->nome) < 0) {
            pos = pos->prox;
        }
        NoAgenda *novo = (NoAgenda *)malloc(sizeof(NoAgenda));
        novo->contato = contato;
        pos->prox->ant = novo;
        novo->prox = pos->prox;
        novo->ant = pos;
        pos->prox = novo;
        agenda->ncont++;
    }
}

void rm_contato(Agenda *agenda, Contato *contato)
{
    NoAgenda *no = agenda->inicio;
    while (no->contato != contato) {
        no = no->prox;
    }
    no->ant->prox = no->prox;
    no->prox->ant = no->ant;
    exclui_contato(contato);
}

void exclui_agenda(Agenda *agenda)
{
    // Limpa os contatos
    NoAgenda *atual = agenda->inicio;
    while (atual != NULL) {
        exclui_contato(atual->contato);
        if (atual->prox != NULL) {
            atual = atual->prox;
            free(atual->ant);
        } else {
            free(atual);
        }
    }
    // Limpa o histórico
    for (NoHistorico *n = agenda->historico->inicio; n != NULL; n = n->prox) {
        free(n);
    }
    free(agenda);
}

void ver_historico(Agenda *agenda)
{
    for (NoHistorico *n = agenda->historico->inicio; n != NULL; n = n->prox) {
        if (n->entrada.contato != NULL) {
            printf("Nome: %s\n\n", n->entrada.contato->nome);
        } else {
            printf("Desconhecido: %s\n\n", n->entrada.numero);
        }
    }
}

void ligar(Agenda *agenda, const char *numero)
{
    if (agenda->historico == NULL) {
        agenda->historico = (Historico *)malloc(sizeof(Historico));
        if (agenda->historico == NULL) {
            puts("Erro ao alocar memoria para o historico\n");
            return;
        }
    }

    NoHistorico *novo = (NoHistorico *)malloc(sizeof(NoHistorico));
    if (novo == NULL) {
        puts("Erro ao alocar memoria para entrada no histórico\n");
        return;
    }
    // Verifica se o numero de telefone pertence a algum contato
    Contato *c = busca_tel(numero);
    if (c != NULL) {
        novo->entrada.contato = c;
    } else {
        novo->entrada.numero = numero;
    }
    novo->prox = agenda->historico->inicio;
    agenda->historico->inicio = novo;
}
