import socket
import sys
import json

if len(sys.argv) != 3:
    print("Numero de parametros inválidos!")
    exit()

dadosGravar = {
    "acao": "D",
    "indicador": 0,
    "valor": 5000,
    "tipoCombustivel": 2,
    "coordenadas": "(-22.21532688695198;-54.75933148596293)"
}

dadosPesquisa = {
    "acao": "P",
    "indicador": 0,
    "raio": 5,
    "tipoCombustivel": 3,
    "coordenadas": "(-22.21532688695198;-54.75933148596293)"
}

def conectarServidor(IP, PORT):
    pacote = {
        "dados": None,
        "SYN": True,
        "ACK": False,
        "FIN": False,
        "ack": 0,
        "seq": 0,
        "proxSeq": 1
    }

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    sock.sendto(json.dumps(pacote).encode(), (IP, PORT))
    data, server = sock.recvfrom(1024)
    resposta = json.loads(data.decode())

    if resposta["SYN"] and resposta["ACK"] and resposta["ack"] == pacote["proxSeq"]:
        dadosDeConfirmacao = pacote
        
        dadosDeConfirmacao["ACK"] = True
        dadosDeConfirmacao["SYN"] = False
        dadosDeConfirmacao["ack"] = resposta["seq"] + 1
        dadosDeConfirmacao["seq"] = pacote["seq"] + 1

        sock.sendto(json.dumps(dadosDeConfirmacao).encode(), (IP, PORT))
        print("Conexão estabilizada")
        return sock

    print("Falha na conexão com o servidor")
    return False


if __name__ == '__main__':
    UDP_IP = sys.argv[1]
    UDP_PORT = int(sys.argv[2])

    
    sock = conectarServidor(UDP_IP, UDP_PORT)

    if sock:
        sock.sendto(json.dumps(dadosGravar).encode(), (UDP_IP, UDP_PORT))

        mensagem, server = sock.recvfrom(1024)

        print(mensagem.decode())