#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h> // conteudo do host
#include <arpa/inet.h>

int main(int argc, char ** argv) {
    char * nome_host = "www.google.com";
    char ip[100];

    struct hostent *maquina;
    struct in_addr **addr_list;
    int i;

    // para se conectar a um host remoto é necessario obter o endereço ip 
    // A função gethostbyname é usada para este propósito
    // Ela pega o nome do domínio como parâmetro e retorna a estrutura do tipo hostent
    // Esta estrutura tem a informação do ip. A biblioteca é a netdb.h

    if ((maquina = gethostbyname(nome_host)) == NULL) {
        // o gethostbyname falhou
        herror("gethostbyname");
        return 1;
    }

    // aqui vamos dar uma cast em h_addr_list para in_addr
    // uma vez que h_addr_list também tem o endereço ip, mas somente no formato longo.

    addr_list = (struct in_addr **) maquina -> h_addr_list;
    for (int i=0; addr_list[i] != NULL; i++) {
        //retorna o primeiro encontrado
        strcpy(ip, inet_ntoa(*addr_list[i]));
    }

    printf("\n%s possui endereço ip: %s\n", nome_host, ip);
    return 0;
}