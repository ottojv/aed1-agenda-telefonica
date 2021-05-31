#include "agenda.h"
#include "contato.h"
#include "telefones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Le os dados de um novo contato, cria e adiciona na agenda
void novo_contato(Agenda *a);

// Faz a busca linear de um contato na agenda por nome e opcionalmente sobrenome
Contato *buscar_nome(Agenda *a);

// Faz a busca por um contato na agenda com um numero de telefone
Contato *buscar_tel(Agenda *a);

// Exibe as informações de um contato na tela e permite edição
int exibir_contato(Agenda *a, Contato *c);

// Exibe o nome de todos os contatos na tela e permite a escolha de um para
// exibir mais informações
void exibir_contatos(Agenda *a);

void ligacao(Agenda *a);

void exibir_historico(Agenda *a);

int main(void)
{
    Agenda *agenda = cria_agenda();
    char nome_arquivo[BUFSIZ];
    char ch = '\0';
    do {
        system("clear");
        puts("i - Importar agenda\n");
        puts("c - Criar contato\n");
        puts("n - Buscar contato por nome\n");
        puts("t - Buscar contato por telefone\n");
        puts("e - Exibir contatos\n");
        puts("h - Exibir historico\n");
        puts("l - Realizar chamada\n");
        puts("s - Salvar e sair\n");

        int proximo = 0;
        Contato *contato = NULL;
        scanf("%c%*c", &ch);
        switch (ch) {
        case 'i':
            puts("Digite o nome do arquivo que deseja importar incluindo o "
                 ".csv");
            scanf("%[^\n]%*c", nome_arquivo);
            importar_agenda(agenda, nome_arquivo);
            break;
        case 'c':
            novo_contato(agenda);
            break;
        case 'n':
            contato = buscar_nome(agenda);
            do {
                proximo = exibir_contato(agenda, contato);
                if (proximo == 1) {
                    contato = contato->proximo;
                } else if (proximo == -1) {
                    contato = contato->anterior;
                }
            } while (proximo != 0);
            break;
        case 't':
            contato = buscar_tel(agenda);
            do {
                proximo = exibir_contato(agenda, contato);
                if (proximo == 1) {
                    contato = contato->proximo;
                } else if (proximo == -1) {
                    contato = contato->anterior;
                }
            } while (proximo != 0);
            break;

        case 'e':
            exibir_contatos(agenda);
            break;

        case 'h':
            exibir_historico(agenda);
            break;

        case 'l':
            ligacao(agenda);
            break;

        default:
            break;
        }
    } while (ch != 's');

    exportar_agenda(agenda, "agenda.csv");
    exclui_agenda(agenda);
    return 0;
}

void novo_contato(Agenda *a)
{
    char buf[BUFSIZ];
    unsigned long len = 0;

    system("clear");
    char *nome = NULL;
    do {
        printf("\nNome: ");
        fgets(buf, BUFSIZ, stdin);
        len = strlen(buf) - 1;
        nome = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
        if (nome != NULL) {
            strcpy(nome, buf);
            nome[len] = '\0'; // remove o '\n' no final
        } else {
            puts("O nome deve ser preenchido");
        }
    } while (nome == NULL);

    printf("\nSobrenome: ");
    fgets(buf, BUFSIZ, stdin);
    len = strlen(buf) - 1;
    char *snome = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
    if (snome != NULL) {
        strcpy(snome, buf);
        snome[len] = '\0'; // remove o '\n' no final
    }

    Contato *cont = NULL;
    char *tel = NULL;
    do {
        printf("\nTelefone: ");
        fgets(buf, BUFSIZ, stdin);
        len = strlen(buf) - 1;
        tel = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
        if (tel != NULL) {
            strncpy(tel, buf, TELSIZ);
            tel[len] = '\0'; // remove o '\n' no final
            cont = busca_tel(tel);
            if (cont) {
                puts("Esse numero ja pertence a outro contato");
            }
        }
    } while (cont);

    printf("\nEmail: ");
    fgets(buf, BUFSIZ, stdin);
    len = strlen(buf) - 1;
    char *email = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
    if (email != NULL) {
        strcpy(email, buf);
        email[len] = '\0'; // remove o '\n' no final
    }

    printf("\nCargo: ");
    fgets(buf, BUFSIZ, stdin);
    len = strlen(buf) - 1;
    char *cg = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
    if (cg != NULL) {
        strcpy(cg, buf);
        cg[len] = '\0'; // remove o '\n' no final
    }

    printf("\nEmpresa: ");
    fgets(buf, BUFSIZ, stdin);
    len = strlen(buf) - 1;
    char *emp = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
    if (emp != NULL) {
        strcpy(emp, buf);
        emp[len] = '\0'; // remove o '\n' no final
    }

    printf("\nObservacoes: ");
    fgets(buf, BUFSIZ, stdin);
    len = strlen(buf) - 1;
    char *obs = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
    if (obs != NULL) {
        strcpy(obs, buf);
        obs[len] = '\0'; // remove o '\n' no final
    }

    Contato *c = cria_contato(nome, snome, tel, email, cg, emp, obs);
    if (c == NULL) {
        free(nome);
        free(snome);
        free(tel);
        free(email);
        free(cg);
        free(emp);
        free(obs);
        puts("\nErro ao criar contato\n");
        return;
    }

    add_contato(a, c);
}

