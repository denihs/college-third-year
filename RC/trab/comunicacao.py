# coding: utf-8
import socket
import os
import json
import time
from threading import Timer

SERVIDOR = "SERVIDOR"
CLIENTE = "CLIENTE"
N_MAX_RETRANSMISSOES = 6

class ReceberDados:
    def __init__(self, sock, tipo=CLIENTE):
        self.sock = sock
        self.tipo = tipo
        self.timer = None
        self.contador = 0

    def setInterval(self, tempo=0.5):
        self.contador += 1
        if self.contador == 5:
            if self.tipo == CLIENTE:
                print("Nenhuma resposta do servidor, tenha certeza que o mesmo está operando. Este cliente será encerrado")
                os._exit(1)
            # Para não derrubar o servidor quando um cliente não responde
            # apenas é enviado uma enviado um pacote vazio do servidor para ele mesmo
            # desta forma ele saberá como tratar esses dados mais adiante
            self.sock.sendto(json.dumps(novoPacote()).encode(), self.sock.getsockname())
        else:
            self.timer = Timer(tempo, self.setInterval, [tempo])
            self.timer.start()

    def resetar(self):
        self.timer.cancel()
        self.contador = 0

    def escutar(self):
        self.setInterval()
        dados = self.sock.recvfrom(1024)
        self.resetar()
        return dados



def novoPacote(dados=None, SYN=False, ACK=False, FIN=False, ack=0, seq=0, proxSeq=0):
    return {
        "dados": dados,
        "SYN": SYN,
        "ACK": ACK,
        "FIN": FIN,
        "ack": ack,
        "seq": seq,
        "proxSeq": proxSeq
    }

def conectar(IP, PORT):
    pacote = novoPacote(SYN=True, proxSeq=1)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(json.dumps(pacote).encode(), (IP, PORT))

    data, (ipServidor, portaServidor) = ReceberDados(sock).escutar()

    resposta = json.loads(data.decode())
    
    if resposta["SYN"] and resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
        dadosDeConfirmacao = novoPacote(ACK=True, ack=resposta["seq"] + 1, seq=1)
        sock.sendto(json.dumps(dadosDeConfirmacao).encode(), (ipServidor, portaServidor))
        return sock, portaServidor
    return False, resposta


def aceitar(dadosCliente, numeroCliente, PORTA):
    dados, ipCliente, portaCliente = dadosCliente
    dados = json.loads(dados.decode())

    if dados["SYN"]:
        # Criando socket para o cliente
        porta = PORTA + numeroCliente
        sockCliente = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sockCliente.bind(("", porta))
        pacote = novoPacote(SYN=True, ACK=True, proxSeq=1, ack=dados["seq"] + 1)
        sockCliente.sendto(json.dumps(pacote).encode(), (ipCliente, portaCliente))

        dados, (ipCliente, portaCliente) = ReceberDados(sockCliente, SERVIDOR).escutar()
        resposta = json.loads(dados.decode())

        if resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
            return sockCliente, (ipCliente, portaCliente)
    return False, (ipCliente, portaCliente)


def transmitir(sock, pacote, IP, PORTA, fazerTransmicao=True, contagem=1):

    if fazerTransmicao:
        sock.sendto(json.dumps(pacote).encode(), (IP, PORTA))

        pacoteRec, server = ReceberDados(sock).escutar()
        pacoteRec = json.loads(pacoteRec.decode())

        if not pacoteRec['ACK'] or pacoteRec["ack"] != pacote["proxSeq"]:
            if contagem < N_MAX_RETRANSMISSOES:
                print("Retransmissão ({}/5)".format(contagem))
            return transmitir(
                sock,
                pacote,
                IP,
                PORTA,
                fazerTransmicao= True if contagem != N_MAX_RETRANSMISSOES else False,
                contagem = contagem + 1
            )
        print("Numero de sequência enviado: {}".format(pacote["seq"]))
        return pacoteRec
    return False