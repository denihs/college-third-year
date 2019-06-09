# coding: utf-8

import socket
import threading
import sys

def gravarDados(comando):
    if len(comando) != 5:
        return "Número inválido de parametros para gravar dados"

    if len(comando[4].split(';')) != 2:
        return "Parâmetro de coordenadas inválido"

    return "Dados gravados"

def executar(comando):
    comandoTratado = comando.strip().split(',')

    acao = comandoTratado[0].lower()

    if acao == 'd':
        return gravarDados(comandoTratado)
    elif acao == 'p':
        print("Pesquisar dados")
    else:
        return "Ação não conhecida: {}".format(acao)

    return "Comando recebido: {}".format(comandoTratado[0])

def log(cliente, comando):
    ipCliente, portaCliente = cliente
    print('-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_')
    print("Cliente conectado: {}:{}".format(ipCliente, portaCliente))
    print("Comando enviado: {}".format(comando))
    print('-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_')

def processarConexao(sock, lock):
    # Recebendo o comando do cliente
    comando, (ipCliente, portaCliente) = sock.recvfrom(1024)

    log((ipCliente, portaCliente), comando)

    # Bloqueando thread para acessar a zona critica do servidor
    lock.acquire()
    resultado = executar(comando.decode())
    lock.release()

    # Enviando resposta para o cliente
    sock.sendto(resultado.encode(), (ipCliente, portaCliente))

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Numero de parametros inválidos!")
        exit()

    IP = '' # É como usar INADDR_ANY
    PORTA = int(sys.argv[1])

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((IP, PORTA))

    print("Servidor aberto na porta: {}".format(PORTA))

    lock = threading.Lock()

    contador = 0

    while True:

        thread = threading.Thread(None, processarConexao, None,(sock, lock))

        thread.start() # iniciando os serviços na thread
        thread.join() # esperando a thread terminar seu job

        contador += 1
        if contador == 1:
            break
    sock.close()