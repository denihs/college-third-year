#include <stdio.h>
#include <stdlib.h>

int TAM = 0;

void exibir(int Matriz[][TAM],int row,int col) {
    int i,j;
    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            printf("%d ",Matriz[i][j]);
        }
        printf("\n");
    }

}


void fill(int m[][TAM], FILE * f) {

    for(int i = 0; i < TAM; i++) {
        for(int j = 0; j < TAM; j++) {
            fscanf(f, "%d ", &m[i][j]);
        }
    }
}

int main() {
    FILE * ma, * mb;
    ma = fopen("matrizA.txt", "r");
    mb = fopen("matrizB.txt", "r");

    if (!ma || !mb) {
            perror("Error while opening the file.\n");
            exit(1);
    }

    fscanf(ma, "%d", &TAM); // Iniciando o Tamanho da matriz com o primeiro valor no arquivo matrizA.txt
    int matrizA[TAM][TAM], matrizB[TAM][TAM];

    fill(matrizA, ma);
    fill(matrizB, mb);

    exibir(matrizA, TAM, TAM);
    exibir(matrizB, TAM, TAM);

    fclose(ma);
    fclose(mb);

   return 0;
}