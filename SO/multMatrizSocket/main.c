#include <errno.h>
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
int PORTA = 5000;


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
    printf("Messagem enviada: %s\n", msg);
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
    sprintf(sResult, "%s %d%c", sResult, index, '\0');
    client(sResult);

}

int countConnections = 0;

void waitConnections(int sd) {
    printf("Se preparando para receber dados\n");
    struct sockaddr_in cad; // estruta que armazena o endereço dos clientes
    int alen, sd2;
    int iBytes;
    char value[2000];
    alen = sizeof(struct sockaddr_in);
    memset((char *) &cad, 0, sizeof(cad)); // Limpa a estrutura sockaddr

    char resp[TAM][TAM * 3];


    // loop do servidor, aceite as conexões e mantenha os pedidos de conexão
    while(countConnections != TAM) {
        if((sd2 = accept(sd, (struct sockaddr *)&cad, (socklen_t *) & alen)) < 0) {
            fprintf(stderr, "Falha na função accept\n");
            exit(1);
        }

        // Recebe a resposta enviada pelo processo filho
        if ((iBytes = recv(sd2 , value , sizeof(value), 0)) < 0){
            fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
            printf("Erro ao receber dados na conexão: %d\n", countConnections);
            exit(1);
        }

        value[iBytes] = '\0';

        // Tratando a ordem em que os dados serão gravados no arquivo
        int index = value[iBytes - 1] - '0';
        value[iBytes - 1] = '\0';
        strcpy(resp[index], value);

        countConnections++;
        close(sd2);
    }

    FILE * result = fopen("resultado.txt", "w"); // Criando arquivo com os resultados


    int i,j;
    for(i=0;i<TAM;i++){
        // Adiciona o resultado no arquivo
        fprintf(result, "%s\n", resp[i]);
    }
    fclose(result);
}

int server() {
    printf("Iniciando servidor\n");
    struct sockaddr_in sad; // estrutura que armazena o endereço do servidor


    int sd; // descritor de socket



    sad.sin_family = AF_INET; // configura familia de protocolos internet
    sad.sin_addr.s_addr = INADDR_ANY; // Qualquer maquina pode se conectar ao servidor
    sad.sin_port = htons(PORTA);


    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket: ");
        exit(1);
    }

    printf("Sock %d\n", sd);

    if (bind(sd, (struct sockaddr *) & sad, sizeof(sad)) < 0) {
        perror("bind: ");
        exit(1);
    }

    if(listen(sd, TAM + 1) < 0) {
        fprintf(stderr, "Falha na função listen\n");
        exit(1);
    }
    printf("Servidor escutando\n");

    return sd;
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