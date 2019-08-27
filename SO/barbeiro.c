#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CLIENTES 6
#define N_CADEIRAS 5

sem_t customers; 
sem_t barber; 
sem_t seats; 
int FreeSeats = N_CADEIRAS; 
  
void Barber() { 
      while(1) { 
            if (FreeSeats == N_CADEIRAS) {
              printf("Barbeiro dormindo\n");
            }
            sem_wait(&customers); 
            /* Protegendo o numero de acentos*/
            sem_wait(&seats); 

            
            FreeSeats++; 
             
            printf("Cliente sentando na cadeira do barbeiro\n");
            printf("Uma cadeira a mais livre: (%d)\n", FreeSeats);
            sem_post(&barber); 
            sem_post(&seats); 
            /* barber is cutting hair.*/
            printf("Barbeiro trabalhando\n");
            sleep(1+rand()%2);
      } 
} 
  
void Customer() { 
      while(1) {
            sleep(1+rand()%10);
            sem_wait(&seats);
            if(FreeSeats > 0) {
                  FreeSeats--; 
                  printf("Cliente sentou para esperar\n");
                  printf("Cadeiras livres %d\n", FreeSeats);
                  
                  sem_post(&customers); 
                  sem_post(&seats); 
                   
                  // Esperando se o barbeiro estiver trabalhando
                  sem_wait(&barber);
                  printf("Cliente tendo corte\n");
            } else { 
                  printf("Cliente deixou a barbeiraria\n");
                  sem_post(&seats); 
            } 
      } 
} 

int main() {
  srand(time(NULL));
  pthread_t thr_clientes[N_CLIENTES], thr_barbeiro;
  int i, id[N_CLIENTES];

  sem_init(&customers, 0, 0);
  sem_init(&barber, 0, 0);
  sem_init(&seats, 0, 1);
  

  for(int i=0; i < N_CLIENTES; i++)
    pthread_create(&thr_clientes[i], NULL, Customer, NULL);

  pthread_create(&thr_barbeiro, NULL, Barber, NULL);
  
  for(int i=0; i < N_CLIENTES; i++)
    pthread_join(thr_clientes[i], NULL);
  pthread_join(thr_barbeiro, NULL);
  
  return 0;
}