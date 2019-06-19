# coding: utf-8

import socket
import threading
import sys
import json
from math import radians, cos, sin, asin, sqrt
from comunicacao import aceitar, novoPacote

def comandoInvalido(comando):
    for chave in comando:
        if not comando[chave] and comando[chave] != 0:
            return "O parâmetro {} não pode ser vazio".format(chave)    

    if len(comando["coordenadas"].split(';')) != 2:
        return "Parâmetro de coordenadas inválido"

    return False

def gravarDados(comando):
    msgComandoInvalido = comandoInvalido(comando)
    if msgComandoInvalido:
        return msgComandoInvalido

    try:
        arq = open("dados.json", "r")
        dados = json.load(arq)
        arq.close()
    except:
        dados = []

    arq = open("dados.json", "w")

    dados.append({
        "tipoCombustivel": comando["tipoCombustivel"],
        "valor": int(comando["valor"]),
        "coordenadas": comando["coordenadas"]
    })

    json.dump(dados, arq, indent=2)

    arq.close()

    return "Dados gravados"

def haversineFormula(latI, lonI, lat, lon):
    """
        Essa formula irá calcular a distância entre dois pontos na terra
        (com lon e lat especificados em graus decimais). Os calculos são
        baseados na terra circular, desconsiderando os efeitos elipsionáis
    """

    # convertendo os graus decimais para radianos
    latI, lonI, lat, lon = map(radians, map(float, [latI, lonI, lat, lon]))

    # aplicando a fórmula
    dlon = lon - lonI
    dlat = lat - latI

    # Quadrado da metade do comprimento do acorde entre os pontos
    a = sin(dlat / 2) ** 2 + cos(latI) * cos(lat) * sin(dlon / 2) ** 2

    # Distância angular em radianos
    c = 2 * asin(min(1, sqrt(a)))

    R = 6371  # Raio da terra em quilômetros

    return R * c

def pegarCoordenadas(coord):
    return coord.replace(" ", "")[1:-1].split(";")

def buscarResultado(dados, tipoCombustivel, raio, coord):
    # filtrando todos os registros com o tipo de combustível requisitado
    resutados = list(filter(lambda item: int(item["tipoCombustivel"]) == tipoCombustivel, dados))

    dadosEncontrados = []

    # Calculado distâncias e salvando os postos dentro do raio
    for item in resutados:
        latP, lonP = pegarCoordenadas(item["coordenadas"])
        lat, lon = pegarCoordenadas(coord)

        distancia = haversineFormula(latP, lonP, lat, lon)

        if distancia <= raio:
            dadosEncontrados.append(item)

    # Buscando dentro os dados encontrados o de menor preço
    if len(dadosEncontrados) > 0:
        return min(dadosEncontrados, key=lambda x: x['valor'])

    return False

def buscarDados(comando):
    msgComandoInvalido = comandoInvalido(comando)
    if msgComandoInvalido:
        return msgComandoInvalido

    try:
        arq = open("dados.json", "r")
        dados = json.load(arq)
        arq.close()
    except:
        return "Nenhum valor encontrado!"

    tipoCombustivel = int(comando["tipoCombustivel"])
    raio = float(comando["raio"])
    coord = comando["coordenadas"]

    resultado = buscarResultado(dados, tipoCombustivel, raio, coord)

    if resultado:
        tc = ['diesel', 'álcool', 'gasolina']
        valor = int(resultado["valor"]) / 1000
        return "Tipo combustível: {}\nPreço: R${: .3f}\ncoordenadas: {}".format(tc[tipoCombustivel], valor, resultado["coordenadas"])


    return "Nenhuma posto encontrado para esses parâmetros."

def executar(comando):
    acao = comando["acao"].lower()

    if acao == 'd':
        return gravarDados(comando)
    elif acao == 'p':
        return buscarDados(comando)
    else:
        return "Ação não conhecida: {}".format(acao)


def log(dados, info):

    if info == "cliente":
        ipCliente, portaCliente = dados
        print('-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_')
        print("Cliente conectado: {}:{}".format(ipCliente, portaCliente))
    
    if info == "comando":
        print('-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_')
        print("Dados recebidos: {}".format(dados))
        print('-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_')

    if info == "erro-cliente":
        ipCliente, portaCliente = dados
        print("Erro ao tentar conectar o cliente {}:{}".format(ipCliente, portaCliente))

def processarConexao(sock, lock):
    resposta, cliente = aceitar(sock)
    if resposta:
        log(cliente, "cliente")
        # Recebendo o comando do cliente
        pacote, (ipCliente, portaCliente) = sock.recvfrom(1024)
        
        pacote = json.loads(pacote.decode())
        
        comando = pacote["dados"]

        log(comando, "comando")

        # Bloqueando thread para acessar a zona critica do servidor
        lock.acquire()
        resultado = executar(comando)
        lock.release()

        pacote = novoPacote(dados=resultado)
        # Enviando resposta para o cliente
        sock.sendto(json.dumps(pacote).encode(), (ipCliente, portaCliente))
    else:
        log(cliente, "erro-cliente")

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Numero de parametros inválidos!")
        exit()

    IP = '' # É como usar INADDR_ANY
    PORTA = int(sys.argv[1])

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((IP, PORTA))

    print("Servidor aberto na porta: {}".format(PORTA))

    lock = threading.Lock()

    contador = 0

    while True:

        thread = threading.Thread(None, processarConexao, None,(sock, lock))

        thread.start() # iniciando os serviços na thread
        thread.join() # esperando a thread terminar seu job

        contador += 1
        if contador == 1:
            break
    sock.close()