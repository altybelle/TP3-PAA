#include "TAD/analise_frequencia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int i;
    int tamanho_texto;
    char *texto;
    char caminho[250];

    FILE *f;

    printf("Insira o caminho do texto: ");
    scanf("%s", caminho);

    f = fopen(caminho, "r");

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