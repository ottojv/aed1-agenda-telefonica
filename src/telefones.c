#include "telefones.h"
#include "agenda.h"
#include "contato.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTABELA ((MAXTELEFONES) * (MAXAGENDA))

// Tabela hash usada na busca pelo contato ao qual um
// numero de telefone esta associado
// Alem da busca, a tabela sera usada quando o usuario ligar para um numero de
// telefone caso o numero pertença a um contato, o contato sera adicionado ao
// historico caso contrario apenas o numero de telefone sera adicionado no
// historico
static Contato *tabela[MAXTABELA];

// Retorna a chave hash associada há um numero de telefone
uint32_t hash(const char *numero);

// Insere um novo elemento na tabela hash
void ins_hash(Contato *contato, const char *numero);

// Remove um elemento da tabela hash
void rm_hash(const char *numero);

Telefones *cria_tel(const char *numero)
{
    Telefones *t = (Telefones *)malloc(sizeof(Telefones));
    t->numero = numero;
    t->proximo = NULL;

    return t;
}

void add_tel(Contato *contato, const char *numero)
{
    if (numero == NULL) {
        return;
    }

    Telefones *novo = cria_tel(numero);
    if (novo == NULL) {
        fprintf(stderr, "Erro ao criar novo telefone\n");
        return;
    }
    novo->proximo = contato->telefones;
    contato->telefones = novo;
    contato->ntelefones++;
    ins_hash(contato, numero);
}

void rm_tel(Contato *contato, const char *numero)
{
    Telefones *telefone = contato->telefones;
    Telefones *anterior = NULL;
    while (strcmp(telefone->numero, numero) != 0) {
        anterior = telefone;
        telefone = telefone->proximo;
    }

    if (telefone == NULL) {
        fprintf(stderr, "Numero não encontrado\n");
        return;
    }

    if (telefone == contato->telefones) {
        contato->telefones = telefone->proximo;
    } else {
        anterior->proximo = telefone->proximo;
    }
    contato->ntelefones--;
    rm_hash(numero);

    free(telefone);
}

Contato *busca_tel(const char *numero)
{
    uint32_t chave = hash(numero);
    while (tabela[chave] != NULL) {
        // Procura nos telefones do contato para verificar se é o contato certo
        Telefones *atual = tabela[chave]->telefones;
        while (atual != NULL) {
            // se o numero atual for igual ao procurado
            if (!strcmp(atual->numero, numero)) {
                return tabela[chave];
            }
            atual = atual->proximo;
        }
        chave++;
    }

    return NULL;
}

uint32_t hash(const char *numero)
{
    const unsigned int kSmallPrime = 16487;
    const unsigned int kLargePrime = 327673;
    uint32_t result = 0;
    for (const char *ch = numero; *ch != '\0'; ch++) {
        result = (result * kSmallPrime + *ch) % kLargePrime;
    }

    return result;
}

void ins_hash(Contato *contato, const char *numero)
{
    uint32_t chave = hash(numero);
    // procura a proxima posição vazia a partir da chave
    while (tabela[chave] != NULL) {
        chave++;
    }
    tabela[chave] = contato;
}

void rm_hash(const char *numero)
{
    uint32_t chave = hash(numero);
    while (tabela[chave] != NULL) {
        // Procura nos telefones do contato para verificar se é o contato certo
        Telefones *atual = tabela[chave]->telefones;
        while (atual != NULL) {
            // se o numero atual for igual ao procurado
            if (atual->numero && !strcmp(atual->numero, numero)) {
                // remove a entrada da tabela
                tabela[chave] = NULL;
                return;
            }
            atual = atual->proximo;
        }
        chave++;
    }
}

void exclui_telefones(Contato *contato)
{
    Telefones *lista = contato->telefones;
    Telefones *proximo = NULL;
    while (lista != NULL) {
        rm_hash(lista->numero);
        if (lista->proximo != NULL) {
            proximo = lista->proximo;
            free(lista);
            lista = proximo;
            contato->telefones = lista;
        } else {
            free(lista);
            lista = NULL;
        }
    }
}

char *string_telefones(Contato *contato)
{
    char *string = NULL;
    if (contato->telefones == NULL) {
        string = (char *)malloc(sizeof(char) * 2);
        strcpy(string, "");
    }
    const unsigned int tam_tel = 15;  // tamanho maximo
    const unsigned int tam_delim = 5; // " ::: "     5 caracteres

    if (contato->ntelefones == 1) {
        // Apenas um numero de telefone nao precisa do delimitador " ::: "
        string = (char *)malloc((tam_tel + 1) * sizeof(char));
    } else {
        string = (char *)malloc(
            (tam_tel * contato->ntelefones + tam_delim * contato->ntelefones) *
            sizeof(char));
    }
    string[0] = '\0'; // para garantir que o strcat ira concatenar na pos certa
    string[(tam_tel * contato->ntelefones) +
           (tam_delim * (contato->ntelefones - 1))] = '\0';

    Telefones *lista = contato->telefones;
    while (lista != NULL) {
        strcat(string, lista->numero);
        if (lista->proximo != NULL) {
            strcat(string, " ::: ");
        }
        lista = lista->proximo;
    }
    return string;
}
