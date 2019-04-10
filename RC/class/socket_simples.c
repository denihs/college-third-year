#include <stdio.h>
#include <sys/socket.h>

int main () {
    int meu_socket;
    //SOCK_DGRAM - UDP
    //SOCK_STREAM - TCP
    //Protocolo 0 indica o IP
    meu_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (meu_socket == -1)
        printf("Erro ao criar o socket!\n");
    else
        printf("Socket criado com sucesso\n");
    return 0;
}