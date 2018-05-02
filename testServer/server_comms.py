import socket
import sys
import math

# Create a TCP/IP socket
BUFFER_STD = 4
PORT = 5000
IP = "localhost"

def packData(content, type):
    size = math.ceil((len(bytes(inp, 'utf-8')))/BUFFER_STD)
    return ((size).to_bytes(4, "little"),(type).to_bytes(4, "little"),bytes(content, 'utf-8'))

def sendData(sock, data):
    sock.sendall(data[0])
    sock.sendall(data[1])
    sock.sendall(data[2])
    
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
    while(True):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(server_address)
        # Get input
        inp = input("message: ")
        # Quit if input is quit
        if(inp == "quit"):
            break
        message = packData(inp, 1)

        # Send data
        print("sending")
        sendData(sock, message)
        # Receive response!
        print("response")
        print(receiveData(sock))
finally:
    print("fin")
    