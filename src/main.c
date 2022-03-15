#include "TAD/analise_frequencia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_LENGTH 256
#define ASCII_A 65
#define ASCII_Z 90

int escolhe_opcoes();
void estado_atual_criptoanalise(char*, char*, char*);
void faz_analise_de_frequencia(analise_frequencia*, char*, int);
int casamento_exato(char*);
void preprocessa_substring(char*, int, int*);

int main(int argc, char **argv) {
    int i;
    int opcao;
    int tamanho_texto;
    int ocorrencias;

    char *texto = NULL;
    char *chave = NULL;
    char *texto_descriptografado = NULL;
    analise_frequencia *analise = NULL;
    

    FILE *f;

    f = fopen(argv[1], "r");

    if (!f) { 
        if (!argv[1]) {
            puts("Favor informar o caminho do arquivo como primeiro parametro do executavel.");
        } else {
            puts("Arquivo nao encontrado. Fechando programa.");
        }
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    texto = (char*) malloc(fsize + 1);
    fread(texto, fsize, 1, f);

    tamanho_texto = strlen(texto);

    texto_descriptografado = (char*) malloc(sizeof(char) * tamanho_texto);
    strcpy(texto_descriptografado, texto);
    chave = (char*) malloc(sizeof(char) * TAMANHO_ALFABETO);

    for (i = 0; i < TAMANHO_ALFABETO; i++) {
        chave[i] = '@';
    }

    do {
        opcao = escolhe_opcoes();
        switch (opcao) {
            case 1:
                estado_atual_criptoanalise(texto, chave, texto_descriptografado);
                break;
            case 2:
                faz_analise_de_frequencia(analise, texto, tamanho_texto);
                break;
            case 3:
                ocorrencias = casamento_exato(texto);
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            default:
                printf("Opcao invalida. Tente novamente...");
        }
    } while (opcao != 6);
}

int escolhe_opcoes() {
    int opcao;
    puts("=================================================================================");
    puts("Modos de operacao: ");
    puts("1. Apresentar o estado atual da criptoanálise;");
    puts("2. Fazer analise de frequencia no texto criptografado;");
    puts("3. Realizar casamento exato de caracteres no texto criptografado;");
    puts("4. Realizar casamento aproximado de caracteres no texto parcialmente decifrado;");
    puts("5. Alterar chave de criptografia;");
    puts("6. Exportar resultado e encerrar o programa.");
    puts("=================================================================================");
    printf("Informe a opcao escolhida: "); scanf("%d", &opcao);
    return opcao;
}

void estado_atual_criptoanalise(char *texto, char *chave, char *texto_decifrado) {
    int i;
    char abecedario[26];
    int tam_texto = strlen(texto);

    puts("=================================================================================");
    puts("=== Texto Criptografado ===");
    printf("%s\n\n", texto);

    puts("=== Chave ===");
    for (i = 0; i < TAMANHO_ALFABETO; i++) {
        abecedario[i] = (char) (i + ASCII_A);
        printf("%c", abecedario[i]);
    }
    puts("");
    for (i = 0; i < TAMANHO_ALFABETO; i++) {
        if (chave[i] != '@') {
            printf("%c", chave[i]);
        } else {
            printf(" ");
        }
    }
    puts("\n\n=== Texto parcialmente decifrado ===");
    puts(texto_decifrado);

    for (i = 0; i < tam_texto; i++) {
        if (texto[i] != texto_decifrado[i]) {
            printf("^");
        } else {
            printf(" ");
        }
    }
    puts("");
}

void faz_analise_de_frequencia(analise_frequencia *analise, char* texto, int tamanho_texto) {
    int i;
    analise = executa_analise(texto, tamanho_texto);

    for (i = 0; i < TAMANHO_ALFABETO; i++) {
        if (analise[i].letra != '@')
            imprime_analise(analise[i]);
        else
            break;
    }
}

int casamento_exato(char *texto) {
    int i, j, res, next_i;
    int M, N;
    int *lps;
    char pattern[256];
    puts("> Qual o padrao utilizado?");
    scanf("%s", pattern);

    M = strlen(pattern);
    N = strlen(texto);

    lps = (int*) malloc(sizeof(int) * M);
    j = 0;

    preprocessa_substring(pattern, M, lps);
    i = res = next_i = 0;

    while (i < N) {
        if (pattern[j] == texto[i]) {
            j++;
            i++;
        }
        if (j == M) {
            j = lps[j - 1];
            res++;

            if (lps[j] != 0) {
                next_i++;
                i = next_i;
                j = 0;
            }
        } else if ((i < N) && (pattern[j] != texto[i])) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }

    printf("Ocorrencias: %d\n", res);
    return res;
}

void preprocessa_substring(char* pattern, int M, int* lps) {
    int i, size;
    
    size = 0;
    i = 1;
    lps[0] = 0;
    while (i < M) {
        if (pattern[i] == pattern[size]) {
            size++;
            lps[i] = size;
            i++;
        } else {
            if (size != 0) {
                size = lps[size - 1];
            }
            else {
                lps[i] = size;
                i++;
            }
        }
    }
}