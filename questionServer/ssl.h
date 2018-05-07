#ifndef SSL_H
#define SSL_H

#include <openssl/ssl.h>
#include <openssl/err.h>
/** @file */ 
/********OPENSSL*************/

void init_openssl();
void cleanup_openssl();
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);

#endif