# coding: utf-8

import sys
import json
from comunicacao import conectar, novoPacote, transmitir

if len(sys.argv) != 4:
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

def enviarDados(sock, IP, PORTA):
    resp = input("Enviar dados? s / n: ")

    auxPacote = novoPacote()

    while resp == 's':
        seq = auxPacote["seq"] + 1
        pacote = novoPacote(dados=dadosPesquisa, seq=seq, proxSeq = seq + 1)

        resp = transmitir(sock, pacote, IP, PORTA)

        if resp:
            auxPacote = pacote

            dados = resp["dados"]
            print(dados)
        else:
            print("Falha ao enviar os dados!")
        resp = input("Enviar dados? s / n")

if __name__ == '__main__':
    UDP_IP = sys.argv[1]
    UDP_PORT = int(sys.argv[2])
    OPORTA = int(sys.argv[3])

    sock, resp = conectar(UDP_IP, UDP_PORT, OPORTA)

    if sock:
        print("Conexão estabilizada")
        enviarDados(sock, UDP_IP, UDP_PORT)

        # Finalizando a conexão
        pac = novoPacote(FIN=True)
        sock.sendto(json.dumps(pac).encode(), (UDP_IP, UDP_PORT))

    else:
        print("Falha na conexão com o servidor")
        if resp["dados"]:
            print(resp["dados"])