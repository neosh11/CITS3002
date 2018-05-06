import socket
import sys
import math
import ssl

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
    
    ssl.wrap_socket(sock, keyfile=None, certfile=None, server_side=False, cert_reqs=ssl.CERT_REQUIRED, ssl_version=ssl.PROTOCOL_TLS, ca_certs=None, do_handshake_on_connect=True, suppress_ragged_eofs=True, ciphers=None)
    
    
    sock.sendall(bytes("helooooooooooooooikojkok", 'utf-8'))

    print(ssl.get_server_certificate(sock, ssl_version=PROTOCOL_TLS, ca_certs=None))
    while(True):
        print(sock.recv(10))

finally:
    print("fin")
    