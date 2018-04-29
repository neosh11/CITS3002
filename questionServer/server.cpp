/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <iostream>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntoa

#define PORT 5000

#define BUFFER_STD 256
/*********FORM OF DATA FROM CLIENT **********
 *          [SIZE][TYPE][DATA][EOM]
 * SIZE = Size of data
 * TYPE = Type of request
 * EOM  = End of message
 ********************************************/
#define SIZE_OF_DSIZE sizeof long   //size of [SIZE]
#define SIZE_OF_DTYPE 16            //size of [TYPE]
#define SIZE_OF_DEOM 10             //size of [EOM]

typedef char Byte;
typedef struct Data {
    long size;
    char type[8];
    Byte *content;
} Data;



using namespace std;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
    int server_fd, client_fd;
    socklen_t client_len;
    char buffer[BUFFER_STD];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    cout<<"Starting Server"<<endl;
    /**************Create a Socket**************/
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("ERROR opening socket");

    /**************Create a Server Address and bind socket**************/
    bzero((char *)&serv_addr, sizeof(serv_addr)); //Sets all values in buffer to Zero
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT); //network byte order
    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error("ERROR on binding");

    /**************Start Listening to Socket**************/
    if (listen(server_fd, 5) < 0)
        error("ERROR on listen");

    cout<<"Server Started on port "<<PORT<<endl;
    /***************RUN FOREVER*******************/
    //ctrl+c to kill
    while (1)
    {

        /**********Accept Client************/
        client_len = sizeof(cli_addr);
        if ((client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, &client_len)) < 0)
            error("ERROR on accept"); //Don't need in future??

        cout<<"client: "<<inet_ntoa(cli_addr.sin_addr)<< ", port: "<<ntohs(cli_addr.sin_port)<<", socket: "<<client_fd<<endl; 
        
        bzero(buffer, 256);
        // Write client's message to buffer
        //TODO
        // Needs an upgrade, need to get bytes rather than chars.
        if ((n = read(client_fd, buffer, 255))< 0)
            error("ERROR reading from socket");

        cout<<"Here is the message: "<<buffer<<endl;
        
        //Write to client
        if ((n = write(client_fd, buffer, strlen(buffer)))< 0)
            error("ERROR writing to socket");

        close(client_fd);
    }
    return 0;
}