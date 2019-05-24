#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>

int TAM = 0;
#define PORTA 5000


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

void client(char * msg) {

    struct hostent *ptrh; //ponteiro para a tabela host
    struct sockaddr_in sad; //estrutura para manter o endereço do server
    int sd; //descritor do socket

    memset((char *) &sad, 0, sizeof(sad)); //limpa a estrutura sockaddr
    sad.sin_family = AF_INET;
    sad.sin_port = htons(PORTA);

    ptrh = gethostbyname("localhost");

    memcpy(&sad.sin_addr, ptrh -> h_addr, ptrh->h_length);

    sd = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(sd, (struct sockaddr *) &sad, sizeof(sad)) < 0) {
        fprintf(stderr, "Falha na conexão");
        exit(1);
    }

    if (send(sd , msg , strlen(msg) , 0) < 0) {
        fprintf(stderr, "Falha o tentar enviar");
        exit(1);
    }
    printf("Message: %s\n", msg);
    close(sd);
}

void calc(int ma[][TAM], int mb[][TAM], int index) {
    char sResult[TAM * 3];
    sResult[0] = '\0';

    int result = 0;

    for (int i = 0; i < TAM; ++i) {
        for (int j = 0; j < TAM; ++j) {
            result += ma[index][j] * mb[j][i];
        }
        sprintf(sResult, "%s %d", sResult, result);
        result = 0;
    }
    sprintf(sResult, "%s%c", sResult, '\0');
    client(sResult);

}

int server() {
    struct sockaddr_in sad; // estrutura que armazena o endereço do servidor


    int sd; // descritor de socket



    sad.sin_family = AF_INET; // configura familia de protocolos internet
    sad.sin_addr.s_addr = INADDR_ANY; // Qualquer maquina pode se conectar ao servidor
    sad.sin_port = htons(PORTA);


    sd = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(sd, (struct sockaddr *) & sad, sizeof(sad)) < 0) {
        fprintf(stderr, "Falha no bind\n");
        exit(1);
    }

    if(listen(sd, TAM + 1) < 0) {
        fprintf(stderr, "Falha na função listen\n");
        exit(1);
    }

    return sd;
}

int countConnections = 0;

void waitConnections(int sd) {
    struct sockaddr_in cad; // estruta que armazena o endereço dos clientes
    int alen, sd2; // tamanho do endereço

    memset((char *) &cad, 0, sizeof(cad)); // Limpa a estrutura sockaddr

    FILE * result = fopen("resultado.txt", "w"); // Criando arquivo com os resultados

    // loop do servidor, aceite as conexões e mantenha os pedidos de conexão
    while(countConnections != TAM) {
        alen = sizeof(cad);

        //Aqui o pai junta os arquivos auxiliares para gerar um único arquivo com o resultado
        char value[2000];
        value[0] = '\0';

        if((sd2 = accept(sd, (struct sockaddr *)&cad, &alen) < 0)) {
            fprintf(stderr, "Falha na função accept\n");
            exit(1);
        }

        // Recebe a resposta enviada pelo processo filho
        recv(sd2 , value , 2000, 0);

        printf("Result: %s\n", value);

        // Adiciona o resultado no arquivo
        fprintf(result, "%s\n", value);

        countConnections++;


        close(sd2);
    }

    fclose(result);
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

    int sd = server();

    //Aqui é feita a criação dos processo que farão a multiplicação das matrizes
    for(int i=0;i<TAM;i++){
        pid[i] = fork();
        proc = i;
        if(pid[i]==0) break;
    }

    if(pid[proc]==0){
        //Aqui deve ser feita a multiplicação e 'salvamento' das matrizes nos arquivos auxiliares.
        calc(matrizA, matrizB, proc);
    }else{
        for(int i=0;i<TAM;i++){
            waitpid(pid[i],NULL,0);
        }
        waitConnections(sd);
        printf("Pai morreu!\n");
    }


    fclose(ma);
    fclose(mb);

   return 0;
}