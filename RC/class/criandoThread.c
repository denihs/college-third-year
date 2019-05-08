#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define num_thread 5

void * imprime_oi(void *threadid) {
    long tid;
    tid = (long) threadid;
    printf("Oi mundo! EU sou a thread #%ld\n", tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t thread[num_thread];
    int rc;
    long t;

    for(t = 0; t < num_thread; t++) {
        printf("dentro da main: criando a thread #%ld\n", t);
        rc = pthread_create(&thread[t], NULL, imprime_oi, (void *) t);
        if (rc) {
            printf("Erro: codigo retornado do pthread_create() é: %d\n", rc);
            exit(1);
        }
    }

    pthread_exit(NULL);
}