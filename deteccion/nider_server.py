from socket import *

serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('localhost', 7777))
print 'Server listo'

while True:
    print 'Esperando Mensaje..'
    message, address = serverSocket.recvfrom(1024)
    print '['+str(address)+']:',message
    serverSocket.sendto("Ok", address)
