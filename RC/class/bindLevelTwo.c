//A função bind associa (ou liga) o socket à uma porta e um endereço em particular.
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // Para usar a função write

int main(int argc, char ** argv) {
    int meu_socket, novo_socket, count;
    struct sockaddr_in servidor, cliente;
    char *mensagem;

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

    char * naruto = "O Naruto pode ser um pouco duro às vezes, talvez você não saiba disso, mas o Naruto também cresceu sem pai. Na verdade ele nunca conheceu nenhum de seus pais, e nunca teve nenhum amigo em nossa aldeia. Mesmo assim eu nunca vi ele chorar, ficar zangado ou se dar por vencido, ele está sempre disposto a melhorar, ele quer ser respeitado, é o sonho dele e o Naruto daria a vida por isso sem hesitar. Meu palpite é que ele se cansou de chorar e decidiu fazer alguma coisa a respeito!\n";
    char * monica = "O sua monica do caralho, vou te quebrar na porrada se eu voltar ai em";
    int c = 0;

    while((novo_socket = accept(meu_socket, (struct sockaddr *) & cliente, (socklen_t *) & count))) {
        if(novo_socket < 0) {
            perror("Erro ao aceitar a conexão\n");
            return 1;
        }

        puts("Conexão aceita\n");

        //Respondendo ao cliente
        mensagem = c ? monica : naruto;
        write(novo_socket, mensagem, strlen(mensagem));
        c++;
    }
    return 0;
}