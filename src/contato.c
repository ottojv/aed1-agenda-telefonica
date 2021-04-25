#include "contato.h"
#include "agenda.h"
#include "telefones.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

Contato *cria_contato(char *nome, char *telefone)
{
    Contato *novo = (Contato *)malloc(sizeof(Contato));
    novo->nome = nome;
    novo->telefones = inicia_tel();
    add_tel(novo, telefone);
    novo->sobrenome = NULL;
    novo->cargo = NULL;
    novo->email = NULL;
    novo->empresa = NULL;
    novo->observacoes = NULL;

    return novo;
}

void exclui_contato(Contato *contato)
{
    exclui_telefones(contato->telefones);
    free(contato->nome);
    free(contato->cargo);
    free(contato->email);
    free(contato->empresa);
    free(contato->observacoes);
    free(contato->sobrenome);
    free(contato);
}

Contato *edita_contato(Contato *contato, char *novo, Campos campo)
{
    switch (campo) {
    case NOME:
        contato->nome = novo;
        break;
    case SOBRENOME:
        contato->sobrenome = novo;
        break;
    case CARGO:
        contato->cargo = novo;
        break;
    case EMAIL:
        contato->email = novo;
        break;
    case EMPRESA:
        contato->empresa = novo;
        break;
    case OBSERVACOES:
        contato->observacoes = novo;
        break;
    default:
        break;
    }

    return contato;
}

void liga_contato(Agenda *agenda, Contato *contato)
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
    novo->entrada.contato = contato;
    novo->prox = agenda->historico->inicio;
    agenda->historico->inicio = novo;
}
