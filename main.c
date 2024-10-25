#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"

#define N 30  // matriz 30x30

char floresta[N][N];
pthread_mutex_t mutex_matriz;
pthread_cond_t cond_central;
int incendios_ativos = 0;

void *sensor(void *arg) {
    Posicao *pos = (Posicao *)arg;
    
    while (1) {
        pthread_mutex_lock(&mutex_matriz);
        
        if (floresta[pos->x][pos->y] == '@') {
            printf("Sensor (%d,%d) detectou fogo!\n", pos->x, pos->y);


            propagaFogo(pos->x, pos->y); // propagação


            if (pos->x == 0 || pos->x == N-1 || pos->y == 0 || pos->y == N-1) { //se borda 
                printf("Sensor na borda (%d,%d) avisando a central!\n", pos->x, pos->y);
                pthread_cond_signal(&cond_central); //central
            }
        }
        pthread_mutex_unlock(&mutex_matriz);
        usleep(100000);  // Tempo para próximo ciclo de verificação
    }
    
    return NULL;
}

void *centralControle(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex_matriz);
        
        while (incendios_ativos == 0) {     //espera incendio
            pthread_cond_wait(&cond_central, &mutex_matriz);
        }
        
        apagaFogo();

        pthread_mutex_unlock(&mutex_matriz);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mutex_matriz, NULL);
    pthread_cond_init(&cond_central, NULL);

    inicializaFloresta();

    pthread_t thread_incendios; //geradora de fogo
    pthread_create(&thread_incendios, NULL, gerarIncendios, NULL);

    pthread_t threads_sensores[N][N]; //sensores
    Posicao posicoes[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            posicoes[i][j].x = i;
            posicoes[i][j].y = j;
            pthread_create(&threads_sensores[i][j], NULL, sensor, &posicoes[i][j]);
        }
    }

    
    pthread_t thread_central; //central de controle
    pthread_create(&thread_central, NULL, centralControle, NULL);

    while (1) {
        system("clear");
        exibeFloresta();
        sleep(1);
    }

    pthread_mutex_destroy(&mutex_matriz);
    pthread_cond_destroy(&cond_central);
    return 0;
}
