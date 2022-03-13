#ifndef ANALISE_FREQUENCIA
#define ANALISE_FREQUENCIA

#define TAMANHO_ALFABETO 52

typedef struct analise_frequencia {
    char letra;
    int contagem;
    float frequencia;
} analise_frequencia;

analise_frequencia *executa_analise(char*, int);
void imprime_analise(analise_frequencia);
int retorna_indice(analise_frequencia*, char);
void ordena_frequencia(analise_frequencia*);
int eh_letra(char);

#endif // ANALISE_FREQUENCIA