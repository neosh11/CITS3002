#ifndef MY_SERVER_H
#define MY_SERVER_H

#include "globals.h"

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



#include <openssl/ssl.h>
#include <openssl/err.h>



#include "QuestionBank.h"

#define PORT 5000
#define BUFFER_STD 4
/*********FORM OF DATA FROM CLIENT **********
 *          [SIZE][TYPE][DATA][EOM]
 * SIZE = Size of data
 * TYPE = Type of request
 * EOM  = End of message
 ********************************************/
#define SIZE_OF_DSIZE sizeof long //size of [SIZE]
#define SIZE_OF_DTYPE 16          //size of [TYPE]
#define SIZE_OF_DEOM 10           //size of [EOM]

typedef char Byte;
typedef struct Data
{
    long size;
    char type[8];
    Byte *content;
} Data;

// Error Handling
void error(const char *msg);

// on Listen
void action(int sock);
void action2(int sock);

#endif


/********OPENSSL*************/

void init_openssl();
void cleanup_openssl();
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);