Contato *buscar_nome(Agenda *a)
{
    if (a == NULL) {
        return NULL;
    }

    char buf[BUFSIZ];
    unsigned long len = 0;

    system("clear");
    printf("Nome: ");
    fgets(buf, BUFSIZ, stdin);
    len = strlen(buf);
    char *nome = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
    if (nome == NULL) {
        return NULL;
        fprintf(stderr, "Campo nome não pode ser vazio ao procurar contato\n");
    }
    strcpy(nome, buf);
    nome[len - 1] = '\0'; // remove o '\n' no final

    printf("\nSobrenome: ");
    fgets(buf, BUFSIZ, stdin);
    len = strlen(buf);
    char *sobrenome =
        buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
    if (sobrenome) {
        strcpy(sobrenome, buf);
        sobrenome[len - 1] = '\0'; // remove o '\n' no final
    }

    return procura_nome(a, nome, sobrenome);
}

Contato *buscar_tel(Agenda *a)
{
    if (a == NULL || a->contatos == NULL) {
        return NULL;
    }

    system("clear");
    printf("Telefone: ");
    char buf[BUFSIZ];
    unsigned long len = 0;
    fgets(buf, BUFSIZ, stdin);
    len = strlen(buf) - 1;
    char *t = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));

    if (t == NULL) {
        return NULL;
    }

    strcpy(t, buf);
    t[len] = '\0'; // remove o '\n' no final
    return busca_tel(t);
}

int exibir_contato(Agenda *a, Contato *c)
{
    if (c == NULL) {
        return 0;
    }

    char ch = '\0';
    do {
        system("clear");
        puts("INFORMACOES DO CONTATO");
        // imprime string vazia se ponteiro for NULL
        printf("n - Nome: %s\n", c->nome ? c->nome : "");
        printf("\ns - Sobrenome: %s\n", c->sobrenome ? c->sobrenome : "");
        printf("\nt - Telefones: %s\n", string_telefones(c));
        printf("\ne - Email: %s\n", c->email ? c->email : "");
        printf("\nc - Cargo: %s\n", c->cargo ? c->cargo : "");
        printf("\nm - Empresa: %s\n", c->empresa ? c->empresa : "");
        printf("\no - Observacoes: %s\n", c->observacoes ? c->observacoes : "");

        puts("\nOPÇÕES");
        puts("p - Proximo contato");
        puts("a - Contato anterior");
        puts("l - Realizar chamada");
        puts("r - Remover contato");
        puts("v - Voltar");
        putchar('\n');

        char buf[BUFSIZ];
        char *str = NULL;
        unsigned long len = 0;
        scanf("%c%*c", &ch);
        switch (ch) {
        case 'n':
            printf("\nDigite o novo nome: ");
            fgets(buf, BUFSIZ, stdin);
            len = strlen(buf);
            str = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
            if (str != NULL) {
                strcpy(str, buf);
                str[len - 1] = '\0'; // remove o '\n' no final
                edita_contato(c, str, NOME);
                // Remove e insere o contato para manter a ordenação
                add_contato(a, rm_contato(a, c));
            }
            break;

        case 's':
            printf("\nDigite o novo sobrenome: ");
            fgets(buf, BUFSIZ, stdin);
            len = strlen(buf);
            str = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
            if (str != NULL) {
                strcpy(str, buf);
                str[len - 1] = '\0'; // remove o '\n' no final
                edita_contato(c, str, SOBRENOME);
                // Remove e insere o contato para manter a ordenação
                add_contato(a, rm_contato(a, c));
            }
            break;

        case 't':
            printf("a - Adicionar telefone\nr - Remover telefone\n");
            scanf("%c%*c", &ch);
            switch (ch) {
            case 'a':
                printf("\nNovo numero: ");
                fgets(buf, BUFSIZ, stdin);
                len = strlen(buf);
                str =
                    buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
                if (str != NULL) {
                    strncpy(str, buf, TELSIZ);
                    str[len - 1] = '\0'; // remove o '\n' no final
                    if (!busca_tel(str)) {
                        add_tel(c, str);
                    } else {
                        puts("Numero ja pertence a um contato");
                        free(str);
                    }
                }
                break;

            case 'r':
                printf("\nNumero: ");
                fgets(buf, BUFSIZ, stdin);
                len = strlen(buf);
                str =
                    buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
                if (str != NULL) {
                    strcpy(str, buf);
                    str[len - 1] = '\0'; // remove o '\n' no final
                    rm_tel(c, str);
                }
                break;

            default:
                break;
            }
            break;

        case 'e':
            printf("\nDigite o novo email: ");
            fgets(buf, BUFSIZ, stdin);
            len = strlen(buf);
            str = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
            if (str != NULL) {
                strcpy(str, buf);
                str[len - 1] = '\0'; // remove o '\n' no final
                edita_contato(c, str, EMAIL);
            }
            break;

        case 'c':
            printf("\nDigite o novo cargo: ");
            fgets(buf, BUFSIZ, stdin);
            len = strlen(buf);
            str = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
            if (str != NULL) {
                strcpy(str, buf);
                str[len - 1] = '\0'; // remove o '\n' no final
                edita_contato(c, str, CARGO);
            }
            break;

        case 'm':
            printf("\nDigite a nova empresa: ");
            fgets(buf, BUFSIZ, stdin);
            len = strlen(buf);
            str = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
            if (str != NULL) {
                strcpy(str, buf);
                str[len - 1] = '\0'; // remove o '\n' no final
                edita_contato(c, str, EMPRESA);
            }
            break;

        case 'o':
            printf("\nDigite a nova observacao: ");
            fgets(buf, BUFSIZ, stdin);
            len = strlen(buf);
            str = buf[0] == '\n' ? NULL : (char *)malloc(len * sizeof(char));
            if (str != NULL) {
                strcpy(str, buf);
                str[len - 1] = '\0'; // remove o '\n' no final
                edita_contato(c, str, OBSERVACOES);
            }
            break;

        case 'l':
            if (c->telefones) {
                ligar(a, c->telefones->numero);
            } else {
                puts("O contato não possui numeros de telefone");
                puts("Pressione enter");
                getchar();
            }
            break;

        case 'r':
            printf("Tem certeza que deseja remover o contato? s/n ");
            scanf("%c%*c", &ch);
            if (ch == 's') {
                exclui_contato(rm_contato(a, c));
                return -1;
            }
            break;

        case 'p':
            return 1;
            break;

        case 'a':
            return -1;
            break;

        case 'v':
            return 0;

        default:
            break;
        }
    } while (ch != 'v');

    return 0;
}

