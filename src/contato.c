#include "contato.h"
#include "agenda.h"
#include "telefones.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

Contato *cria_contato(char *nome, char *sobrenome, char *telefone, char *email,
                      char *cargo, char *empresa, char *observacoes)
{
    Contato *novo = (Contato *)malloc(sizeof(Contato));
    novo->nome = nome;
    novo->sobrenome = sobrenome;
    novo->cargo = cargo;
    novo->email = email;
    novo->empresa = empresa;
    novo->observacoes = observacoes;
    novo->telefones = NULL;
    novo->ntelefones = 0;
    add_tel(novo, telefone);
    novo->proximo = NULL;
    novo->anterior = NULL;

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
        free(contato->nome);
        contato->nome = novo;
        break;
    case SOBRENOME:
        free(contato->sobrenome);
        contato->sobrenome = novo;
        break;
    case CARGO:
        free(contato->cargo);
        contato->cargo = novo;
        break;
    case EMAIL:
        free(contato->email);
        contato->email = novo;
        break;
    case EMPRESA:
        free(contato->empresa);
        contato->empresa = novo;
        break;
    case OBSERVACOES:
        free(contato->observacoes);
        contato->observacoes = novo;
        break;
    default:
        break;
    }

    return contato;
}
