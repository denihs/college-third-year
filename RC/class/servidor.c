// Servidor simples
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>

int visitas = 0;


int main(int argc, char ** argv) {
    struct hostent *ptrh; //Ponteiro para a entrada da tabela do host
    struct protoent *ptrp; // Ponteiro para entrada da tabela do protocolo
    struct sockaddr_in sad; // estrutura que armazena o endereço do servidor
    struct sockaddr_in cad; // estruta que armazena o endereço dos clientes

    int sd, sd2; // descritor de socket
    int porta; // numero da porta do protocolo
    int alen; // tamanho do endereço
    char buf[1000]; //buffer para armazenar as mensagens enviadas pelo servidor

    memset((char *) &cad, 0, sizeof(sad)); // Limpa a estrutura sockaddr

    sad.sin_family = AF_INET; // configura familia de protocolos internet
    sad.sin_addr.s_addr = INADDR_ANY; // Qualquer maquina pode se conectar ao servidor

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

    //Mapeamento do protocolo tcp para o número de porta
    if ((int *) (ptrp = getprotobyname("tcp")) == 0) {
        fprintf(stderr, "Falha ao mapear \"tcp\" para o numero de protocolo");
        exit(1);
    }

    //cria o socket

    sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);

    if (bind(sd, (struct sockaddr *) & sad, sizeof(sad)) < 0) {
        fprintf(stderr, "Falha no bind\n");
        exit(1);
    }

    if(listen(sd, 5) < 0) {
        fprintf(stderr, "Falha na função listen\n");
        exit(1);
    }

    // loop do servidor, aceite as conexões e mantenha os pedidos de conexão
    fprintf(stderr, "Servidor ativo e funcionando\n");
    while(1) {
        alen = sizeof(cad);
        fprintf(stderr, "SERVIDOR: Aguardando conexões...\n");

        if((sd2 = accept(sd, (struct sockaddr *)&cad, &alen) < 0)) {
            fprintf(stderr, "Falha na função accept\n");
            exit(1);
        }
        visitas++;
        sprintf(buf, "Este servidor foi contactado %d vez%s\n", visitas, visitas == 1 ? "." : "es.");
        fprintf(stderr, "SERVIDOR: %s", buf);

        send(sd2, buf, strlen(buf), 0);
        close(sd2);
    }

    return 0;
}