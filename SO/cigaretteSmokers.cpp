//
// Created by denyhs on 28/09/19.
//
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


sem_t mutex;

sem_t agentSem;
sem_t tobacco;
sem_t match;
sem_t paper;

bool isTobacco = false, isPaper = false, isMatch = false;
sem_t tobaccoSem;
sem_t matchSem;
sem_t paperSem;


int getRand(int range){
    return rand() % range;
}

void* pusherA(void *) {
    while (true) {
        sem_wait(&tobacco);
        sem_wait(&mutex);
            if (isPaper) {
                isPaper = false;
                sem_post(&matchSem);
            } else if (isMatch) {
                isMatch = false;
                sem_post(&paperSem);
            } else {
                isTobacco = true;
            }
        sem_post(&mutex);
    }
}

void* pusherB(void *) {
    while (true) {
        sem_wait(&paper);
        sem_wait(&mutex);
            if (isTobacco) {
                isTobacco = false;
                sem_post(&matchSem);
            } else if (isMatch) {
                isMatch = false;
                sem_post(&tobaccoSem);
            } else {
                isPaper = true;
            }
        sem_post(&mutex);
    }
}

void* pusherC(void *) {
    while (true) {
        sem_wait(&match);
        sem_wait(&mutex);
            if (isTobacco) {
                isTobacco = false;
                sem_post(&paperSem);
            } else if (isPaper) {
                isPaper = false;
                sem_post(&tobaccoSem);
            } else {
                isMatch = true;
            }
        sem_post(&mutex);
    }
}

void* smokerWithTobacco(void *) {
    while (true) {
        sem_wait(&tobaccoSem);
        printf("\n\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
        printf("Fumante com o tabaco: Fazendo o cigarro.\n");
        sem_post(&agentSem);
        printf("Fumante com o tabaco: Fumando...\n");
        printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
    }
}

void* smokerWithMatch(void *) {
    while (true) {
        sem_wait(&matchSem);
        printf("\n\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
        printf("Fumante com o fósfaro: Fazendo o cigarro.\n");
        sem_post(&agentSem);
        printf("Fumante com o fósfaro: Fumando...\n");
        printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
    }
}

void* smokerWithPaper(void *) {
    while (true) {
        sem_wait(&paperSem);
        printf("\n\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
        printf("Fumante com o papel: Fazendo o cigarro.\n");
        sem_post(&agentSem);
        printf("Fumante com o papel: Fumando...\n");
        printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
    }
}

void* agent(void*) {
    int sleepNum = 1;
    while (true) {
        int num = getRand(3);

        switch (num) {
            case 0: {
                sem_wait(&agentSem);
                printf("\n===> Colocando na mesa: Tabaco e papel\n");
                sem_post(&tobacco);
                sem_post(&paper);
                break;
            }
            case 1: {
                sem_wait(&agentSem);
                printf("\n===> Colocando na mesa: Papel e fósfaro\n");
                sem_post(&paper);
                sem_post(&match);
                break;
            }
            case 2: {
                sem_wait(&agentSem);
                printf("\n===> Colocando na mesa: Fósfaro e tabaco\n");
                sem_post(&tobacco);
                sem_post(&match);
                break;
            }
            default:
                printf("\n===> Um número inválido foi gerado: %d\n", num);
        }
        sleep(sleepNum);
    }
}

int main() {
    srand(time(NULL));
    pthread_t pusherAT, pusherBT, pusherCT, sTobaccoT, sPaperT, sMatchT, agentT;

    sem_init(&mutex, 0, 1);

    sem_init(&agentSem, 0, 1);
    sem_init(&tobacco, 0, 0);
    sem_init(&match, 0, 0);
    sem_init(&paper, 0, 0);

    sem_init(&tobaccoSem, 0, 0);
    sem_init(&matchSem, 0, 0);
    sem_init(&paperSem, 0, 0);

    pthread_create(&agentT, NULL, agent, NULL);
    pthread_create(&pusherAT, NULL, pusherA, NULL);
    pthread_create(&pusherBT, NULL, pusherB, NULL);
    pthread_create(&pusherCT, NULL, pusherC, NULL);
    pthread_create(&sTobaccoT, NULL, smokerWithTobacco, NULL);
    pthread_create(&sPaperT, NULL, smokerWithPaper, NULL);
    pthread_create(&sMatchT, NULL, smokerWithMatch, NULL);


    pthread_join(agentT, NULL);
    pthread_join(pusherAT, NULL);
    pthread_join(pusherBT, NULL);
    pthread_join(pusherCT, NULL);
    pthread_join(sTobaccoT, NULL);
    pthread_join(sPaperT, NULL);
    pthread_join(sMatchT, NULL);

    return 0;
}