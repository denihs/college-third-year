//Programa para scanear porta de servidores
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char ** argv) {
    struct hostent *host;
    struct sockaddr_in server;

    int busca, i, sock, inicio, fim;
    char nome_maquina[100];

    //primeiramente vamos fornecer o nome da máquina que seá escaneada
    printf("Digite o nome da máquina ou seu IP: ");
    fgets(nome_maquina, 100, stdin);

    //porta de inicio de escaneamento
    printf("Digite o número inicial da porta: ");
    scanf("%d", &inicio);

    //porta final de escaneamento
    printf("Digite o número final da porta: ");
    scanf("%d", &fim);

    //inicializando a estrutura sockaddr_in
    strncpy((char *) &server, "", sizeof(server));
    server.sin_family = AF_INET;

    //usar endereço ip direto
    if (isdigit(nome_maquina[0])) {
        printf("Executando o inet_addr...\n");
        server.sin_addr.s_addr = inet_addr(nome_maquina);
        printf("Executado!\n");
    } 

    //Resolver o nome da maquina (hostname) para o endereço IP

    else if((host = gethostbyname(nome_maquina)) != 0) {
        printf("Executando gethostbyname\n");
        strncpy((char *) &server.sin_addr, (char *) host->h_addr, sizeof(server.sin_addr));
        printf("Executado!\n");
    } else {
        herror(nome_maquina);
        exit(2);
    }

    //começar a escanear as portar fazendo um loop

    printf("Inicio do scaneamento");
    for(int i = inicio; i <= fim; i++) {
        //preencher o numero da porta
        server.sin_port= htons(i);

        //criar a conexão socket
        sock = socket(AF_INET, SOCK_STREAM, 0);

        //conectar usando socket e a estrutura e a estrutura sockaddr_in
        busca = connect(sock, (struct sockaddr*) &server, sizeof(server));

        if (busca < 0 ) {
            printf("Falha na função conecte! Porta fechada %d\n", i);
            fflush(stdout);
        } else {
            printf("%-5d aberta\n", i);
        }
        close(sock);
    }
    printf("\r");
    fflush(stdout);

    return 0;
}