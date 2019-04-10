#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
//Para poder usar a função close
#include <unistd.h>
// Essa biblioteca é necessária para usarmos inet_addr
// permite converter um endereço IP em formato longo
#include <arpa/inet.h> 

int main () {
    int meu_socket;
    struct sockaddr_in servidor;

    char * mensagem, resposta_servidor[2000];

    //criando socket
    //SOCK_DGRAM - UDP
    //SOCK_STREAM - TCP
    //Protocolo 0 indica o IP
    meu_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (meu_socket == -1) {
        printf("Erro ao criar o socket!\n");
        return 1;
    }
    printf("Socket criado com sucesso\n");

    servidor.sin_addr.s_addr = inet_addr("200.181.121.141");// Endereço do google br
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(80);

    //conectar a um servidor remoto
    if (connect (meu_socket, (struct sockaddr *) &servidor, sizeof(servidor)) < 0) {
        puts("Erro ao conectar!\n");
        return 1;
    }

    puts("Conectado\n");

    //enviando alguns dados
    mensagem = "GET / HTTP/1.1\r\n\r\n"; // comando para buscar a página principal do site

    if (send(meu_socket, mensagem, strlen(mensagem), 0) < 0) {
        puts("Falha ao enviar\n");
        return 1;
    }

    puts("Dados enviados!\n");

    //recebendo resposta do servidor
    if (recv(meu_socket, resposta_servidor, 2000, 0) < 0) {
        puts("Falha ao receber a resposta\n");
        return 1;
    }
    puts("Resposta recebida\n");
    puts(resposta_servidor);

    close(meu_socket);

    return 0;
}