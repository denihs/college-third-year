import socket
import sys

if len(sys.argv) != 2:
    print("Numero de parametros inválidos!")
    exit()

IP = '' # É como usar INADDR_ANY
PORTA = int(sys.argv[1])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((IP, PORTA))

print("Servidor aberto na porta: {}".format(PORTA))

while True:
    mensagem, (ipCliente, portaCliente) = sock.recvfrom(1024)
    print ("received message:", mensagem.decode(), ipCliente, portaCliente)
    sock.sendto("Hey boy".encode(), (ipCliente, portaCliente))