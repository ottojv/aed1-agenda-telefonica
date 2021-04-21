#include "contato.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "telefones.h"

// Os parametros nopcionais e campos_opcionais identificam a quant. e quais
// campos opcionais serão preenchidos ao criar o contato
// Se nenhum campo adicional for preenchido nopcionais deve ser 0
Contato *cria_contato(const char *nome, const char *telefone,
                      uint8_t nopcionais, const Campos campos_opcionais[], ...)
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

    // argumentos opcionais
    if (nopcionais > 0) {
        va_list args;
        va_start(args, campos_opcionais);
        while (nopcionais--) {
            switch (campos_opcionais[nopcionais]) {
            case SOBRENOME:
                novo->sobrenome = va_arg(args, const char *);
                break;
            case TELEFONES:
                add_tel(novo, va_arg(args, const char *));
                break;
            case CARGO:
                novo->cargo = va_arg(args, const char *);
                break;
            case EMAIL:
                novo->email = va_arg(args, const char *);
                break;
            case EMPRESA:
                novo->empresa = va_arg(args, const char *);
                break;
            case OBSERVACOES:
                novo->observacoes = va_arg(args, const char *);
                break;
            default:
                break;
            }
        }
        va_end(args);
    }

    return novo;
}

void exclui_contato(Contato *contato)
{
    free(contato);
}

// Similar à função cria_contato
// @param ncampos quantos campos serão editados
// @param campos quais campos serão editados
Contato *edita_contato(Contato *contato, uint8_t ncampos, const Campos campos[],
                       ...)
{
    // Função chamada sem informar os campos que serão editados
    if (ncampos == 0 || campos == NULL) {
        fprintf(stderr, "edita_contato chamada sem informar os"
                        " campos que serão editados\n"); 
        return NULL;
    }

    va_list args;
    va_start(args, campos);
    while (ncampos--) {
        switch (campos[ncampos]) {
        case NOME:
            contato->nome = va_arg(args, const char *);
            break;
        case SOBRENOME:
            contato->sobrenome = va_arg(args, const char *);
            break;
        case CARGO:
            contato->cargo = va_arg(args, const char *);
            break;
        case EMAIL:
            contato->email = va_arg(args, const char *);
            break;
        case EMPRESA:
            contato->empresa = va_arg(args, const char *);
            break;
        case OBSERVACOES:
            contato->observacoes = va_arg(args, const char *);
            break;
        default:
            break;
        }
    }

    return contato;
}
