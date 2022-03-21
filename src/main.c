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
void casamento_aproximado(char*);
void alterar_chave(char*, char*, int*);
void preprocessa_substring(char*, int, int*);
void exportar(char*, char*);

int main(int argc, char **argv) {
    int i;
    int opcao;
    int tamanho_texto;
    int ocorrencias;

    char *texto = NULL;
    char *chave = NULL;
    char *texto_descriptografado = NULL;
    int *estado_descriptografia = NULL;
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
    estado_descriptografia = (int*) malloc(sizeof(int) * tamanho_texto);

    for (i = 0; i < tamanho_texto; i++) {
        estado_descriptografia[i] = 0;
    }

    strcpy(texto_descriptografado, texto);
    chave = (char*) malloc(sizeof(char) * TAMANHO_ALFABETO);

    for (i = 0; i < TAMANHO_ALFABETO; i++) {
        chave[i] = ' ';
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
                casamento_aproximado(texto_descriptografado);
                break;
            case 5:
                alterar_chave(texto_descriptografado, chave, estado_descriptografia);
                break;
            case 6:
                exportar(chave, texto_descriptografado);
                break;
            default:
                printf("Opcao invalida. Tente novamente...");
        }
    } while (opcao != 6);

    puts("=================================================================================");
    printf("Arquivos exportados com sucesso. Saindo do programa...\n");
    puts("=================================================================================");

    return 0;
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
    printf("Informe a opcao escolhida: "); 
    scanf("%d", &opcao);
    getc(stdin);
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
        printf("%c", chave[i]);
    }
    puts("\n\n=== Texto parcialmente decifrado ===");
    
    for (i = 0; i < tam_texto; i++) {
        if (texto[i] != texto_decifrado[i]) {
            printf("\033[1;31m%c\033[0m", texto_decifrado[i]);
        } else {
            printf("%c",texto_decifrado[i]);
        }
    }
    puts("");
}

void faz_analise_de_frequencia(analise_frequencia *analise, char* texto, int tamanho_texto) {
    int i;
    analise = executa_analise(texto, tamanho_texto);

    for (i = 0; i < TAMANHO_ALFABETO; i++) {
        if (analise[i].letra != ' ')
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

void casamento_aproximado(char *texto_descriptografado) {
    int res;
    int tamanho_texto, tamanho_padrao;

    char pattern[256];
    int erros;

    puts("> Qual o padrao e a tolerancia utilizados?");
    scanf("%s", pattern);
    scanf("%d", &erros);

    res = 0;
    tamanho_texto = strlen(texto_descriptografado);
    tamanho_padrao = strlen(pattern);

    if (erros > tamanho_padrao + 1) {
        erros = tamanho_padrao + 1;
    }

    int M[ASCII_LENGTH], i, j, r_i, r_ant, r_novo;
    int *R;
    R = (int*) malloc(sizeof(int) * (tamanho_padrao + 1));

    for (i = 0; i < ASCII_LENGTH; i++) 
        M[i] = 0;
    for (i = 1; i <= tamanho_padrao; i++) 
        M[pattern[i - 1] + 127] |= 1 << (tamanho_padrao - i);
    
    R[0] = 0;
    r_i = 1 << (tamanho_padrao - 1);

    for (i = 1; i <= erros; i++)
        R[i] = (1 << (tamanho_padrao - i)) | R[i - 1];
    for (i = 0; i < tamanho_texto; i++) {
        r_ant = R[0];
        r_novo = (((r_ant) >> 1) | r_i) & M[texto_descriptografado[i] + 127];
        R[0] = r_novo;
        for (j = 1; j <= erros; j++) {
            r_novo = ((R[j] >> 1) & M[texto_descriptografado[i] + 127]) | r_ant | (((r_ant | r_novo)) >> 1);
            r_ant = R[j];
            R[j] = r_novo | r_i;
        }
        if ((r_novo & 1) != 0)
            if (i + 1 > tamanho_padrao - 1) {
                res++;
            }
    }

    printf("Ocorrencias: %d\n", res);
}

void alterar_chave(char *texto_descriptografado, char* chave, int *estado_descriptografia) {
    int i, j;
    int tamanho_texto, tamanho_chave;
    char original, criptografada;
    puts("Informe a letra original, seguida da letra para a qual foi mapeada:");
    printf("> "); 
    scanf("%c", &original);
    getc(stdin);
    scanf("%c", &criptografada);

    chave[((int)original) - ASCII_A] = criptografada;

    tamanho_texto = strlen(texto_descriptografado);
    tamanho_chave = (ASCII_Z - ASCII_A) + 1;

    for (i = 0; i < tamanho_texto; i++) {
        for (j = 0; j < tamanho_chave; j++) {
            if (((int)texto_descriptografado[i]) == j + ASCII_A && estado_descriptografia[i] != 1 && chave[j] != ' ') {
                texto_descriptografado[i] = chave[j];
                estado_descriptografia[i] = 1;
            }
        }
    }

    printf("Registrado: %c -> %c\n", original, criptografada);
}

void exportar(char *chave, char *texto_descriptografado) {
    FILE *f = NULL;
    char caminho_arquivo[250];

    printf("Digite o nome e formato do arquivo em que sera salva a chave: ");
    scanf("%s", caminho_arquivo);
    f = fopen(caminho_arquivo, "w");

    if (!f) {
        printf("Erro: impossivel salvar o arquivo.\n");
        return;
    }

    fprintf(f, "%s", chave);
    fclose(f);

    printf("Digite o nome e formato do arquivo em que sera salvo o texto descriptografado: ");
    scanf("%s", caminho_arquivo);

    f = fopen(caminho_arquivo, "w");
    
    if (!f) {
        printf("Erro: impossivel salvar o arquivo.\n");
        return;
    }
    
    fprintf(f, "%s", texto_descriptografado);
    fclose(f);
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