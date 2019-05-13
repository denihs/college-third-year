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
    pid_t pid[TAM];
    int proc;
       
    // child process
    fill(matrizA, ma);
    fill(matrizB, mb);

    //Aqui é feita a criação dos processo que farão a multiplicação das matrizes
    for(int i=0;i<TAM;i++){
        pid[i] = fork();
        proc = i;
        if(pid[i]==0) break;
        printf("%d - %d \n",pid[i],i);
    }

    if(pid[proc]==0){
        //Aqui deve ser feita a multiplicação e 'salvamento' das matrizes nos arquivos auxiliares.
        calc(matrizA, matrizB, proc);
        printf("%d %d %d %d\n",pid[0],pid[1],pid[2],pid[3]);
        printf("Processo %d morreu!\n",getpid());
    }else{
        for(int i=0;i<TAM;i++){
            waitpid(pid[i],NULL,0);
        }
        //Aqui o pai junta os arquivos auxiliares para gerar um único arquivo com o resultado
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

            fclose(f);
        
        }
        fclose(result);
        printf("Pai morreu!\n");
    }


    fclose(ma);
    fclose(mb);

   return 0;
}