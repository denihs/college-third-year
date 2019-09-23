#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
 char elem;
 struct Node *prox;
};
typedef struct Node node;


void inicia(node *PILHA)
{
 PILHA->prox = NULL;
}

int vazia(node *PILHA)
{
 if(PILHA->prox == NULL)
  return 1;
 else
  return 0;
}

node *aloca(char elem)
{
 node *novo=(node *) malloc(sizeof(node));
 if(!novo){
  printf("Sem memoria disponivel!\n");
  exit(1);
 }else{
  novo->elem = elem;
  return novo;
 }
}

void libera(node *PILHA)
{
 if(!vazia(PILHA)){
  node *proxNode,
          *atual;

  atual = PILHA->prox;
  while(atual != NULL){
   proxNode = atual->prox;
   free(atual);
   atual = proxNode;
  }
 }
}

void showStack(node * p) {
 if(vazia(p)) {
  printf("Pilha esvaziada! \n");
 }
 node * temp = p;
 while (temp) {
  printf("%c ", temp->elem);
  temp = temp->prox;
 }
 printf("\n");
}

void push(node *PILHA, char elem)
{
 node *novo=aloca(elem);
 novo->prox = NULL;

 if(vazia(PILHA))
  PILHA->prox=novo;
 else{
  node *tmp = PILHA->prox;

  while(tmp->prox != NULL)
   tmp = tmp->prox;

  tmp->prox = novo;
 }
 showStack(PILHA);
}


node *pop(node *PILHA)
{
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
  showStack(PILHA);
  return ultimo;
 }
}

node *verifyWord(char * word, int size) {
 char a = 'a';
 char b = 'b';
 char letter;
 int isValid;
 int isBTime = 0;

 node *PILHA = aloca(' ');

 inicia(PILHA);

 if (word[0] != a) {
  printf("Palavra não aceita! A palavra deve começar com o caracter 'a'.\n");
  return PILHA;
 }


 for(int i = 0; i < size; i++) {
   letter = word[i];
   isValid = vazia(PILHA);

   if (letter == a && !isBTime) {
    push(PILHA, a);
   } else if (letter == b && !isValid) {
    isBTime = 1;
    pop(PILHA);
   } else {
    if (isValid) printf("Palavra não aceita! A quantidade de B's é maior que a de A's,\n");
    else printf("Palavra não aceita! O caracter  '%c' não faz parte do alfabeto.\n", letter);

    return PILHA;
   }
 }

 if (vazia(PILHA)) {
  printf("Palavra aceita\n");
  return PILHA;
 }
 printf("Palavra não aceita! A quantidade de A's é maior que a de B's.\n");
 return PILHA;
}

int main(int argc, char *argv[])
{

 char * word = argv[1];
 if (!word) {
  printf("Passe a palavra como argumento\n");
  return 0;
 }

 int wordSize = strlen(word);

if (wordSize < 2) {
 printf("A palavra deve ter um tamanho maior ou igual a dois\n");
 return 0;
}


 int opt;

 libera(verifyWord(word, wordSize));
 return 0;
}
