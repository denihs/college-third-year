//A função bind associa (ou liga) o socket à uma porta e um endereço em particular.
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char ** argv) {
    int meu_socket, novo_socket, count;
    struct sockaddr_in servidor, cliente;

    meu_socket = socket(AF_INET, SOCK_STREAM, 0);

    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY;
    servidor.sin_port = htons(8888);

    if (bind(meu_socket, (struct sockaddr * ) & servidor, sizeof(servidor)) < 0) {
        puts("Erro no bind\n");
        return 1;
    }

    puts("Bind executado com sucesso!\n");

    //Listen serve para ouvir as conexões
    listen(meu_socket, 3);

    //aceitando as conexões
    puts("Aguardando novas conexões...\n");
    count = sizeof(struct sockaddr_in);

    novo_socket = accept(meu_socket, (struct sockaddr *) & cliente, (socklen_t *) & count);

    if(novo_socket < 0) {
        perror("Erro ao aceitar a conexão\n");
        return 1;
    }

    puts("Conexão aceita\n");

    return 0;
}