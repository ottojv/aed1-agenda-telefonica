#include "agenda.h"
#include "contato.h"
#include "telefones.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Le o cabeçalho de um arquivo csv para identificar em quais colunas estão os
// dados que serão importados pelo programa
// Cada parametro iniciado com col é um ponteiro para um inteiro que armazenará
// a coluna em que o respectivo campo se encontra
// No caso dos telefones que podem estar armazenados em mais de
// uma coluna no csv o ponteiro referencia o inicio de um array de inteiros
// e o parametro ntel indica armazenará quantas colunas do arquivo csv contem
// telefones
void parse_header(FILE *arquivo, int *col_nome, int *col_sobrenome,
                  int *col_empresa, int *col_cargo, int *col_email,
                  int *col_tel, int *ntel, int *col_obs);

// Le os dados de um contato (uma linha) do arquivo csv, extraindo as
// informações relevantes e cria um novo contato
void parse_line(Agenda *agenda, FILE *arquivo, int *col_nome,
                int *col_sobrenome, int *col_empresa, int *col_cargo,
                int *col_email, int *col_tel, int *ntel, int *col_obs);

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
    // agenda vazia
    if (agenda->contatos == NULL) {
        agenda->contatos = novo;
        agenda->ncontatos++;
        novo->proximo = novo;
        novo->anterior = novo;
        return;
    }

    // agenda cheia
    if (agenda->ncontatos == agenda->maxcontatos) {
        fprintf(stderr, "Erro ao adicionar novo contato, agenda está cheia\n");
        exclui_contato(novo);
        return;
    }

    // nome completo do contato usado para procurar posição de inserção
    char *novo_nome_completo = NULL;
    if (novo->sobrenome) {
        novo_nome_completo = (char *)calloc(
            strlen(novo->nome) + strlen(novo->sobrenome) + 2, sizeof(char));
        strcpy(novo_nome_completo, novo->nome);
        strcat(novo_nome_completo, " ");
        strcat(novo_nome_completo, novo->sobrenome);
    } else {
        novo_nome_completo = novo->nome;
    }

    // procura posição de inserção
    Contato *anterior = agenda->contatos;
    do {
        char *anterior_nome_completo = NULL;
        if (anterior->sobrenome) {
            anterior_nome_completo = (char *)calloc(
                strlen(anterior->nome) + strlen(anterior->sobrenome) + 2,
                sizeof(char));
            strcat(anterior_nome_completo, anterior->nome);
            strcat(anterior_nome_completo, " ");
            strcat(anterior_nome_completo, anterior->sobrenome);
        } else {
            anterior_nome_completo = anterior->nome;
        }

        // compara nomes completos do contato novo e do atual na lista
        if (strcmp(anterior_nome_completo, novo_nome_completo) >= 0) {
            anterior = anterior->anterior;

            // atualiza ponteiro para inicio da agenda
            if (anterior->proximo == agenda->contatos) {
                agenda->contatos = novo;
            }

            if (anterior_nome_completo != anterior->proximo->nome) {
                free(anterior_nome_completo);
            }

            break;
        }

        if (anterior_nome_completo != anterior->nome) {
            free(anterior_nome_completo);
        }

        anterior = anterior->proximo;
    } while (anterior != agenda->contatos);

    if (novo_nome_completo != novo->nome) {
        free(novo_nome_completo);
    }

    novo->anterior = anterior;
    novo->proximo = anterior->proximo;
    anterior->proximo = novo;
    novo->proximo->anterior = novo;

    agenda->ncontatos++;
}

Contato *rm_contato(Agenda *agenda, Contato *contato)
{
    if (!agenda->ncontatos) {
        agenda->contatos = NULL;
    } else {
        contato->anterior->proximo = contato->proximo;
        contato->proximo->anterior = contato->anterior;
        if (contato == agenda->contatos) {
            agenda->contatos = contato->proximo;
        }
    }
    contato->proximo = NULL;
    contato->anterior = NULL;
    agenda->ncontatos--;

    return contato;
}

Contato *procura_nome(Agenda *agenda, const char *nome, const char *sobrenome)
{
    if (!nome) {
        fprintf(stderr, "É necessario digitar um nome para buscar\n");
        return NULL;
    }

    Contato *contato = agenda->contatos;
    int igual = 0;

    // procura contato pelo primeiro nome
    while ((igual = strcmp(contato->nome, nome))) {
        if (igual > 1) {
            // O nome do contato atual na lista ja vem depois do nome
            // procurado
            return NULL;
        }
        contato = contato->proximo;
    }

    // procura contato pelo sobrenome
    if (sobrenome) {
        while (contato->sobrenome && !strcmp(contato->nome, nome) &&
               strcmp(contato->sobrenome, sobrenome) != 0) {
            contato = contato->proximo;
        }
    }

    if (strcmp(contato->nome, nome) == 0) {
        if (sobrenome && strcmp(contato->sobrenome, sobrenome) != 0) {
            return NULL;
        }
        return contato;
    }

    return NULL;
}

