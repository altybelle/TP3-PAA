#include "analise_frequencia.h"

#include <stdio.h>
#include <stdlib.h>

analise_frequencia *executa_analise(char *texto, int tamanho_texto) {
    int i, j;
    int total_letras;

    analise_frequencia *analise;
    analise_frequencia temp = {' ', 0, 0};

    analise = (analise_frequencia*) malloc(sizeof(analise_frequencia) * TAMANHO_ALFABETO);
    
    for (i = 0; i < TAMANHO_ALFABETO; i++) {
        analise[i] = temp;
    }

    j = total_letras = 0;
    
    for (i = 0; i < tamanho_texto; i++) {
        if (eh_letra(texto[i])) {
            total_letras++;
            int indice = retorna_indice(analise, texto[i]);
            if (indice != -1) {
                analise[indice].contagem++;
            } else {
                analise_frequencia item = { texto[i], 1, 0 };
                analise[j] = item;
                j++;
            }
        }
    }

    for (i = 0; i < TAMANHO_ALFABETO; i++) {
        if (analise[i].letra != ' ') {
            analise[i].frequencia = (((float)analise[i].contagem) / ((float)total_letras)) * 100.0;
        } else {
            break;
        }
    }

    ordena_frequencia(analise);

    return analise;
}

int retorna_indice(analise_frequencia *analise, char letra) {
    int i;
    for (i = 0; i < TAMANHO_ALFABETO; i++)
        if (analise[i].letra == letra)
            return i;
    return -1;
}

void imprime_analise(analise_frequencia analise) {
    printf("%c\t\t%d\t\t%.2f%%\n", analise.letra, analise.contagem, analise.frequencia);
}

void ordena_frequencia(analise_frequencia* analise) {
    int i, j;
    analise_frequencia chave;
    for (i = 1; i  < TAMANHO_ALFABETO; i++) {
        chave = analise[i];
        j = i - 1;
        while (j >= 0 && analise[j].contagem < chave.contagem) {
            analise[j + 1] = analise[j];
            j = j - 1;
        }
        analise[j + 1] = chave;
    }
}

int eh_letra(char caractere) {
    return 1 * (caractere >= 65 && caractere <= 90);
}
