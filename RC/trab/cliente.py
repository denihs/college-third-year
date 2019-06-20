# coding: utf-8

import sys
import json
from comunicacao import conectar, novoPacote

if len(sys.argv) != 3:
    print("Numero de parâmetros inválidos!")
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
        resp = input("Enviar dados? s / n: ")

        while resp == 's':
            sock.sendto(json.dumps(pacote).encode(), (UDP_IP, UDP_PORT))
            pacoteRec, server = sock.recvfrom(1024)
            dados = json.loads(pacoteRec.decode())["dados"]
            print(dados)
            resp = input("Enviar dados? s / n")
        # Finalizando a conexão
        pac = novoPacote(FIN=True)
        sock.sendto(json.dumps(pac).encode(), (UDP_IP, UDP_PORT))

    else:
        print("Falha na conexão com o servidor")