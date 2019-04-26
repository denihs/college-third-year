//cliente genérico para acesso aos servidores multiplexing. fprk e threads
// executar como ./cliente_generico 5

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void client_func(int num_clientes);

int main(int argc, char ** argv) {
    int pid, x, num_clientes;

    num_clientes = argc > 1 ? atoi(argv[1]) : 5;

    for(x = 0; x < num_clientes; x++) {
        // fork() system call para gerar um numero de processos filhos para simular multiplos clientes
        if ((pid = fork()) == 0) {
            client_func(x + 1); //Se fork() retorna 0, então ele esta retornando o processo pai
            exit(0);
        }
    }
    
    wait(NULL);
    return 0;
}

void client_func(int num_clientes) {
    int x, sock;
    struct sockaddr_in servidor;
    char buffer[25];

    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY;
    servidor.sin_port = 0;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (const struct sockaddr *) &servidor, sizeof(servidor));

    servidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    servidor.sin_port = htons(2000);

    if (connect(sock, (const struct sockaddr *) &servidor, sizeof(servidor)) != 0) {
        perror("Cliente error");
        return 0;
    }

    snprintf(buffer, 128, "dados do cliente: %i", num_clientes);
    sleep(1);
    
    printf("\nCliente #%i enviou %i caracteres\n", num_clientes, send(sock, buffer, strlen(buffer), 0));
    sleep(1);

    printf("Cliente #%i recebeu %i caracteres\n", num_clientes, recv(sock, buffer, 25, 0));
    sleep(1);

    close(sock);
}