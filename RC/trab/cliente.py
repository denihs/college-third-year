import socket
import sys

if len(sys.argv) != 3:
    print("Numero de parametros inválidos!")
    exit()


UDP_IP = sys.argv[1]
UDP_PORT = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.sendto("P, 0, 0, 5, (-22.21532688695198;-54.75933148596293)".encode(), (UDP_IP, UDP_PORT))

mensagem, server = sock.recvfrom(1024)

print(mensagem.decode())