#include "telefones.h"
#include "agenda.h"
#include "contato.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTABELA ((MAXTEL) * (MAXAGENDA))

// Tabela hash usada na busca pelo contato ao qual um
// numero de telefone esta associado
// A tabela sera usada quando o usuario ligar para um numero de telefone
// caso o numero pertença a um contato, o contato sera adicionado ao historico
// caso contrario apenas o numero de telefone sera adicionado no historico
static Contato *tabela[MAXTABELA];
// Retorna a chave hash associada há um numero de telefone
uint32_t hash(const char *numero);
// Insere um novo elemento na tabela hash
void ins_hash(Contato *contato, const char *numero);
// Remove um elemento da tabela hash
void rm_hash(const char *numero);

Telefones *inicia_tel()
{
    return &(Telefones){NULL, 0, MAXTEL};
}

void add_tel(Contato *contato, char *numero)
{
    // Se não existir lista de telefones inicializada
    if (contato->telefones == NULL) {
        contato->telefones = inicia_tel();
        if (contato->telefones == NULL) {
            return;
        }
    }
    Telefones *lista = contato->telefones;
    // Verifica se a lista ja esta cheia
    if (lista->ntel == lista->nmax) {
        puts("Não foi possível adicionar um novo numero"
             ", pois a lista de telefones ja esta cheia\n\n");
        return;
    }

    // Verifica se ja existe contato com o numero
    Contato *c = busca_tel(numero);
    if (c != NULL) {
        printf("Não foi possível adicionar o número %s ao contato %s\n", numero,
               contato->nome);
        printf("O numero ja esta associado a %s\n\n\n", c->nome);
        return;
    }

    // Inicia um novo nó na lista de contatos e o insere no inicio da lista
    NoTelefone *novo = (NoTelefone *)malloc(sizeof(NoTelefone));
    if (novo == NULL) {
        puts("Não foi possível alocar memoria para o telefone\n\n");
        return;
    }
    ins_hash(contato, numero);
    novo->numero = numero;
    novo->ant = NULL;
    novo->prox = lista->inicio;
    lista->inicio = novo;
    lista->ntel++;
    if (novo->prox != NULL) {
        novo->prox->ant = novo;
    }
}

void rm_tel(Contato *contato, const char *numero)
{
    NoTelefone *atual = contato->telefones->inicio;
    while (strcmp(atual->numero, numero) != 0) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        puts("Numero não encontrado");
        return;
    }

    rm_hash(numero);
    contato->telefones->ntel--;
    if (atual == contato->telefones->inicio) {
        contato->telefones->inicio = atual->prox;
    }
    if (atual->prox != NULL) {
        atual->prox->ant = atual->ant;
    }
    if (atual->ant != NULL) {
        atual->ant->prox = atual->prox;
    }

    free(atual);
}

uint8_t lista_cheia(Telefones *lista)
{
    return lista->ntel == lista->nmax;
}

Contato *busca_tel(const char *numero)
{
    uint32_t chave = hash(numero);
    while (tabela[chave] != NULL) {
        // Procura nos telefones do contato para verificar se é o contato certo
        NoTelefone *atual = tabela[chave]->telefones->inicio;
        while (atual != NULL) {
            // se o numero atual for igual ao procurado
            if (!strcmp(atual->numero, numero)) {
                return tabela[chave];
            }
            atual = atual->prox;
        }
        chave++;
    }

    return NULL;
}

uint32_t hash(const char *numero)
{
    const unsigned int kSmallPrime = 16487;
    const unsigned int max = MAXTABELA;
    uint32_t result = 0;
    for (const char *ch = numero; *ch != '\0'; ch++) {
        result = (result * kSmallPrime + *ch) % max;
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
        NoTelefone *atual = tabela[chave]->telefones->inicio;
        while (atual != NULL) {
            // se o numero atual for igual ao procurado
            if (!strcmp(atual->numero, numero)) {
                // remove a entrada da tabela
                tabela[chave] = NULL;
            }
            atual = atual->prox;
        }
        chave++;
    }
}

void exclui_telefones(Telefones *lista)
{
    NoTelefone *atual = lista->inicio;
    while (atual != NULL) {
        if (atual->prox != NULL) {
            atual = atual->prox;
            free(atual->ant);
        } else {
            free(atual);
        }
    }
    free(lista);
}

int verifica_telefone(char *tel)
{
    unsigned int len = strlen(tel);
    unsigned int maxlen = 10; // xxxxx-xxx
    unsigned int minlen = 9;  // xxxxxyyyy
    if (len > maxlen || len <= minlen) {
        return 0;
    }
    for (unsigned int i = 0; i < len; ++i) {
        if (i == 5 && len == maxlen && tel[i] != '-') {
            return 0;
        }
        if (!isdigit(tel[i])) {
            return 0;
        }
    }

    return 1;
}

char *string_telefones(Telefones *lista)
{
    const unsigned int size_tel = 10; // xxxxx-xxxx -> 10 caracteres
    const unsigned int size_del = 5;  // " ::: " -> 5 caracteres
    if (lista == NULL || lista->ntel == 0) {
        return "";
    }

    // Apenas um numero de telefone nao precisa do separador " ::: "
    char *string;
    if (lista->ntel == 1) {
        string = (char *)malloc((size_tel + 1) * sizeof(char));
    } else {
        string = (char *)malloc(
            (size_tel * lista->ntel + size_del * lista->ntel) * sizeof(char));
    }
    string[0] = '\0';
    string[size_tel * lista->ntel + size_del * lista->ntel - 1] = '\0';

    NoTelefone *tel = lista->inicio;
    while (tel != NULL) {
        // Transforma telefones da forma xxxxxyyyy para xxxxx-xxxx
        if (strlen(tel->numero) == 9) {
            char inicio[6]; // Cinco primeiros caracteres
            strncpy(inicio, tel->numero, 5);
            inicio[5] = '\0';
            strcat(string, inicio);
            strcat(string, "-");
            // Ultimos 4 caracteres
            strncat(string, strtok(tel->numero, inicio), 4);
        } else {
            strcat(string, tel->numero);
        }
        if (tel->prox != NULL) {
            strcat(string, " ::: ");
        }
        tel = tel->prox;
    }
    return string;
}
