import socket
import sys
import math

# Create a TCP/IP socket
BUFFER_STD = 4
PORT = 5000
IP = "localhost"



# Connect the socket to the port where the server is listening
server_address = (IP, PORT)
print('connecting to %s port %s' % server_address)
inp = ""
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(server_address)

    while(true):
        print(sock.recv(10))

finally:
    print("fin")
    