#ifndef MY_SERVER_H
#define MY_SERVER_H

#include "globals.h"
#include "ssl.h"

#include <iostream>
#include <string.h>
#include <cmath>
#include <regex>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntoa

#include "QuestionBank.h"
/** @file */ 

///The default port the Server runs on if no port specified
#define PORT 5000

///The size of 1 piece of content
/** The content needs to be split into pieces of size BUFFER_STD for the server to work properly*/
#define BUFFER_STD 4
/*********FORM OF DATA FROM CLIENT **********
 *          [SIZE][TYPE][DATA][EOM]
 * SIZE = Size of data
 * TYPE = Type of request
 * EOM  = End of message
 ********************************************/

///Defines a Byte through a char
/** 1 byte == 1 char*/
typedef char Byte;

/// all transmitted info == 1 data
typedef struct Data
{
    ///the number of pieces of content
    long size;
    ///The type of transmission requested
    char type[8];
    ///Actual content
    Byte *content;
} Data;


///Server object
/** This object looks after running the Server, it has been made modular so it's easy to implement once the core is designed*/
class Server
{
    public:
    Server();
    Server(int);
    ~Server();

    void initOpenSSL();
    int run();

    int getPort();
    void setCtx(SSL_CTX *);
    SSL_CTX * getCtx();
    void setMainLoop(std::function<void(SSL_CTX * ctx,int server_fd, int client_fd)>);

    private:
    ///Port on which the server runs
    int port;
    ///Context for openSSL
    SSL_CTX *ctx;
    std::function<void(SSL_CTX * ctx,int server_fd, int client_fd)> mainLoop = NULL;
};

///This loop runs when no loop is specified
void defaultDataLoop(SSL_CTX * ctx,int server_fd, int client_fd);
///This loop runs when SSL is enabled
void defaultSSLDataLoop(SSL_CTX * ctx,int server_fd, int client_fd);
/// Error Handling
void error(const char *msg);

// on Listen
void action(int sock);
void actionSSL(SSL *ssl);

#endif
