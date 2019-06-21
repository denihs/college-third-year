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

def conectar(IP, PORT, OPORTA):
    pacote = novoPacote(SYN=True, proxSeq=1)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((IP, OPORTA))
    sock.sendto(json.dumps(pacote).encode(), (IP, PORT))
    
    data, server = sock.recvfrom(1024)
    resposta = json.loads(data.decode())
    
    if resposta["SYN"] and resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
        dadosDeConfirmacao = novoPacote(ACK=True, ack=resposta["seq"] + 1, seq=1)
        sock.sendto(json.dumps(dadosDeConfirmacao).encode(), (IP, PORT))
        return sock, None
    return False, resposta


def aceitar(sock, dadosCliente):
    dados, ipCliente, portaCliente = dadosCliente
    dados = json.loads(dados.decode())
    print(dados)
    if dados["SYN"]:
        pacote = novoPacote(SYN=True, ACK=True, proxSeq=1, ack=dados["seq"] + 1)
        sock.sendto(json.dumps(pacote).encode(), (ipCliente, portaCliente))
        
        dados, (ipCliente, portaCliente) = sock.recvfrom(1024)
        resposta = json.loads(dados.decode())

        if resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
            return True, (ipCliente, portaCliente)
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