void exclui_agenda(Agenda *agenda)
{
    if (agenda->contatos) {
        Contato *excluir = agenda->contatos;
        do {
            Contato *prox = excluir->proximo;
            exclui_contato(excluir);
            excluir = prox;
        } while (excluir != agenda->contatos);
    }

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
        novo->conhecido = 1;
    } else {
        novo->entrada.numero = numero;
        novo->conhecido = 0;
    }

    // Se o historico estiver cheio remove a ultima entrada (mais antiga)
    if (agenda->nhistorico >= agenda->maxhistorico) {
        Historico *ultimo = agenda->historico->anterior;
        ultimo->anterior->proximo = ultimo->proximo;
        ultimo->proximo->anterior = ultimo->anterior;
        free(ultimo);
        agenda->nhistorico--;
    }

    if (agenda->nhistorico == 0) {
        // Insere no inicio
        agenda->historico = novo;
        novo->proximo = novo;
        novo->anterior = novo;
    } else {
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

    // A coluna em que cada campo esta armazenado no csv
    int col_nome = -1;
    int col_sobrenome = -1;
    int col_empresa = -1;
    int col_cargo = -1;
    int col_email = -1;
    int col_tel[9];
    int n_tel = 0; // quantas colunas com telefone foram encontrada no arquivo
    int col_obs = -1;
    parse_header(arq, &col_nome, &col_sobrenome, &col_empresa, &col_cargo,
                 &col_email, col_tel, &n_tel, &col_obs);
    while (!feof(arq)) {
        parse_line(agenda, arq, &col_nome, &col_sobrenome, &col_empresa,
                   &col_cargo, &col_email, col_tel, &n_tel, &col_obs);
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

    if (agenda->contatos != NULL) {
        Contato *atual = agenda->contatos;
        do {
            // Usa string de formatação para achar os campos relevantes na linha
            // l l da As colunas do csv são separadas por virgula e as colunas
            // elevantes são NOME: 2 SOBRENOME: 4 OBSERVACOES: 26 TELEFONES: 31
            // EMAIL: 33
            // CARGO: 35
            // EMPRESA: 37
            // Não importa corretamente se algum contato tiver campos com tags
            // e tipo definida Ex.: Telefone - residencial / trabalho / fax /
            // etc
            const char *f = "%s,%s,,%s,,,,,,,,,,,,,,,,,,,,,,%s,,,* "
                            "myContacts,,%s,,%s,,%s,,%s,,,,\n";

            char *nome_completo = NULL;
            if (atual->nome && atual->sobrenome) {
                nome_completo = (char *)malloc(
                    (strlen(atual->nome) + strlen(atual->sobrenome) + 1) *
                    sizeof(char));
                nome_completo[0] = '\0';
                strcat(nome_completo, atual->nome);
                strcat(nome_completo, " ");
                strcat(nome_completo, atual->sobrenome);
            } else if (atual->nome) {
                nome_completo = atual->nome;
            } else if (atual->sobrenome) {
                nome_completo = atual->sobrenome;
            } else {
                nome_completo = (char *)malloc(sizeof(char));
                nome_completo[0] = '\0';
            }
            // Escreve as informações do contato no arquivo
            char *telefones = string_telefones(atual);
            fprintf(arq, f, nome_completo, atual->nome,
                    (atual->sobrenome != NULL ? atual->sobrenome : ""),
                    (atual->observacoes != NULL ? atual->observacoes : ""),
                    (atual->email != NULL ? atual->email : ""), telefones,
                    (atual->empresa != NULL ? atual->empresa : ""),
                    (atual->cargo != NULL ? atual->cargo : ""));
            free(telefones);

            if (nome_completo != atual->nome &&
                nome_completo != atual->sobrenome) {
                free(nome_completo);
            }

            atual = atual->proximo;
        } while (atual != agenda->contatos);
    }
    fclose(arq);
}

void parse_header(FILE *arquivo, int *col_nome, int *col_sobrenome,
                  int *col_empresa, int *col_cargo, int *col_email,
                  int *col_tel, int *ntel, int *col_obs)
{
    char buffer[BUFSIZ];
    if (fgets(buffer, BUFSIZ, arquivo) == NULL) {
        return;
    }

    int col = 1;
    int i_tel = 0;
    char *token = strtok(buffer, ",");
    while (token != NULL) {
        if (strstr(token, "Given Name") && *col_nome == -1) {
            *col_nome = col;
        } else if (strstr(token, "Family Name") && *col_sobrenome == -1) {
            *col_sobrenome = col;
        } else if (strstr(token, "Notes") && *col_obs == -1) {
            *col_obs = col;
        } else if (strstr(token, "E-mail 1 - Value") && *col_email == -1) {
            // Um contato no arquivo importado pode ter multiplos emails, porem
            // o programa so importará o primeiro email encontrado
            *col_email = col;
        } else if (strstr(token, "Phone 1 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Phone 2 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Phone 3 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Phone 4 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Phone 5 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Phone 6 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Phone 7 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Phone 8 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Phone 9 - Value")) {
            col_tel[i_tel] = col;
            i_tel++;
        } else if (strstr(token, "Organization 1 - Name")) {
            *col_empresa = col;
        } else if (strstr(token, "Organization 1 - Title")) {
            *col_cargo = col;
        }
        col++;
        token = strtok(NULL, ",");
    }
    *ntel = i_tel;
}

void parse_line(Agenda *agenda, FILE *arquivo, int *col_nome,
                int *col_sobrenome, int *col_empresa, int *col_cargo,
                int *col_email, int *col_tel, int *ntel, int *col_obs)
{
    char buffer[BUFSIZ];
    if (fgets(buffer, BUFSIZ, arquivo) == NULL) {
        return;
    }

    char *cargo = NULL;
    char *email = NULL;
    char *empresa = NULL;
    char *nome = NULL;
    char *obs = NULL;
    char *sobrenome = NULL;
    char *telefones[9] = {NULL};
    char *s = NULL;

    int col = 0;            // a coluna no arquivo csv
    int itel = 0;           // indice do array das colunas de telefone
    unsigned long ibuf = 0; // indice do buffer
    do {
        size_t tam = 0;
        if (col + 1 == *col_nome) {
            // Le o buffer da posição atual até a proxima virgula
            s = strtok(buffer + ibuf, ",");
            tam = strlen(s) + 1;
            nome = (char *)malloc(tam * sizeof(char));
            strcpy(nome, s);
            nome[tam] = '\0';

            // atualiza o indice para acompanhar a posição apos a leitura
            ibuf += tam - 1;
            col++;
        } else if (col + 1 == *col_sobrenome) {
            s = strtok(buffer + ibuf, ",");
            tam = strlen(s) + 1;
            sobrenome = (char *)malloc(tam * sizeof(char));
            strcpy(sobrenome, s);
            sobrenome[tam] = '\0';

            ibuf += tam - 1;
            col++;
        } else if (col + 1 == *col_obs) {
            s = strtok(buffer + ibuf, ",");
            tam = strlen(s) + 1;
            obs = (char *)malloc(tam * sizeof(char));
            strcpy(obs, s);
            obs[tam] = '\0';

            ibuf += tam - 1;
            col++;
        } else if (col + 1 >= col_tel[0] && col + 1 <= col_tel[*ntel - 1]) {
            if (abs(col - col_tel[0]) % 2 == col_tel[0] % 2 &&
                buffer[ibuf] != ',') {
                s = strtok(buffer + ibuf, ",");
                tam = strlen(s) + 1;
                telefones[itel] = (char *)malloc(tam * sizeof(char));
                strcpy(telefones[itel], s);
                telefones[itel][tam] = '\0';

                ibuf += tam - 1;
                col++;
                itel++;
            }
        } else if (col + 1 == *col_email) {
            s = strtok(buffer + ibuf, ",");
            tam = strlen(s) + 1;
            email = (char *)malloc(tam * sizeof(char));
            strcpy(email, s);
            email[tam] = '\0';

            ibuf += tam;
            col += 1;
        } else if (col + 1 == *col_cargo) {
            s = strtok(buffer + ibuf, ",");
            tam = strlen(s) + 1;
            cargo = (char *)malloc(tam * sizeof(char));
            strcpy(cargo, s);
            cargo[tam] = '\0';

            ibuf += tam - 1;
            col++;
        } else if (col + 1 == *col_empresa) {
            s = strtok(buffer + ibuf, ",");
            tam = strlen(s) + 1;
            empresa = (char *)malloc(tam * sizeof(char));
            strcpy(empresa, s);
            empresa[tam] = '\0';

            ibuf += tam - 1;
            col++;
        }
        if (buffer[ibuf] == ',') {
            col++;
        }
        buffer[ibuf++] = '\0';
    } while (buffer[ibuf] != '\0');

    // Se um contato tiver multiplos numeros de telefone
    // Os numeros estarão na mesma coluna separados por " ::: "
    // Cria um novo contato com o primeiro numero de telefone
    Contato *cont = cria_contato(nome, sobrenome, strtok(telefones[0], ":::"),
                                 email, cargo, empresa, obs);
    if (cont == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para contato na importacao\n");
        return;
    }
    // Adiciona os numeros restantes
    // Numeros restantes na primeira posição do vetor
    while (1) {
        char *tel = strtok(NULL, ":::");
        if (tel == NULL) {
            break;
        }
        // Evita contatos com numeros de telefone duplicados
        if (busca_tel(tel) == NULL) {
            add_tel(cont, tel);
        }
    }
    // Numeros nas outras posicoes do vetor
    for (int i = 1; i < *ntel; i++) {
        // Adiciona primeiro numero
        char *tel = strtok(telefones[i], ":::");
        if (tel != NULL && busca_tel(tel) == NULL) {
            add_tel(cont, tel);
        }

        // Numeros restantes
        while (1) {
            tel = strtok(NULL, " ::: ");
            if (tel == NULL) {
                break;
            }
            // Evita contatos com numeros de telefone duplicados
            if (busca_tel(tel) == NULL) {
                add_tel(cont, tel);
            }
        }
    }
    add_contato(agenda, cont);
}
