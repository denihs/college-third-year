// Este servidor utiliza a função fork() para gerenciar múltiplos clientes. Ao se fazer a chamanda
// do sistema fork(), cria-se uma duplicata exata do programa e, um novo processo é iniciado para essa cópia

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ioctl.h> 


//A biblioteca <sys/wait.h> e <signal.h> são necessárias para evitar a criação de zombies.
// Zombies são processos filhos que aparecem quando os processos pai deixam de existir sem ser feita a chamada
// wait() ou waitpid() nos processos filhos.
#include <sys/wait.h>
#include <signal.h>

//Manipulador de sinais. Ele simplesmente faz a chamada waitpid para todo filho que for desconectado
void singchild_handler(int signo) {
    while(waitpid(-1, NULL, WNOHANG) > 0);
    // A ideia de se chamar um laço é que não se tem certeza que há uma correlação um para um entre os filhos
    // desconectados e as chamadas do manipulador de sinais;
    //  Vale lembrar que o posix não permite a criação de filas nas chamadas de sinais.
    // Ou seja, pode acontecer de chamar o manipulador após vários filhos terem sidos desconectados
}

int main(int argc, char ** argv) {
    struct sockaddr_in servidor;
    int meu_socket, novo_socket, resultado, leitor, valor, pid;
    char buffer[25];

    meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    valor = 1;
    resultado = setsockopt(meu_socket, SOL_SOCKET, SO_REUSEADDR, &valor, sizeof(servidor));

    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY;
    servidor.sin_port = htons(2000);

    resultado = bind(meu_socket, (struct sockaddr *) & servidor, sizeof(servidor));
    resultado = listen(meu_socket, 5);

    signal(SIGCHLD, singchild_handler); //ativando o manipulador de sinais antes de ativar o loop

    while(1){
        novo_socket = accept(meu_socket, NULL, NULL);
        //Antes da chamada ser aceita ou retornada chama-se o fork() para a criação de novos processos
        if ((pid = fork()) == 0) {
            printf("\nProcesso filho %i criado\n", getpid());
            close(meu_socket); //Uma vez que o processo filho foi criado, fecha-se o processo listen. Lembre-se todos os processos são copiados do processo pai para o processo filho
            leitor = recv(novo_socket, buffer, 25, 0);
            buffer[leitor] = '\0';
            send(novo_socket, buffer, leitor, 0);
            close(novo_socket);
            // Esta linha só é alcançada no processo pai. Uma vez que o processo filho tem uma cópia de socket cliente. O processo pai tem a sua referência e a decrementa no contador do kernel
            printf("Processo filho %i terminado.\n", getpid());
            exit(0);
        }
        close(novo_socket);
    }
    

    return 0;

}
