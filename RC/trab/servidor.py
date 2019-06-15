# coding: utf-8

import socket
import threading
import sys
import json

def comandoInvalido(comando):
    if len(comando) != 5:
        return "Número inválido de parametros para gravar dados"

    if len(comando[4].split(';')) != 2:
        return "Parâmetro de coordenadas inválido"

    return False

def gravarDados(comando):
    msgComandoInvalido = comandoInvalido(comando)
    if msgComandoInvalido:
        return msgComandoInvalido

    try:
        arq = open("dados.json", "r")
        dados = json.load(arq)
        arq.close()
    except:
        dados = []

    arq = open("dados.json", "w")

    dados.append({
        "tipoCombustivel": comando[2],
        "valor": comando[3],
        "coordenadas": comando[4]
    })

    json.dump(dados, arq)

    arq.close()

    return "Dados gravados"

def buscarDados(comando):
    msgComandoInvalido = comandoInvalido(comando)
    if msgComandoInvalido:
        return msgComandoInvalido

    try:
        arq = open("dados.json", "r")
        dados = json.load(arq)
        arq.close()
    except:
        return "Nenhum valor encontrado!"

    tipoCombustivel = int(comando[2])
    resutados = list(filter(lambda item: int(item["tipoCombustivel"]) == tipoCombustivel, dados))

    if len(resutados) > 0:
        tc = ['diesel', 'álcool', 'gasolina']
        valor = int(resutados[0]["valor"]) / 1000
        return "Tipo combustível: {}\nPreço: R${}\ncoordenadas: {}".format(tc[tipoCombustivel], valor, resutados[0]["coordenadas"])


    return "Nenhuma posto encontrado para esses parâmetros."

def executar(comando):
    comandoTratado = comando.replace(" ", "").split(',')
    acao = comandoTratado[0].lower()

    if acao == 'd':
        return gravarDados(comandoTratado)
    elif acao == 'p':
        return buscarDados(comandoTratado)
    else:
        return "Ação não conhecida: {}".format(acao)


def log(cliente, comando):
    ipCliente, portaCliente = cliente
    print('-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_')
    print("Cliente conectado: {}:{}".format(ipCliente, portaCliente))
    print("Comando recebido: {}".format(comando.decode()))
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