#include "TAD/analise_frequencia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_A 65
#define ASCII_Z 90

int escolhe_opcoes();
void estado_atual_criptoanalise(char*, char*, char*);
void faz_analise_de_frequencia(analise_frequencia*, char*, int);
int conta_ocorrencias(char*, char*);

int main(int argc, char **argv) {
    int i;
    int opcao;
    int tamanho_texto;

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

int conta_ocorrencias(char *texto, char *busca) {
    int contagem = 0;
    const char *string_auxiliar = texto;
    while ((string_auxiliar = strstr(string_auxiliar, busca))) {
        contagem++;
        string_auxiliar++;
    }
    return contagem;
}