// Cliente simples
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

extern int errno;
char localhost[]="localhost"; // neste programa o cliente roda na própria máq.


int main(int argc, char * argv[]) {
    struct hostent *ptrh; //ponteiro para a tabela host
    struct protoent *ptrp; //ponteiro para a entrada de protocolo
    struct sockaddr_in sad; //estrutura para manter o endereço do server
    int sd; //descritor do socket
    int porta;
    char *host; //ponteiro para o nome do host
    int n; //variável contador
    char buf[1000]; //grava dados recebidos do servidor

    memset((char *) &sad, 0, sizeof(sad)); //limpa a estrutura sockaddr
    sad.sin_family = AF_INET;

    if (argc > 1) { // verifica se o argumento espicificado é valido
        porta = atoi(argv[1]); // converte o argumento para binário
    } else {
        porta = 5200; // usa o número de porta padrão
    }

    if (porta > 0) { // verificação de valores inválidos
        sad.sin_port = htons((u_short)porta);
    } else {
       fprintf(stderr, "Numero de porta invalido %s\n", argv[1]);
       exit(1);
    }

    if (argc > 1) host = argv[1];
    else host = localhost;

    ptrh = gethostbyname(host);

    memcpy(&sad.sin_addr, ptrh -> h_addr, ptrh->h_length);
   //Mapeamento do protocolo tcp para o número de porta
    if ((int *) (ptrp = getprotobyname("tcp")) == 0) {
        fprintf(stderr, "Falha ao mapear \"tcp\" para o numero de protocolo");
        exit(1);
    }

    sd = socket(PF_INET, SOCK_STREAM, ptrp -> p_proto);
    if (connect(sd, (struct sockaddr *) &sad, sizeof(sad)) < 0) {
        fprintf(stderr, "Falha na conexão");
        exit(1);
    }

    n = recv(sd, buf, sizeof(buf), 0);
    while(n > 0) {
        buf[n] = '\0';
        fprintf(stderr, "Cliente %s", buf);
        n = recv(sd, buf, sizeof(buf), 0);
    }
    close(sd);

    return 0;
}