#include "agenda.h"
#include "contato.h"
#include "telefones.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Le os dados de um contato (uma linha) do arquivo csv, extraindo as
// informações relevantes e cria um novo contato
void parse_line(Agenda *agenda, FILE *arquivo);

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

    while (!feof(arq)) {
        parse_line(agenda, arq);
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
    if (agenda->contatos != NULL) {
        Contato *atual = agenda->contatos;
        do {
            const char *f = "%s,%s,%s,%s,%s,%s,%s\n";

            // Escreve as informações do contato no arquivo
            char *telefones = string_telefones(atual);
            fprintf(arq, f, atual->nome,
                    (atual->sobrenome != NULL ? atual->sobrenome : ""),
                    (atual->observacoes != NULL ? atual->observacoes : ""),
                    (atual->email != NULL ? atual->email : ""), telefones,
                    (atual->empresa != NULL ? atual->empresa : ""),
                    (atual->cargo != NULL ? atual->cargo : ""));
            free(telefones);
            atual = atual->proximo;
        } while (atual != agenda->contatos);
    }
    fclose(arq);
}

void parse_line(Agenda *agenda, FILE *arquivo)
{
    char buffer[BUFSIZ];
    if (fgets(buffer, BUFSIZ, arquivo) == NULL) {
        return;
    }

    // Le cada campo separado por uma virgula pulando os campos vazios
    char nome_buf[BUFSIZ] = {'\0'};
    char sobrenome_buf[BUFSIZ] = {'\0'};
    char telefones[BUFSIZ] = {'\0'};
    char email_buf[BUFSIZ] = {'\0'};
    char cargo_buf[BUFSIZ] = {'\0'};
    char empresa_buf[BUFSIZ] = {'\0'};
    char obs_buf[BUFSIZ] = {'\0'};
    sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]%*c", nome_buf,
           sobrenome_buf, obs_buf, email_buf, telefones, empresa_buf,
           cargo_buf);

    size_t tam = strlen(nome_buf);
    char *nome = (char *)malloc(tam * sizeof(char));
    strcpy(nome, nome_buf);

    tam = strlen(sobrenome_buf);
    char *sobrenome = (char *)malloc(tam * sizeof(char));
    strcpy(sobrenome, sobrenome_buf);

    tam = strlen(email_buf);
    char *email = (char *)malloc(tam * sizeof(char));
    strcpy(email, email_buf);

    tam = strlen(cargo_buf);
    char *cargo = (char *)malloc(tam * sizeof(char));
    strcpy(cargo, cargo_buf);

    tam = strlen(empresa_buf);
    char *empresa = (char *)malloc(tam * sizeof(char));
    strcpy(empresa, empresa_buf);

    tam = strlen(obs_buf);
    char *obs = (char *)malloc(tam * sizeof(char));
    strcpy(obs, obs_buf);

    // Se um contato tiver multiplos numeros de telefone
    // Os numeros estarão na mesma coluna separados por " ::: "
    // Cria um novo contato com o primeiro numero de telefone
    Contato *cont = cria_contato(nome, sobrenome, strtok(telefones, ":::"),
                                 email, cargo, empresa, obs);
    if (cont == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para contato na importacao\n");
        return;
    }
    // Adiciona os numeros restantes
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
    add_contato(agenda, cont);
}
