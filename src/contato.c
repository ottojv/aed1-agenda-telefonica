#include "contato.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "telefones.h"

// o ottojv TODO arrumar telefone para possibilitar multiplos telefones
// Os parametros
// identificar quantos e quais campos opcionais serão preenchidos ao criar o contato
// Se nenhum campo adicional for preenchido quant_opcionais deve ser <= 0
Contato *CriaContato(const char *nome, const char *telefone,
                     uint8_t quantOpcionais, const Campos camposOpcionais[],
                     ...)
{
    Contato *contato = (Contato *)malloc(sizeof(Contato));
    contato->nome = nome;
    contato->telefones = telefone;
    contato->sobrenome = NULL;
    contato->cargo = NULL;
    contato->email = NULL;
    contato->empresa = NULL;
    contato->observacoes = NULL;

    // argumentos opcionais
    if (quantOpcionais > 0) {
        va_list args;
        va_start(args, camposOpcionais);
        while (quantOpcionais--) {
            switch (camposOpcionais[quantOpcionais]) {
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
        va_end(args);
    }

    return contato;
}

void ExcluiContato(Contato *contato)
{
    free(contato);
}

// Similar à função CriaContato
// o parametro campos é utilizado para identificar quais campos serão alterados
Contato *EditaContato(Contato *contato, uint8_t quant_campos,
                      const Campos campos[], ...)
{
    // Função chamada sem informar os campos que serão editados
    if (quant_campos == 0 || campos == NULL) {
        fprintf(stderr, "EditaContato chamada sem informar os"
                        " campos que serão editados\n"); 
        return NULL;
    }

    va_list args;
    va_start(args, campos);
    while (quant_campos--) {
        switch (campos[quant_campos]) {
        case NOME:
            contato->nome = va_arg(args, const char *);
            break;
        case TELEFONES:
            contato->telefones = va_arg(args, const char *);
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
        }
    }

    return contato;
}
