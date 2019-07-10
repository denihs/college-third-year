import servidor
import sys

if len(sys.argv) != 2:
    print("Numero de parametros inválidos!")
    exit()

IP = ''  # É como usar INADDR_ANY
PORTA = int(sys.argv[1])

servidor.setup(IP, PORTA)