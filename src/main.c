#include "TAD/analise_frequencia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int conta_ocorrencias(char*, char*);

int main(int argc, char **argv) {
    int i;
    int tamanho_texto;
    char *texto;

    FILE *f;

    f = fopen(argv[1], "r");

    if (!f) return 0;

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    texto = (char*) malloc(fsize + 1);
    fread(texto, fsize, 1, f);

    tamanho_texto = strlen(texto);

    analise_frequencia *analise = executa_analise(texto, tamanho_texto);

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