import socket
import json

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
    
    data, server = sock.recvfrom(1024)
    resposta = json.loads(data.decode())
    
    if resposta["SYN"] and resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
        dadosDeConfirmacao = novoPacote(ACK=True, ack=resposta["seq"] + 1, seq=1)
        sock.sendto(json.dumps(dadosDeConfirmacao).encode(), (IP, PORT))
        return sock
    return False

def aceitar(sock):
    dados, (ipCliente, portaCliente) = sock.recvfrom(1024)
    dados = json.loads(dados.decode())
    if dados["SYN"]:
        pacote = novoPacote(SYN=True, ACK=True, proxSeq=1, ack=dados["seq"] + 1)
        sock.sendto(json.dumps(pacote).encode(), (ipCliente, portaCliente))
        
        dados, (ipCliente, portaCliente) = sock.recvfrom(1024)
        resposta = json.loads(dados.decode())

        if resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
            return True, (ipCliente, portaCliente)
    return False, (ipCliente, portaCliente)