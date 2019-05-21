import socket
import sys

if len(sys.argv) != 3:
    print("Numero de parametros inválidos!")
    exit()


UDP_IP = sys.argv[1]
UDP_PORT = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print(UDP_IP, UDP_PORT)
sock.sendto("Everything is gonna be just fine".encode(), (UDP_IP, UDP_PORT))