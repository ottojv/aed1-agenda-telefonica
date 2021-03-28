#include <lista.h>
#include <stdlib.h>

Lista *lista_cria()
{
    Lista *lista = (Lista *)malloc(sizeof(Lista));
    if (lista != NULL)
    {

        lista->cabeca = NULL;
        lista->tamanho = 0;
    }

    return lista;
}

ListaNo *lista_no_cria(void *elemento)
{
    ListaNo *no = (ListaNo *)malloc(sizeof(ListaNo));
    if (no != NULL)
    {
        no->elemento = elemento;
        no->ant = NULL;
        no->prox = NULL;
    }

    return no;
}

void lista_insere(Lista *lista, void *elemento)
{
    ListaNo *novo = lista_no_cria(elemento);
    ListaNo *atual = lista->cabeca;

    // insere ordenadamente
    while (atual->prox != NULL)
    {
        if (atual->compara(atual, novo) < 0 && atual->prox->compara(atual, novo) >= 0)
        {
            novo->prox = atual->prox;
            novo->ant = atual;
            atual->prox = novo;
            novo->prox->ant = novo;

            break;
        }

        if (atual->prox == NULL)
        {
            atual->prox = novo;
            novo->ant = atual;
        }
    }
}

void lista_remove(Lista *lista, void *elemento)
{
    ListaNo *atual = lista->cabeca;

    while (atual != NULL)
    {
        if (atual->elemento == elemento)
        {
            atual->ant->prox = atual->prox;
            if (atual->prox != NULL)
            {
                atual->prox->ant = atual->ant
            }

            elemento->limpa();
            free(atual);
        }
    }
}