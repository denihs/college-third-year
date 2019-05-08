//Código thread usando passagem de argumentos
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define num_thread 8
char *messagem[num_thread];


void * imprime_oi(void *threadid) {
    long tid;
    sleep(2);
    tid = (long) threadid;
    printf("Thread %ld: %s\n", tid, messagem[tid]);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[num_thread];
    long tid[num_thread];
    int rc, t;

    messagem[0] = "O Naruto";
    messagem[1] = "pode ser um";
    messagem[2] = "pouco duro as vezes...";
    messagem[3] = "Talvez vc não saiba disso";
    messagem[4] = "mas o Naruto tbm cresceu sem pai";
    messagem[5] = "na verdade";
    messagem[6] = "ele nunca conheceu nenhum de seus pais!";
    messagem[7] = "Mas nunca vi ele chorar ou reclamar";

    for(t = 0; t < num_thread; t++) {
        tid[t]=t;
        printf("Criando thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, imprime_oi, (void *) tid[t]);
        if (rc) {
            printf("Erro: codigo retornado do pthread_create() é: %d\n", rc);
            exit(1);
        }
    }

    pthread_exit(NULL);
}