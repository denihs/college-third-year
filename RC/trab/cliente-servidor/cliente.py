import cliente
import sys

if len(sys.argv) != 3:
    print("Numero de parâmetros inválidos!")
    exit()
UDP_IP = sys.argv[1]
UDP_PORT = int(sys.argv[2])

cliente.setup(UDP_IP, UDP_PORT)