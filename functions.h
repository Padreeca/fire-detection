#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define N 30

typedef struct {
    int x, y;
} Posicao;

void inicializaFloresta();
void *gerarIncendios(void *arg);
void propagaFogo(int x, int y);
void apagaFogo();
void exibeFloresta();

#endif
