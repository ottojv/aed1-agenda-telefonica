#include "agenda.h"
#include "contato.h"
#include "telefones.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX (1024);

Agenda *cria_agenda()
{
    return &(Agenda){NULL, NULL, 0, UINT16_MAX};
}

void add_contato(Agenda *agenda, Contato *contato)
{
    // Agenda vazia
    if (agenda->inicio == NULL) {
        agenda->inicio = (NoAgenda *)malloc(sizeof(NoAgenda));
        agenda->inicio->contato = contato;
        agenda->inicio->prox = NULL;
        agenda->inicio->ant = NULL;
        agenda->ncont++;
        return;
    }
    // Agenda cheia
    if (agenda->ncont == agenda->nmax) {
        puts("Erro ao adicionar novo contato, agenda está cheia");
        exclui_contato(contato);
        return;
    }
    NoAgenda *pos = agenda->inicio;
    while (strcmp(pos->prox->contato->nome, contato->nome) < 0) {
        pos = pos->prox;
    }
    if (contato->sobrenome != NULL) {
        while (pos->prox->contato->sobrenome != NULL &&
               strcmp(pos->prox->contato->nome, contato->nome) < 0) {
            pos = pos->prox;
        }
        NoAgenda *novo = (NoAgenda *)malloc(sizeof(NoAgenda));
        novo->contato = contato;
        pos->prox->ant = novo;
        novo->prox = pos->prox;
        novo->ant = pos;
        pos->prox = novo;
        agenda->ncont++;
    }
}

void rm_contato(Agenda *agenda, Contato *contato)
{
    NoAgenda *no = agenda->inicio;
    while (no->contato != contato) {
        no = no->prox;
    }
    no->ant->prox = no->prox;
    no->prox->ant = no->ant;
    exclui_contato(contato);
}

void exclui_agenda(Agenda *agenda)
{
    // Limpa os contatos
    NoAgenda *atual = agenda->inicio;
    while (atual != NULL) {
        exclui_contato(atual->contato);
        if (atual->prox != NULL) {
            atual = atual->prox;
            free(atual->ant);
        } else {
            free(atual);
        }
    }
    // Limpa o histórico
    for (NoHistorico *n = agenda->historico->inicio; n != NULL; n = n->prox) {
        free(n);
    }
    free(agenda);
}

void ver_historico(Agenda *agenda)
{
    for (NoHistorico *n = agenda->historico->inicio; n != NULL; n = n->prox) {
        if (n->entrada.contato != NULL) {
            printf("Nome: %s\n\n", n->entrada.contato->nome);
        } else {
            printf("Desconhecido: %s\n\n", n->entrada.numero);
        }
    }
}

void ligar(Agenda *agenda, const char *numero)
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
    // Verifica se o numero de telefone pertence a algum contato
    Contato *c = busca_tel(numero);
    if (c != NULL) {
        novo->entrada.contato = c;
    } else {
        novo->entrada.numero = numero;
    }
    novo->prox = agenda->historico->inicio;
    agenda->historico->inicio = novo;
}

