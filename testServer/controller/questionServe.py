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
server_address = (IP, PORT)

def packData(content, type):
    size = math.ceil((len(bytes(content, 'utf-8')))/BUFFER_STD)
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
    return content_data.decode()

def createSSLSocket():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    context = ssl.SSLContext(ssl.PROTOCOL_TLS)
    # certificates required
    context.verify_mode = ssl.CERT_REQUIRED
    # path of ca

    #add this servers certs tooo
    pubcert = path.dirname(path.realpath(__file__)) +'/../certificates/certificate.pem'
    keycert = path.dirname(path.realpath(__file__)) +'/../certificates/key.pem'
    context.load_cert_chain(certfile=pubcert, keyfile=keycert)
    
    cert =  path.dirname(path.realpath(__file__)) +'/../certificates/ca-valid.crt'
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


def getQuestion(number):
    message = packData(f"question#{number}", 1)
    return start_send(message)

def markQuestion(number, answer):
    message = packData(f"mark#{number}#{answer}", 1)
    return start_send(message)

def sizeQuestion():
    message = packData("size", 1)
    return start_send(message)

# print(getQuestion(1))
# print(getQuestion(0))
# print(markQuestion(10, 1))