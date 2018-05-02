/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include "server.h"

using namespace std;


// Server variables+ functions
void initServer();

QuestionBank questionBank = QuestionBank("./resources/questions.data");

int main(int argc, char *argv[])
{
    //Kill Zombies!!
    /*******Signal catcher for killing zombies********/
    signal(SIGCHLD, SIG_IGN);

    int server_fd, client_fd;
    socklen_t client_len;
    struct sockaddr_in serv_addr, cli_addr;

    cout << "Starting Server" << endl;
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

    cout << "Server Started on port " << PORT << endl;
    /***************RUN FOREVER*******************/
    //ctrl+c to kill

    client_len = sizeof(cli_addr);
    while (1)
    {
        /**********Accept Client************/
        if ((client_fd = accept(server_fd, (struct sockaddr *)&cli_addr, &client_len)) < 0)
            error("ERROR on accept"); //Don't need in future??

        cout << "client: " << inet_ntoa(cli_addr.sin_addr) << ", port: " << ntohs(cli_addr.sin_port) << ", socket: " << client_fd << endl;

        switch (fork())
        {
        case -1:
            error("ERROR on fork");
            break;
        case 0:
            close(server_fd);
            action2(client_fd);
            exit(0);
            break;
        default:
            close(client_fd);
        }
        close(client_fd);
    }
    close(server_fd);
    return 0;
}

void initServer()
{
    // Put stuff here
}
