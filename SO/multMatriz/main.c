#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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

void calc(int ma[][TAM], int mb[][TAM], int index) {

    char name[50];

    sprintf(name, "processo%d.txt", index + 1);

    FILE * f = fopen(name, "w");

    if(f == NULL) {
        printf("Erro, nao foi possivel abrir o arquivo de processo #%d\n", index + 1);
        exit(1);
    }

    int result = 0;

    for (int i = 0; i < TAM; ++i) {
        for (int j = 0; j < TAM; ++j) {
            result += ma[index][j] * mb[j][i];
        }
        fprintf(f, "%d ", result);
        result = 0;
    }

    fclose(f);
}

void createProcess(int ma[][TAM], int mb[][TAM], int index) {
    if (index < 0) {
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        // fork failed
        perror("fork()  falied\n");
        exit(1);
    }
    if (pid == 0) return;

    calc(ma, mb, index);

    createProcess(ma, mb, --index);

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

    pid_t pid = fork();

    int matrizA[TAM][TAM], matrizB[TAM][TAM];

    if (pid < 0) {
        // fork failed
        printf("fork()  falied\n");
        return 1;
    }

    // Criando os processos e fazendo os calculos
    if (pid == 0) {
        // child process

        fill(matrizA, ma);
        fill(matrizB, mb);

        createProcess(matrizA, matrizB, TAM - 1);

        return 0;
    }

    // Esperando pelo fim dos processos
    wait(NULL);

    char info[TAM * 3];

    FILE * result = fopen("resultado.txt", "w"); // Criando arquivo com os resultados

    // Varrendo todos os arquivos de processos criados para pegar as respostas
    for (int i = 0; i < TAM; ++i) {
        char name[50];

        sprintf(name, "processo%d.txt", i + 1);

        FILE * f = fopen(name, "r");

        // Transferindo o resultado de um processo para o arquivo de resposta
        fgets(info, sizeof(info), f);
        fprintf(result, "%s\n", info);
    }


    fclose(ma);
    fclose(mb);
    fclose(result);

    printf("\n\nDone\n");

   return 0;
}