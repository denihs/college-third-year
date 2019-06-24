#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <pthread.h> 

#define MAX_ITENS 10

int tam, nProduzido;


struct Node{
 int num;
 struct Node *prox;
};
typedef struct Node node;

void inicia(node *PILHA) {
 PILHA->prox = NULL;
 nProduzido=0;
 tam=0;
}


int vazia(node *PILHA) {
    return PILHA->prox == NULL;
}

node *aloca(int n) {
    node *novo=(node *) malloc(sizeof(node));
    if(!novo){
        printf("Sem memoria disponivel!\n");
        exit(1);
    }
    printf("Produzido: %d\n", n);
    novo->num = n;
    return novo;
 
}

void push(node *PILHA, int n) {
    node *novo=aloca(n);
    novo->prox = NULL;

    if(vazia(PILHA))
    PILHA->prox=novo;
    else{
    node *tmp = PILHA->prox;

    while(tmp->prox != NULL)
    tmp = tmp->prox;

    tmp->prox = novo;
    }
    tam++;
}


node *pop(node *PILHA) {
 if(PILHA->prox == NULL){
  printf("PILHA ja vazia\n\n");
  return NULL;
 }else{
  node *ultimo = PILHA->prox,
              *penultimo = PILHA;

  while(ultimo->prox != NULL){
   penultimo = ultimo;
   ultimo = ultimo->prox;
  }

  penultimo->prox = NULL;
  tam--;
  return ultimo;
 }
}

const produtor(node *P) {
    while(1) {
        int n = nProduzido++;
        if (tam == MAX_ITENS) {
            printf("Produtor dormindo!\n");
        }
        while(tam >= MAX_ITENS);
        push(P, n);
    }
}

const consumidor(node *P) {
    while(1) {
        if (!tam) {
            printf("Consumidor dormindo!\n");
        }
        while(tam <= 0);
        node *ultimo = pop(P);
        printf("Consumido: %d\n", ultimo->num);
    }
}

int main() {

    node *PILHA = (node *) malloc(sizeof(node));
    if(!PILHA){
        printf("Sem memoria disponivel!\n");
        exit(1);
    }
    inicia(PILHA);
    
    pthread_t prod_thread_id, cons_thread_id; 

    pthread_create(&prod_thread_id, NULL, produtor, (node *) PILHA);
    pthread_create(&cons_thread_id, NULL, consumidor, (node *) PILHA);
    
    pthread_join(prod_thread_id, NULL); 
    pthread_join(cons_thread_id, NULL); 

    free(PILHA);
    return 0;
}
