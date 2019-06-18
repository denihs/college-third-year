import socket
import sys
import json

if len(sys.argv) != 3:
    print("Numero de parametros inválidos!")
    exit()


UDP_IP = sys.argv[1]
UDP_PORT = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

dadosGravar = {
    "acao": "D",
    "indicador": "0",
    "valor": "5000",
    "tipoCombustivel": "2",
    "coordenadas": "(-22.21532688695198;-54.75933148596293)"
}

dadosPesquisa = {
    "acao": "P",
    "indicador": "0",
    "raio": "5",
    "tipoCombustivel": "3",
    "coordenadas": "(-22.21532688695198;-54.75933148596293)"
}

sock.sendto(json.dumps(dadosPesquisa).encode(), (UDP_IP, UDP_PORT))

mensagem, server = sock.recvfrom(1024)

print(mensagem.decode())