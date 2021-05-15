#include "agenda.h"
#include "contato.h"
#include "telefones.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Agenda *cria_agenda()
{
    Agenda *nova = (Agenda *)malloc(sizeof(Agenda));
    nova->contatos = NULL;
    nova->historico = NULL;
    nova->ncontatos = 0;
    nova->maxcontatos = MAXCONTATOS;
    nova->nhistorico = 0;
    nova->maxhistorico = MAXTELEFONES;
    return nova;
}

void add_contato(Agenda *agenda, Contato *novo)
{
    // Agenda vazia
    if (agenda->contatos == NULL) {
        agenda->contatos = novo;
        agenda->ncontatos++;
        novo->proximo = novo;
        novo->anterior = novo;
        return;
    }

    // Agenda cheia
    if (agenda->ncontatos == agenda->maxcontatos) {
        fprintf(stderr, "Erro ao adicionar novo contato, agenda está cheia\n");
        exclui_contato(novo);
        return;
    }

    Contato *anterior = agenda->contatos;
    // Se o nome do primeiro contato for lexicograficamente superior
    if (strcmp(anterior->nome, novo->nome) >= 0) {
        // Inserir no inicio
        anterior = anterior->anterior;
    } else {
        // Procura a posição de inserção com base no primeiro nome
        if (anterior->proximo != anterior) {
            while (strcmp(anterior->proximo->nome, novo->nome) < 0) {
                anterior = anterior->proximo;
            }
        }

        // Procura posição com base no sobrenome caso primeiro nome seja igual
        if (novo->sobrenome != NULL) {
            // Enquanto o proximo existir, tiver sobrenome, o nome for igual e o
            // sobrenome for lexicograficamente menor
            if (strcmp(anterior->sobrenome, novo->sobrenome) > 0) {
                while (anterior->proximo->sobrenome &&
                       !strcmp(anterior->proximo->nome, novo->nome) &&
                       strcmp(anterior->proximo->nome, novo->nome) < 0) {
                    anterior = anterior->proximo;
                }
            }
        }
    }
    // Inserção no inicio atualiza ponteiro para inicio da lista
    if (anterior == agenda->contatos->anterior) {
        agenda->contatos = novo;
    }
    novo->anterior = anterior;
    novo->proximo = anterior->proximo;
    anterior->proximo = novo;
    novo->proximo->anterior = novo;

    agenda->ncontatos++;
}

Contato *rm_contato(Agenda *agenda, Contato *contato)
{
    agenda->ncontatos--;
    if (!agenda->ncontatos) {
        agenda->contatos = NULL;
    } else {
        contato->anterior->proximo = contato->proximo;
        contato->proximo->anterior = contato->anterior;
        if (contato == agenda->contatos) {
            agenda->contatos = contato->proximo;
        }
        contato->proximo = NULL;
        contato->anterior = NULL;
    }

    return contato;
}

Contato *procura_nome(Agenda *agenda, const char *nome, const char *sobrenome)
{
    Contato *contato = agenda->contatos;
    int igual = 0;
    if (!nome) {
        fprintf(stderr, "É necessario digitar um nome para buscar\n");
    }
    while ((igual = strcmp(contato->nome, nome))) {
        if (igual > 1) {
            // O nome do contato atual na lista ja vem depois do nome
            // procurado
            return NULL;
        }
        contato = contato->proximo;
    }

    if (sobrenome) {
        while (contato->sobrenome && !strcmp(contato->nome, nome) &&
               (igual = strcmp(contato->sobrenome, sobrenome))) {
            if (igual == 0) {
                return contato;
            }
            contato = contato->proximo;
        }
    }

    if (!strcmp(contato->nome, nome)) {
        if (sobrenome && strcmp(contato->sobrenome, sobrenome)) {
            return NULL;
        }
        return contato;
    }

    return NULL;
}

void exclui_agenda(Agenda *agenda)
{
    // Limpa os contatos
    Contato *contato = agenda->contatos;
    while (contato) {
        Contato *prox = contato->proximo;
        exclui_contato(contato);
        contato = prox;
    }
    // Limpa o histórico
    Historico *historico = agenda->historico;
    while (historico) {
        Historico *prox = historico->proximo;
        free(historico);
        historico = prox;
    }
}

void ligar(Agenda *agenda, const char *numero)
{
    Historico *novo = (Historico *)malloc(sizeof(Historico));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para entrada no histórico\n");
        return;
    }

    // Verifica se o numero de telefone pertence a algum contato
    Contato *c = busca_tel(numero);
    if (c != NULL) {
        novo->entrada.contato = c;
    } else {
        novo->entrada.numero = numero;
    }

    // Se o historico estive cheio remove a ultima entrada (mais antiga)
    if (agenda->nhistorico >= agenda->maxhistorico) {
        Historico *ultimo = agenda->historico->anterior;
        ultimo->anterior->proximo = ultimo->proximo;
        ultimo->proximo->anterior = ultimo->anterior;
        free(ultimo);
        agenda->nhistorico--;
    }

    if (!agenda->nhistorico) {
        agenda->historico = novo;
        novo->proximo = novo;
        novo->anterior = novo;
    } else {
        // Insere no inicio
        Historico *historico = agenda->historico;
        novo->proximo = historico;
        novo->anterior = historico->anterior;
        historico->anterior->proximo = novo;
        historico->anterior = novo;
        agenda->historico = novo;
    }
    agenda->nhistorico++;
}

