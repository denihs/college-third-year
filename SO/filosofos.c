#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
  
#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (numFil + 4) % N 
#define RIGHT (numFil + 1) % N 

int state[N]; 
int filo[N] = { 0, 1, 2, 3, 4 }; 
  
sem_t mutex; 
sem_t S[N]; 
  
void test(int numFil) 
{ 
    if (state[numFil] == HUNGRY 
        && state[LEFT] != EATING 
        && state[RIGHT] != EATING) { 
        state[numFil] = EATING; 
  
        sleep(1+rand()%5); 
  
        printf("O filósofo %d pegou os garfos %d e %d\n", 
                      numFil + 1, LEFT + 1, numFil + 1); 
  
        printf("O filósofo %d está comendo\n", numFil + 1); 
  
        // usado para acordar filósofos famintos durando o "putfork"
        sem_post(&S[numFil]); 
    } 
} 
  
void take_fork(int numFil) 
{ 
  
    sem_wait(&mutex); 

    state[numFil] = HUNGRY; 
  
    printf("O filosofo %d está com fome\n", numFil + 1); 
  
    // Come se o vizinho não está comendo
    test(numFil); 
  
    sem_post(&mutex); 
  
    // se não é possivel comer ainda, espera até ser sinalizado
    sem_wait(&S[numFil]); 
  
    sleep(1+rand()%5); 
} 
  

void put_fork(int numFil) 
{ 
  
    sem_wait(&mutex); 

    state[numFil] = THINKING; 
  
    printf("O filósofo %d colocou os garfos %d e %d na mesa\n", 
           numFil + 1, LEFT + 1, numFil + 1); 
    printf("O filósofo %d está pensando\n", numFil + 1); 
  
    test(LEFT); 
    test(RIGHT); 
  
    sem_post(&mutex); 
} 
  
void* filosofo(void* num) 
{ 
  
    while (1) { 
  
        int* i = num; 
        sleep(1+rand()%5);
  
        take_fork(*i); 
  
        sleep(1+rand()%5); 
  
        put_fork(*i); 
    } 
} 
  
int main() 
{ 
    srand(time(NULL));
    int i; 
    pthread_t thread_id[N]; 
  
    // iniciando os semáforos
    sem_init(&mutex, 0, 1); 
  
    for (i = 0; i < N; i++) 
  
        sem_init(&S[i], 0, 0); 
  
    for (i = 0; i < N; i++) { 

        pthread_create(&thread_id[i], NULL, 
                       filosofo, &filo[i]); 
  
        printf("O filosofo %d está pensando\n", i + 1); 
    } 
  
    for (i = 0; i < N; i++) 
        pthread_join(thread_id[i], NULL); 
} 