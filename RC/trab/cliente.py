# coding: utf-8

import sys
import json
from comunicacao import conectar, novoPacote, transmitir

if len(sys.argv) != 3:
    print("Numero de parâmetros inválidos!")
    exit()

def tratarEntrada(d):
    if d.lower() == 'sair':
        return d

    # Validando o numero de parâmetros
    dados = d.replace(' ', '').split(',')
    if len(dados) != 4:
        print("Numero inválido de parâmetros\n")
        return None

    # Validando a ação à ser executada
    if dados[0].lower() not in ["d", "p"]:
        print("Tipo de ação inválida. Os valores permitidos são: D ou P\n")
        return None

    # Validando o raio/preço
    x = None
    try:
        x = int(dados[1])
        if x <= 0:
            raise Exception()
    except:
        print("Você deve fornecer um inteiro positivo para o {}\n".format("raio" if dados[0].lower() == 'p' else "preço"))
        return None

    # Validando o tipo de combustível
    if dados[2].lower() not in ["0", "1", "2"]:
        print("Tipo de combustível inválido. Os valores permitidos são:\n0 - diesel\n1 - álcool\n2 - gasolina\n")
        return None

    # Validando as coordenadas
    coordenadas = dados[3].split(";")
    if len(coordenadas) != 2 or dados[3][0] != '(' or dados[3][-1] != ')':
        print("Coordenadas inválidas\n")
        return None
    coordenadas = dados[3][1:-1].split(";")
    try:
        float(coordenadas[0])
        float(coordenadas[1])
    except:
        print("As coordenas devem ser numéricas\n")
        return None

    # Preparando dados como um objeto para serem retornados
    dadosEnvio = {
        "acao": dados[0],
        "tipoCombustivel": dados[2],
        "coordenadas": dados[3]
    }

    if dadosEnvio["acao"].lower() == "d":
        dadosEnvio["valor"] = dados[1]
    else:
        dadosEnvio["raio"] = dados[1]

    return dadosEnvio


def enviarDados(sock, IP, PORTA):
    entrada = True

    auxPacote = novoPacote()

    while entrada != 'sair':
        entrada = input("Entrada: ")

        dados = tratarEntrada(entrada)

        if dados and dados != 'sair':
            seq = auxPacote["seq"] + 1
            pacote = novoPacote(dados=dados, seq=seq, proxSeq = seq + 1)

            resp = transmitir(sock, pacote, IP, PORTA)

            if resp:
                respDados = resp["dados"]
                del resp["dados"]
                print("\nPacote recebido: {}".format(json.dumps(resp, indent=2)))
                if dados["acao"].lower() == 'p':
                    print("\nDados do posto encontrado")
                    print("------------------------------")
                print(respDados)
                print("\n")

                auxPacote = pacote
            else:
                print("Falha ao enviar os dados!")
                print("\n")


if __name__ == '__main__':
    UDP_IP = sys.argv[1]
    UDP_PORT = int(sys.argv[2])

    sock, resp = conectar(UDP_IP, UDP_PORT)

    if sock:
        porta = resp
        print("Conexão estabilizada\n")
        enviarDados(sock, UDP_IP, porta)

        # Finalizando a conexão
        pac = novoPacote(FIN=True)
        sock.sendto(json.dumps(pac).encode(), (UDP_IP, porta))

    else:
        print("Falha na conexão com o servidor")
        if resp["dados"]:
            print(resp["dados"])
            print("\n")
    sock.close()