Agenda *importar_agenda(Agenda *agenda, const char *arquivo)
{
    FILE *arq = fopen(arquivo, "r");
    if (arq == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo");
        return NULL;
    }

    if (agenda == NULL) {
        agenda = cria_agenda();
        if (agenda == NULL) {
            fprintf(stderr, "Erro ao criar agenda");
            return NULL;
        }
    }

    // Le e ignora a primeira linha do arquivo (cabeçalho)
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

        // As colunas do csv são separadas por virgula
        // e as colunas relevantes são
        // NOME: 2
        // SOBRENOME: 4
        // OBSERVACOES: 26
        // TELEFONES: 31
        // EMAIL: 33
        // CARGO: 35
        // EMPRESA: 37
        // Não importa corretamente se algum contato tiver campos com tags
        // de tipo definida
        // Ex.: Telefone - residencial / trabalho / fax
        const char *s = strtok(buffer, ",");
        int col = 1;
        do {
            size_t tam = 0;
            switch (col) {
            case 2:
                tam = strlen(s) + 1;
                nome = (char *)malloc(tam * sizeof(char));
                strcpy(nome, s);
                nome[tam] = '\0';
                break;
            case 3:
                tam = strlen(s) + 1;
                sobrenome = (char *)malloc(tam * sizeof(char));
                strcpy(sobrenome, s);
                sobrenome[tam] = '\0';
                break;
            case 26:
                tam = strlen(s) + 1;
                obs = (char *)malloc(tam * sizeof(char));
                strcpy(obs, s);
                obs[tam] = '\0';
                break;
            case 31:
                tam = strlen(s) + 1;
                telefones = (char *)malloc(tam * sizeof(char));
                strcpy(telefones, s);
                telefones[tam] = '\0';
                break;
            case 33:
                tam = strlen(s) + 1;
                email = (char *)malloc(tam * sizeof(char));
                strcpy(email, s);
                email[tam] = '\0';
                break;
            case 35:
                tam = strlen(s) + 1;
                cargo = (char *)malloc(tam * sizeof(char));
                strcpy(cargo, s);
                cargo[tam] = '\0';
                break;
            case 37:
                tam = strlen(s) + 1;
                empresa = (char *)malloc(tam * sizeof(char));
                strcpy(empresa, s);
                empresa[tam] = '\0';
                break;
            }
            col++;
            s = strtok(NULL, ",");
        } while (s != NULL);

        // Se um contato tiver multiplos numeros de telefone
        // Os numeros estarão na mesma coluna separados por " ::: "
        // Cria um novo contato com o primeiro de telefone
        Contato *cont =
            cria_contato(nome, sobrenome, strtok(telefones, " ::: "), email,
                         cargo, empresa, obs);
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
        fprintf(stderr, "Erro ao abrir arquivo");
    }

    // Cabeçalho do arquivo
    fprintf(arq,
            "Name,Given Name,Additional Name,Family Name,Yomi Name,Given Name "
            "Yomi,Additional Name Yomi,Family Name Yomi,Name Prefix,Name "
            "Suffix,Initials,Nickname,Short Name,Maiden "
            "Name,Birthday,Gender,Location,Billing Information,Directory "
            "Server,Mileage,Occupation,Hobby,Sensitivity,Priority,Subject,"
            "Notes,"
            "Language,Photo,Group Membership,E-mail 1 - Type,E-mail 1 - "
            "Value,Phone 1 - Type,Phone 1 - Value,Organization 1 - "
            "Type,Organization 1 - Name,Organization 1 - Yomi "
            "Name,Organization "
            "1 - Title,Organization 1 - Department,Organization 1 - "
            "Symbol,Organization 1 - Location,Organization 1 - Job "
            "Description\n");
    Contato *atual = agenda->contatos;
    while (atual != NULL) {
        // Usa string de formatação para achar os campos relevantes na linha l
        // l da As colunas do csv são separadas por virgula e as colunas
        // elevantes são NOME: 2 SOBRENOME: 4 OBSERVACOES: 26 TELEFONES: 31
        // EMAIL: 33
        // CARGO: 35
        // EMPRESA: 37
        // Não importa corretamente se algum contato tiver campos com tags
        // e  ipo definida Ex.: Telefone - residencial / trabalho / fax /
        // etc
        const char *f = ",%s,,%s,,,,,,,,,,,,,,,,,,,,,,%s,,,* "
                        "myContacts,,%s,,%s,,%s,,%s,,,,\n";

        // Escreve as informações do contato no arquivo
        fprintf(arq, f, atual->nome,
                (atual->sobrenome != NULL ? atual->sobrenome : ""),
                (atual->observacoes != NULL ? atual->observacoes : ""),
                string_telefones(atual),
                (atual->email != NULL ? atual->email : ""),
                (atual->cargo != NULL ? atual->cargo : ""),
                (atual->empresa != NULL ? atual->empresa : ""));
    }
    fclose(arq);
}
