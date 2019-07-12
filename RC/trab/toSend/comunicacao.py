# coding: utf-8
import socket
import os
import json
from threading import Timer

SERVIDOR = "SERVIDOR"
CLIENTE = "CLIENTE"
N_MAX_RETRANSMISSOES = 6


def novoPacote(dados=None, SYN=False, ACK=False, FIN=False, ack=0, seq=0, proxSeq=0, checksum=None):
    return {
        "dados": dados,
        "SYN": SYN,
        "ACK": ACK,
        "FIN": FIN,
        "ack": ack,
        "seq": seq,
        "proxSeq": proxSeq,
        "checksum": checksum
    }

def checksum(dados):
    checksumDados = None
    try: # Tentando recuperar um checksum existente
        checksumDados = int(dados["checksum"])
    except:
        pass

    try: # Tentando excluir um checksum existente
        del dados["checksum"]
    except:
        pass

    string = json.dumps(dados)

    sum = 0

    for caracter in string:
        byte = ord(caracter) # caracter em ascii
        sum = (sum + byte) & 0xFF # me retorna sempre um valor entre 0 e 255, não importa o sum. Tenho a garantia que não terei um valor cuja representação em binário tenha mais de 8 bits

    complementoUm = sum ^ 0xFF
    complementoDois = complementoUm + 1
    sum = complementoDois & 0xFF # novamente garantindo que o checksum esteja entre 0 e 255

    return sum, sum == checksumDados

class ReceberDados:
    def __init__(self, sock, tipo=CLIENTE):
        self.sock = sock
        self.tipo = tipo
        self.timer = None
        self.contador = 0

    def setInterval(self, tempo=0.5):
        self.contador += 1
        if not self.contador % 10 and self.tipo == CLIENTE:
            print ("Tentando conexão com o servidor ({}/5)".format(int(self.contador/10)))
        if self.contador == 50:
            if self.tipo == CLIENTE:
                print("\nNenhuma resposta do servidor, tenha certeza que o mesmo está operando. Este cliente será encerrado\n")
                os._exit(1)
            # Para não derrubar o servidor quando um cliente não responde
            # apenas é enviado uma enviado um pacote vazio do servidor para ele mesmo
            # desta forma ele saberá como tratar esses dados mais adiante
            self.sock.sendto(json.dumps(novoPacote()).encode(), self.sock.getsockname())
            self.resetar()
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

def conectar(IP, PORT):
    # Cria um novo pacote com a flag de sincronização ativada e se espera que o ack recebido seja 1
    pacote = novoPacote(SYN=True, proxSeq=1)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # Envia o pacote para o servidor
    sock.sendto(json.dumps(pacote).encode(), (IP, PORT))

    data, (ipServidor, portaServidor) = ReceberDados(sock).escutar()

    resposta = json.loads(data.decode())

    # Dos dados recebidos, se espera as flags ACK e SYN, e que o ack seja igual ao proxSeq do pacote enviado
    if resposta["SYN"] and resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:

        # Caso o servidor retorne os dados esperados, é enviado um novo pacote com a flag ACK, o ack e o numero de sequência desse pacote
        dadosDeConfirmacao = novoPacote(ACK=True, ack=resposta["seq"] + 1, seq=1)
        sock.sendto(json.dumps(dadosDeConfirmacao).encode(), (ipServidor, portaServidor))
        return sock, portaServidor
    return False, resposta


def aceitar(dadosCliente, numeroCliente, PORTA):
    dados, ipCliente, portaCliente = dadosCliente
    dados = json.loads(dados.decode())

    if dados["SYN"]:
        # Um novo socket sera criado para o novo cliente
        # Cada cliente tera um thread vinculada a uma porta
        porta = PORTA + numeroCliente
        sockCliente = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sockCliente.bind(("", porta))

        # Um novo pacote eh criado para responder a solicitacao do cliente
        pacote = novoPacote(SYN=True, ACK=True, proxSeq=1, ack=dados["seq"] + 1)
        sockCliente.sendto(json.dumps(pacote).encode(), (ipCliente, portaCliente))

        dados, (ipCliente, portaCliente) = ReceberDados(sockCliente, SERVIDOR).escutar()
        resposta = json.loads(dados.decode())

        # Com a resposta do cliente, retornamos o novo socket que sera usado pela thread do cliente
        if resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
            return sockCliente, (ipCliente, portaCliente)
    return False, (ipCliente, portaCliente)


def transmitir(sock, pacote, IP, PORTA, fazerTransmicao=True, contagem=1):
    if fazerTransmicao:
        sum, _ = checksum(pacote)
        pacote["checksum"] = sum
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
        return pacoteRec
    return False