# coding: utf-8
import socket
import json
import time

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
    
    data, (ipServidor, portaServidor) = sock.recvfrom(1024)
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
        
        dados, (ipCliente, portaCliente) = sockCliente.recvfrom(1024)
        resposta = json.loads(dados.decode())

        if resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
            return sockCliente, (ipCliente, portaCliente)
    return False, (ipCliente, portaCliente)


def transmitir(sock, pacote, IP, PORTA, fazerTransmicao=True, contagem=0):

    if fazerTransmicao:
        sock.sendto(json.dumps(pacote).encode(), (IP, PORTA))

        pacoteRec, server = sock.recvfrom(1024)
        pacoteRec = json.loads(pacoteRec.decode())

        if not pacoteRec['ACK'] or pacoteRec["ack"] != pacote["proxSeq"]:
            print("Retransmitindo! ack recebido: {}".format(pacoteRec["ack"]))
            time.sleep(2)
            transmitir(
                sock,
                pacote,
                IP,
                PORTA,
                fazerTransmicao= True if contagem != 5 else False,
                contagem = contagem + 1
            )
        print("Numero de sequência enviado: {}".format(pacote["seq"]))
        return pacoteRec
    return False