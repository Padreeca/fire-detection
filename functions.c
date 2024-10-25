#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

extern char floresta[N][N];
extern pthread_mutex_t mutex_matriz;
extern int incendios_ativos;

void inicializaFloresta() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            floresta[i][j] = '-';  //inicia livre
        }
    }
}

void *gerarIncendios(void *arg) {
    while (1) {
        int x = rand() % N;
        int y = rand() % N;
        
        pthread_mutex_lock(&mutex_matriz);
        if (floresta[x][y] == '-') {
            floresta[x][y] = '@';  // fogo
            incendios_ativos++;
        }
        pthread_mutex_unlock(&mutex_matriz); 
        sleep(3); //fogo em 3s
    }
    return NULL;
}

void propagaFogo(int x, int y) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
            if (floresta[nx][ny] == '-') {
                floresta[nx][ny] = '@';  // Propaga o fogo
            }
        }
    }
}

void apagaFogo() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (floresta[i][j] == '@') {
                floresta[i][j] = '/';  //apagou
                incendios_ativos--;
            }
        }
    }
}

void exibeFloresta() {
    pthread_mutex_lock(&mutex_matriz);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", floresta[i][j]);
        }
        printf("\n");
    }
    pthread_mutex_unlock(&mutex_matriz);
}
