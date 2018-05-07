#include "server.h"

Server::Server() : port(PORT) {}
Server::Server(int port) : port(port) {}

Server::~Server()
{
    std::cout << "closing server" << '\n';
}

///Initializes Server with openSSL context :0
void Server::initOpenSSL()
{
    /********OPENSSL CONTEXT***********/
    ctx = create_context();
    configure_context(ctx);

    //Use SSL loop
    mainLoop = defaultSSLDataLoop;
}

///Gets port that the server is running on
int Server::getPort()
{
    return port;
}

///Sets the SSL_CCONTEXT
void Server::setCtx(SSL_CTX *ctx)
{
    this->ctx = ctx;
}

///Gets the SSL_CTX
SSL_CTX *Server::getCtx()
{
    return ctx;
}

///Sets the main loop of the server
void Server::setMainLoop(std::function<void(SSL_CTX * ctx,int server_fd, int client_fd)> mainLoop)
{
    this->mainLoop = mainLoop;
}

///Runs the Server &hearts;
int Server::run()
{

    int server_fd, client_fd;
    socklen_t client_len;
    struct sockaddr_in serv_addr, cli_addr;

    /**************Create a Socket**************/
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("ERROR opening socket");

    std::cout << "Starting Server"
              << "\n";
    /**************Create a Server Address and bind socket**************/
    bzero((char *)&serv_addr, sizeof(serv_addr)); //Sets all values in buffer to Zero
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port); //network byte order
    if (::bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error("ERROR on binding");

    /**************Start Listening to Socket**************/
    if (listen(server_fd, 5) < 0)
        error("ERROR on listen");

    std::cout << "Server Started on port " << port << '\n';
    /***************RUN FOREVER*******************/
    //ctrl+c to kill

    client_len = sizeof(cli_addr);
    while (1)
    {
        /**********Accept Client************/
        if ((client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, &client_len)) < 0)
            error("ERROR on accept"); //Don't need in future??

        std::cout << "client: " << inet_ntoa(cli_addr.sin_addr) << ", port: " << ntohs(cli_addr.sin_port) << ", socket: " << client_fd << '\n';

        if (mainLoop == NULL)
        {
            setMainLoop(defaultDataLoop);
            mainLoop(ctx, server_fd, client_fd);
        }
        else
        {
            mainLoop(ctx, server_fd, client_fd);
        }

        /* Terminate communication on a socket */
        if (close(client_fd) < 0)
            error("failed to close");
    }
    close(server_fd);

    SSL_CTX_free(getCtx());
    cleanup_openssl();
    return 0;
}

void defaultDataLoop(SSL_CTX *ctx,int server_fd, int client_fd)
{
    switch (fork())
        {
        case -1:
            error("ERROR on fork");
            break;
        case 0:
            close(server_fd);
            action(client_fd);
            exit(0);
            break;
        default:
            close(client_fd);
        }
}

void defaultSSLDataLoop(SSL_CTX * ctx,int server_fd, int client_fd)
{

    X509 *client_cert = NULL;
    char *str;

    /************CREATED SSL***************/
    SSL *ssl = NULL;
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);

    if (SSL_accept(ssl) <= 0)
    {
        std::cout << "fail at accept SSL\n";
        ERR_print_errors_fp(stderr);
    }

    else
    {
        /* Get the client's certificate (optional) */
        client_cert = SSL_get_peer_certificate(ssl);
        if (client_cert != NULL)
        {
            printf("Client certificate:\n");
            str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
            if (str == NULL)
                error("No client certificate");
            // RETURN_NULL(str);
            printf("\t subject: %s\n", str);

            str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
            if (str == NULL)
                error("No client certificate");
            printf("\t issuer: %s\n", str);

            X509_free(client_cert);
        }

        /*------- DATA EXCHANGE - Receive message and send reply. -------*/
        /* Receive data from the SSL client */

        actionSSL(ssl);
        if (SSL_shutdown(ssl) < 0)
        {
            error("failed to shutdown SSL");
        }

        std::cout << "Closing sll \n";
        /* Free the SSL structure */
        SSL_free(ssl);
    }

}