Agenda *importar_agenda(Agenda *agenda, const char *arquivo)
{
    FILE *arq = fopen(arquivo, "r");
    if (arq == NULL) {
        puts("Erro ao abrir arquivo");
    }

    if (agenda == NULL) {
        agenda = cria_agenda();
        if (agenda == NULL) {
            printf("Erro ao criar agenda");
            return NULL;
        }
    }

    // Pula primeira linha que contem cabeçalho da tabela
    char buffer[BUFSIZ];
    fgets(buffer, BUFSIZ, arq);
    // Percorre as linhas do arquivo
    while (!feof(arq)) {
        // Le uma linha do csv
        fgets(buffer, BUFSIZ, arq);

        char *cargo = NULL;
        char *email = NULL;
        char *empresa = NULL;
        char *nome = NULL;
        char *obs = NULL;
        char *sobrenome = NULL;
        char *telefones = NULL;

        // Usa string de formatação para achar os campos relevantes na linha lida
        // As colunas do csv são separadas por virgula e as colunas relevantes são
        // NOME: 2
        // SOBRENOME: 4
        // OBSERVACOES: 26
        // TELEFONES: 31
        // EMAIL: 33
        // CARGO: 35
        // EMPRESA: 37
        // Não importa corretamente se algum contato tiver campos com tags de tipo definida
        // Ex.: Telefone - residencial / trabalho / fax / etc
        const char *f = "%*s,%s,%*s,%s,%*s,%*s,%*s,%*s,%*s,%*s,%*s,%*s,%*s,%*s,"
                        "%*s,%*s,%*s,%*s,%*s,%*s,%*s,%*s,%*s,%*s,%*s,%s,%*s,"
                        "%*s,%*s,%*s,%s,%*s,%s,%*s,%s,%*s,%s,%*s,%*s,%*s,";
        // E descarta os campos que não serão informados
        sscanf(buffer, f, nome, sobrenome, obs, telefones, email, cargo,
               empresa);

        // Se um contato tiver multiplos numeros de telefone
        // Os numeros estarão na mesma coluna separados por " ::: "
        // Cria um novo contato com o primeiro de telefone
        Contato *cont = cria_contato(nome, strtok(telefones, " ::: "));
        if (cont == NULL) {
            return NULL;
        }
        // Adiciona os numeros restantes
        while (1) {
            char *tel = strtok(telefones, " ::: ");
            if (tel == NULL) {
                break;
            }
            add_tel(cont, tel);
        }
        // Adiciona os campos não vazios restantes
        if (!strcmp(sobrenome, "")) {
            edita_contato(cont, sobrenome, SOBRENOME);
        }
        if (!strcmp(obs, "")) {
            edita_contato(cont, obs, OBSERVACOES);
        }
        if (!strcmp(email, "")) {
            edita_contato(cont, email, EMAIL);
        }
        if (!strcmp(cargo, "")) {
            edita_contato(cont, cargo, CARGO);
        }
        if (!strcmp(empresa, "")) {
            edita_contato(cont, empresa, EMPRESA);
        }
        add_contato(agenda, cont);
    }
    fclose(arq);
    return agenda;
}

void exportar_agenda(Agenda *agenda, const char *arquivo)
{
    FILE *arq = fopen(arquivo, "w");
    if (arq == NULL) {
        puts("Erro ao abrir arquivo");
    }

    // Cabeçalho do arquivo
    fprintf(
        arq,
        "Name,Given Name,Additional Name,Family Name,Yomi Name,Given Name "
        "Yomi,Additional Name Yomi,Family Name Yomi,Name Prefix,Name "
        "Suffix,Initials,Nickname,Short Name,Maiden "
        "Name,Birthday,Gender,Location,Billing Information,Directory "
        "Server,Mileage,Occupation,Hobby,Sensitivity,Priority,Subject,Notes,"
        "Language,Photo,Group Membership,E-mail 1 - Type,E-mail 1 - "
        "Value,Phone 1 - Type,Phone 1 - Value,Organization 1 - "
        "Type,Organization 1 - Name,Organization 1 - Yomi Name,Organization "
        "1 - Title,Organization 1 - Department,Organization 1 - "
        "Symbol,Organization 1 - Location,Organization 1 - Job Description\n");
    NoAgenda *atual = agenda->inicio;
    while (atual != NULL) {
        // Usa string de formatação para achar os campos relevantes na linha lida
        // As colunas do csv são separadas por virgula e as colunas relevantes são
        // NOME: 2
        // SOBRENOME: 4
        // OBSERVACOES: 26
        // TELEFONES: 31
        // EMAIL: 33
        // CARGO: 35
        // EMPRESA: 37
        // Não importa corretamente se algum contato tiver campos com tags de tipo definida
        // Ex.: Telefone - residencial / trabalho / fax / etc
        const char *f = ",%s,,%s,,,,,,,,,,,,,,,,,,,,,,%s,,,* "
                        "myContacts,,%s,,%s,,%s,,%s,,,,\n";
        // Escreve as informações do contato no arquivo
        fprintf(
            arq, f, atual->contato->nome,
            (atual->contato->sobrenome != NULL ? atual->contato->sobrenome
                                               : ""),
            (atual->contato->observacoes != NULL ? atual->contato->observacoes
                                                 : ""),
            string_telefones(atual->contato->telefones),
            (atual->contato->email != NULL ? atual->contato->email : ""),
            (atual->contato->cargo != NULL ? atual->contato->cargo : ""),
            (atual->contato->empresa != NULL ? atual->contato->empresa : ""));
    }
    fclose(arq);
}
