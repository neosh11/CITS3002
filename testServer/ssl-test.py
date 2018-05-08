import socket
import sys
import math
import ssl
import socket
from os import path

# Create a TCP/IP socket
BUFFER_STD = 4
PORT = 5000
IP = "localhost"

def packData(content, type):
    size = math.ceil((len(bytes(inp, 'utf-8')))/BUFFER_STD)
    return ((size).to_bytes(4, "little"),(type).to_bytes(4, "little"),bytes(content, 'utf-8'))

def sendData(sock, data):
    sock.send(data[0])
    sock.send(data[1])
    sock.send(data[2])

    
def receiveData(sock):
    size_data = int.from_bytes(sock.recv(4), byteorder='little')
    type_data = int.from_bytes(sock.recv(4), byteorder='little')
    # content_data = sock.recv(BUFFER_STD*size_data)
    content_data=b''
    first = True
    for i in range (0,size_data):
        if(first):
            first = False
            content_data = sock.recv(BUFFER_STD)
        else:
            content_data += sock.recv(BUFFER_STD)
    return (size_data, type_data, content_data.decode())


def createSSLSocket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    context = ssl.SSLContext(ssl.PROTOCOL_TLS)
    # certificates required
    context.verify_mode = ssl.CERT_REQUIRED
    # path of ca

    #add this servers certs tooo
    pubcert = path.dirname(path.realpath(__file__)) +'/certificates/certificate.pem'
    keycert = path.dirname(path.realpath(__file__)) +'/certificates/key.pem'
    context.load_cert_chain(certfile=pubcert, keyfile=keycert)
    
    cert =  path.dirname(path.realpath(__file__)) +'/certificates/ca-valid.crt'
    context.load_verify_locations(cert)
    context.check_hostname = True

    sslsock = context.wrap_socket(sock = sock, server_side=False, do_handshake_on_connect=True, suppress_ragged_eofs=True, server_hostname='localhost', session=None)
    sslsock.connect(server_address)
    return sslsock

def start_send(message):
    sslsock = createSSLSocket()
    sendData(sslsock, message)
    returnData = receiveData(sslsock)
    return returnData

# Connect the socket to the port where the server is listening
server_address = (IP, PORT)
print('connecting to %s port %s' % server_address)
inp = ""
try:
    print(ssl.get_server_certificate(server_address, ssl_version=ssl.PROTOCOL_TLS, ca_certs=None))
    while(True):
        # Get input
        inp = input("message: ")
        # Quit if input is quit
        if(inp == "quit"):
            break
        message = packData(inp, 1)
        print(start_send(message))

finally:
    print("fin")
    