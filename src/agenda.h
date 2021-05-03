// Aluno: João Victor Otto
// Matricula: 202000180
//
// A agenda armazena os contatos em listas duplamente encadeadas
// Alem disso a biblioteca agenda também inclui funções de realizar chamadas,
// e visualizar o historico de chamadas, que são implementadas através de filas
// É possível importar arquivos .csv seguindo o padrão Google e
// exportar para o mesmo formato

#ifndef AGENDA_H
#define AGENDA_H
#define MAXAGENDA (UINT16_MAX)

#include <stdint.h>

typedef struct contato Contato;
// Uma entrada no historico pode ser um contato
// ou um numero de telefone avulso
typedef struct nohistorico NoHistorico;
struct nohistorico {
    union entrada {
        Contato *contato;
        const char *numero;
    } entrada;
    NoHistorico *prox;
};
typedef struct historico Historico;
struct historico {
    NoHistorico *inicio;
    uint8_t nentradas;
    uint8_t nmax;
};

typedef struct noagenda NoAgenda;
struct noagenda {
    Contato *contato;
    NoAgenda *prox;
    NoAgenda *ant;
};

typedef struct agenda {
    NoAgenda *inicio;
    Historico *historico;
    uint16_t ncont;
    const uint16_t nmax;
} Agenda;

// Aloca memoria e inicializa uma nova agenda
Agenda *cria_agenda();

// Cria e insere um novo contato na agenda
// mantendo uma ordenação alfabetica priorizando o primeiro nome
// ou em caso de nomes iguais, o sobrenome
void add_contato(Agenda *agenda, Contato *contato);

// Remove o contato da agenda
void rm_contato(Agenda *agenda, Contato *contato);

// Procura por um contato utilizando o nome como chave de busca
Contato *procura_nome(Agenda *agenda, const char *nome);

// Procura por um contato utilizando um numero de telefone como chave de busca
Contato *procura_tel(Agenda *agenda, const char *telefone);

// Importa um arquivo.csv seguindo o padrão Google
// Não importa corretamente se algum contato tiver campos com tags de tipo definida
// Ex.: Telefone - residencial / trabalho / fax / etc
// Pode ser chamada passando NULL para agenda, nesse caso uma nova agenda é criada
Agenda *importar_agenda(Agenda *agenda, const char *arquivo);

// Exporta a agenda atual para um arquivo.csv
void exportar_agenda(Agenda *agenda, const char *arquivo);

// Libera a memoria alocada por cada contato e pela agenda
void exclui_agenda(Agenda *agenda);

// Imprime o historico de chamadas na tela
void ver_historico(Agenda *agenda);

// Realiza uma ligação para um contato, adicionando a chamada no historico
void ligar(Agenda *agenda, const char *numero);
#endif
