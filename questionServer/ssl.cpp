#include "server.h"

///Initilizes openSSL
void init_openssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

///Cleans up openSSL
void cleanup_openssl()
{
    EVP_cleanup();
}

///Creates context
SSL_CTX *create_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    /* The actual protocol version used will be negotiated to the highest version mutually supported by the client and the server.
    The supported protocols are SSLv3, TLSv1, TLSv1.1 and TLSv1.2
    Applications should use these methods, and avoid the version-specific methods 
    */

//    method = TLS_server_method(); TOO Modern for old openssl
    method = SSLv23_server_method();
    ctx = SSL_CTX_new(method);

    if (!ctx)
    {
        error("Can't create SSL_CTX");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

///Configures context
void configure_context(SSL_CTX *ctx)
{
    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "certs/cert.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "certs/key.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    /* Check if the server certificate and private-key matches */
    if (!SSL_CTX_check_private_key(ctx))
    {
        error("Private key does not match the certificate public key\n");
        exit(EXIT_FAILURE);
    }

    // Require certificates from other server !!
    if (!SSL_CTX_load_verify_locations(ctx, "certs/ca-valid.crt", NULL))
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_verify_depth(ctx, 1);
}
