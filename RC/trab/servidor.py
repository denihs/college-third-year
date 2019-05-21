import socket
import sys

if len(sys.argv) != 2:
    print("Numero de parametros inválidos!")
    exit()

UDP_ID = socket.gethostname()
UDP_PORT = int(sys.argv[1])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_ID, UDP_PORT))

print("Servidor aberto em: {}:{}".format(socket.gethostbyname(UDP_ID), UDP_PORT))

while True:
    data, (ip, port) = sock.recvfrom(1024) # buffer size is 1024 bytes
    print ("received message:", data.decode(), ip, port)
    sock.sendto("Que viadao bonito".encode(), (ip, port))