void exibir_contatos(Agenda *a)
{
    system("clear");
    unsigned int max = a->ncontatos;
    Contato *contatos[max];

    unsigned int i = 0;
    for (i = 0; i < max; i++) {
        if (i == 0) {
            contatos[i] = a->contatos;
        } else {
            contatos[i] = contatos[i - 1]->proximo;
        }
        printf("%d - %s %s\n\n", i + 1, contatos[i]->nome,
               contatos[i]->sobrenome ? contatos[i]->sobrenome : "");
    }

    printf("Quantidade de contatos: %d\n", a->ncontatos);
    printf("Digite 0 para voltar\n");
    printf("Digite o indice do contato que deseja visualizar: ");
    scanf("%d%*c", &i);

    if (i == 0) {
        return;
    }

    if (i <= max) {
        Contato *contato = contatos[i - 1];
        int opcao = 0;
        do {
            Contato *anterior = contato->anterior;
            opcao = exibir_contato(a, contato);
            max = a->ncontatos; // Atualiza em caso de remoção
            if (opcao == 1) {
                contato = contato->proximo;
            } else if (opcao == -1) {
                contato = anterior;
            }
        } while (opcao != 0 && a->contatos != NULL);
    } else {
        puts("Indice invalido");
        puts("Pressione enter");
        getchar();
    }
}

void exibir_historico(Agenda *a)
{
    system("clear");
    unsigned int max = a->nhistorico;
    Historico *historico = a->historico;

    unsigned int i = 0;
    for (i = 0; i < max; i++) {
        if (historico->conhecido) {
            Contato *contato = historico->entrada.contato;
            printf("%s %s\n\n", contato->nome,
                   contato->sobrenome ? contato->sobrenome : "");
        } else {
            printf("Desconhecido - %s\n\n", historico->entrada.numero);
        }
        historico = historico->proximo;
    }

    puts("\n\nPressione enter\n");
    getchar();
}

void ligacao(Agenda *a)
{
    system("clear");
    printf("Digite o numero: ");
    char buf[BUFSIZ];
    fgets(buf, BUFSIZ, stdin);
    char *str =
        buf[0] == '\n' ? NULL : (char *)calloc((TELSIZ + 1), sizeof(char));
    if (str != NULL) {
        strncpy(str, buf, 15);
        int len = strlen(str);
        str[len - 1] = '\0'; // remove o '\n' no final
        ligar(a, str);
    } else {
        puts("Numero invalido\n");
        puts("Pressione enter\n");
        getchar();
    }
}
