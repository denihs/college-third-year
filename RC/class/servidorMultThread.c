/*
    Thread -> compartilhamento de memória uso pesado da memória
    usar semáforo ou mutexes para gerenciar.
    Funcionamento: um processo por cliente. Um processo pai gera várias
    threads servidores. Cada thread servidora atende um cliente.
    Lembrado que o cliente generico que implementamos está configurado
    para 5 clientes. Então, rodar esse programa com ./nome 5
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sched.h>

void * thread_proc(void * arg);

int main(int argc, char * argv[]) {
    struct sockaddr_in servidor;
    int ouvir_socket, resultado, nfilhos = 1;
    pthread_t threads_id;

    int x, valor;

    if (argc > 1) {
        nfilhos = atoi(argv[1]);
    }

    ouvir_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /*
        SO_REUSEADDR -> Durante a compilacao é comum estartar e parar varias vezes o servidor. O linux tende
         a mantar o endereço e a porta reservada para a aplicação. O SO_REUSEADDR  libera tanto a porta como o endereço
    */

   valor = 1;
   resultado = setsockopt(ouvir_socket, SOL_SOCKET, SO_REUSEADDR, &valor, sizeof(valor));
   if(resultado < 0) {
       perror("Servidor");
   }

   servidor.sin_family = AF_INET;
   servidor.sin_port = htons(5200);
   servidor.sin_addr.s_addr = INADDR_ANY;

   resultado = listen(ouvir_socket, 5);
   if (resultado < 0) {
       perror("Listen");
   }

   for(x=0; x < nfilhos; x++) {
       resultado = pthread_create(&threads_id, NULL, thread_proc, (void *) ouvir_socket);
       if (resultado < 0) {
           perror("Não foi possivel criar threads\n");
           return 0;
       }
       sched_yield();
       /*
            A função sched_yield() força a thread que está rodando a abandonar o processador até que ela se torne novamente
            a cabeça de lista de threads. Não leva argumentos
       */
      pthread_join(threads_id, NULL);
   }
}

void *thread_proc(void *arg) {
    int ouvir_socket, sock;
    char buffer[25];
    int nlidos;
    ouvir_socket = (int) arg;

    while(1) {
        sock = accept(ouvir_socket, NULL, NULL);
        printf("Cliente conectado a thread filha %i com pid %i\n", pthread_self(), getpid());
        nlidos = recv(sock, buffer, 25, 0);
        buffer[nlidos] = '\0';
        printf("%s\n", buffer);
        send(sock, buffer, nlidos, 0);
        close(sock);
        printf("Cliente desconectado da thread filha %i com pid %i.\n", pthread_self(), getpid());
    }
}