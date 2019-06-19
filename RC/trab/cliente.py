import sys
import json
from comunicacao import conectar, novoPacote

if len(sys.argv) != 3:
    print("Numero de parametros inválidos!")
    exit()

dadosGravar = {
    "acao": "D",
    "indicador": 0,
    "valor": 4999,
    "tipoCombustivel": 1,
    "coordenadas": "(-22.21532688695198;-54.75933148596293)"
}

dadosPesquisa = {
    "acao": "P",
    "indicador": 0,
    "raio": 5,
    "tipoCombustivel": 2,
    "coordenadas": "(-22.21532688695198;-54.75933148596293)"
}

if __name__ == '__main__':
    UDP_IP = sys.argv[1]
    UDP_PORT = int(sys.argv[2])

    pacote = novoPacote(dados=dadosPesquisa)

    sock = conectar(UDP_IP, UDP_PORT)

    if sock:
        print("Conexão estabilizada")
        sock.sendto(json.dumps(pacote).encode(), (UDP_IP, UDP_PORT))

        pacote, server = sock.recvfrom(1024)
        dados = json.loads(pacote.decode())["dados"]
        print(dados)
    else:
        print("Falha na conexão com o servidor")