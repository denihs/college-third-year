//servidor usando a abordagem multiplexing

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ioctl.h> //permitir ao SO organizar a fila de conexão dos clientes

//Uso de macros
//VOID FD_SET(int fd, fd_set *set): uma flag para o descritor que deve ser observado

//VOID FD_CLR(int fd, fd_set *set): reseta a flag relacionada ao descritor

//INT FD_ISSET(int fd, fd_set * set): após o retorno da função select(), FD_ISSET determina qual descritor será marcado(flag) ou não.

//VOID FD_ZERO(fd_set *set): limpa o conjunto associado aos descritores

int main(int argc, char * atgv[]) {
    struct sockaddr_in servidor;
    fd_set readset, testset;
    int meu_socket;
    int novo_socket;
    char buffer[25];
    int resultado, n_lidos, x, valor;

    meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    valor = 1;
    resultado = setsockopt(meu_socket, SOL_SOCKET, SO_REUSEADDR, &valor, sizeof(valor));

    //SO_REUSEADDR: durante a compilação é comum estatar e parar várias vezes o servidor. O linux tente a manter o endereço e a porta reservada para a aplicação. Essa opção libera esses argumentos
    //O setsockopt define as opções de argumento no nível de protocolo. O nível de argumento especifica o nível de protocolo onde as opções residem. Para especificar as opções no nível socket usa-se o argumento SOL_SOCKET
    
    if (resultado < 0) {
        perror("Servirdor error");
        return 0;
    }

    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY;
    servidor.sin_port = htons(2000);

    resultado = bind(meu_socket, (struct sockaddr *) &servidor, sizeof(servidor));
    if (resultado < 0) {
        perror("Error no bind");
        return 0;
    }

    resultado = listen(meu_socket, 5);
    if (resultado < 0) {
        perror("Error no listen");
        return 0;
    }


    FD_ZERO(&readset);
    FD_SET(meu_socket, &readset);

    while(1) {
        testset = readset;
        resultado =  select(FD_SETSIZE, &testset, NULL, NULL, NULL);
        if (resultado < 0) {
            perror("Error no select()");
            return 0;
        }
        for(x = 0; x < FD_SETSIZE; x++) {
            if (FD_ISSET(x, &testset)) {
                if (x == meu_socket) {
                    novo_socket = accept(meu_socket, NULL, NULL);
                    FD_SET(novo_socket, &readset);
                } else {
                    n_lidos = recv(x, buffer, 25, 0);
                    if (n_lidos <= 0) {
                        close(meu_socket);
                        FD_CLR(x, &readset);
                        printf("\nCliente do descritor #%i desconectado\n", x);
                    } else { // Recebi dados do cliente
                        buffer[n_lidos] = '\0';
                        printf("%s", buffer);
                        send(x, buffer, n_lidos, 0);
                    }
                }
            }
        }
    }

    return 0;
}
