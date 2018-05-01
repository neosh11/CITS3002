#include "server.h"

using namespace std;


void printStr(char * str, int n){
    for(int i = 0; i < n; i++){
        cout << str[i];
    }
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void action(int sock)
{
    int n;
    char buffer[BUFFER_STD];
    // Write client's message to buffer
    //TODO
    // Needs an upgrade, need to get bytes rather than chars.
    std::cout << "Here is the message: ";

    while (1)
    {
        bzero(buffer, BUFFER_STD);
        // Write client's message to buffer
        //TODO
        // Needs an upgrade, need to get bytes rather than chars.
        if ((n = read(sock, buffer, BUFFER_STD - 1)) < 0)
            error("ERROR reading from socket");

        cout << buffer;

        //Write to client
        if ((n = write(sock, buffer, n)) < 0)
            error("ERROR writing to socket");

        if (n != BUFFER_STD - 1)
            break;
    }
    cout << endl;
}

void action2(int sock)
{
    int n;
    int size_buff;
    int type_buff;

    /********GET SIZE FROM HEADER********/
    if ((n = read(sock, &size_buff, 4)) < 0)
        error("ERROR reading from socket");
    cout << size_buff << "\n";

    if(n == 0){
        return;
    }
    // if(n==0){}
    if ((n = write(sock, &size_buff, n)) < 0)
        error("ERROR writing to socket");
    
    if(n == 0){
        return;
    }
    /********GET TYPE FROM HEADER********/
    if ((n = read(sock, &type_buff, 4)) < 0)
        error("ERROR reading from socket");
    cout << type_buff << "\n";
    // if(n==0){}
    if ((n = write(sock, &type_buff, n)) < 0)
        error("ERROR writing to socket");

    char content_buff[BUFFER_STD];
    for (int i = 0; i < size_buff; i++)
    {
        bzero(content_buff, BUFFER_STD);
        if ((n = read(sock, content_buff, BUFFER_STD)) < 0)
            error("ERROR reading from socket"); //remove
        printStr(content_buff, n);
        // if(n==0){}
        if ((n = write(sock, content_buff, n)) < 0)
            error("ERROR writing to socket"); //remove
    }
    cout << endl;
}
