import socket
import sys
import math
import ssl
import socket

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

# Connect the socket to the port where the server is listening
server_address = (IP, PORT)
print('connecting to %s port %s' % server_address)
inp = ""
try:
    print(ssl.get_server_certificate(server_address, ssl_version=ssl.PROTOCOL_TLS, ca_certs=None))
    while(True):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        context = ssl.SSLContext(ssl.PROTOCOL_TLS)
        sslsock = context.wrap_socket(sock = sock, server_side=False, do_handshake_on_connect=True, suppress_ragged_eofs=True, server_hostname=None, session=None)
        sslsock.connect(server_address)

        # Get input
        inp = input("message: ")
        # Quit if input is quit
        if(inp == "quit"):
            break
        message = packData(inp, 1)
        print(message)

        # Send data
        print("sending")
        sendData(sslsock, message)
        # Receive response!
        print("response")
        print(receiveData(sslsock))


finally:
    print("fin")
    