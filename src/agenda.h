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

#define MAXAGENDA UINT16_MAX
#define MAXTELEFONES 5
#define MAXCONTATOS UINT16_MAX

#include <stdint.h>

typedef struct contato Contato;

typedef struct historico Historico;
struct historico {
    // Uma entrada no historico pode ser um contato
    // ou um numero de telefone avulso
    union entrada {
        Contato *contato;
        const char *numero;
    } entrada;
    int conhecido;
    Historico *proximo;
    Historico *anterior;
};

typedef struct agenda {
    Contato *contatos;
    Historico *historico;
    unsigned int ncontatos;
    unsigned int maxcontatos;
    unsigned int nhistorico;
    unsigned int maxhistorico;
} Agenda;

// Aloca memoria e inicializa uma nova agenda
Agenda *cria_agenda();

// Cria e insere um novo contato na agenda
// mantendo uma ordenação alfabetica priorizando o primeiro nome
// ou em caso de nomes iguais, o sobrenome
void add_contato(Agenda *agenda, Contato *novo);

// Remove o contato da agenda
Contato* rm_contato(Agenda *agenda, Contato *contato);

// Procura por um contato utilizando o nome como chave de busca
Contato *procura_nome(Agenda *agenda, const char *nome, const char *sobrenome);

// Importa um arquivo.csv seguindo o padrão Google
// Não importa corretamente se algum contato tiver campos com tags de tipo
// efinida Ex.: Telefone - residencial / trabalho / fax / etc Pode ser chamada
// passando NULL para agenda, nesse caso uma nova agenda é criada
Agenda *importar_agenda(Agenda *agenda, const char *arquivo);

// Exporta a agenda atual para um arquivo.csv
void exportar_agenda(Agenda *agenda, const char *arquivo);

// Libera a memoria alocada por cada contato e pela agenda
void exclui_agenda(Agenda *agenda);

// Realiza uma ligação para um contato, adicionando a chamada no historico
void ligar(Agenda *agenda, const char *numero);
#endif
