/**
 * @file lista.h
 * @author Joao Victor Otto
 * @brief Interface para listas duplamente circulares genericas 
 * 
 */
#ifndef LISTA_H
#define LISTA_H

/**
 * @brief Cada nó da lista
 * @param elemento ponteiro para struct qualquer que armazenara os dados do nó da lista
 * A struct elemento devera ter ponteiros para funcoes de comparacao e limpeza
 * seguindo a definicao:
 * elemento->compara(a, b): retorna -1 se a < b, 0 se a == b e 1 se a > b
 * elemento->limpa(): executa rotina de liberacao de memoria
 */
typedef struct lista_no
{
    void *elemento;
    lista_no *prox;
    lista_no *ant;
} ListaNo;

/**
 * @brief Interface para abstrair a implementação da lista
 * 
 */
typedef struct lista
{
    lista_no *cabeca;
    unsigned int tamanho;
} Lista;

/**
 * @brief Aloca memoria e inicializa uma nova lista genérica
 * 
 * @return Lista* ponteiro para a lista;
 */
Lista *lista_cria();
/**
 * @brief Insere um novo elemento na lista, a inserção sera feita de forma
 * ordenada de acordo com a função de comparação do tipo do elemento da lista.
 * A função não verifica se os tipos inseridos são compativeis entre si.
 * 
 * @param lista ponteiro para a lista na qual o elemento sera inserido
 * @param elemento ponteiro para o elemento que sera inserido
 */
void lista_insere(Lista *lista, void *elemento);

/**
 * @brief Remove um elemento da lista
 * 
 * @param lista a lista cujo elemento sera removido
 * @param elemento o elemento que deverá ser removido
 */
void lista_remove(Lista *lista, void *elemento);

/**
 * @brief Procura um elemento na lista por uma chave de busca.
 * 
 * @param lista a lista na qual o elemento sera procurado
 * @param chave a chave de busca que sera usada para encontrar o elemento
 * @return void* ponteiro para o elemento encontrado ou NULL caso não tenha
 * sido encontrado
 */
void *lista_busca(Lista *lista, void *chave);

#endif