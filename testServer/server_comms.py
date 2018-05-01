import socket
import sys

# Create a TCP/IP socket


# Connect the socket to the port where the server is listening
server_address = ('localhost', 5000)
print('connecting to %s port %s' % server_address)
inp = ""
try:
    # while(inp !="quit"):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(server_address)
        # Send data
        # inp = input("message: ")
        # message = inp
        # print('sending "%s"' % message)

        size = 4

        message =(421452).to_bytes(4, "little")
        message += bytes("hell",'utf-8')

        sock.sendall(message)
        print(len(message))
        # Look for the response
        amount_received = 0
        amount_expected = len(message)
        # print(bytes(message, 'utf-8'))
        first = True
        while amount_received < amount_expected:
            data = sock.recv(16)
            if(first):
                first= False
                totdata = data
            else:
                totdata+=data
            amount_received += len(data)
        print('received: ')
        print(totdata)
        print('closing socket')
        sock.close()
finally:
    print("fin")
    